/*
 * File:   main.c
 * Author: rynmcd
 *
 * Created on April 21, 2014, 4:10 PM
 */

#include <stdlib.h>
#include <p18f25k22.h>
//#include <sw_uart.h>
#include <usart.h>
#include <timers.h>
#include <delays.h>
#pragma setup FOSC = ECHP
#pragma setup WDTEN = OFF
#pragma setup PBADEN = OFF
#pragma setup PRICLKEN = OFF
#pragma setup FCMEN = OFF
#pragma setup IESO = OFF
/*
#define SWTXDpin 6
#define SWRXDpin 7
*/

#define CLOCK 20000000
#define BAUD 9600

/*
 *
 */

/*
void DelayTXBitUART();
void DelayRXHalfBitUART();
void DelayRXBitUART();
*/

void main(void)
{
    unsigned char myWord[5];
    myWord[0] = 'a';
    myWord[1] = 's';
    myWord[2] = 's';
    myWord[3] = ' ';
    myWord[4] = '\0';

    //  Set all of PORTC as outputs. TX and RX must be set as outputs first
    TRISC=0x00;

    //  Configure UART
    Open1USART(USART_TX_INT_OFF & USART_RX_INT_OFF & USART_ASYNCH_MODE & USART_EIGHT_BIT & USART_BRGH_HIGH, 129);
    while(1)
    {
        puts1USART(myWord);
    }
}

/*
void DelayTXBitUART() {
    Delay1TCY(((((2*CLOCK) / (4*BAUD)) + 1) / 2) - 12);
}
void DelayRXHalfBitUART() {
    Delay1TCY(((((2*CLOCK) / (8*BAUD)) + 1) / 2) - 9);
}
void DelayRXBitUART() {
    Delay1TCY(((((2*CLOCK) / (4*BAUD)) + 1) / 2) - 14);
}
 */
