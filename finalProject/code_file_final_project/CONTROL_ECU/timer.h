/*
 * timer.h
 *
 *  Created on: Apr 21, 2025
 *      Author: Rivo
 */

#ifndef TIMER_H_
#define TIMER_H_
#include"std_types.h"
/******************************************************TIMER_0**********************************************************************/
#define Enable_OC0_PORT_ID PORTB_ID
#define Enable_OC0_PIN_ID  PIN3_ID
typedef enum {
    TIMER0,
    TIMER1,
    TIMER2
} Timer_ID;

typedef enum {
    NO_PRESCALER,
    TIMER_PRESCALER_8,
    TIMER_PRESCALER_64,
    TIMER_PRESCALER_256,
    TIMER_PRESCALER_1024
} Timer_Clock;

typedef enum {
    TIMER_MODE_NORMAL,
    TIMER_MODE_COMPARE
} Timer_Mode;

typedef struct {
    uint16 timer_InitialValue;
    uint16 timer_compare_MatchValue;
    Timer_ID timer_ID;
    Timer_Clock timer_clock;
    Timer_Mode timer_mode;
} Timer_ConfigType;

void Timer_init(const Timer_ConfigType *Config_Ptr);
void Timer_deInit(Timer_ID timer_id);
void Timer_setCallBack(void (*a_ptr)(void), Timer_ID timer_id);

#endif /* TIMER_H_ */
