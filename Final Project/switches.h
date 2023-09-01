#ifndef SWITCHES_H_
#define SWITCHES_H_

/*
 * switches.h
 *
 *  Created on: 11/05/2023
 *      Author: Amber Waymouth (awa155)
 *              Arabella Cryer (acr151)
 *
 *  Header file for switches module
 */

/**********************************************************
 * Header files
 **********************************************************/
#include <stdint.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/interrupt.h"
#include "inc/hw_ints.h"
#include "yaw.h"
#include "states.h"

/**********************************************************
 * Constants
 **********************************************************/
#define SW_PERIPH SYSCTL_PERIPH_GPIOA
#define SW_PORT GPIO_PORTA_BASE
//Left switch
#define SW2_GPIO_PIN GPIO_PIN_6
#define SWITCH_LEFT_NORMAL true
//Right switch
#define SW1_GPIO_PIN GPIO_PIN_7
#define SWITCH_RIGHT_NORMAL true

#define NUM_SWITCH_POLLS 5
#define NUM_SWITCHES 2

/**********************************************************
 * Define the switch states
 **********************************************************/
enum switchNames {RIGHT_SWITCH = 0, LEFT_SWITCH};
enum switchStates {SWITCH_DOWN = 0, SWITCH_UP, SWITCH_NO_CHANGE};

/**********************************************************
 * Initialise switch 1
 **********************************************************/
void
initSwitch(void);

/**********************************************************
 * Get the value of SW1
 **********************************************************/
uint8_t
getSwitchOneValue(void);

/**********************************************************
 * Get the value of SW2
 **********************************************************/
uint8_t
getSwitchTwoValue(void);

/**********************************************************
 * Updates the switch values
 * Change in switch state only occurs after NUM_SWITCH_POLLS have read the
 * switch in the opposite direction
 * Switch state is then changed and switch_flag is set
 **********************************************************/
void
updateSwitches(void);

/**********************************************************
 * Check if reset switch is HIGH
 * Calls SysCtlReset() to reset system
 **********************************************************/
void
checkResetSwitch(void);

/**********************************************************
 * @return uint8_t
 * @param switch number
 * checkSwitch returns the new switch logical state if the switch state
 * (SWITCH_UP, SWITCH_DOWN) has changed otherwise return SWITCH_UNCHANGED
 **********************************************************/
uint8_t
checkSwitch (uint8_t switchName);

#endif /* SWITCHES_H_ */
