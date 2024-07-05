/*******************************************************************************
 *
 * 	Module: Buzzer
 *
 * 	File Name: buzzer.c
 *
 *	Layer: HAL
 *
 *  Description: need some fixes
 *
 *  Author: Hussein El-Shamy
 *
 *******************************************************************************/

#include "buzzer.h"
#include "gpio.h"

void BUZZER_init(void){
	GPIO_setupPinDirection(BUZZER_PORT_ID,BUZZER_PIN_ID,PIN_OUTPUT);
	GPIO_writePin(BUZZER_PORT_ID,BUZZER_PIN_ID,LOGIC_LOW);
}

void BUZZER_off(void){
	GPIO_writePin(BUZZER_PORT_ID,BUZZER_PIN_ID,LOGIC_LOW);
}

void BUZZER_on(void){
	GPIO_writePin(BUZZER_PORT_ID,BUZZER_PIN_ID,LOGIC_HIGH);
}
