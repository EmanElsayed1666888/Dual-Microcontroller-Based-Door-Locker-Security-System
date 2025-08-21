/*
 * MOTOR.c
 *
 *  Created on: Mar 12, 2025
 *      Author: Dell
 */
#include"MOTOR.h"
#include"gpio.h"
#include"PWM.h"
#include"std_types.h"
#include"common_macros.h"
#include<avr/io.h>
void DcMotor_Init(void){
	GPIO_setupPinDirection(PORTB_ID,PIN0_ID,PIN_OUTPUT);
	GPIO_setupPinDirection(PORTB_ID,PIN1_ID,PIN_OUTPUT);
	GPIO_setupPinDirection(PORTB_ID,PIN3_ID,PIN_OUTPUT);
	GPIO_writePin(PORTB_ID ,PIN0_ID,LOGIC_LOW);
	GPIO_writePin(PORTB_ID ,PIN1_ID,LOGIC_LOW);
}
void DcMotor_Rotate(DcMotor_State state, uint8 speed){

	PWM_Timer0_Start(speed);
	 switch(state){
	 case CW:
			GPIO_writePin(PORTB_ID ,PIN0_ID,LOGIC_HIGH);
			GPIO_writePin(PORTB_ID ,PIN1_ID,LOGIC_LOW);
		 break;
	 case A_CW:
			GPIO_writePin(PORTB_ID ,PIN0_ID,LOGIC_LOW);
			GPIO_writePin(PORTB_ID ,PIN1_ID,LOGIC_HIGH);
		 break;
	 default :
			GPIO_writePin(PORTB_ID ,PIN0_ID,LOGIC_LOW);
			GPIO_writePin(PORTB_ID ,PIN1_ID,LOGIC_LOW);
	 }


}
