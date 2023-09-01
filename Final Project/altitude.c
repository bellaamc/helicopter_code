/*
 * altitude.c
 *
 *  Created on: 10/05/2023
 *      Author: Amber Waymouth (awa155)
 *              Arabella Cryer (acr151)
 *      Description: Module converts adc readings to an equivalent altitude and outputs it.
 *      The module also sets desired altitude points to allow the helicopter to move
 *      towards the desired altitude.
 */

#include "altitude.h"

//*****************************************************************************
// Global variables
//*****************************************************************************
static int32_t setAltitude;
static int32_t currentAdc;
static int32_t initialAdc = 0;
static int32_t currentAltitude;
static circBuf_t g_inBuffer;        // Buffer of size BUF_SIZE integers (sample values)

//*****************************************************************************
// Reads the values from the adc on Tiva Board to get Helicopter altitude
//
// @return The sampled and averaged adc reading from a circular buffer
//*****************************************************************************
int32_t
getAdcOutput(void)
{
    int32_t sum = 0;
    size_t i;
    for (i = 0; i < BUF_SIZE; i++)
    {
        sum = sum + readCircBuf (&g_inBuffer);
    }
    // Calculate and display the rounded mean of the buffer contents
    return (2 * sum + BUF_SIZE) / 2 / BUF_SIZE;
}

//*****************************************************************************
// Finds the error between the desired and the actual altitude of the heli
//
// @return int32_t The sampled and averaged adc reading from a circular buffer
//*****************************************************************************
int32_t
getAltitudeError(void) {
    // Process altitude gets the most up to date adc reading as an altitude
    currentAltitude = processAltitude();
    return setAltitude - currentAltitude;
}

//*****************************************************************************
//
// The handler for the ADC conversion complete interrupt.
// Writes to the circular buffer.
//
//*****************************************************************************
void
ADCIntHandler(void)
{
    uint32_t ulValue;

    //
    // Get the single sample from ADC0.  ADC_BASE is defined in
    // inc/hw_memmap.h
    ADCSequenceDataGet(ADC0_BASE, 3, &ulValue);
    //
    // Place it in the circular buffer (advancing write index)
    writeCircBuf (&g_inBuffer, ulValue);
    //
    // Clean up, clearing the interrupt
    ADCIntClear(ADC0_BASE, 3);
}

//*****************************************************************************
//
// The interrupt handler for the for SysTick interrupt
//
//*****************************************************************************
void
SysTickIntHandler(void)
{
    // Polls buttons, scheduler, adc and switches every system tick
    ADCProcessorTrigger(ADC0_BASE, 3);
    updateButtons();
    updateSwitches();
    updateScheduleTicks();
    checkResetSwitch();
}

//*****************************************************************************
//
// Initialises the ADC interrupt to read voltages, corresponding to different altitudes
//
//*****************************************************************************
void
initADC (void)
{
    //
    // The ADC0 peripheral must be enabled for configuration and use.
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);

    // Enable sample sequence 3 with a processor signal trigger.  Sequence 3
    // will do a single sample when the processor sends a signal to start the
    // conversion.
    ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);

    //
    // Configure step 0 on sequence 3.  Sample channel 0 (ADC_CTL_CH0) in
    // single-ended mode (default) and configure the interrupt flag
    // (ADC_CTL_IE) to be set when the sample is done.  Tell the ADC logic
    // that this is the last conversion on sequence 3 (ADC_CTL_END).  Sequence
    // 3 has only one programmable step.  Sequence 1 and 2 have 4 steps, and
    // sequence 0 has 8 programmable steps.  Since we are only doing a single
    // conversion using sequence 3 we will only configure step 0.  For more
    // on the ADC sequences and steps, refer to the LM3S1968 datasheet.
    ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_CH9 | ADC_CTL_IE |
                             ADC_CTL_END);

    //
    // Since sample sequence 3 is now configured, it must be enabled.
    ADCSequenceEnable(ADC0_BASE, 3);

    //
    // Register the interrupt handler
    ADCIntRegister (ADC0_BASE, 3, ADCIntHandler);

    //
    // Enable interrupts for ADC0 sequence 3 (clears any outstanding interrupts)
    ADCIntEnable(ADC0_BASE, 3);

    initCircBuf (&g_inBuffer, BUF_SIZE);
}

//*****************************************************************************
//
// Initialises the starting altitude as the current state of the adc averaged buffer
//
//*****************************************************************************
void initialiseAdcValue(void) {
    // Gets the current adc output from the buffer
    initialAdc = getAdcOutput();
}

//*****************************************************************************
// @param adcOutput The averaged output from the adc module, read from circular buffer
//
// @param initialAdc The first adc reading, indicated where the helicopter's initial position is
//
// @return int32_t The percentage altitude of the helicopters position
//
// Initialises the ADC interrupt to read voltages, corresponding to different altitudes
//
//*****************************************************************************
int32_t
percentageCalculator(int32_t adcOutput, double initialAdc)
{
    // Converts initial and current adc to a percentage
    double scaledInitial = initialAdc * PERCENTAGE_CONVERSION;
    double scaledCurrent = adcOutput * PERCENTAGE_CONVERSION;
    // Scales both readings by the voltage and divides the number of steps in adc
    double scaledInitialVoltage = (scaledInitial * MAX_VOLTAGE)/ADC_STEPS;
    double scaledVoltageOutput = (scaledCurrent * MAX_VOLTAGE)/ADC_STEPS;
    // Inversely proportional, so returns the negative of the difference between initial and current voltages
    return -(scaledVoltageOutput-scaledInitialVoltage);
}

//*****************************************************************************
// @param int16_t The change in altitude
//
// Increases the altitude by a certain change
//
//*****************************************************************************
void updateAltitude(int16_t change) {
    setAltitude += change;
    // Prevents the altitude from going out of bounds
    if (setAltitude > MAX_ALTITUDE) {
        setAltitude = MAX_ALTITUDE;
    } else if (setAltitude < MIN_ALTITUDE) {
        setAltitude = MIN_ALTITUDE;
    }
}


//*****************************************************************************
// @return int16_t The desired altitude
//
// Initialises the ADC interrupt to read voltages, corresponding to different altitudes
//
//*****************************************************************************
int16_t
getAltitudeSetpoint(void) {
    return setAltitude;
}

//*****************************************************************************
// @return int16_t The desired altitude
//
// Returns the altitude conversion of adc samples
//
//*****************************************************************************
int16_t
processAltitude(void) {
    // Receives the most recent adc value and processes it as a percentage
    currentAdc = getAdcOutput();
    currentAltitude = percentageCalculator(currentAdc, initialAdc);
    return currentAltitude;
}

//*****************************************************************************
// @return int16_t The initial adc reading, indicating 0 altitude
//
// Returns the initial altitude of altitude conversion
//
//*****************************************************************************
int32_t
getInitialAdc(void) {
    return initialAdc;
}

//*****************************************************************************
// @param value sets the desired altitude
//
// Sets the value for the heli to fly to
//
//*****************************************************************************
void
setAltitudeValue(int32_t value) {
    setAltitude = value;
}
