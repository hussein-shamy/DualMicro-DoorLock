 /******************************************************************************
 *
 * Module: DC Motor
 *
 * File Name: dcmotor.c
 *
 * Description: header file for DC Motor driver
 *
 * Author: Hussein El-Shamy
 *
 *******************************************************************************/
#include "dcmotor.h"
#include "gpio.h"
#include "pwm_timer0.h"

void DcMotor_Init(void){

	GPIO_setupPinDirection(DCMOTOR_PORT_ID,DCMOTOR_PIN_IN1_ID,PIN_OUTPUT);
	GPIO_setupPinDirection(DCMOTOR_PORT_ID,DCMOTOR_PIN_IN2_ID,PIN_OUTPUT);

	GPIO_writePin(DCMOTOR_PORT_ID,DCMOTOR_PIN_IN1_ID,LOGIC_LOW);
	GPIO_writePin(DCMOTOR_PORT_ID,DCMOTOR_PIN_IN2_ID,LOGIC_LOW);
}


void DcMotor_Rotate(DcMotor_State state,uint8 speed){


	switch (state) {
	case Stop:
		GPIO_writePin(DCMOTOR_PORT_ID,DCMOTOR_PIN_IN1_ID,LOGIC_LOW);
		GPIO_writePin(DCMOTOR_PORT_ID,DCMOTOR_PIN_IN2_ID,LOGIC_LOW);
		break;
	case CW:
		GPIO_writePin(DCMOTOR_PORT_ID,DCMOTOR_PIN_IN1_ID,LOGIC_LOW);
		GPIO_writePin(DCMOTOR_PORT_ID,DCMOTOR_PIN_IN2_ID,LOGIC_HIGH);
		break;
	case CCW:
		GPIO_writePin(DCMOTOR_PORT_ID,DCMOTOR_PIN_IN1_ID,LOGIC_HIGH);
		GPIO_writePin(DCMOTOR_PORT_ID,DCMOTOR_PIN_IN2_ID,LOGIC_LOW);
		break;
}

	PWM_Timer0_Start(speed);
}
