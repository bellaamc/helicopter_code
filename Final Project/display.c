/*
 * display.c
 *
 *  Created on: 10/05/2023
 *      Author: Amber Waymouth (awa155)
 *              Arabella Cryer (acr151)
 *      Description: Module updates the display as the parameters change.
 *      This updates the orbitOLED display booster pack on the tiva board
 */

#include "display.h"

// *******************************************************
// Initialises the oled display on Tiva Board
// *******************************************************
void
initDisplay (void)
{
  // intialise the Orbit OLED display
    OLEDInitialise ();
}

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
displayParameters(int32_t altitude, int32_t yawVal, int32_t yawDecimal, int32_t altitudePwm, int32_t yawPwm)
{
    char string[17];
    usnprintf(string, sizeof(string), "Altitude = %3d%%" , altitude);
    OLEDStringDraw (string, 0, 0);
    usnprintf (string, sizeof(string), "Yaw = %3d.%1d  ", yawVal, yawDecimal );
    OLEDStringDraw (string, 0, 1);
    usnprintf (string, sizeof(string), "Main Pwm = %4d%%  ", altitudePwm);
    OLEDStringDraw (string, 0, 2);
    usnprintf (string, sizeof(string), "Tail Pwm = %4d%%", yawPwm);
    OLEDStringDraw (string, 0, 3);
}

// ***************************************************************************
// The function that is attached to the scheduler
// ***************************************************************************
void
displaySchedulerFunc(void) {
    displayParameters(processAltitude(), getYawOutput(), getYawDecimal(), getAltitudePwm(), getYawPwm());
}
