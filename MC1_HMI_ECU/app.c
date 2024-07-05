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

#include "app.h"
#include "lcd.h"
#include "keypad.h"
#include "string.h"
#include "uart.h"
#include "timer.h"
#include "util/delay.h"

volatile uint8 g_flag = 0;

/*******************************************************************************
 CALL-BACK FUNCTIONS
 ********************************************************************************/

/**
 * @brief Timer callback to control the LCD display during a specific state.
 *
 * This function is a timer callback that controls the content displayed on the LCD
 * during a specific state (LOCKING_TIME). It clears the screen and updates the message.
 */

static void APP_timerControlLCD(void) {
	static uint8 tickThreeSeconds = 0;
	tickThreeSeconds++;
	if (tickThreeSeconds == LOCKING_TIME) {
		LCD_clearScreen();
		LCD_displayStringRowColumn(0, 0, "Door is Locking");
	} else if (tickThreeSeconds == END_TIME) {
		g_flag = 1;
		Timer1_deInit();
		tickThreeSeconds = 0;
	}
}

/**
 * @brief Timer callback to control the error state duration.
 *
 * This function is a timer callback that controls the error state duration. It is called to clear
 * the LCD screen and reset the timer after a specific period.
 */

static void APP_timerControlErrorState(void) {
	static uint8 tickThreeSeconds = 0;
	tickThreeSeconds++;
	if (tickThreeSeconds == WARNING_TIME) {
		LCD_clearScreen();
		Timer1_deInit();
		tickThreeSeconds = 0;
		g_flag = 1;
	}
}

/*******************************************************************************
 FUNCTIONS DEFINITION
 ********************************************************************************/

/**
 * @brief Initialize UART and LCD modules.
 *
 * This function initializes the UART and LCD modules with specific configurations.
 * It sets up the UART for communication and the LCD for display.
 */

void APP_init(void) {
	UART_ConfigType UART_Config_Data = { bit_8, Enabled_Even, bit_1, 9600 };
	UART_init(&UART_Config_Data);
	LCD_init();
	LCD_clearScreen();
	LCD_displayStringRowColumn(0, 3, "Welcome :)");
	LCD_displayStringRowColumn(1, 2, "Door System");
	_delay_ms(1000);

}

/**
 * @brief Send an error message and set a timer for controlling error state.
 *
 * This function sends an error message via UART, displays an error message on the LCD,
 * and sets a timer to control the error state. The timer callback function is used to
 * handle the error state.
 */

void APP_sendError(void) {

	UART_sendByte(APP_SEND_ERROR);
	LCD_clearScreen();
	LCD_displayStringRowColumn(0, 0, "ERROR :(");
	Timer_ConfigType timerConfigData = { 0, CTC_VALUE, F_CPU_1024, CTC_MODE };
	TIMER1_init(&timerConfigData);
	Timer1_setCallBack(APP_timerControlErrorState);
	while (g_flag != 1)
		;
	g_flag = 0;
}

/**
 * @brief Create and send a password to the control MCU.
 *
 * This function allows the user to create a password by entering it twice for confirmation.
 * The password is entered through a keypad, and it is displayed on an LCD screen as asterisks.
 * After confirming the password, it is sent to the Control_ECU for storage if the two entered
 * passwords match. If they don't match, the user is given a limited number of attempts.
 *
 * @return uint8_t The function returns one of the following status codes:
 * - SUCCESS: The password was created and saved successfully.
 * - RE_CALL: The password did not match, and it's the first function call.
 * - FATAL_ERROR: The password did not match, and the maximum number of attempts is reached.
 *
 *	[UPDATE]: Instead of compare the two passwords there, and send
 *	if the two passwords are the same [NOW] we send the two passwords
 *	and in Control MCU they will be compared
 */

