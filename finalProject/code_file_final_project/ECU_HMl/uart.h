 /******************************************************************************
 *
 * Module: UART
 *
 * File Name: uart.h
 *
 * Description: Header file for the UART AVR driver
 *
 * Author: Mohamed Tarek
 *
 *******************************************************************************/

#ifndef UART_H_
#define UART_H_

#include "std_types.h"

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
typedef enum{
	 Char_Size_5bit , Char_Size_6bit , Char_Size_7bit , Char_Size_8bit , Char_Size_9bit = 7
} UART_BitDateType;
typedef enum{
	parity_Disable , parity_reserved , parity_Even , parity_odd
}UART_ParityType;
typedef enum{
	STOP_1BIT , STOP_2BIT
}UART_StopBitType ;
typedef struct{
	UART_BitDateType  bit_data ;
	UART_ParityType   parity;
	UART_StopBitType  stop_bit ;
	uint32 baud_rate;
}UART_ConfigType;

/*
You need to modify the UART_init function implemented in the UART session to take a
Pointer to the configuration structure with type UART_ConfigType.
 */
void UART_init(const UART_ConfigType * Config_Ptr);
/*
 * Description :
 * Functional responsible for send byte to another UART device.
 */
void UART_sendByte(const uint8 data);

/*
 * Description :
 * Functional responsible for receive byte from another UART device.
 */
uint8 UART_recieveByte(void);

/*
 * Description :
 * Send the required string through UART to the other UART device.
 */
void UART_sendString(const uint8 *Str);

/*
 * Description :
 * Receive the required string until the '#' symbol through UART from the other UART device.
 */
void UART_receiveString(uint8 *Str); // Receive until #

#endif /* UART_H_ */
