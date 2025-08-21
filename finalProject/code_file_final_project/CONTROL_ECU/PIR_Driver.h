/*
 * PIR_Driver.h
 *
 *  Created on: Apr 8, 2025
 *      Author: Rivo
 */

#ifndef PIR_DRIVER_H_
#define PIR_DRIVER_H_
#include"std_types.h"
#include "gpio.h"
#define PIR_PORT_ID PORTC_ID
#define PIR_PIN_ID  PIN2_ID
void PIR_init(void);
uint8 PIR_getState(void);
#endif /* PIR_DRIVER_H_ */
