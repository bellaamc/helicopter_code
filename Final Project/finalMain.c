/*
 * finalMain.c
 *
 *  Created on: 30/04/2023
 *      Author: Amber Waymouth (awa155)
 *              Arabella Cryer (acr151)
 *
 *  Module containing main kernel for helicopter functionality
 */

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "driverlib/adc.h"
#include "OrbitOLED/OrbitOLEDInterface.h"
#include "buttons4.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "utils/ustdlib.h"
#include "stdlib.h"
#include "OrbitOLED/OrbitOLEDInterface.h"
#include "buttons4.h"
#include "circBufT.h"
#include "yaw.h"
#include "states.h"
#include "altitude.h"
#include "pwm.h"
#include "pidController.h"
#include "display.h"
#include "scheduler.h"
#include "switches.h"
#include "uartHeli.h"

//*****************************************************************************
// Constants
//*****************************************************************************
#define CONTROL_SCHEDULER_RATE SYSTICK_RATE_HZ / 15
#define BUTTON_SCHEDULER_RATE SYSTICK_RATE_HZ / 30
#define DISPLAY_SCHEDULER_RATE SYSTICK_RATE_HZ / 10
#define STATE_MACHINE_SCHEDULER_RATE SYSTICK_RATE_HZ / 15
#define UART_SCHEDULER_RATE SYSTICK_RATE_HZ / 2

/*************************************************************
 * SysTick interrupt
 ************************************************************/
void
initSysTick (void)
{
    //
    // Set up the period for the SysTick timer.  The SysTick
    // timer period is set as a function of the system clock.
    SysTickPeriodSet (SysCtlClockGet() / SAMPLE_RATE_HZ);
    //
    // Register the interrupt handler
    SysTickIntRegister (SysTickIntHandler);
    //
    // Enable interrupt and device
    SysTickIntEnable ();
    SysTickEnable ();
}

//*****************************************************************************
// Main loop
//*****************************************************************************
int
main(void)
{
    // Disable interrupts to prevent initialisations being disrupted by interrupts
    // Prevent uninitialised interrupts from occurring
    IntMasterDisable();
    //Initialisations
    initClock();
    initSwitch();
    setState(LANDED);
    initUart();
    initSysTick();
    initADC();
    initPWMPeripherals();
    initButtons ();
    initialisePWM ();
    initDisplay ();
    initialiseTaskList();
    interruptSetQuadratureEncoder();
    interruptSetReference();
    initialiseTask(updateControl, CONTROL_SCHEDULER_RATE, 0);
    initialiseTask(checkButtonState, BUTTON_SCHEDULER_RATE, 1);
    initialiseTask(displaySchedulerFunc, DISPLAY_SCHEDULER_RATE, 2);
    initialiseTask(stateMachine, STATE_MACHINE_SCHEDULER_RATE, 3);
    initialiseTask(updateUART, UART_SCHEDULER_RATE, 4);
    IntMasterEnable();

    SysCtlDelay (SysCtlClockGet()/8); // Delay to allow crystal to settle for three cycles
    initialiseAdcValue();

    startScheduler();
}



