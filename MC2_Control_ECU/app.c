/******************************************************************************
 *
 * Module: Application
 *
 * File Name: app.c
 *
 * Description: application code
 *
 * Author: Hussein El-Shamy
 *
 *******************************************************************************/

/*******************************************************************************
 INCLUDING NECESSARY HEADER FILES
 ********************************************************************************/

#include <util/delay.h>
#include "std_types.h"
#include "app.h"
#include "uart.h"
#include "dcmotor.h"
#include "buzzer.h"
#include "timer.h"
#include "external_eeprom.h"
#include "string.h"
#include "twi.h"

/*******************************************************************************
 CALL-BACK FUNCTIONS
 ********************************************************************************/

/**
 * @brief Timer callback to control the DC motor rotation during different states.
 *
 * This function is a timer callback that controls the rotation of a DC motor during
 *  different states (HOLDING_STATE, LOCKING_STATE, END_STATE).
 */

static void APP_timerControlMotor(void) {
	static uint8 tickThreeSeconds = 0;
	tickThreeSeconds++;

	switch (tickThreeSeconds) {
	case HOLDING_TIME:
		DcMotor_Rotate(Stop, 50);
		break;
	case LOCKING_TIME:
		DcMotor_Rotate(CCW, 50);
		break;
	case END_TIME:
		DcMotor_Rotate(Stop, 50);
		Timer1_deInit();
		tickThreeSeconds = 0;
		break;
	}
}

/**
 * @brief Timer callback to control error state, turning off the buzzer.
 *
 * This function is a timer callback that controls the error state
 * and turns off the buzzer after a specific time period (WARNING_TIME).
 */

static void APP_timerControlErrorState(void) {
	static uint8 tickThreeSeconds = 0;
	tickThreeSeconds++;
	if (tickThreeSeconds == WARNING_TIME) {
		BUZZER_off();
		Timer1_deInit();
		tickThreeSeconds = 0;
	}
}

/*******************************************************************************
 FUNCTIONS DEFINITION
 ********************************************************************************/

/**
 * @brief Initialize the application components.
 *
 * This function initializes the UART communication, TWI (I2C) communication, DC motor, and buzzer components.
 * It configures UART and TWI settings and initializes these peripherals.
 * It also performs the necessary initialization for the DC motor and buzzer.
 */

void APP_init(void) {
	UART_ConfigType UART_Config_Data = { bit_8, Enabled_Even, bit_1, 9600 };
	UART_init(&UART_Config_Data);
	TWI_ConfigType TWI_Config_Data = { 400000, 1 };
	TWI_init(&TWI_Config_Data);
	DcMotor_Init();
	BUZZER_init();
}
/**
 * @brief Save a password in EEPROM.
 *
 * This function receives a password through UART, verifies the confirmation,
 * and writes the password to the EEPROM memory.
 * It expects to receive a password of length PASSWORD_LENGTH via UART communication.
 * Upon receiving the complete password, it stores the password in EEPROM memory.
 * If the received passwords match, it sends an acknowledgment (SUCCESS) to the HMI microcontroller.
 * If the passwords don't match, it sends a failure code (FAILED) to the HMI microcontroller.
 *
 *	[UPDATE]: Instead of receiving the one password after checking for
 *	the similarity the save in EEPROM [NOW] we receive the two entered passwords
 *	and checking that are equal if they are equal we will send SUCSSES byte and save
 *	in EEPROM if not the function will send FAILED byte via UART
 *
 */

