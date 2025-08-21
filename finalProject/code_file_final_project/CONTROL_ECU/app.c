/*
 * app.c
 *
 *  Created on: Apr 21, 2025
 *      Author: Mahmoud Gamal
 */
#include"Motor.h"
#include"Buzzer.h"
#include"gpio.h"
#include"twi.h"
#include"uart.h"
#include"std_types.h"
#include"timer.h"
#include"PIR_Driver.h"
#include"external_eeprom.h"
#include"PWM.h"
#include<util/delay.h>
#include<avr/io.h>
#define TICKS_PER_SECOND 7812
typedef enum {
	IDLE , OPENDOOR , WAITPIR , CLOSEDOOR , ALARM
}SystemState;
/**********************************************************/
uint8 savepassword(uint8 *pw);
uint8 receivePass_CTRL_ECU(uint8 *pw);
uint8 compare_pass(uint8 *pw1 , uint8 *pw2);
uint8 verify_password(uint8 *pw);
void timer1_callback(void);
void handleDoor(void);
void handle_Alarm(void);
void init(void);
/**********************************************************/
SystemState state = IDLE ;
uint8 WRONG_Attempts=0;
uint8 PIR_wait = 0 ;
uint8 counter_second = 0 ;

int main(){
	init();
	DDRC |=(1<<3);
	if(UART_recieveByte() == 0xff){
		UART_sendByte(0xff);
	}
	else{
		while(1);
	}
	uint8 pw1[5],pw2[5],cmd;

	while(1){
		if (state == WAITPIR && PIR_wait && !PIR_getState() ){
			PIR_wait = 0 ;
			_delay_ms(1000);
			UART_sendByte(3);
			DcMotor_Rotate(A_CW , 100);
			state = CLOSEDOOR ;
			counter_second = 0 ;
			Timer_ConfigType t1 = {0,TICKS_PER_SECOND,TIMER1,TIMER_PRESCALER_1024,TIMER_MODE_COMPARE};
			Timer_init(&t1);
			Timer_setCallBack(timer1_callback , TIMER1);
		}
		if(state == IDLE){
			cmd = UART_recieveByte();
			switch(cmd){
			case 'P':
			receivePass_CTRL_ECU(pw1);
			receivePass_CTRL_ECU(pw2);
			UART_sendByte(compare_pass(pw1,pw2)&&savepassword(pw1));
			break ;
			case '+':
			case '-':
				receivePass_CTRL_ECU(pw1);
				if(receivePass_CTRL_ECU(pw1)){
					UART_sendByte(1);
					WRONG_Attempts = 0 ;
					if(cmd == '+') handleDoor();
				}	else{
					UART_sendByte(0);
					WRONG_Attempts++;
					if(WRONG_Attempts >= 3)handle_Alarm();
				}
				break ;
			default:
				UART_sendByte(0);
				break ;
			}

		}
	}
}
void init(void){
	SREG   |= (1<<7);
	DcMotor_Init();
	Buzzer_init();
	PIR_init();
	TWI_ConfigType twiConfig={0x50,TWI_BIT_RATE_100K};
	TWI_init(&twiConfig);
	UART_ConfigType UART = {Char_Size_8bit , parity_Disable,STOP_1BIT , 9600};
			UART_init(&UART);
}
uint8 savepassword(uint8 *pw){
	for(uint8 i = 0 ; i < 5 ; i++){
		if(EEPROM_writeByte(0x0000+i,pw[i]) != SUCCESS ){
			return 0 ;
		}
		_delay_ms(10);
	}
	return 1 ;
}

uint8 receivePass_CTRL_ECU(uint8 *pw){
	for(uint8 i =0 ; i < 5 ; i++){
		pw[i]=UART_recieveByte();
		_delay_ms(10);
	}
	return 1 ;
}
uint8 compare_pass(uint8 *pw1 , uint8 *pw2){
	for(uint8 i = 0 ; i < 5 ; i++){
		if(pw1[i]!= pw2[i]) return 0 ;
	}
	return 1 ;
}
uint8 verify_password(uint8 *pw){
	uint8 store[5];
	for(uint8 i = 0 ; i< 5  ; i++){
		if(EEPROM_readByte(0x0000+i,&store[i])!= SUCCESS) return 0 ;
		_delay_ms(10);
	}
	return compare_pass(pw , store) ;
}
void timer1_callback(void){
	counter_second++;
	if(state == OPENDOOR &&counter_second >=15 ){
		Timer_deInit(TIMER1);
		counter_second = 0 ;
		DcMotor_Rotate(STOP,0);
		if(PIR_getState()){
			UART_sendByte(2);
			state = WAITPIR ;
			PIR_wait = 1 ;
		}
		else {
			UART_sendByte(3);
			DcMotor_Rotate(A_CW , 100);
			state = CLOSEDOOR ;
			Timer_ConfigType t1 = {0,TICKS_PER_SECOND,TIMER1,TIMER_PRESCALER_1024,TIMER_MODE_COMPARE};
			Timer_init(&t1);
			Timer_setCallBack(timer1_callback , TIMER1);
		}
	}
	else if(state == CLOSEDOOR && counter_second >=15){
		Timer_deInit(TIMER1);
		counter_second = 0 ;
		DcMotor_Rotate(STOP , 0 );
		UART_sendByte(4);
		state = IDLE ;
	}
	else if(state == ALARM && counter_second >= 60){
		Buzzer_off();
		Timer_deInit(TIMER1);
		counter_second = 0 ;
		PORTC |= (1 << 3);
		WRONG_Attempts = 0 ;
		state = IDLE ;
		UART_sendByte(5);
	}
}

void handleDoor(void){
	UART_sendByte(1);
	state = OPENDOOR;
	counter_second = 0 ;
	DcMotor_Rotate(CW , 100);
	Timer_ConfigType t1 = {0,TICKS_PER_SECOND,TIMER1,TIMER_PRESCALER_1024,TIMER_MODE_COMPARE};
	Timer_init(&t1);
	Timer_setCallBack(timer1_callback , TIMER1);
}
void handle_Alarm(void){
	state = ALARM ;
		counter_second = 0 ;
		Buzzer_on();
		Timer_ConfigType t1 = {0,TICKS_PER_SECOND,TIMER1,TIMER_PRESCALER_1024,TIMER_MODE_COMPARE};
		Timer_init(&t1);
		Timer_setCallBack(timer1_callback , TIMER1);

}


