#include "LCD_20x4.h"


uint8_t Lcd_Track_LineNum=1;         						//Variable to track the line numbers
uint8_t Lcd_Track_CursorPos=0;       						//Variable to track the cursor
uint8_t ARR_LcdLineNumAddress[LCD_NUM_OF_LINES]={0x80,0xc0,0x90,0xd0};	//Address positions of line numbers

///* local function to generate some delay */
//void Lcd_delay(int cnt)
//{
//    int i;
//    for(i=0;i<cnt;i++);
//}

void Lcd_delay(uint32_t delayms)
{
	HAL_Delay(delayms);
}


/* Function send a complete byte on the Data bus */
void sendByte(char byte)
{
    LCD_DATA_BUS_PORT->ODR &=~(LCD_dataBusMask);                  // Clear previous data
    LCD_DATA_BUS_PORT->ODR |= (((byte >>0x00) & 0x01) << LCD_D0);
    LCD_DATA_BUS_PORT->ODR |= (((byte >>0x01) & 0x01) << LCD_D1);
    LCD_DATA_BUS_PORT->ODR |= (((byte >>0x02) & 0x01) << LCD_D2);
    LCD_DATA_BUS_PORT->ODR |= (((byte >>0x03) & 0x01) << LCD_D3);
    LCD_DATA_BUS_PORT->ODR |= (((byte >>0x04) & 0x01) << LCD_D4);
    LCD_DATA_BUS_PORT->ODR |= (((byte >>0x05) & 0x01) << LCD_D5);
    LCD_DATA_BUS_PORT->ODR |= (((byte >>0x06) & 0x01) << LCD_D6);
    LCD_DATA_BUS_PORT->ODR |= (((byte >>0x07) & 0x01) << LCD_D7);
}





/* Function to send the data to LCD */
void Lcd_DataWrite(char dat)
{
    sendByte(dat);                     //Send the data
    LCD_CTRL_BUS_PORT->ODR |= (1<<LCD_RS);  // Send HIGH pulse on RS pin for selecting data register
    LCD_CTRL_BUS_PORT->ODR &= ~(1<<LCD_RW); // Send LOW pulse on RW pin for Write operation
    LCD_CTRL_BUS_PORT->ODR |= (1<<LCD_EN);  // Generate a High-to-low pulse on EN pin
    //Lcd_delay(100);
    Lcd_delay(1);
    LCD_CTRL_BUS_PORT->ODR &= ~(1<<LCD_EN);

    //Lcd_delay(10000);
    Lcd_delay(2);
}

