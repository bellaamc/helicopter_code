/*
 * switches.c
 *
 *  Created on: 11/05/2023
 *      Author: Amber Waymouth (awa155)
 *              Arabella Cryer (acr151)
 *
 *  Module for takeoff/landing switch (SW1) and reset switch (SW2)
 */


#include "switches.h"

/**********************************************************
 * Static variables
 **********************************************************/
static bool switch_state[NUM_SWITCHES];     // Stores the state for each switch
static uint8_t switch_count[NUM_SWITCHES];  // Debouncing
static bool switch_flag[NUM_SWITCHES];      // Binary flag indicating if a switch value has changed
static bool switch_normal[NUM_SWITCHES];    // Default state of switch

/**********************************************************
 * Initialise switch 1
 **********************************************************/
void
initSwitch(void)
{
    int i;
    //SW1 (takeoff/landing)
    SysCtlPeripheralEnable(SW_PERIPH);
    GPIOPinTypeGPIOInput(SW_PORT, SW1_GPIO_PIN);
    GPIOPadConfigSet(SW_PORT, SW1_GPIO_PIN, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPD);
    switch_normal[LEFT_SWITCH] = SWITCH_LEFT_NORMAL;
    //SW2 (reset switch)
    switch_normal[RIGHT_SWITCH] = SWITCH_RIGHT_NORMAL;
    GPIOPinTypeGPIOInput(SW_PORT, SW2_GPIO_PIN);
    GPIOPadConfigSet(SW_PORT, SW2_GPIO_PIN, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPD);

    for (i = 0; i < NUM_SWITCHES; i++)
        {
        switch_state[i] = switch_normal[i];
        switch_count[i] = 0;
        switch_flag[i] = false;
        }
}

/**********************************************************
 * @return uint8_t
 * Gets and returns the value of switch 1
 **********************************************************/
uint8_t
getSwitchOneValue(void)
{
    return GPIOPinRead(SW_PORT, SW1_GPIO_PIN);
}

/**********************************************************
 * @return uint8_t
 * Gets and returns the value of switch 2
 **********************************************************/
uint8_t
getSwitchTwoValue(void)
{
    return GPIOPinRead(SW_PORT, SW2_GPIO_PIN);
}

/**********************************************************
 * Updates the switch values
 * Change in switch state only occurs after NUM_SWITCH_POLLS have read the
 * switch in the opposite direction
 * Switch state is then changed and switch_flag is set
 **********************************************************/
void
updateSwitches(void) {
    bool switch_value[NUM_SWITCHES];
    int i;

    // Read the pins; true means HIGH, false means LOW
    switch_value[RIGHT_SWITCH] = (getSwitchOneValue() == SW1_GPIO_PIN);
    switch_value[LEFT_SWITCH] = (getSwitchTwoValue() == SW2_GPIO_PIN);
    // Iterate through the switches, updating switch variables as required
    for (i = 0; i < 2; i++)
    {
        if (switch_value[i] != switch_state[i])
        {
            switch_count[i]++;
            if (switch_count[i] >= NUM_SWITCH_POLLS)
            {
                switch_state[i] = switch_value[i];
                switch_flag[i] = true;    // Reset by call to checkSwitch()
                switch_count[i] = 0;
            }
        }
        else
            switch_count[i] = 0;
    }
}


/**********************************************************
 * @return uint8_t
 * @param switch number
 * checkSwitch returns the new switch logical state if the switch state
 * (SWITCH_UP, SWITCH_DOWN) has changed otherwise return SWITCH_UNCHANGED
 **********************************************************/

uint8_t
checkSwitch (uint8_t switchName)
{
    if (switch_flag[switchName])
    {
        switch_flag[switchName] = false;
        if (switch_state[switchName] == switch_normal[switchName])
            return SWITCH_UP;
        else
            return SWITCH_DOWN;
    }
    return SWITCH_NO_CHANGE;
}

/**********************************************************
 * Check if reset switch is HIGH
 * Calls SysCtlReset() to reset system
 **********************************************************/
void
checkResetSwitch() {
    if (checkSwitch(LEFT_SWITCH) == SWITCH_DOWN) {
        SysCtlReset();
    }
}

