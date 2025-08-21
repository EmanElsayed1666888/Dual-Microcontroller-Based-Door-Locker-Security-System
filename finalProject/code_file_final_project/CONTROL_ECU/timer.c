/*
 * timer.c
 *
 *  Created on: Apr 21, 2025
 *      Author: Rivo
 */
#include "timer.h"
#include "common_macros.h"
#include <avr/io.h>
#include "gpio.h"
#include <avr/interrupt.h>

static volatile void (*g_callBackPtr_T0)(void) = NULL_PTR;
static volatile void (*g_callBackPtr_T1)(void) = NULL_PTR;
static volatile void (*g_callBackPtr_T2)(void) = NULL_PTR;
/**********************************************************************TIMER0*************************************************************/
ISR(TIMER0_COMP_vect)
{
	if(g_callBackPtr_T0 != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_callBackPtr_T0)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}

ISR(TIMER1_COMP_vect)
{
	if(g_callBackPtr_T1 != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_callBackPtr_T1)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}
ISR(TIMER2_COMP_vect)
{
	if(g_callBackPtr_T2 != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_callBackPtr_T2)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}
void Timer_init(const Timer_ConfigType * Config_Ptr){
	if(Config_Ptr == NULL_PTR)return ;
	if(Config_Ptr->timer_ID == TIMER0){
		TCNT0 = Config_Ptr->timer_InitialValue ;
		if(Config_Ptr->timer_mode == TIMER_MODE_COMPARE){
			TCCR0 |= (1 << WGM01);
			OCR0 = (uint8)(Config_Ptr->timer_compare_MatchValue);
			TIMSK |=(1<<OCIE0);
		}
	}
	else if(Config_Ptr->timer_mode == TIMER1){
		TCNT1 = Config_Ptr->timer_InitialValue;
		if(Config_Ptr->timer_mode == TIMER_MODE_COMPARE){
			TCCR1B |=(1<<WGM12);
			OCR1A = Config_Ptr->timer_compare_MatchValue ;
			TIMSK |=(1<<OCIE1A);
		}

	}
	else if(Config_Ptr->timer_mode == TIMER2){
		TCNT2 = Config_Ptr->timer_InitialValue ;
		if(Config_Ptr->timer_mode ==TIMER_MODE_COMPARE){
			TCCR2 |=(1<<WGM21);
			OCR2 = (uint8)(Config_Ptr->timer_compare_MatchValue);
			TIMSK |=(1<<OCIE2);
		}
	}
	switch(Config_Ptr->timer_ID){
	case TIMER0 :
		switch(Config_Ptr->timer_clock){
		case NO_PRESCALER :
			TCCR0 |=(1<<CS00);
			break ;
		case TIMER_PRESCALER_8:
			TCCR0 |=(CS01);
			break;
		case TIMER_PRESCALER_64:
			TCCR0 |=(1<<CS01)|(1<<CS00);
			break ;
		case TIMER_PRESCALER_256:
			TCCR0 |=(1<<CS02);
			break ;
		case TIMER_PRESCALER_1024 :
			TCCR0 |=(1<<CS02)|(1<<CS00);
			break;
		}
		break ;
	case TIMER1 :
		switch(Config_Ptr->timer_clock){
		case NO_PRESCALER :
			TCCR1B |=(1<<CS10);
			break ;
		case TIMER_PRESCALER_8:
			TCCR1B |=(CS11);
			break;
		case TIMER_PRESCALER_64:
			TCCR1B |=(1<<CS11)|(1<<CS10);
			break ;
		case TIMER_PRESCALER_256:
			TCCR1B |=(1<<CS12);
			break ;
		case TIMER_PRESCALER_1024 :
			TCCR1B |=(1<<CS12)|(1<<CS10);
			break;
		}
		break ;
	case TIMER2 :
		switch(Config_Ptr->timer_clock){
		case NO_PRESCALER :
			TCCR2 |=(1<<CS20);
			break ;
		case TIMER_PRESCALER_8:
			TCCR2 |=(CS21);
			break;
		case TIMER_PRESCALER_64:
			TCCR2 |=(1<<CS21)|(1<<CS20);
			break ;
		case TIMER_PRESCALER_256:
			TCCR2 |=(1<<CS22);
			break ;
		case TIMER_PRESCALER_1024 :
			TCCR2 |=(1<<CS22)|(1<<CS20);
			break;
		}
		break ;
	}
}
void Timer_deInit(Timer_ID timer_type){
	if(timer_type == TIMER0 ){
		TCCR0 = 0 ;
		OCR0  = 0 ;
		TCNT0 = 0 ;
		CLEAR_BIT(TIMSK,OCIE0);
		CLEAR_BIT(TIMSK,TOIE0);
	}
	else if(timer_type == TIMER1){
		TCCR1A = 0 ;
		TCCR1B = 0 ;
		TCNT1 =  0 ;
		OCR1A = 0 ;
		OCR1B = 0 ;
		CLEAR_BIT(TIMSK,TICIE1);
		CLEAR_BIT(TIMSK,OCIE1A);
		CLEAR_BIT(TIMSK,OCIE1B);
		CLEAR_BIT(TIMSK,TOIE1);
	}
	else if(timer_type == TIMER2){
		TCCR2 = 0 ;
		OCR2  = 0 ;
		TCNT2 = 0 ;
		CLEAR_BIT(TIMSK,OCIE2);
		CLEAR_BIT(TIMSK,TOIE2);
	}

}

void Timer_setCallBack(void(*a_ptr)(void), Timer_ID a_timer_ID ){
	if(a_timer_ID == TIMER0){
		g_callBackPtr_T0 = a_ptr;
	}
	else if(a_timer_ID == TIMER1){
		g_callBackPtr_T1 = a_ptr;
	}
	else if(a_timer_ID == TIMER2){
		g_callBackPtr_T2 = a_ptr;
	}
}
