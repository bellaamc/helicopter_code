/*
 * pwm.h
 *
 *  Created on: 30/04/2023
 *      Author: Arabella Cryer (acr151)
 *              Amber Waymouth (awa155)
 *
 *  Header file for PWM generation (pwm.c)
 */

#ifndef _PWM_H_
#define _PWM_H_

/**********************************************************
 * Header files
 **********************************************************/
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/pin_map.h" //Needed for pin configure
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/pwm.h"
#include "driverlib/systick.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "buttons4.h"
#include "altitude.h"

/**********************************************************
 * Constants
 **********************************************************/
// PWM configuration
#define PWM_RATE_HZ        250
#define PWM_MIN_DUTY       3
#define PWM_MAX_DUTY       60
#define PWM_DIVIDER_CODE   SYSCTL_PWMDIV_4
#define PWM_DIVIDER        4

//  PWM Hardware Details M0PWM7 (gen 3)
//  ---Main Rotor PWM: PC5, J4-05 - altitude
//  ---Tail Rotor PWM: PF7 - yaw
#define PWM_ALTITUDE_BASE        PWM0_BASE
#define PWM_YAW_BASE             PWM1_BASE
#define PWM_ALTITUDE_GEN         PWM_GEN_3
#define PWM_YAW_GEN              PWM_GEN_2
#define PWM_ALTITUDE_OUTNUM      PWM_OUT_7
#define PWM_YAW_OUTNUM           PWM_OUT_5
#define PWM_ALTITUDE_OUTBIT      PWM_OUT_7_BIT
#define PWM_YAW_OUTBIT           PWM_OUT_5_BIT
#define PWM_ALTITUDE_PERIPH_PWM  SYSCTL_PERIPH_PWM0
#define PWM_YAW_PERIPH_PWM       SYSCTL_PERIPH_PWM1
#define PWM_ALTITUDE_PERIPH_GPIO SYSCTL_PERIPH_GPIOC
#define PWM_YAW_PERIPH_GPIO      SYSCTL_PERIPH_GPIOF
#define PWM_ALTITUDE_GPIO_BASE   GPIO_PORTC_BASE
#define PWM_YAW_GPIO_BASE        GPIO_PORTF_BASE
#define PWM_ALTITUDE_GPIO_CONFIG GPIO_PC5_M0PWM7
#define PWM_YAW_GPIO_CONFIG      GPIO_PF1_M1PWM5
#define PWM_ALTITUDE_GPIO_PIN    GPIO_PIN_5
#define PWM_YAW_GPIO_PIN         GPIO_PIN_1

#define SYSTICK_RATE_HZ    150

//*****************************************************************************
// Initialisation functions for the clock (incl. SysTick), ADC, display
//*****************************************************************************
void
initClock (void);

/*********************************************************
 * initialisePWM
 * M0PWM7, PC5 is used for the main rotor motor
 * M1PWM5, PF7 is used for the tail rotor motor
 *********************************************************/
void
initialisePWM (void);

/********************************************************
 * Function to set the freq, duty cycle of M0PWM7
 ********************************************************/
void
setAltitudePwm (int32_t duty_update_val);

/*********************************************************
 * initialiseYawPWM() initialises the PWM for the tail rotor
 *********************************************************/
void
initialiseYawPWM(void);

/*********************************************************
 * initialisesAltitudePWM() initialises the PWM for the main rotor
 *********************************************************/
void
initialiseAltitudePWM(void);

/********************************************************
 * Function to set the freq, duty cycle of M0PWM7
 ********************************************************/
void
setYawPwm (int32_t duty_update_val);

/********************************************************
 * Initialise PWM peripheral
 ********************************************************/
void
initPWMPeripherals(void);

/*********************************************************
 * Stop the tail rotor
 *********************************************************/
void
stopTailPWM (void);

/*********************************************************
 * Stop the main rotor
 *********************************************************/
void
stopMainPWM(void);

/*********************************************************
 * Start the main rotor
 *********************************************************/
void
startMainPWM(void);

/*********************************************************
* Start the tail rotor
*********************************************************/
void
startTailPWM (void);

//*****************************************************************************
// @return int32_t
// getYawPWM() returns the value of the current tail rotor duty cycle
//*****************************************************************************
int32_t
getYawPwm(void);

//*****************************************************************************
// @return int32_t
// getAltitudePWM() returns the value of the current main rotor duty cycle
//*****************************************************************************
int32_t
getAltitudePwm(void);

#endif /* _PWM_H_ */
