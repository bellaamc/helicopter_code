/*
 * display.h
 *
 *  Created on: 10/05/2023
 *      Author: Amber Waymouth (awa155)
 *              Arabella Cryer (acr151)
 *
 *  Header file for display module
 */

#ifndef DISPLAY_H_
#define DISPLAY_H_

//*******************************************************************************************
// Header files
//*******************************************************************************************
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "OrbitOLED/OrbitOLEDInterface.h"
#include "utils/ustdlib.h"
#include "yaw.h"
#include "altitude.h"
#include "pwm.h"

// ***************************************************************************
// Initialises the oled display on Tiva Board
// ***************************************************************************
void
initDisplay (void);

// ***************************************************************************
// @param altitude Inputs the current altitude from the adc and processed altitude
//
// @param yawVal Inputs the current yaw value from the quadrature encoder
//
// @param yawDecimal The decimal point precision for the yaw reading
//
// @param altitudePwm the pwm corresponding to the main rotor
//
// @param yawPwm The pwm corresponding to the tail rotor
//
// Displays the paramters on the orbitOLED screen
// ***************************************************************************
void
displayParameters(int32_t altitude, int32_t yawVal, int32_t yawDecimal, int32_t altitudePwm, int32_t yawPwm);

// ***************************************************************************
// The function that is attached to the scheduler
// ***************************************************************************
void
displaySchedulerFunc(void);

#endif /* DISPLAY_H_ */
