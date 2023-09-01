/*
 * yaw.c
 *
 *  Created on: 10/05/2023
 *      Author: Amber Waymouth (awa155)
 *              Arabella Cryer (acr151)
 *
 *  Module for yaw functionality
 */

#include "yaw.h"

//*****************************************************************************
// Global variables
//*****************************************************************************
static uint8_t currentYaw = 0;
static uint8_t previousYaw = 0;

//Raw yaw like 448 val
static int16_t yaw = 0;

//Number from -180 to 180
static int16_t yawAngle = 0;

//Desired yaw value
static int16_t setYaw = 0;

// Binary flag indicating reference yaw has been found
static bool findReference = false;

//*****************************************************************************
// @return int16_t yaw angle from the static variable
//
// Converts quadrature encoding ticks to an angle
//*****************************************************************************
int16_t
getYawOutput(void)
{
    yawAngle = (yaw * FULL_CIRCLE) / FULL_CIRCLE_SLOTS;
    return yawAngle;
}

//*****************************************************************************
// @return int16_t yaw angle from the static variable
//
// Finds the extra decimal point of precision for the yaw angle
//*****************************************************************************
int16_t
getYawDecimal(void)
{
    int16_t decimalVal = abs(((yaw * FULL_CIRCLE * FLOAT_CONVERSION)/ FULL_CIRCLE_SLOTS) % FLOAT_CONVERSION);
    return decimalVal;
}

//*****************************************************************************
// Sets the interrupt for the quadrature encoder for both edges of both encoder pins
//*****************************************************************************
void
interruptSetQuadratureEncoder(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    GPIOPinTypeGPIOInput(YAW_BASE, GPIO_PIN_0|GPIO_PIN_1);
    GPIOPadConfigSet(YAW_BASE, GPIO_PIN_0|GPIO_PIN_1, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
    GPIOIntEnable(YAW_BASE, GPIO_INT_PIN_0 | GPIO_INT_PIN_1);
    GPIOIntTypeSet(YAW_BASE, YAW_A_PIN | YAW_B_PIN, GPIO_BOTH_EDGES);
    GPIOIntRegisterPin(YAW_BASE, YAW_A_PIN | YAW_B_PIN, quadratureHandler);
    IntRegister(INT_GPIOB, quadratureHandler);
    IntEnable(INT_GPIOB);
}

//*****************************************************************************
// The handler for when the yaw quadrature encoder is detected
//*****************************************************************************
void
quadratureHandler(void)
{
    GPIOIntClear(YAW_BASE, YAW_A_PIN | YAW_B_PIN);
    previousYaw = currentYaw;
    uint8_t yaw_a = GPIOPinRead(YAW_BASE, YAW_A_PIN);
    uint8_t yaw_b = GPIOPinRead(YAW_BASE, YAW_B_PIN);
    currentYaw = yaw_a | yaw_b;
    switch(currentYaw) {
    case(0b00):
        if (previousYaw == 0b10) {
            yaw--;
        } else if (previousYaw == 0b01) {
            yaw++;
        }
        break;
    case(0b01):
        if (previousYaw == 0b00) {
            yaw--;
        } else if (previousYaw == 0b11) {
            yaw++;
        }
        break;
    case(0b10):
        if (previousYaw == 0b11) {
            yaw--;
        } else if (previousYaw == 0b00) {
            yaw++;
        }
        break;
    case(0b11):
        if (previousYaw == 0b01) {
            yaw--;
        } else if (previousYaw == 0b10) {
            yaw++;
        }
        break;
    }
    if (yaw == FULL_CIRCLE_SLOTS/2) {
        yaw = -FULL_CIRCLE_SLOTS/2;
    } else if (yaw == (-FULL_CIRCLE_SLOTS/2)-1) {
        yaw = (FULL_CIRCLE_SLOTS/2)-1;
    }
}

//*****************************************************************************
// @return int16_t the yaw for the helicopter to move towards
//
// Returns the static variable for the desired yaw
//*****************************************************************************
int16_t
getYawSetpoint(void)
{
    return setYaw;
}

//*****************************************************************************
// @return int16_t The error between the desired and the true yaw angle
//
// Calculates and returns the error between the desired and true yaw angle,
// converts it to find the shortest distance to the desired angle.
//*****************************************************************************
int16_t
getYawError(void) {
    yawAngle = getYawOutput();
    int16_t yawError = setYaw - yawAngle;
    if (yawError > 180) {
        yawError = (360 - yawError);
    } else if (yawError < -180) {
        yawError = -(360 + yawError);
    }
    return yawError;
}

//*****************************************************************************
// @param int16_t change in yaw angle
//
// Updates the set yaw based on the desired change from the current set yaw
//*****************************************************************************
void
setYawSetpoint(int16_t change)
{

    setYaw += change;
    if (setYaw > 180) {
        setYaw -= 360;
    } else if (setYaw < -180) {
        setYaw += 360;
    }
}

//*****************************************************************************
// Sets the interrupt for the reference pin
//*****************************************************************************
void
interruptSetReference(void)
{
     SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
     GPIOPinTypeGPIOInput(REF_BASE, REF_PIN);
     GPIOPadConfigSet(REF_BASE, REF_PIN, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
     GPIOIntEnable(REF_BASE, REF_PIN);
     GPIOIntTypeSet(REF_BASE, REF_PIN, GPIO_FALLING_EDGE);
     GPIOIntRegisterPin(REF_BASE, REF_PIN, yawReferenceHandler);
     IntRegister(INT_GPIOC, yawReferenceHandler);
     IntEnable(INT_GPIOC);
 }

//*****************************************************************************
// Handles the interrupt called by the reference pin. Resets the yaw angles to 0,
// indicating the 0 degree point.
//*****************************************************************************
void
yawReferenceHandler(void)
{
    GPIOIntClear(REF_BASE, REF_PIN);
    if ((getState() == TAKING_OFF) || (getState() == FINDING_REF)) {
        uint8_t reference_read = GPIOPinRead(REF_BASE, REF_PIN);
        yaw = 0;
        currentYaw = 0;
        previousYaw = 0;
        setYaw = 0;
        findReference = true;
    }
}

//*****************************************************************************
// Sets the yaw to go back to 0 degrees
//*****************************************************************************
void
resetYaw(void) {
    setYaw = 0;
}

//*****************************************************************************
// Resets the reference interrupt handler boolean
//*****************************************************************************
void
updateReference(void) {
    findReference = false;
}

//*****************************************************************************
// @return bool Returns true once the reference interrupt is called
//
// Gets the helicopter to do a 360 turn until the reference point is found.
//*****************************************************************************
bool
fullRevolution(void) {
    if (!findReference) {
        setYawPwm(REVOLUITON_PWM);
    }
    return findReference;
}
