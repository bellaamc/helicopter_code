#ifndef _STATES_H_
#define _STATES_H_

/*
 * states.h
 *
 *  Created on: 02/05/2023
 *      Author: Amber Waymouth (awa155)
 *              Arabella Cryer (acr151)
 *
 *  Header file for the states module
 */

/**********************************************************
 * Header files
 **********************************************************/
#include <stdint.h>
#include <stdbool.h>
#include "buttons4.h"
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "driverlib/adc.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/debug.h"
#include "utils/ustdlib.h"
#include "stdlib.h"
#include "states.h"
#include "altitude.h"
#include "buttons4.h"
#include "pwm.h"
#include "yaw.h"
#include "switches.h"
#include "display.h"

/**********************************************************
 * Constants
 **********************************************************/
// Change in altitude for increase and decrease
#define ALTITUDE_INCREASE 10
#define ALTITUDE_DECREASE -10
// Change in yaw for CW and CCW
#define YAW_INCREASE 15
#define YAW_DECREASE -15
// Change in altitude PWM
#define ALTITUDE_DUTY_STEP 10
// Change in yaw PWM
#define YAW_DUTY_STEP 5
// Duty cycle for when helicopter is landing
#define LANDING_PWM 10

// enum defining helicopter states
typedef enum { LANDED = 0,
                       TAKING_OFF,
                       FLYING,
                       FINDING_REF,
                       LANDING
} helicopterState_t;

/**********************************************************
 * @return helicopterState_t
 * getState() return the current state of the helicopter
 **********************************************************/
helicopterState_t getState(void);

/**********************************************************
 * setState() sets the current state of the helicopter
 **********************************************************/
void setState (helicopterState_t state);

/**********************************************************
 * @return char*
 * heliStateStr() returns a string of the current helicopter state
 **********************************************************/
char* heliStateStr(void);

/**********************************************************
 * checkButtonState() checks the state of the helicopter then the
 * state of the buttons if the helicopter is FLYING and
 * updates altitude and yaw with the defined step values
 **********************************************************/
void checkButtonState (void);

/**********************************************************
 * stateMachine() sets the helicopter functionality based on
 * the current state
 **********************************************************/
void
stateMachine(void);

#endif /* _STATES_H_ */
