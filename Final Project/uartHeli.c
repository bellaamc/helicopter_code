/*
 * uartHeli.c
 *
 *  Created on: 10/05/2023
 *      Author: Amber Waymouth (awa155)
 *              Arabella Cryer (acr151)
 *      Module for UART serial transmission of helicopter information
 */


#include "uartHeli.h"

/********************************************************
 * Static variables
 ********************************************************/
static char uartString[UART_VAL_LEN]; // String of length 16 for storing information to be transmitted

/********************************************************
 * Initialise UART peripherals and GPIO pins and UART configurations
 ********************************************************/
void
initUart(void)
{
    // Enable GPIO port A which is used for UART0 pins.
    //
    SysCtlPeripheralEnable(UART_USB_PERIPH_UART);
    SysCtlPeripheralEnable(UART_USB_PERIPH_GPIO);
    //
    // Select the alternate (UART) function for these pins.
    //
    GPIOPinTypeUART(UART_USB_GPIO_BASE, UART_USB_GPIO_PINS);
    GPIOPinConfigure (GPIO_PA0_U0RX);
    GPIOPinConfigure (GPIO_PA1_U0TX);

    UARTConfigSetExpClk(UART_USB_BASE, SysCtlClockGet(), BAUD_RATE,
            UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
            UART_CONFIG_PAR_NONE);
    UARTFIFOEnable(UART_USB_BASE);
    UARTEnable(UART_USB_BASE);
}

/********************************************************
 * @param char

 * UARTSend() takes a point to an input buffer and transmits
 * each character in the buffer one at a time
 ********************************************************/
void
UARTSend(char *charInputBuffer)
{
    while(*charInputBuffer)
    {
        UARTCharPut(UART_USB_BASE, *charInputBuffer);
        charInputBuffer++;
    }
}

/********************************************************
 * Function to update the information displayed on the terminal by
 * regularly sending information to the terminal via UART
 ********************************************************/
void
updateUART(void) {
    // Main rotor PWM duty cycle
    usprintf(uartString, "Main Duty %d\r\n", getAltitudePwm());
    UARTSend(uartString);

    // Tail rotor PWM duty cycle
    usprintf(uartString, "Tail Duty %d\r\n", getYawPwm());
    UARTSend(uartString);

    // Current helicopter state
    usprintf(uartString, "State %s\r\n", heliStateStr());
    UARTSend(uartString);

    // Desired altitude
    usprintf(uartString, "Desired Alt %4d\r\n", getAltitudeSetpoint());
    UARTSend(uartString);

    // Actual altitude
    usprintf(uartString, "Actual Alt %4d\r\n", processAltitude());
    UARTSend(uartString);

    // Desired yaw
    usprintf(uartString, "Desired Yaw %4d.0 \r\n", getYawSetpoint());
    UARTSend(uartString);

    // Actual yaw
    usprintf(uartString, "Actual Yaw %3d.%1d\r\n\n", getYawOutput(), getYawDecimal());
    UARTSend(uartString);
}
