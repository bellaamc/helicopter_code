/*
 * altitude.h
 *
 *  Created on: 10/05/2023
 *      Author: Amber Waymouth (awa155)
 *              Arabella Cryer (acr151)
 *      Description: Module converts adc readings to an equivalent altitude and outputs it.
 *      The module also sets desired altitude points to allow the helicopter to move
 *      towards the desired altitude.
 */

#ifndef ALTITUDE_H_
#define ALTITUDE_H_

//*****************************************************************************
// Includes
//*****************************************************************************
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/adc.h"
#include "driverlib/sysctl.h"
#include "circBufT.h"
#include "altitude.h"
#include "states.h"
#include "switches.h"
#include "pidController.h"
#include "scheduler.h"

//*****************************************************************************
// Constants
//*****************************************************************************

#define BUF_SIZE 35
#define SAMPLE_RATE_HZ 150
#define ADC_STEPS 4096
#define MAX_VOLTAGE 3.3
#define ALTITUDE_INCREASE 10
#define PERCENTAGE_CONVERSION 100
#define MAX_ALTITUDE 100
#define MIN_ALTITUDE 0
#define HOVER_ALTITUDE 10
#define MAX_ALTITUDE 100
#define MIN_ALTITUDE 0


//*****************************************************************************
// Reads the values from the adc on Tiva Board to get Helicopter altitude
//
// @return The sampled and averaged adc reading from a circular buffer
//*****************************************************************************
int32_t
getAdcOutput(void);

//*****************************************************************************
// Finds the error between the desired and the actual altitude of the heli
//
// @return int32_t The sampled and averaged adc reading from a circular buffer
//*****************************************************************************
int32_t
getAltitudeError(void);

//*****************************************************************************
//
// The interrupt handler for the for SysTick interrupt, for adc module
//
//*****************************************************************************
void
SysTickIntHandler(void);

//*****************************************************************************
// @return int16_t The initial adc reading, indicating 0 altitude
//
// Returns the initial altitude of altitude conversion
//
//*****************************************************************************
int32_t
getInitialAdc(void);

//*****************************************************************************
//
// Initialises the starting altitude as the current state of the adc averaged buffer
//
//*****************************************************************************
void initialiseAdcValue(void);

//*****************************************************************************
//
// The handler for the ADC conversion complete interrupt.
// Writes to the circular buffer.
//
//*****************************************************************************
void
ADCIntHandler(void);

//*****************************************************************************
//
// Initialises the ADC interrupt to read voltages, corresponding to different altitudes
//
//*****************************************************************************
void
initADC (void);

//*****************************************************************************
// @param adcOutput The averaged output from the adc module, read from circular buffer
//
// @param initialAdc The first adc reading, indicated where the helicopter's initial position is
//
// @return double The float corresponding to the percentage altitude of the helicopters position
//
// Initialises the ADC interrupt to read voltages, corresponding to different altitudes
//
//*****************************************************************************
int32_t
percentageCalculator(int32_t adcOutput, double initialAdc);

//*****************************************************************************
// Set the altitude to the desired percentage. Takes the desired altitude as input
//*****************************************************************************
void updateAltitude(int16_t change);

//*****************************************************************************
// @return int16_t The desired altitude
//
// Returns the altitude conversion of adc samples
//
//*****************************************************************************
int16_t
processAltitude(void);

//*****************************************************************************
// @return double The desired altitude
//
// Initialises the ADC interrupt to read voltages, corresponding to different altitudes
//
//*****************************************************************************
int16_t
getAltitudeSetpoint(void);

//*****************************************************************************
// @param value sets the desired altitude
//
// Sets the value for the heli to fly to
//
//*****************************************************************************
void
setAltitudeValue(int32_t value);

#endif /* ALTITUDE_H_ */
