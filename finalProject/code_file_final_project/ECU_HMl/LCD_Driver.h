/*
 * Module: LCD_driver Driver
 *
 * File Name: LCD_driver.h
 *
 * Created on: 20 Mar 2025
 *
 * Author: Mahmoud Gamal Mohamed
 */
#ifndef LCD_DRIVER_H_
#define LCD_DRIVER_H_
#include "std_types.h"
#define LCD_NUM_BIT_MODE    8
#if((LCD_NUM_BIT_MODE != 4) && (LCD_NUM_BIT_MODE !=8))
#error "Number of Data bits should be equal to 4 or 8"
#endif

/*LCD PORTS AND PINS ID*/
#define PORT_ID_RS           PORTC_ID
#define PIN_ID_RS            PIN0_ID
#define PORT_ID_Enable       PORTC_ID
#define PIN_ID_Enable	     PIN1_ID
#define PORT_DATA_ID         PORTA_ID
#if(LCD_NUM_BIT_MODE == 4 )
#define LCD_DATA_D4          PIN3_ID
#define LCD_DATA_D5	         PIN4_ID
#define LCD_DATA_D6          PIN5_ID
#define LCD_DATA_D7          PIN6_ID
#endif
/*LCD Command*/
#define LCD_Clear                            0x01
#define LCD_GO_TO_Home                       0x02
#define LCD_TWO_LINE_EIGHT_PIN_MODE          0X38
#define LCD_TWO_LINE_FOUR_PIN_MODE           0X28
#define LCD_TWO_LINES_FOUR_BITS_MODE_INIT1   0x33
#define LCD_TWO_LINES_FOUR_BITS_MODE_INIT2   0x32
#define LCD_Cursor_ON                        0x0E
#define LCD_Cursor_OFF                       0X0C
#define LCD_Cursor_location                  0X80

void LCD_sendCommand(uint8 command);
void LCD_displayCharacter(uint8 DATA);
void LCD_init(void);
void LCD_displayString(uint8*string);
void LCD_MOVE_Cursor(uint8 ROW , uint8 COL);
void LCD_Display_String_ROW_COL(uint8 ROW , uint8 COL , char *Str);
void LCD_Clear_Command(void);
void LCD_Intger_TO_String(int data_num);


#endif /* LCD_DRIVER_H_ */
