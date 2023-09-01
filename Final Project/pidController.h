/*
 * pidController.h
 *
 *  Created on: 02/05/2023
 *      Author: Amber Waymouth (awa155)
 *              Arabella Cryer (acr151)
 *
 *  Header file for controlling the pwm output, with altitude and yaw references.
 */

#ifndef PIDCONTROLLER_H_
#define PIDCONTROLLER_H_

//*******************************************************************************************
// Header files
//*******************************************************************************************
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/pin_map.h" //Needed for pin configure
#include "driverlib/debug.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "buttons4.h"
#include "altitude.h"
#include "yaw.h"
#include "pwm.h"
#include "states.h"
//*******************************************************************************************
// Constants
//*******************************************************************************************
#define KP_ALTITUDE 6
#define KI_ALTITUDE 3
#define KP_YAW 12
#define KI_YAW 3 //3
#define KD_YAW 1
#define OUTPUT_MAX 70
#define OUTPUT_MIN 3
#define UNDERFLOW_ADJUSTMENT 100
//*******************************************************************************************
// PID controller for the altitude of the helicopter
//
//*******************************************************************************************
void
altitudeController(void);

//*******************************************************************************************
// PID controller for the yaw angle of the helicopter
//
//*******************************************************************************************
void
yawController(void);

//*******************************************************************************************
// Task to assign to scheduler to run both controllers
//
//*******************************************************************************************
void
updateControl(void);

#endif /* PIDCONTROLLER_H_ */