void APP_savePassword(void) {
	uint8_t i = 0;
	uint8_t rxFirstPassword[PASSWORD_LENGTH] = { 0 };
	uint8_t rxSecondPassword[PASSWORD_LENGTH] = { 0 };
	uint8_t passwordMatch = SUCCESS;

	// Notify HMI microcontroller that the password saving process is starting
	UART_sendByte(ACKNOWLEDGEMENT);

	// Receive the first part of the password
	for (i = 0; i < PASSWORD_LENGTH; i++) {
		rxFirstPassword[i] = UART_recieveByte();
	}

	// Receive the second part of the password
	for (i = 0; i < PASSWORD_LENGTH; i++) {
		rxSecondPassword[i] = UART_recieveByte();
	}

	// Compare the two entered passwords
	for (i = 0; i < PASSWORD_LENGTH; i++) {
		if (rxFirstPassword[i] != rxSecondPassword[i]) {
			passwordMatch = FAILED;
			break;
		}
	}

	// Process based on the password matching result
	if (passwordMatch == SUCCESS) {
		// Send an acknowledgment of successful password storage
		UART_sendByte(SUCCESS);

		// Write the password to EEPROM
		for (uint8_t i = 0; i < PASSWORD_LENGTH; i++) {
			EEPROM_writeByte(EEPROM_START_ADDRESS + i, rxFirstPassword[i]);
			_delay_ms(10); // Delay to ensure EEPROM write completion
		}
	} else if (passwordMatch == FAILED) {
		// Send a failure code to indicate password mismatch
		UART_sendByte(FAILED);
	}
}

/**
 * @brief Check a received password against a stored password in EEPROM.
 *
 * This function receives a password through UART and compares it to a stored password in EEPROM.
 * It expects to receive a password of length PASSWORD_LENGTH via UART communication.
 * The function reads the stored password from EEPROM and compares each character.
 * If the received password matches the stored password, it sends a SUCCESS response via UART.
 * If the passwords do not match, it sends a FAILED response via UART.
 *
 * @return The function returns SUCCESS if the passwords match, and FAILED if they do not.
 */

uint8 APP_checkPassword(void) {

	uint8 receivedPassword[PASSWORD_LENGTH] = { 0 };
	uint8 storedPassword[PASSWORD_LENGTH] = { 0 };
	uint8 passwordMatch = SUCCESS;

	for (uint8 i = 0; i < PASSWORD_LENGTH; i++) {
		receivedPassword[i] = UART_recieveByte();
	}

	for (uint8 i = 0; i < PASSWORD_LENGTH; i++) {
		EEPROM_readByte(EEPROM_START_ADDRESS + i, &storedPassword[i]);
	}

	for (uint8 i = 0; i < PASSWORD_LENGTH; i++) {
		if (receivedPassword[i] != storedPassword[i]) {
			passwordMatch = FAILED; // Passwords do not match
			break; // Exit the loop early
		}
	}

	/* Send the result */
	if (passwordMatch) {
		UART_sendByte(SUCCESS);
	} else {
		UART_sendByte(FAILED);
	}

	return passwordMatch;
}

/**
 * @brief Open the door with motor control.
 *
 * This function is responsible for opening the door by controlling a DC motor. It initializes a timer
 * to manage the motor's rotation, and then it rotates the motor in a specific direction (CW) with a
 * specified speed (50% duty cycle). The timer callback function `APP_timerControlMotor` manages the
 * rotation and the stopping of the motor after a specific time period.
 */

void APP_openDoor(void) {
	Timer_ConfigType timerConfigData = { 0, CTC_VALUE, F_CPU_1024, CTC_MODE };
	TIMER1_init(&timerConfigData);
	Timer1_setCallBack(APP_timerControlMotor);
	DcMotor_Rotate(CW, 50);
}

/**
 * @brief Handle an error occurrence and activate the buzzer.
 *
 * This function is responsible for handling an error occurrence in the system. It activates a timer
 * to control the error state and manages the activation of a buzzer. The timer callback function
 * `APP_timerControlErrorState` is used to control the error state. The buzzer is turned on to alert
 * users about the error condition.
 */

void APP_errorOccurred(void) {
	Timer_ConfigType timerConfigData = { 0, CTC_VALUE, F_CPU_1024, CTC_MODE };
	TIMER1_init(&timerConfigData);
	Timer1_setCallBack(APP_timerControlErrorState);
	BUZZER_on();
}

