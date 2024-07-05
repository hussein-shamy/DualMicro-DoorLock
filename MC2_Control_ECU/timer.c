/*******************************************************************************
 *
 * 	Module: Timer
 *
 * 	File Name: timer.c
 *
 *  Description: SW driver for timer in ATmega32
 *
 *  Author: Hussein El-Shamy
 *
 *******************************************************************************/

/*********************************************************************************
 *                              Functions Prototypes                             *
 ********************************************************************************/

#include "timer.h"
#include "common_macros.h"
#include <avr/io.h>
#include <avr/interrupt.h>

/*********************************************************************************
 *                              Global Variables                                 *
 ********************************************************************************/

/* Global variables to hold the address of the call back function in the application */
static volatile void (*timer1_callBackPtr)(void) = NULL_PTR;

/*********************************************************************************
 *                        Interrupt Service Routine		                         *
 ********************************************************************************/

/* Overflow Mode */

ISR(TIMER1_OVF_vect) {

	if (timer1_callBackPtr != NULL_PTR) {

		timer1_callBackPtr(); /*  Call The Global Pointer to Func   */
	}

}

ISR(TIMER1_COMPA_vect){
	if (timer1_callBackPtr != NULL_PTR) {

		timer1_callBackPtr(); /*  Call The Global Pointer to Func   */
	}

}

/*********************************************************************************
 *                              Functions Definitions                            *
 ********************************************************************************/

void TIMER1_init(const Timer_ConfigType *Config_Ptr) {

	/* non-PWM */
	TCCR1A |= (1 << FOC1A);

	/* Pre-load Value */
	TCNT1 = Config_Ptr->initial_value;

	/* Wave Generation Mode */
	switch (Config_Ptr->mode) {
	case NORMAL_MODE:

		TCCR1A &= 0xFC;
		TCCR1B &= 0xE7;
		/* By default 0xFFFF */
		/* Overflow Interrupt Enable */
		TIMSK |= (1 << TOIE1);
		break;

	case CTC_MODE:

		TCCR1A &= 0xFC;
		TCCR1B = (TCCR1B & 0xE7) | (1 << WGM12);
		/* Output compare value */
		OCR1A = Config_Ptr->compare_value;
		/* CTC_A Interrupt Enable */
		TIMSK |= (1 << OCIE1A);
		break;
	}

	/* CLock select and start counting */
	TCCR1B = (TCCR1B & 0xF8) | (Config_Ptr->prescaler & 0x07);

}


void Timer1_deInit(void) {
	TCCR1B &= 0xF8;
}


void Timer1_setCallBack(void (*a_ptr)(void)) {
	timer1_callBackPtr = a_ptr;
	return;
}
