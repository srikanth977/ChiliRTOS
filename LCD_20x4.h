#ifndef __LCD_20X4_H_
#define __LCD_20X4_H_

#include <stdint.h>
#include "STM32F4_RTOS_BSP.h"
/*
 * pin assignment reference: https://www.winstar.com.tw/products/character-lcd-display-module/20x4-lcd-display.html
Pin No. 	Symbol 		Level 		PIN ONBOARD     Description
1 			VSS 		0V 			-               Ground
2 			VDD 		5.0V 		-               Supply Voltage for logic
3 			VO 			(Variable) 	-               Operating voltage for LCD
4 			RS 			H/L 		PA8             H: DATA, L: Instruction code
5 			R/W 		H/L 		PA9             H: Read  L: Write
6 			E 			H,H?L 		PA10            Chip enable signal
7 			DB0 		H/L 		PB0             Data bus line
8 			DB1 		H/L 		PB1             Data bus line
9 			DB2 		H/L 		PB2             Data bus line
10 			DB3 		H/L 		PB3             Data bus line
11 			DB4 		H/L 		PB4             Data bus line
12 			DB5 		H/L 		PB5             Data bus line
13 			DB6 		H/L 		PB6             Data bus line
14 			DB7 		H/L 		PB7             Data bus line
15 			A 			- 			-               Power supply for B/L(+)
16 			K 			- 			-               Power supply for B/L(-)

*/
 
 
 /* Configure the data bus and Control bus as per the hardware connection */
#define LCD_DATA_BUS_PORT     			GPIOB
#define LCD_CTRL_BUS_PORT   			GPIOA
#define LCD_D0     						0
#define LCD_D1     						1
#define LCD_D2     						2
#define LCD_D3     						3
#define LCD_D4     						4
#define LCD_D5     						5
#define LCD_D6     						6
#define LCD_D7     						7

#define LCD_RS     						8
#define LCD_RW     						9
#define LCD_EN     						10

/***************************************************************************************************
                        				LCD SIZE
****************************************************************************************************/
#define LCD_NUM_OF_LINES				4
#define LCD_NUM_OF_CHARS    			20


/***************************************************************************************************
                        List of commonly used LCD Commands
****************************************************************************************************/
#define CMD_LCD_CLEAR						0x01u
#define CMD_LCD_RETURN_HOME          		0x02u
#define CMD_DECREMENT_CURSOR				0x04u	//Shift cursor to left
#define CMD_INCREMENT_CURSOR				0x06u	//Shift cursor to right
#define CMD_SHIFT_DISPLAY_RIGHT				0x05u
#define CMD_SHIFT_DISPLAY_LEFT				0x07u
#define CMD_DISPLAY_OFF_CURSOR_OFF			0x08u
#define CMD_DISPLAY_OFF_CURSOR_ON			0x0Au
#define CMD_DISPLAY_ON_CURSOR_OFF    		0x0Cu
#define CMD_DISPLAY_ON_CURSOR_ON     		0x0Eu
#define CMD_DISPLAY_ON_CURSOR_BLINK  		0x0Fu
#define CMD_SHIFT_ENTIRE_DISPLAY_TO_LEFT	0x10u
#define CMD_SHIFT_ENTIRE_DISPLAY_TO_RIGHT	0x1Cu
#define CMD_FORCE_CURSOR_TO_BEGINNING_L1	0x80u
#define CMD_FORCE_CURSOR_TO_BEGINNING_L2	0xC0u
#define CMD_LCD_FOUR_BIT_MODE        		0x28u
#define CMD_LCD_EIGHT_BIT_MODE       		0x38u
#define LCD_SCROLL_DELAY_MS    				200

/***************************************************************************************************
                  Masks for configuring the DataBus and Control Bus direction
****************************************************************************************************/
#define PortDataBusConfig  				((1<<LCD_D0)|(1<<LCD_D1)|(1<<LCD_D2)|(1<<LCD_D3)|(1<<LCD_D4)|(1<<LCD_D5)|(1<<LCD_D6)|(1<<LCD_D7))
#define PortCtrlBusConfig  				((1<<LCD_RS)|(1<<LCD_RW)|(1<<LCD_EN))
#define LCD_dataBusMask    				((1<<LCD_D0)|(1<<LCD_D1)|(1<<LCD_D2)|(1<<LCD_D3)|(1<<LCD_D4)|(1<<LCD_D5)|(1<<LCD_D6)|(1<<LCD_D7))

extern uint8_t Lcd_Track_LineNum;         						//Variable to track the line numbers
extern uint8_t Lcd_Track_CursorPos;       						//Variable to track the cursor
extern uint8_t ARR_LcdLineNumAddress[4];						//Address positions of line numbers

void Lcd_SetCursor(uint8_t Lcd_LineNum, uint8_t Lcd_CursorPos);
void Lcd_GoToNextLine(void);
void Lcd_Init(uint8_t Lcd_Line_Count, uint8_t Lcd_Char_Count_PerLine);
void Lcd_GoToLine(uint8_t Lcd_LineNum);
void Lcd_Clear(void);
void Lcd_delay(uint32_t delayms);
void sendByte(char byte);
void Lcd_CmdWrite(char cmd);
void Lcd_DataWrite(char dat);
void Lcd_MX_GPIO_Init(void);
void Lcd_DisplayChar(char Lcd_CharData);
void Lcd_DisplayString(char *ptr_stringPointer_u8);
void LCD_ScrollMessage(uint8_t Lcd_LineNum, char *ptr_msgPointer_u8);
 
#endif
