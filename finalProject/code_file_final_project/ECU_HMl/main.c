/*
 * main.c
 *
 *  Created on: Apr 21, 2025
 *      Author: Mahmoud Gamal
 */
#include"keypad.h"
#include"LCD_Driver.h"
#include"uart.h"
#include"timer.h"
#include"std_types.h"
#include<avr/io.h>
#include <avr/interrupt.h>
#include<util/delay.h>
#define TICKS_PER_SECOND 7812


void MAINOptions(void);
void send_password(uint8 *pw);
uint8 create_password(void);
void openDoor(void);
void changePassword(void);
void HMI_ECU_lockout(void);
void lockout_timer_callback(void);
void StartLockOutTimer(void);


typedef enum{
	IDLE , UnlockingDoor , Waiting_pepole , LockingDoor , SystemLocked
}system_state;
system_state currentState = IDLE ;
volatile uint8 lockout_seconds = 0;
int main(){
	init();
	UART_sendByte(0xff);
	if(UART_recieveByte()!=0xff){
		LCD_Clear_Command();
		LCD_displayString("Sync Failed");
		while(1);
	}
	create_password();
	while(1){
		if(currentState == IDLE){
			MAINOptions();
			uint8 key = KEYPAD_getPressedKey();
			if(key == '+'){
				openDoor();
			}
			else if(key == '-'){
				changePassword();
			}
		}
	}
}
void MAINOptions(void){
	LCD_Clear_Command();
	LCD_displayString("+ : open door");
	LCD_MOVE_Cursor(1,0);
	LCD_displayString("- : change pass");
}
void send_password(uint8 *pw){
	for(uint8 i = 0 ; i < 5 ; i++ ){
		UART_sendByte(pw[i]);
		_delay_ms(10);
	}
}
void changePassword(void){
	uint8 pw[5], attempts = 0 , response ;
	while(attempts < 3 ){
		LCD_Clear_Command();
		LCD_displayString("Plz enter pass: ");
		LCD_MOVE_Cursor(1,0);
		get_password(pw);
		UART_sendByte('-');
		_delay_ms(10);
		send_password(pw);
		response = UART_recieveByte();
		if(response == 1){
			if(create_password()){
				LCD_Clear_Command();
				LCD_displayString("pass changed");
				_delay_ms(1000);
			}
			currentState = IDLE ;
			MAINOptions();
			return ;
		}
		else{
			attempts++ ;
			LCD_Clear_Command();
			LCD_displayString("Wrong PASS");
			_delay_ms(1000);
		}

	}
	HMI_ECU_lockout();
}

void openDoor(void){
	uint8 pw[5], attempts = 0 , response ;
	while(attempts < 3){
		LCD_Clear_Command();
		LCD_displayString("Plz enter pass: ");
		LCD_MOVE_Cursor(1,0);
		get_password(pw);
		UART_sendByte('+');
		_delay_ms(10);
		send_password(pw);
		response = UART_recieveByte();
		if(response == 1){

			LCD_Clear_Command();
			LCD_displayString("DOOR Unlocking");
			currentState = UnlockingDoor ;
			while(1){
				response = UART_recieveByte();
				if(response == 2){
					LCD_Clear_Command();
					LCD_displayString("wait for people");
					currentState = Waiting_pepole ;

				}
				else if(response == 3){
					LCD_Clear_Command();
					LCD_displayString("Door Locking");
					currentState = LockingDoor ;
				}
				else if (response == 4 ){
					currentState = IDLE ;
					MAINOptions();
					return ;
				}
			}
		}
		else{
			attempts++;
			LCD_Clear_Command();
			LCD_displayString("wrong pass");
			_delay_ms(1000);
		}
	}
	HMI_ECU_lockout();
}
void get_password(uint8 *pw){
	for(uint8 i = 0 ; i < 5 ; i ++){
		uint8 pass ;
		do{
			pass =KEYPAD_getPressedKey();
		}while(pass < '0'||pass > '9');
		pw[i]=pass ;
		LCD_displayCharacter('*');
		_delay_ms(500);
	}
	while(KEYPAD_getPressedKey()!= '#');
}

uint8 create_password(void){
	uint8 pw1[5],pw2[5] , response ;
	while(1){
		LCD_Clear_Command();
		LCD_displayString("Plz enter pass:");
		LCD_MOVE_Cursor(1,0);
		get_password(pw1);
		LCD_Clear_Command();
		LCD_displayString("Plz enter pass:");
		LCD_Display_String_ROW_COL(1,0,"same pass:");
		get_password(pw2);
		UART_sendByte('p');
		_delay_ms(10);
		send_password(pw1);
		send_password(pw2);
		response = UART_recieveByte();
		if(response == 1){
			LCD_Clear_Command();
			LCD_displayString("PASS Saved");
			_delay_ms(1000);
			return 1 ;
		}
		else{
			LCD_Clear_Command();
			LCD_displayString("MIS Match");
			_delay_ms(1000);
		}
	}
}

void HMI_ECU_lockout(void){
	LCD_Clear_Command();
	LCD_displayString("System Locked");
	UART_sendByte('L');
	currentState = SystemLocked;
	StartLockOutTimer();
}

void lockout_timer_callback(void){
	lockout_seconds++;
	if(lockout_seconds >= 60){
		Timer_deInit(TIMER1);
		lockout_seconds = 0;
		currentState = IDLE ;
		LCD_Clear_Command();
		LCD_displayString("Welcome back");
		_delay_ms(1000);
	}
}
void StartLockOutTimer(void){
	 lockout_seconds = 0;
		Timer_ConfigType t1 = {0,TICKS_PER_SECOND,TIMER1,TIMER_PRESCALER_1024,TIMER_MODE_COMPARE};
		Timer_init(&t1);
		Timer_setCallBack(lockout_timer_callback , TIMER1);
}
void init(void){
	SREG   |= (1<<7);
		LCD_init();
		UART_ConfigType UART = {Char_Size_8bit , parity_Disable,STOP_1BIT , 9600};
		UART_init(&UART);
}
