/*
 * PIR_Driver.c
 *
 *  Created on: Apr 8, 2025
 *      Author: Rivo
 */
#include"PIR_Driver.h"
#include"common_macros.h"
#include"gpio.h"
void PIR_init(void){
	/*▪ Description: Function to initialize the PIR driver
	▪ Inputs: None
	▪ Return: None
	*/
	GPIO_setupPinDirection(PIR_PORT_ID , PIR_PIN_ID , PIN_INPUT);
	GPIO_writePin(PIR_PORT_ID,PIR_PIN_ID,LOGIC_HIGH);
}
uint8 PIR_getState(void){
	/*▪ Description: Function to return PIR State
	  ▪ Inputs: None
      ▪ Return: uint8 */
	return GPIO_readPin(PIR_PORT_ID , PIR_PIN_ID ) ;
}
