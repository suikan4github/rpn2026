/*
 * calc.cpp
 *
 *  Created on: Jan 8, 2022
 *      Author: takemasa
 */

#include "main.h"

#include <stdio.h>
#include <cstring>
#include <rpnengine.hpp>

extern SPI_HandleTypeDef hspi1;

rpn_engine::Console *console;

const int kDigitsNum = 9;
const int kDigitsSize = 3;

// template VFD data with blank segment
uint8_t vfd_data[kDigitsNum][kDigitsSize] ={
							{0x00, 0x08, 0x00},	// digit 8,
							{0x00, 0x04, 0x00},	// digit 7,
							{0x00, 0x02, 0x00},	// digit 6,
							{0x00, 0x01, 0x00},	// digit 5,
							{0x00, 0x00, 0x80},	// digit 4,
							{0x00, 0x00, 0x40},	// digit 3,
							{0x00, 0x00, 0x20},	// digit 2,
							{0x00, 0x00, 0x10},	// digit 1,
							{0x00, 0x00, 0x08},	// digit 0,
						};


// This function is call from the AntiChattering class, if the
// Key is surely pressed after anti-chattering process.
// row, col are physical key matrix position( zero origin ).
void KeyPerssedCallBackFunction(unsigned row, unsigned col)
{
	bool is_func_key_pressed = console->GetIsFuncKeyPressed();
	bool is_hex_mode = console->GetIsHexMode();

	rpn_engine::Op opcode =  rpn_engine::EncodeKey(row, col, is_func_key_pressed, is_hex_mode);

	console->Input(opcode);

}

extern "C" void exec_calc()
{
	const int kRowCount = 9;					// size of row in the key matrix
	const int kColCount = 3;					// size of col in the key matrix
	const int kChatteringThreashold = 10;	// how many continuous input is needed to change the state
	rpn_engine::AntiChattering *anti_chatter[kRowCount][kColCount];
	rpn_engine::SegmentDecoder *segment_decoder;

	char old_text[10] = "00000000";		// initialize the buffer to store the previous state.
	int old_decimal_point_position=7;	// initialize the buffer to store the previous state.


	printf("\n\nHello!\n");

	// create console object
   console = new rpn_engine::Console(" RPN-210 ");

	// initialize the anti chattering objects with threshold, callback and its position.
	for (unsigned int row=0; row<kRowCount; row++)
		for ( unsigned int col=0; col<kColCount; col++)
			anti_chatter[row][col]= new rpn_engine::AntiChattering(
															kChatteringThreashold,
															kChatteringThreashold,
															KeyPerssedCallBackFunction,
															row,
															col);

	// hardware dependent segment mapping.
	segment_decoder = new rpn_engine::SegmentDecoder(
			1<<2,	// a
			1<<0,	// b
			1<<5,	// c
			1<<4,	// d
			1<<6,	// e
			1<<1,	// f
			1<<7,	// g
			1<<3	// DP
			);

	// Set Blank pin to "L" to show the display.
	HAL_GPIO_WritePin(BLANK_GPIO_Port, BLANK_Pin, GPIO_PIN_RESET);

	while(1)
	{
		  char digits_str[kDigitsNum+1];	// with zero termination
		  int decimal_point_position;

		  // Scan all 9 digits
		  for ( int digit = 0; digit < kDigitsNum; digit ++)
		  {
			  // clean up the digit data
			  vfd_data[digit][0] = 0;
			  vfd_data[digit][1] &= 0x7F;	// clear MSB
			  vfd_data[digit][2] &= 0xF8;	// clear bit 2:0

			  console->GetText(digits_str);										// get text to display;
			  decimal_point_position = console->GetDecimalPointPosition();	// decimal point 0 is right of the digit 0. ( Right most )
			  if (decimal_point_position != console->kDecimalPointNotDisplayed) {	// Do we need to display decimal point?
				  decimal_point_position = 8 - decimal_point_position; 				// Convert the DP position to the array index.
				  vfd_data[decimal_point_position ][1] |= 0x80;						// set decimal point. Byte 1 bit 8 is the DP segment of VFD.
			  }

			  uint8_t segment;								// segment pattern.

			  if  ( digit == 0 && console->GetIsFuncKeyPressed()){	// Is func key pressed && left most digit?
				  segment = segment_decoder->decode('_'); 			// So, let's display func key status sign.
				  vfd_data[digit][0] |= segment >> 4;					// set USB 4 bits
				  vfd_data[digit][2] |= segment & 0x07;				// set LSB 3 bits
			  }
			  segment=	segment_decoder->decode(digits_str[digit]) ;	// Display the number to this digit.
			  vfd_data[digit][0] |= segment >> 4;						// set USB 4 bits
			  vfd_data[digit][2] |= segment & 0x07;					// set LSB 3 bits

			  // Transmit a digits data.
			  // This poling transmission define the duration of the display of a digit.
			  // To change the duration, change the clock configuration of SPI.
			  HAL_SPI_Transmit(&hspi1, vfd_data[digit], 3, 100);

			  // Assert load pulse
			  HAL_GPIO_WritePin(LOAD_GPIO_Port, LOAD_Pin, GPIO_PIN_SET);
			  // Deassert load pulse.
			  HAL_GPIO_WritePin(LOAD_GPIO_Port, LOAD_Pin, GPIO_PIN_RESET);

			  // Dummy transmission to give the settling time of key.
			  HAL_SPI_Transmit(&hspi1, vfd_data[digit], 3, 100);

			  // read key state
			  anti_chatter[digit][0]->Input(
					  ( HAL_GPIO_ReadPin(KEY0_GPIO_Port, KEY0_Pin) == GPIO_PIN_SET)?
							  rpn_engine::kklHigh :
							  rpn_engine::kklLow
			  );
			  anti_chatter[digit][1]->Input(
					  ( HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin) == GPIO_PIN_SET)?
							  rpn_engine::kklHigh :
							  rpn_engine::kklLow
			  );
			  anti_chatter[digit][2]->Input(
					  ( HAL_GPIO_ReadPin(KEY2_GPIO_Port, KEY2_Pin) == GPIO_PIN_SET)?
							  rpn_engine::kklHigh :
							  rpn_engine::kklLow
			  );

			  if (!std::strcmp(digits_str,old_text) && (decimal_point_position == old_decimal_point_position))
				  ; // do notihng
			  else	// if not equal
			  {
				  std::strcpy(old_text,digits_str);						// save digits_str;
				  old_decimal_point_position = decimal_point_position;  // save decimal point position
				  char print_txt[12];
				  if (old_decimal_point_position == console->kDecimalPointNotDisplayed)
					  std::strcpy(print_txt, old_text); // copy as is
				  else
				  {	// need to copy with decimal point
					  int dppos = old_decimal_point_position + 1;
					  for ( int i=0; i<dppos; i++)
						  print_txt[i] = old_text[i];
					  print_txt[dppos] = '.';
					  for ( int i= dppos; i<9; i++)
						  print_txt[i+1] = old_text[i];
					  print_txt[10] = 0;
				  }
				  printf("%s\n", print_txt);
			  }
		  } // for

	}


}
