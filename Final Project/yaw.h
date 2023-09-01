/*
 * yaw.h
 *
 *  Created on: 10/05/2023
 *      Author: Amber Waymouth (awa155)
 *              Arabella Cryer (acr151)
 *
 *  Header file for yaw functionality
 */

#ifndef _YAW_H_
#define _YAW_H_

//*****************************************************************************
// Includes header files
//*****************************************************************************
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
#include "switches.h"
#include "states.h"
#include "pwm.h"

//*****************************************************************************
// Constants
//*****************************************************************************
#define YAW_BASE GPIO_PORTB_BASE
#define YAW_A_PIN GPIO_PIN_0
#define YAW_B_PIN GPIO_PIN_1
#define REF_BASE GPIO_PORTC_BASE
#define REF_PIN GPIO_PIN_4
#define YAW_INCREASE 15
#define YAW_NO_CHANGE 0
#define REVOLUITON_PWM 10
#define FULL_CIRCLE_SLOTS 448
#define FULL_CIRCLE 360
#define HALF_CIRCLE 180
#define FLOAT_CONVERSION 10

//*****************************************************************************
// @return int16_t the yaw for the helicopter to move towards
//
// Returns the static variable for the desired yaw
//*****************************************************************************
int16_t
getYawSetpoint(void);

//*****************************************************************************
// @return int16_t yaw angle from the static variable
//
// Converts quadrature encoding ticks to an angle
//*****************************************************************************
int16_t
getYawOutput(void);

//*****************************************************************************
// @return int16_t yaw angle from the static variable
//
// Finds the extra decimal point of precision for the yaw angle
//*****************************************************************************
int16_t
getYawDecimal(void);

//*****************************************************************************
// Sets the interrupt for the quadrature encoder for both edges of both encoder pins
//*****************************************************************************
void
interruptSetQuadratureEncoder(void);

//*****************************************************************************
// The handler for when the yaw quadrature encoder is detected
//*****************************************************************************
void
quadratureHandler(void);

//*****************************************************************************
// @return int16_t The error between the desired and the true yaw angle
//
// Calculates and returns the error between the desired and true yaw angle,
// converts it to find the shortest distance to the desired angle.
//*****************************************************************************
int16_t
getYawError(void);

//*****************************************************************************
// @param int16_t change in yaw angle
//
// Updates the set yaw based on the desired change from the current set yaw
//*****************************************************************************
void
setYawSetpoint(int16_t change);

//*****************************************************************************
// Handles the interrupt called by the reference pin. Resets the yaw angles to 0,
// indicating the 0 degree point.
//*****************************************************************************
void
findingYawReference(void);

//*****************************************************************************
// Sets the interrupt for the reference pin
//*****************************************************************************
void
interruptSetReference(void);

//*****************************************************************************
// Sets the yaw to go back to 0 degrees
//*****************************************************************************
void
resetYaw(void);

//*****************************************************************************
// Resets the reference interrupt handler boolean
//*****************************************************************************
void
updateReference(void);

//*****************************************************************************
// @return bool Returns true once the reference interrupt is called
//
// Gets the helicopter to do a 360 turn until the reference point is found.
//*****************************************************************************
bool
fullRevolution(void);

//*****************************************************************************
// Handles the interrupt called by the reference pin. Resets the yaw angles to 0,
// indicating the 0 degree point.
//*****************************************************************************
void
yawReferenceHandler(void);

#endif /* _YAW_H_ */
