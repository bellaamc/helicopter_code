/*
 * pidController.c
 *
 *  Created on: 02/05/2023
 *      Author: Amber Waymouth (awa155)
 *              Arabella Cryer (acr151)
 *
 *  Source file for controlling the pwm output, with altitude and yaw references.
 */

#include "pidController.h"

//*******************************************************************************************
// Static variables
//*******************************************************************************************
static int32_t altitudeIntegral = 0;
static int32_t yawIntegral = 0;
static int32_t prevYawErr = 0;
static bool yawControl = true;

//*******************************************************************************************
// PID controller for the altitude of the helicopter
//*******************************************************************************************
void
altitudeController(void) {
    // Gets the current error from the desired vs current altitude
    int32_t error = getAltitudeError();
    // removes underflow when adjusting pwm
    error = error * UNDERFLOW_ADJUSTMENT;
    // Calculates values for P, I and D control
    int32_t P = KP_ALTITUDE * error;
    int32_t dI = KI_ALTITUDE * error;
    int32_t control = P + (altitudeIntegral + dI);
    control = control / UNDERFLOW_ADJUSTMENT;
    // Prevents control from being too small or big
    if (control > OUTPUT_MAX) {
        control = OUTPUT_MAX;
    } else if (control < OUTPUT_MIN) {
        control = OUTPUT_MIN;
    } else {
        altitudeIntegral += dI;
    }
    setAltitudePwm(control);
}

//*******************************************************************************************
// @param enable determines whether the yaw control is on or not
//
// Enables and disables control of the yaw angle
//*******************************************************************************************
void
enableYawControl(bool enable) {
    yawControl = enable;
}



//*******************************************************************************************
// PID controller for the yaw angle of the helicopter
//*******************************************************************************************
void
yawController(void) {
    // Checks if the state machine wants yawControl to be on or not for finding the reference
    if (yawControl) {
        int32_t error = getYawError();
        // Removes underflow, since calculations use integers
        error = error * UNDERFLOW_ADJUSTMENT;
        // Calculates values for P, I and D control
        int32_t P = KP_YAW * error;
        int32_t D = (KD_YAW) * (error - prevYawErr) ;
        int32_t dI = KI_YAW * error;
        int32_t control = P + (yawIntegral + dI) + D;
        // Sets error for derivative gain
        prevYawErr = error;
        control = control / UNDERFLOW_ADJUSTMENT;
        // Prevents control from being too large or small
        if (control > OUTPUT_MAX) {
            control = OUTPUT_MAX;
        } else if (control < OUTPUT_MIN) {
            control = OUTPUT_MIN;
        } else {
            yawIntegral += dI;
        }
        setYawPwm(control);
    }
}

//*******************************************************************************************
// Task to assign to scheduler to run both controllers
//*******************************************************************************************
void
updateControl(void) {
    altitudeController();
    yawController();
}
