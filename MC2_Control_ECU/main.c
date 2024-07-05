/******************************************************************************
 *
 * Module: Common - Main File
 *
 * File Name: main.c
 *
 * Description: main file of MC2_Control_ECU
 *
 * Author: Hussein El-Shamy
 *
 *******************************************************************************/

/*******************************************************************************
 INCLUDING NECESSARY HEADER FILES
 ********************************************************************************/

#include "app.h"
#include "uart.h"
#include <util/delay.h>
#include <avr/io.h>

/*******************************************************************************
 MAIN FUNCTION
 ********************************************************************************/

int main(void) {

	/* Enable I-bit (Interrupts) */
	SREG |= (1 << 7);

	/* to store the return state of the functions */
	uint8 FuncState = SUCCESS;

	/* to store the received command from MC1 */
	uint8 receivedCommand = APP_SAVE_PASS;

	/* Initialize UART, Buzzer and DC Motor  */
	APP_init();

	/* Super Loop */
	while (1) {

		/* Wait until receiving command from MC1_HMI_ECU */
		receivedCommand = UART_recieveByte();

		/* [Received Command]
		 * [1] Save the password >> in case of the first time or change password
		 * [2] Check the password >> in case of the open the door
		 * [3] Error Handling >> in case of un-correct entered password three times */

		switch (receivedCommand) {

		case APP_CHECK_PASS:

			/* receive the pass (array of char) via UART
			 * compare the received password with the stored password
			 * send SUCCESS/FAILED to another ECU */
			FuncState = APP_checkPassword();
			break;

		case APP_SAVE_PASS:
			/* receive the pass (array of char) via UART
			 * store it in eeprom*/
			APP_savePassword();
			break;

		case APP_OPEN_DOOR:
			/* Open Door */
			if (FuncState == SUCCESS)
				APP_openDoor();

			break;

		case APP_SEND_ERROR:
			/* Turn on Buzzer for 1 Minute */
			APP_errorOccurred();
			break;
			/* End of Switch */
		}
		/* End of Super Loop */
	}
	/* End of Main Function */
	return 0;
}
