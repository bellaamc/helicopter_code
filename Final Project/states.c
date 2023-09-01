/*
 * states.c
 *
 *  Created on: 02/05/2023
 *      Author: Amber Waymouth (awa155)
 *              Arabella Cryer (acr151)
 *
 *  Header file for the states module
 */

#include "states.h"

/**********************************************************
 * Static variables
 **********************************************************/
static helicopterState_t state;     // Stores the current state of the helicopter

static bool landingCheckForRef = 0; // Binary flag for detecting the yaw reference
static bool checkForReference = 0;  // Binary flag for detecting the yaw reference

/**********************************************************
 * @return helicopterState_t
 * getState() return the current state of the helicopter
 **********************************************************/
helicopterState_t getState(void)
{
    return state;
}

/**********************************************************
 * setState() sets the current state of the helicopter
 **********************************************************/
void setState (helicopterState_t heliState)
{
    state = heliState;
}

/**********************************************************
 * @return char*
 * heliStateStr() returns a string of the current helicopter state
 **********************************************************/
char* heliStateStr(void) {
    switch (state) {
        case LANDED: return "Landed";
        case TAKING_OFF: return "Taking off";
        case FLYING: return "Flying";
        case FINDING_REF: return "Finding Ref";
        case LANDING: return "Landing";
        default: return "";
    }
}

/**********************************************************
 * checkButtonState() checks the state of the helicopter then the
 * state of the buttons if the helicopter is FLYING and
 * updates altitude and yaw with the defined step values
 **********************************************************/
void checkButtonState (void)
{
    if (getState() == FLYING) {
        // Increase altitude by 10%
        // Set altitude PWM
        if (checkButton(UP) == PUSHED) {
            updateAltitude(ALTITUDE_INCREASE);
            setAltitudePwm(ALTITUDE_DUTY_STEP);
        }
        // Decrease altitude by 10%
        // Set altitude PWM
        if (checkButton(DOWN) == PUSHED) {
            updateAltitude(ALTITUDE_DECREASE);
            setAltitudePwm(-ALTITUDE_DUTY_STEP);
        }
        // Increase yaw by 15 degrees
        // Set yaw PWM
        if (checkButton(RIGHT) == PUSHED) {
            setYawSetpoint(YAW_INCREASE);
            setYawPwm(YAW_DUTY_STEP);
        }
        // Decrease altitude by 15 degrees
        // Set yaw PWM
        if (checkButton(LEFT) == PUSHED) {
            setYawSetpoint(YAW_DECREASE);
            setYawPwm(-YAW_DUTY_STEP);
        }
    }
}

/**********************************************************
 * stateMachine() sets the helicopter functionality based on
 * the current state
 **********************************************************/
void
stateMachine(void)
{
    helicopterState_t helicopterState = getState();
    switch(helicopterState) {
        // Stop all PWM output
        // Poll to check if the switch state has changed to UP
        // Change to TAKING_OFF when switch state changes
        case LANDED:
            stopTailPWM();
            stopMainPWM();
            if (checkSwitch(RIGHT_SWITCH) == SWITCH_UP) {
                setState(TAKING_OFF);
            }
            break;

        // Start main and tail rotor PWM output
        // Hover at 10% altitude
        // Disable yaw control for finding reference yaw
        // Change to FLYING once reference yaw found
        case TAKING_OFF:
            startTailPWM();
            startMainPWM();
            enableYawControl(false);
            setAltitudeValue(HOVER_ALTITUDE);
            checkForReference = fullRevolution();
            if (checkForReference) {
                setState(FLYING);
            }
            break;

        // Ensure PWM is on
        // Enable yaw control
        // Poll for switch down change
        // Change to FINDING_REF if switch is put down
        case FLYING:
            startTailPWM();
            startMainPWM();
            enableYawControl(true);
            if (checkSwitch(RIGHT_SWITCH) == SWITCH_DOWN) {
                setState(FINDING_REF);
                updateReference();
            }
            break;

        // Look for the reference yaw
        // Change to LANDING once reference yaw found
        case FINDING_REF:
            landingCheckForRef = fullRevolution();
            enableYawControl(false);
            if (landingCheckForRef) {
                enableYawControl(true);
                setState(LANDING);
            }
            break;

        // Change to landing PWM
        // Change to LANDED when altitude <= 1
        case LANDING:
            setAltitudePwm(LANDING_PWM);
            if (processAltitude() <= 1) {
                setState(LANDED);
            }
            break;
    }

}

