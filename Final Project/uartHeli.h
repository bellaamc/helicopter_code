/*
 * uartHeli.h
 *
 *  Created on: 10/05/2023
 *      Author: Amber Waymouth (awa155)
 *              Arabella Cryer (acr151)
 *
 *  Header file for uartHeli module
 */

#ifndef UARTHELI_H_
#define UARTHELI_H_

/********************************************************
 * Header files
 ********************************************************/
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "stdlib.h"
#include "utils/ustdlib.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "utils/uartstdio.h"
#include "driverlib/uart.h"
#include "driverlib/pin_map.h"
#include "yaw.h"
#include "altitude.h"
#include "pwm.h"

/********************************************************
 * Constants
 ********************************************************/
//---USB Serial comms: UART0, Rx:PA0 , Tx:PA1
#define BAUD_RATE 9600
#define UART_USB_BASE           UART0_BASE
#define UART_USB_PERIPH_UART    SYSCTL_PERIPH_UART0
#define UART_USB_PERIPH_GPIO    SYSCTL_PERIPH_GPIOA
#define UART_USB_GPIO_BASE      GPIO_PORTA_BASE
#define UART_USB_GPIO_PIN_RX    GPIO_PIN_0
#define UART_USB_GPIO_PIN_TX    GPIO_PIN_1
#define UART_USB_GPIO_PINS      UART_USB_GPIO_PIN_RX | UART_USB_GPIO_PIN_TX
#define UART_CONFIGURATIONS UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE
#define BAUD_RATE 9600
#define STR_LEN 16
#define UART_VAL_LEN STR_LEN + 1
#define MAX_UART_TICKS 120

/********************************************************
 * Initialise UART peripherals and GPIO pins and UART configurations
 ********************************************************/
void
initUart(void);

/********************************************************
 * @param char

 * UARTSend() takes a point to an input buffer and transmits
 * each character in the buffer one at a time
 ********************************************************/
void
UARTSend(char *charInputBuffer);

/********************************************************
 * Function to update the information displayed on the terminal by
 * regularly sending information to the terminal via UART
 ********************************************************/
void
updateUART(void);

#endif /* UARTHELI_H_ */
