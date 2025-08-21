/*
 * Module: LCD_driver Driver
 *
 * File Name: LCD_driver.c
 *
 * Created on: 20 Mar 2025
 *
 * Author: Mahmoud Gamal Mohamed
 */
#include<util/delay.h>
#include"common_macros.h"
#include"gpio.h"
#include "LCD_Driver.h"
void LCD_sendCommand(uint8 command) {
	GPIO_writePin(PORT_ID_RS, PIN_ID_RS, LOGIC_LOW);
	_delay_ms(1);
	GPIO_writePin(PORT_ID_Enable, PIN_ID_Enable, LOGIC_HIGH);
	_delay_ms(1);
#if(LCD_NUM_BIT_MODE == 4)
	GPIO_writePin(PORT_DATA_ID, LCD_DATA_D4,GETBIT(command , 4));
	GPIO_writePin(PORT_DATA_ID, LCD_DATA_D5,GETBIT(command , 5));
	GPIO_writePin(PORT_DATA_ID, LCD_DATA_D6,GETBIT(command , 6));
	GPIO_writePin(PORT_DATA_ID, LCD_DATA_D7,GETBIT(command , 7));
	_delay_ms(1);
	GPIO_writePin(PORT_ID_Enable, PIN_ID_Enable, LOGIC_LOW);
	_delay_ms(1);
	GPIO_writePin(PORT_ID_Enable, PIN_ID_Enable, LOGIC_HIGH);
	_delay_ms(1);
	GPIO_writePin(PORT_DATA_ID, LCD_DATA_D4,GETBIT(command , 0));
	GPIO_writePin(PORT_DATA_ID, LCD_DATA_D5,GETBIT(command , 1));
	GPIO_writePin(PORT_DATA_ID, LCD_DATA_D6,GETBIT(command , 2));
	GPIO_writePin(PORT_DATA_ID, LCD_DATA_D7,GETBIT(command , 3));
	_delay_ms(1);
	GPIO_writePin(PORT_ID_Enable, PIN_ID_Enable, LOGIC_LOW);
	_delay_ms(1);

#elif(LCD_NUM_BIT_MODE == 8)
	GPIO_writePort( PORT_DATA_ID, command);
	_delay_ms(1);
	GPIO_writePin(PORT_ID_Enable, PIN_ID_Enable, LOGIC_LOW);
	_delay_ms(1);
#endif
}
void LCD_displayCharacter(uint8 DATA) {
	GPIO_writePin(PORT_ID_RS, PIN_ID_RS, LOGIC_HIGH);
	_delay_ms(1);
	GPIO_writePin(PORT_ID_Enable, PIN_ID_Enable, LOGIC_HIGH);
	_delay_ms(1);
#if(LCD_NUM_BIT_MODE == 4)
	GPIO_writePin(PORT_DATA_ID, LCD_DATA_D4,GETBIT(DATA , 4));
	GPIO_writePin(PORT_DATA_ID, LCD_DATA_D5,GETBIT(DATA , 5));
	GPIO_writePin(PORT_DATA_ID, LCD_DATA_D6,GETBIT(DATA, 6));
	GPIO_writePin(PORT_DATA_ID, LCD_DATA_D7,GETBIT(DATA , 7));
	_delay_ms(1);
	GPIO_writePin(PORT_ID_Enable, PIN_ID_Enable, LOGIC_LOW);
	_delay_ms(1);
	GPIO_writePin(PORT_ID_Enable, PIN_ID_Enable, LOGIC_HIGH);
	_delay_ms(1);
	GPIO_writePin(PORT_DATA_ID, LCD_DATA_D4,GETBIT(DATA , 0));
	GPIO_writePin(PORT_DATA_ID, LCD_DATA_D5,GETBIT(DATA , 1));
	GPIO_writePin(PORT_DATA_ID, LCD_DATA_D6,GETBIT(DATA , 2));
	GPIO_writePin(PORT_DATA_ID, LCD_DATA_D7,GETBIT(DATA , 3));
	_delay_ms(1);
	GPIO_writePin(PORT_ID_Enable, PIN_ID_Enable, LOGIC_LOW);
	_delay_ms(1);
#elif(LCD_NUM_BIT_MODE == 8)
	GPIO_writePort( PORT_DATA_ID, DATA);
	_delay_ms(1);
	GPIO_writePin(PORT_ID_Enable, PIN_ID_Enable, LOGIC_LOW);
	_delay_ms(1);
#endif
}
void LCD_init(void) {
	GPIO_setupPinDirection(PORT_ID_RS, PIN_ID_RS, PIN_OUTPUT);
	GPIO_setupPinDirection(PORT_ID_Enable, PIN_ID_Enable, PIN_OUTPUT);
#if(LCD_NUM_BIT_MODE == 4)
	GPIO_setupPinDirection(PORT_DATA_ID , LCD_DATA_D4 , PIN_OUTPUT);
	GPIO_setupPinDirection(PORT_DATA_ID , LCD_DATA_D5 , PIN_OUTPUT);
	GPIO_setupPinDirection(PORT_DATA_ID , LCD_DATA_D6 , PIN_OUTPUT);
	GPIO_setupPinDirection(PORT_DATA_ID , LCD_DATA_D7 , PIN_OUTPUT);
	LCD_sendCommand(LCD_TWO_LINES_FOUR_BITS_MODE_INIT1 );
	LCD_sendCommand(LCD_TWO_LINES_FOUR_BITS_MODE_INIT2 );
	LCD_sendCommand(LCD_TWO_LINE_FOUR_PIN_MODE);
#elif(LCD_NUM_BIT_MODE == 8)
	GPIO_setupPortDirection(PORT_DATA_ID, PORT_OUTPUT);
	_delay_ms(20);
	LCD_sendCommand(LCD_TWO_LINE_EIGHT_PIN_MODE);// 4BIT Mode 2 line
#endif
	LCD_sendCommand(LCD_Cursor_OFF);// Cursor Off
	LCD_sendCommand(LCD_Clear);//Clear Display
}
void LCD_displayString(uint8 *string) {
	uint8 i = 0 ;
	while (string[i] != '\0') {
		LCD_displayCharacter(string[i]);
		i++;
	}

}
void LCD_MOVE_Cursor(uint8 ROW , uint8 COL){
	uint8 lcd_memory_address ;
	switch(ROW){
	case 0 :
		lcd_memory_address = COL ;
		break ;
	case 1:
		lcd_memory_address=COL+0x40;
		break ;
	case 2:
		lcd_memory_address=COL+0x10;
		break ;
	case 3:
		lcd_memory_address=COL+0x50;
		break ;
	}
	LCD_sendCommand(lcd_memory_address |LCD_Cursor_location );
}
void LCD_Display_String_ROW_COL(uint8 ROW , uint8 COL , char *Str){
	LCD_MOVE_Cursor(ROW , COL);
	LCD_displayString(Str);
}
void LCD_Clear_Command(void){
	LCD_sendCommand(LCD_Clear);
}
void LCD_Intger_TO_String(int data_num){
char buff[16];
itoa(data_num , buff , 10 );// 10 is base
LCD_displayString(buff);
}
