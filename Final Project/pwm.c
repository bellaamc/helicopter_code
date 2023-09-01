/*
 * pwm.c
 *
 *  Created on: 30/04/2023
 *      Author: Amber Waymouth (awa155)
 *              Arabella Cryer (acr151)
 *
 *  Module for PWM generation
 */

#include "pwm.h"

//*****************************************************************************
// Static variables
//*****************************************************************************
static int32_t altitude_duty = 0; // Stores current altitude duty cycle
static int32_t yaw_duty = 0;      // Stores current yaw duty cycle

//*****************************************************************************
// @return int32_t
// getYawPWM() returns the value of the current tail rotor duty cycle
//*****************************************************************************
int32_t
getYawPwm(void)
{
    return yaw_duty;
}

//*****************************************************************************
// @return int32_t
// getAltitudePWM() returns the value of the current main rotor duty cycle
//*****************************************************************************
int32_t
getAltitudePwm(void)
{
    return altitude_duty;
}

//*****************************************************************************
// Initialisation functions for the clock (incl. SysTick), ADC, display
//*****************************************************************************
void
initClock (void)
{
    // Set the clock rate to 20 MHz
    SysCtlClockSet (SYSCTL_SYSDIV_10 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN |
                   SYSCTL_XTAL_16MHZ);
    SysCtlPWMClockSet(PWM_DIVIDER_CODE);
    SysCtlDelay(100);
}


/*********************************************************
 * initialisePWM
 * M0PWM7, PC5 is used for the main rotor motor
 * M1PWM5, PF7 is used for the tail rotor motor
 *********************************************************/
void
initialisePWM (void)
{
    initialiseAltitudePWM();
    initialiseYawPWM();
    // Initialisation is complete, so turn on the output.
    PWMOutputState(PWM_ALTITUDE_BASE, PWM_ALTITUDE_OUTBIT, true);
    PWMOutputState(PWM_YAW_BASE, PWM_YAW_OUTBIT, true);
}

/*********************************************************
 * Stop the tail rotor
 *********************************************************/
void
stopTailPWM (void)
{
    PWMOutputState(PWM_YAW_BASE, PWM_YAW_OUTBIT, false);
}

/*********************************************************
 * Stop the main rotor
 *********************************************************/
void
stopMainPWM(void)
{
    PWMOutputState(PWM_ALTITUDE_BASE, PWM_ALTITUDE_OUTBIT, false);
}

/*********************************************************
 * Start the tail rotor
 *********************************************************/
void
startTailPWM  (void)
{
    PWMOutputState(PWM_YAW_BASE, PWM_YAW_OUTBIT, true);
}

/*********************************************************
 * Start the main rotor
 *********************************************************/
void
startMainPWM(void)
{
   PWMOutputState(PWM_ALTITUDE_BASE, PWM_ALTITUDE_OUTBIT, true);
}


/*********************************************************
 * initialisesAltitudePWM() initialises the PWM for the main rotor
 *********************************************************/
void
initialiseAltitudePWM(void){
    // Enable main rotor peripherals
    SysCtlPeripheralEnable(PWM_ALTITUDE_PERIPH_PWM);
    SysCtlPeripheralEnable(PWM_ALTITUDE_PERIPH_GPIO);

    // Configure main rotor PWM GPIO pins
    GPIOPinConfigure(PWM_ALTITUDE_GPIO_CONFIG);
    GPIOPinTypePWM(PWM_ALTITUDE_GPIO_BASE, PWM_ALTITUDE_GPIO_PIN);

    // Configure main rotor PWM output
    PWMGenConfigure(PWM_ALTITUDE_BASE, PWM_ALTITUDE_GEN, PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC);
    setAltitudePwm(3);
    PWMGenEnable(PWM_ALTITUDE_BASE, PWM_ALTITUDE_GEN);
    PWMOutputState(PWM_ALTITUDE_BASE, PWM_ALTITUDE_OUTBIT, false);
}

