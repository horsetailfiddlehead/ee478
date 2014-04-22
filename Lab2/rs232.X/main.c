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
//#include <delays.h>
#pragma setup FOSC = ECHP
#pragma setup WDTEN = OFF

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

int main() {
    OpenTimer0( TIMER_INT_OFF & T0_SOURCE_INT & T0_PS_1_32 );

    Open1USART( USART_TX_INT_OFF & USART_RX_INT_OFF & USART_ASYNCH_MODE &
                USART_EIGHT_BIT & USART_CONT_RX & USART_BRGH_HIGH, 129 );
    
    while (1) {
        Write1USART("hello");
        
    }
    
    Close1USART();

    return (0);
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
