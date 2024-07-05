/******************************************************************************
 *
 * Module: Application
 *
 * File Name: app.h
 *
 * Description: application code
 *
 * Author: Hussein El-Shamy
 *
 *******************************************************************************/

#ifndef APP_H_
#define APP_H_

/*******************************************************************************
 INCLUDING NECESSARY HEADER FILES
 ********************************************************************************/

#include "std_types.h"

/*******************************************************************************
 DEFINITONS & STATIC CONFIGURATION
 ********************************************************************************/

/* Timing State values */
#define HOLDING_TIME         5      /* Represents the holding state time for a specific operation */
#define LOCKING_TIME         6      /* Represents the locking state time for a specific operation */
#define END_TIME             11     /* Represents the end state time for a specific operation */
#define WARNING_TIME         20     /* Time duration for a warning condition [3*20 = 60 seconds] */

/* Timer configuration */
#define CTC_VALUE            23437  /* Constant for a specific timer compare value [3 seconds real-time] */

/* Application command codes */
#define APP_SAVE_PASS        200    /* Request code for saving a password */
#define APP_CHECK_PASS       201    /* Request code for checking a password */
#define APP_SEND_ERROR       202    /* Request code for sending an error message */
#define APP_OPEN_DOOR		 203   	/* Request code for sending an open door message */
/* Error and success states */
#define FATAL_ERROR          4      /* Code indicating a fatal error condition */
#define RE_CALL              5      /* Code indicating the need to re-call a function */
#define FAILED               0      /* Code indicating a failed operation or condition */
#define SUCCESS              1      /* Code indicating a successful operation or condition */

/* Password-related constants */
#define PASSWORD_LENGTH      5      /* Length of a password, typically 5 characters */
#define EEPROM_START_ADDRESS 0x0311 /* Starting address in EEPROM for storing data */

/* Acknowledgment code */
#define ACKNOWLEDGEMENT      2      /* Code indicating acknowledgment or confirmation */

/*******************************************************************************
 FUNCTION PROTOTYPE
 ********************************************************************************/

/* @brief Initialize the application components.*/
void APP_init(void);

/* @brief Save a password in EEPROM.*/
void APP_savePassword(void);

/* @brief Check a received password against a stored password in EEPROM.*/
uint8 APP_checkPassword(void);

/* @brief Open the door with motor control.*/
void APP_openDoor(void);

/* @brief Handle an error occurrence and activate the buzzer.*/
void APP_errorOccurred(void);

#endif /* APP_H_ */