/*********************************************************
 * initialiseYawPWM() initialises the PWM for the tail rotor
 *********************************************************/
void
initialiseYawPWM(void){
    // Enable tail rotor peripherals
    SysCtlPeripheralEnable(PWM_YAW_PERIPH_PWM);
    SysCtlPeripheralEnable(PWM_YAW_PERIPH_GPIO);

    // Configure tail rotor PWM GPIO pins
    GPIOPinConfigure(PWM_YAW_GPIO_CONFIG);
    GPIOPinTypePWM(PWM_YAW_GPIO_BASE, PWM_YAW_GPIO_PIN);

    // Configure tail rotor PWM output
    PWMGenConfigure(PWM_YAW_BASE, PWM_YAW_GEN, PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC);
    setYawPwm(3);
    PWMGenEnable(PWM_YAW_BASE, PWM_YAW_GEN);
    PWMOutputState(PWM_YAW_BASE, PWM_YAW_OUTBIT, false);
}

/********************************************************
 * Function to set the freq, duty cycle of M0PWM7
 ********************************************************/
void
setAltitudePwm (int32_t duty_update_val)
{
    // Assign altitude duty to max duty (60%) when duty is greater that 60
    // Assign altitude duty to min duty (3%) when duty is less than 3
    altitude_duty = duty_update_val;
    if (altitude_duty > PWM_MAX_DUTY) {
        altitude_duty = PWM_MAX_DUTY;
    } else if (altitude_duty < PWM_MIN_DUTY) {
        altitude_duty = PWM_MIN_DUTY;
    }
    // Calculate the PWM period corresponding to the freq.
    uint32_t ui32Period = SysCtlClockGet() / PWM_DIVIDER / PWM_RATE_HZ;

    PWMGenPeriodSet(PWM_ALTITUDE_BASE, PWM_ALTITUDE_GEN, ui32Period);
    PWMPulseWidthSet(PWM_ALTITUDE_BASE, PWM_ALTITUDE_OUTNUM, ui32Period * altitude_duty / 100);
}

/********************************************************
 * Function to set the freq, duty cycle of M1PWM5
 ********************************************************/
void
setYawPwm (int32_t duty_update_val)
{
    // Assign yaw duty to max duty (60%) when duty is greater that 60
    // Assign yaw duty to min duty (3%) when duty is less than 3
    if (duty_update_val > PWM_MAX_DUTY) {
        duty_update_val = PWM_MAX_DUTY;
    } else if (yaw_duty < PWM_MIN_DUTY) {
        duty_update_val = PWM_MIN_DUTY;
    }

    // Calculate the PWM period corresponding to the freq.
    uint32_t ui32Period = SysCtlClockGet() / PWM_DIVIDER / PWM_RATE_HZ;

    PWMGenPeriodSet(PWM_YAW_BASE, PWM_YAW_GEN, ui32Period);
    PWMPulseWidthSet(PWM_YAW_BASE, PWM_YAW_OUTNUM, ui32Period * duty_update_val / 100);
    yaw_duty = duty_update_val;
}

/********************************************************
 * Initialise PWM peripherals
 ********************************************************/
void
initPWMPeripherals(void)
{

    // As a precaution, make sure that the peripherals used are reset
    SysCtlPeripheralReset (PWM_ALTITUDE_PERIPH_GPIO); // Used for PWM output
    SysCtlPeripheralReset (PWM_YAW_PERIPH_GPIO); // Used for PWM output
    SysCtlPeripheralReset (PWM_ALTITUDE_PERIPH_PWM);  // Main Rotor PWM, tail rotor pwm
    SysCtlPeripheralReset (PWM_YAW_PERIPH_PWM);  // Main Rotor PWM, tail rotor pwm
    SysCtlPeripheralReset (UP_BUT_PERIPH);        // UP button GPIO
    SysCtlPeripheralReset (DOWN_BUT_PERIPH);      // DOWN button GPIO
}
