/******************************************************************************
 *
 * Module: Common - Main File
 *
 * File Name: main.c
 *
 * Description: main file of MC1_HMI_ECU
 *
 * Author: Hussein El-Shamy
 *
 *******************************************************************************/

/*******************************************************************************
 INCLUDING NECESSARY HEADER FILES
 ********************************************************************************/

#include "app.h"
#include "uart.h"
#include <avr/io.h>
#include "lcd.h"
#include "util/delay.h"

/*******************************************************************************
 MAIN FUNCTION
 ********************************************************************************/

int main(void) {

	/* Enable I-bit*/
	SREG |= (1 << 7);

	/* to store the selected option of user */
	uint8 MainOptionState = 0;

	/* to store the return state of the functions */
	uint8 FuncState = 0;

	/* Initialize UART and LCD */
	APP_init();

	/*============================================
	 * 				Creating Password
	 *===========================================*/
	do {
		/* [LOOP] Execute a loop a maximum of [MAX_NUM_REP] times,
		 * adhering to the allowed repetition limit */
		FuncState = APP_createChangePassword();
		if (FuncState == FATAL_ERROR) {
			/* [WRONG PASSWORD]
			 * Send Error Command via UART AND Display Error Message */
			APP_sendError();
		} else if (FuncState == SUCCESS) {
			break;
		}
	} while (FuncState == RE_CALL || FuncState == FATAL_ERROR);

	/*============================================
	 * 				Super Loop
	 *===========================================*/
	while (1) {
		/* Display Main Option Menu and store the selected option
		 * [+] Open Door	[-]Change Password */
		MainOptionState = APP_displayMainOption();

		switch (MainOptionState) {
		/*============================================
		 * 				Main Option Menu
		 *===========================================*/
		case OPEN_DOOR:
			/*============================================
			 * 				[1] Open Door
			 *===========================================*/
			do {
				/* [LOOP] Execute a loop a maximum of [MAX_NUM_REP] times,
				 * adhering to the allowed repetition limit */
				FuncState = APP_checkPassword();
				if (FuncState == SUCCESS) {
					/* [CORRECT PASSWORD]
					 * Send Open Door Command via UART AND Display Door State */
					APP_openDoor();
					break;
				} else if (FuncState == FATAL_ERROR) {
					/* [WRONG PASSWORD]
					 * Send Error Command via UART AND Display Error Message */
					APP_sendError();
					break;
				}
			} while (FuncState == RE_CALL);

			break;

		case CHANGE_PASS:
			/*============================================
			 * 				[2] Change Password
			 *===========================================*/
			do {
				/* [LOOP] Execute a loop a maximum of [MAX_NUM_REP] times,
				 * adhering to the allowed repetition limit */

				FuncState = APP_checkPassword();
				if(FuncState == SUCCESS){
				APP_createChangePassword();
				}
				else if (FuncState == FATAL_ERROR) {
					/* [WRONG PASSWORD]
					 * Send Error Command via UART AND Display Error Message */
					APP_sendError();
					break;
				}
			} while (FuncState == RE_CALL);
			break;
			/* End of switch condition scope*/
		}
		/* End of super loop */
	}
	/* End of Main Function*/
	return 0;
}

