/*******************************************************************************
 *
 * 	Module: Timer
 *
 * 	File Name: timer.h
 *
 *  Description: SW driver for timer in ATmega32
 *
 *  Author: Hussein El-Shamy
 *
 *******************************************************************************/


#ifndef TIMER_H_
#define TIMER_H_

/********************************************************************************
 *						INCLUDING NECESSARY HEADER FILES						*
 ********************************************************************************/

#include "std_Types.h"
#include "common_macros.h"
#include "gpio.h"

/********************************************************************************
 *						DEFINTIONS and STATIC CONFIGURATION 					*
 ********************************************************************************/



/********************************************************************************
 *								 TYPE DEFINITON 								*
 ********************************************************************************/

typedef enum
{
	NO_CLOCK,F_CPU_CLOCK,F_CPU_8,F_CPU_64,F_CPU_256,F_CPU_1024,EXTERNAL_FALLING,EXTERNAL_RISING
}Timer1_Prescaler;

typedef enum
{
	NORMAL_MODE,
	CTC_MODE=4,
}Timer1_Mode;

typedef struct
{
	uint16 initial_value;
	uint16 compare_value ;
	Timer1_Prescaler prescaler;
	Timer1_Mode mode;
}Timer_ConfigType;


/*********************************************************************************
 *                              Functions Prototypes                             *
 ********************************************************************************/

void TIMER1_init(const Timer_ConfigType * Config_Ptr);

void Timer1_deInit(void);

void Timer1_setCallBack(void(*a_ptr)(void));


#endif /* TIMER_H_ */
