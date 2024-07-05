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
#define HOLDING_TIME        5       /* Door holding state time (5*3= 15 sec) */
#define LOCKING_TIME        6       /* Door locking state time (6*3= 18 sec) */
#define END_TIME            11      /* End state time (11 *3 = 33 sec)*/
#define WARNING_TIME        20      /* Warning timer duration (60 seconds) */

/* Timer configurations */
#define CTC_VALUE           23437   /* Value for Compare register (CTC) mode = 3 second */

/* Application command codes */
#define APP_SAVE_PASS       200     /* Command code for saving the password */
#define APP_CHECK_PASS      201     /* Command code for checking the password */
#define APP_SEND_ERROR      202     /* Command code for sending an error */
#define APP_OPEN_DOOR		203   	/* Request code for sending an open door message */

/* Error and success states */
#define FATAL_ERROR         4       /* Fatal error state */
#define RE_CALL             5       /* Request to re-enter data state */
#define FAILED              0       /* Operation or verification failed */
#define SUCCESS             1       /* Operation or verification successful */

/* Password-related constants */
#define PASSWORD_LENGTH     5       /* Length of the password */

/* User choices */
#define OPEN_DOOR           '+'     /* User chooses to open the door */
#define CHANGE_PASS         '-'     /* User chooses to change the password */
#define ENTER_BUTTON        '='     /* Enter button symbol */

/* Maximum number of consecutive attempts */
#define MAX_NUM_REP          3       /* Maximum number of consecutive attempts */

/* Acknowledgment code */
#define ACKNOWLEDGEMENT      2       /* Acknowledgment signal */

/*******************************************************************************
 FUNCTION PROTOTYPE
 ********************************************************************************/

/* @brief Initialize UART and LCD modules.*/
void APP_init(void);

/* @brief Send an error message and set a timer for controlling error state.*/
void APP_sendError(void);

/* @brief Create and save a password.*/
uint8 APP_createChangePassword(void);

/* @brief Display the main options on an LCD and wait for user input.*/
uint8 APP_displayMainOption(void);

/* @brief Open the door by entering a password.*/
uint8 APP_checkPassword(void);

/* @brief Display the door state on the LCD during door unlocking.*/
void APP_openDoor(void);

#endif /* APP_H_ */