void Lcd_MX_GPIO_Init(void)
{
  GPIO_InitTypeDef Lcd_GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();


  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LCD_CTRL_BUS_PORT, LCD_RS_Pin|LCD_RW_Pin|LCD_EN_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LCD_DATA_BUS_PORT, LCD_D0_Pin|LCD_D1_Pin|LCD_D2_Pin|LCD_D3_Pin
                          |LCD_D4_Pin|LCD_D5_Pin|LCD_D6_Pin|LCD_D7_Pin, GPIO_PIN_RESET);


  /*Configure GPIO pins : LCD_RS_Pin LCD_RW_Pin LCD_EN_Pin */
  Lcd_GPIO_InitStruct.Pin = LCD_RS_Pin|LCD_RW_Pin|LCD_EN_Pin;
  Lcd_GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  Lcd_GPIO_InitStruct.Pull = GPIO_NOPULL;
  Lcd_GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(LCD_CTRL_BUS_PORT, &Lcd_GPIO_InitStruct);

  /*Configure GPIO pins : LCD_D0_Pin LCD_D1_Pin LCD_D2_Pin LCD_D3_Pin
                           LCD_D4_Pin LCD_D5_Pin LCD_D6_Pin LCD_D7_Pin */
  Lcd_GPIO_InitStruct.Pin = LCD_D0_Pin|LCD_D1_Pin|LCD_D2_Pin|LCD_D3_Pin
                          |LCD_D4_Pin|LCD_D5_Pin|LCD_D6_Pin|LCD_D7_Pin;
  Lcd_GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  Lcd_GPIO_InitStruct.Pull = GPIO_NOPULL;
  Lcd_GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(LCD_DATA_BUS_PORT, &Lcd_GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* Function to send the command to LCD */
void Lcd_CmdWrite(char cmd)
{
    sendByte(cmd);                      //Send the Command
    LCD_CTRL_BUS_PORT->ODR &= ~(1<<LCD_RS); // Send LOW pulse on RS pin for selecting Command register
    LCD_CTRL_BUS_PORT->ODR &= ~(1<<LCD_RW); // Send LOW pulse on RW pin for Write operation
    LCD_CTRL_BUS_PORT->ODR |= (1<<LCD_EN);  // Generate a High-to-low pulse on EN pin
    //Lcd_delay(100);
    Lcd_delay(2);
    LCD_CTRL_BUS_PORT->ODR &= ~(1<<LCD_EN);

    //Lcd_delay(10000);
    Lcd_delay(5);
}

void Lcd_Init(uint8_t Lcd_Line_Count, uint8_t Lcd_Char_Count_PerLine)
{
	ARR_LcdLineNumAddress[2] =  0x90 + (Lcd_Char_Count_PerLine & 0x0fu);
	ARR_LcdLineNumAddress[3] =  0xd0 + (Lcd_Char_Count_PerLine & 0x0fu);
	Lcd_CmdWrite(CMD_LCD_EIGHT_BIT_MODE); // Initialize the LCD for 8-bit 5x7 matrix type
    Lcd_CmdWrite(CMD_DISPLAY_ON_CURSOR_ON);     // Display ON cursor ON
    Lcd_Clear();                             // Clear the LCD and go to First line First Position
}

void Lcd_Clear(void)
{
    Lcd_CmdWrite(CMD_LCD_CLEAR);    // Clear the LCD and go to First line First Position
    Lcd_GoToLine(1);
}

void Lcd_GoToLine(uint8_t Lcd_LineNum)
{
	Lcd_Track_CursorPos = 0;
    Lcd_Track_LineNum = Lcd_LineNum;
    Lcd_CmdWrite(ARR_LcdLineNumAddress[Lcd_LineNum-1]);
}

void Lcd_DisplayString(char *ptr_stringPointer_u8)
{
    while((*ptr_stringPointer_u8)!=0)
        Lcd_DisplayChar(*ptr_stringPointer_u8++); // Loop through the string and display char by char
}

void Lcd_DisplayChar(char Lcd_CharData)
{
    if((Lcd_Track_CursorPos>=20) || (Lcd_CharData=='\n'))
    {
        /* If the cursor has reached to end of line on page1
        OR NewLine command is issued Then Move the cursor to next line */
        Lcd_GoToNextLine();
    }
    else			/* Display the character if its not newLine Char */
    {
        Lcd_DataWrite(Lcd_CharData); /* Display the data and keep track of cursor */
        Lcd_Track_CursorPos++;
    }
}

void  Lcd_GoToNextLine()
{
    /*Increment the current line number.
      In case it exceeds the limit, rool it back to first line */
    Lcd_Track_LineNum++;
    Lcd_Track_CursorPos = 0x00;
    if(Lcd_Track_LineNum >= 4)		//roll to 1st line if currently in last line
        Lcd_Track_LineNum = 1;
    Lcd_CmdWrite(ARR_LcdLineNumAddress[Lcd_Track_LineNum-1]);
}

void Lcd_SetCursor(uint8_t Lcd_LineNum, uint8_t Lcd_CursorPos)
{
	Lcd_Track_CursorPos = Lcd_CursorPos;
    Lcd_Track_LineNum = Lcd_LineNum;
    Lcd_CmdWrite(ARR_LcdLineNumAddress[Lcd_LineNum-1]+Lcd_CursorPos);
}


void LCD_ScrollMessage(uint8_t Lcd_LineNum, char *ptr_msgPointer_u8)
{
    unsigned char i,j,z=0;
    unsigned char spacecount=0;


    if(Lcd_LineNum >= LCD_NUM_OF_LINES)
        Lcd_LineNum = 1; // Select first line if the Lcd_LineNum is out of range
    /* Loop to display the complete string,    each time 16 chars are displayed and
            pointer is incremented to point to next char */
    Lcd_GoToLine(Lcd_LineNum);     //Move the Cursor to first line

    Lcd_CmdWrite(CMD_DISPLAY_ON_CURSOR_OFF);             //Disable the Cursor

    for(i=0;ptr_msgPointer_u8[i];i++)
    {
    	Lcd_SetCursor(Lcd_LineNum,0);
        for(j=0;(j<LCD_NUM_OF_CHARS) && (ptr_msgPointer_u8[i+j]);j++)
        {
            //Display first 16 Chars or till Null char is reached
            Lcd_DisplayChar(ptr_msgPointer_u8[i+j]);
        }
        if(i==0)
        {
        	continue;
        }
        else
        {
        	++spacecount;
        }

        while( z<spacecount)
        //while( j<LCD_NUM_OF_CHARS)
        {
            /*If the chars to be scrolled are less than MaxLcdChars,
              then display remaining chars with blank spaces*/
            Lcd_DisplayChar(' ');
            ++z;
        }
        z=0;
        Lcd_delay(LCD_SCROLL_DELAY_MS);
    }
    Lcd_SetCursor(Lcd_LineNum,0);
    Lcd_DisplayChar(' ');
    Lcd_SetCursor(Lcd_LineNum,0);
    Lcd_CmdWrite(CMD_DISPLAY_ON_CURSOR_ON);              // Finally enable the Cursor
}




//WORK IN PROGRESS CODE
//void LCD_ScrollMessage2(uint8_t Lcd_LineNum, char *ptr_msgPointer_u8)
//{
//    unsigned char i,j,z=0,zz=0,spacecount=0;
//    unsigned char sizeofArray=1;
//    unsigned char charcount=0;
//    Lcd_CmdWrite(CMD_DISPLAY_ON_CURSOR_ON);
//    //Lcd_CmdWrite(CMD_DECREMENT_CURSOR);
//    if(Lcd_LineNum >= LCD_NUM_OF_LINES)
//        Lcd_LineNum = 1; // Select first line if the Lcd_LineNum is out of range
//    /* Loop to display the complete string,    each time 16 chars are displayed and
//            pointer is incremented to point to next char */
//    //Lcd_GoToLine(Lcd_LineNum);     //Move the Cursor to first line
//    Lcd_Track_CursorPos=LCD_NUM_OF_CHARS-1;
//    //Finding size of pointer array
//    for (i=0;ptr_msgPointer_u8[i];i++)
//    {
//    	++sizeofArray;
//    }
//    for(i=0;i<sizeofArray+20;i++)
//    {
//			if(charcount>19 && charcount<sizeofArray){
//					char counter=0;
//			    			Lcd_SetCursor(Lcd_LineNum,0);
//			    			for(zz=charcount-LCD_NUM_OF_CHARS+1;(ptr_msgPointer_u8[zz] && counter<=20);zz++)
//			    			{
//			    				Lcd_DisplayChar(ptr_msgPointer_u8[zz]);
//			    				++counter;
//			    			}
//			}
//			else if(charcount >=sizeofArray){
//			Lcd_SetCursor(Lcd_LineNum,0);
//			for(zz=charcount-LCD_NUM_OF_CHARS+1;ptr_msgPointer_u8[zz] ;zz++)
//						    			{
//						    				Lcd_DisplayChar(ptr_msgPointer_u8[zz]);
//
//						    				++spacecount;
//						    			}
//			while( z<spacecount)
//			        //while( j<LCD_NUM_OF_CHARS)
//			        {
//			            /*If the chars to be scrolled are less than MaxLcdChars,
//			              then display remaining chars with blank spaces*/
//			            Lcd_DisplayChar(' ');
//			            ++z;
//			        }
//			        z=0;
//			}
//
//			else{
//				Lcd_SetCursor(Lcd_LineNum,19-charcount);
//				    			for(z=0;(ptr_msgPointer_u8[z] && z<=charcount);z++)
//				    			    		{
//				    			    			Lcd_DisplayChar(ptr_msgPointer_u8[z]);
//				    			    		}
//
//			}
//			++charcount;
//			    		Lcd_delay(LCD_SCROLL_DELAY_MS);
//	}
////    		if (charcount>19 && charcount<sizeofArray)
////    		{
////    			char counter=0;
////    			Lcd_SetCursor(Lcd_LineNum,0);
////    			for(zz=charcount-LCD_NUM_OF_CHARS+1;(ptr_msgPointer_u8[zz] && counter<=20);zz++)
////    			{
////    				Lcd_DisplayChar(ptr_msgPointer_u8[zz]);
////    				++counter;
////    			}
////    		}
////    		else
////    		{
////    			Lcd_SetCursor(Lcd_LineNum,19-charcount);
////    			for(z=0;(ptr_msgPointer_u8[z] && z<=charcount);z++)
////    			    		{
////    			    			Lcd_DisplayChar(ptr_msgPointer_u8[z]);
////    			    		}
////    		}
//
////    		++charcount;
////    		Lcd_delay(LCD_SCROLL_DELAY_MS);
////    	}
//    	//Lcd_Track_CursorPos=Lcd_Track_CursorPos-1;
//    	//Lcd_DisplayChar(ptr_msgPointer_u8[i+j]);
//
//
//    	//Lcd_SetCursor(Lcd_LineNum,19);
////        for(j=0;(j<LCD_NUM_OF_CHARS) && (ptr_msgPointer_u8[i+j]);j++)
////        {
////            //Display first 16 Chars or till Null char is reached
////
////        }
////        if(i==0)
////        {
////        	continue;
////        }
////        else
////        {
////        	++spacecount;
////        }
////
////        while( z<spacecount)
////        //while( j<LCD_NUM_OF_CHARS)
////        {
////            /*If the chars to be scrolled are less than MaxLcdChars,
////              then display remaining chars with blank spaces*/
////            Lcd_DisplayChar(' ');
////            ++z;
////        }
////        z=0;
//        //Lcd_delay(LCD_SCROLL_DELAY_MS);
////    }
//    //Lcd_SetCursor(Lcd_LineNum,0);
//    //Lcd_DisplayChar(' ');
//   // Lcd_SetCursor(Lcd_LineNum,0);
//   // Lcd_CmdWrite(CMD_DISPLAY_ON_CURSOR_ON);              // Finally enable the Cursor
//}