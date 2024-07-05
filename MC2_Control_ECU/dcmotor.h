 /******************************************************************************
 *
 * Module: DC Motor
 *
 * File Name: dcmotor.h
 *
 * Description: header file for DC Motor driver
 *
 * Author: Hussein El-Shamy
 *
 *******************************************************************************/

#ifndef DCMOTOR_H_
#define DCMOTOR_H_

#include "std_types.h"
#include "gpio.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

#define DCMOTOR_PORT_ID			PORTB_ID
#define DCMOTOR_PIN_IN1_ID		PIN0_ID
#define DCMOTOR_PIN_IN2_ID		PIN1_ID

typedef enum{
	Stop,CW,CCW
}DcMotor_State;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

void DcMotor_Init(void);

void DcMotor_Rotate(DcMotor_State state,uint8 speed);

#endif /* DCMOTOR_H_ */
