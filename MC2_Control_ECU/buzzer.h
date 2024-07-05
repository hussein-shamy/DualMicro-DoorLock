/*******************************************************************************
 *
 * 	Module: Buzzer
 *
 * 	File Name: buzzer.h
 *
 *	Layer: HAL
 *
 *  Description:
 *
 *  Author: Hussein El-Shamy
 *
 *******************************************************************************/

#ifndef BUZZER_H_
#define BUZZER_H_

/********************************************************************************
*						INCLUDING NECESSARY HEADER FILES						*
********************************************************************************/

#include "std_Types.h"
#include "gpio.h"

/********************************************************************************
*						DEFINTIONS and STATIC CONFIGURATION 					*
********************************************************************************/

/* Configuration for the #1 Seven Segment Unit */
#define BUZZER_PORT_ID			PORTC_ID
#define BUZZER_PIN_ID			PIN5_ID

/*********************************************************************************
 *                              Functions Prototypes                             *
 ********************************************************************************/

void BUZZER_init(void);

void BUZZER_off(void);

void BUZZER_on(void);


#endif /* BUZZER_H_ */