uint8_t APP_createChangePassword(void) {
	// Static variable to keep track of function calls
	static uint8_t funcCallCount = 0;
	uint8_t txFirstPassword[PASSWORD_LENGTH] = { 0 };
	uint8_t txSecondPassword[PASSWORD_LENGTH] = { 0 };
	uint8_t i = 0;
	uint8_t state = SUCCESS;

	// Increment the function call count
	funcCallCount++;

	LCD_clearScreen();
	LCD_displayStringRowColumn(0, 0, "ENTER NEW");
	LCD_displayStringRowColumn(1, 0, "PASSWORD");
	_delay_ms(1000);

	// Display a message on the LCD
	LCD_clearScreen();
	LCD_displayString("Plz Enter Pass:");
	LCD_moveCursor(1, 0);

	// Receive the first part of the password
	while (i < PASSWORD_LENGTH) {
		txFirstPassword[i] = KEYPAD_getPressedKey();
		LCD_displayCharacter('*');
		i++;
		_delay_ms(500); // Press time delay
	}

	// Wait for the Enter button to be pressed
	while (KEYPAD_getPressedKey() != ENTER_BUTTON)
		;
	_delay_ms(500); // Press time delay

	// Display a message for re-entering the password
	LCD_clearScreen();
	LCD_displayStringRowColumn(0, 0, "Plz Re-Enter the");
	LCD_displayStringRowColumn(1, 0, "same pass:");

	i = 0;

	// Receive the second part of the password
	while (i < PASSWORD_LENGTH) {
		txSecondPassword[i] = KEYPAD_getPressedKey();
		LCD_displayCharacter('*');
		i++;
		_delay_ms(500); // Press time delay
	}

	// Wait for the Enter button to be pressed
	while (KEYPAD_getPressedKey() != ENTER_BUTTON)
		;
	_delay_ms(500); // Press time delay

	// Send a request to save the passwords
	UART_sendByte(APP_SAVE_PASS);

	// Wait for an acknowledgment from the UART
	while (UART_recieveByte() != ACKNOWLEDGEMENT)
		;

	// Send the first password to be saved
	for (uint8_t i = 0; i < PASSWORD_LENGTH; i++) {
		UART_sendByte(txFirstPassword[i]);
	}

	// Send the second password to be saved
	for (uint8_t i = 0; i < PASSWORD_LENGTH; i++) {
		UART_sendByte(txSecondPassword[i]);
	}

	// Receive the state of the password saving process
	state = UART_recieveByte();

	if (state == SUCCESS) {
		// Reset the function call count and return SUCCESS
		funcCallCount = 0;
		state = SUCCESS;
	} else if (state == FAILED) {
		if (funcCallCount >= MAX_NUM_REP) {
			// Handle the case of a password mismatch and reaching the maximum attempts
			state = FATAL_ERROR;
		} else {
			// Handle the case of a password mismatch and the first function call
			LCD_clearScreen();
			LCD_displayString("Not the same");
			LCD_displayStringRowColumn(1,0,"No. Attempt ");
			LCD_intgerToString(funcCallCount);
			_delay_ms(500);
			state = RE_CALL;
		}
	}
	return state;
}

/**
 * @brief Display the main options on an LCD and wait for user input.
 *
 * This function displays the main system options on an LCD screen and waits for the user to
 * choose between opening the door or changing the password. The user's choice is received
 * through a keypad.
 *
 * @return uint8 The function returns one of the following options based on user choice:
 * - OPEN_DOOR: User chooses to open the door.
 * - CHANGE_PASS: User chooses to change the password.
 */

uint8 APP_displayMainOption(void) {

	uint8 key = 0;
	uint8 state = 0;

	LCD_clearScreen();

	LCD_displayStringRowColumn(0, 0, "+ : Open Door");

	LCD_displayStringRowColumn(1, 0, "- : Change Pass");

	do {
		key = KEYPAD_getPressedKey();
		_delay_ms(500); /* Press time */
	} while (key != OPEN_DOOR && key != CHANGE_PASS);

	switch (key) {
	case OPEN_DOOR:
		state = OPEN_DOOR;
		break;
	case CHANGE_PASS:
		state = CHANGE_PASS;
		break;
	}
	return state;
}

/**
 * @brief Open the door by entering a password.
 *
 * This function allows the user to enter a password to open the door.
 * It communicates with the keypad, UART, and LCD for this purpose.
 *
 * @return An error code indicating the outcome of the door opening attempt.
 */

uint8 APP_checkPassword(void) {
	static uint8 funcCallCount = 0;
	funcCallCount++;

	uint8 pass[PASSWORD_LENGTH] = { 0 };
	uint8 i = 0;
	uint8 state = 0;
	uint8 receivedByte = 0;

	LCD_clearScreen();
	LCD_displayStringRowColumn(0, 0, "CHECKING ...");
	_delay_ms(500);

	LCD_clearScreen();
	LCD_displayStringRowColumn(0, 0, "Plz Enter Pass:");
	LCD_moveCursor(1, 0);

	while (i < PASSWORD_LENGTH) {
		pass[i] = KEYPAD_getPressedKey();
		LCD_displayCharacter('*');
		i++;
		_delay_ms(500); // Use a separate delay function
	}

	while (KEYPAD_getPressedKey() != '=')
		;
	_delay_ms(500); // Use a separate delay function

	UART_sendByte(APP_CHECK_PASS);
	_delay_ms(15); // Use a separate delay function

	for (uint8 i = 0; i < PASSWORD_LENGTH; i++) {
		UART_sendByte(pass[i]);
	}

	receivedByte = UART_recieveByte();

	switch (receivedByte) {
	case SUCCESS:
		funcCallCount = 0;
		state = SUCCESS;
		break;
	case FAILED:
		if (funcCallCount >= MAX_NUM_REP) {
			funcCallCount = 0;
			state = FATAL_ERROR;
		} else {
			LCD_clearScreen();
			LCD_displayString("Wrong Password");
			LCD_displayStringRowColumn(1,0,"No. Attempt ");
			LCD_intgerToString(funcCallCount);
			_delay_ms(500);
			state = RE_CALL;
		}
		break;
	}

	return state;
}

/**
 * @brief Display the door state on the LCD during door unlocking.
 *
 * This function displays a message on the LCD to indicate that the door is unlocking.
 * It also utilizes a timer to control the LCD display.
 */

void APP_openDoor(void) {
	UART_sendByte(APP_OPEN_DOOR);
	LCD_clearScreen();
	LCD_displayStringRowColumn(0, 0, "Door is Unlocking");
	Timer_ConfigType timerConfigData = { 0, CTC_VALUE, F_CPU_1024, CTC_MODE };
	TIMER1_init(&timerConfigData);
	Timer1_setCallBack(APP_timerControlLCD);
	while (g_flag != 1)
		;
	g_flag = 0;
}
