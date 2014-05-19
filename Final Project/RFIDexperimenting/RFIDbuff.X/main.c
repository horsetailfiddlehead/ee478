/* 
 * File:   ioBuffer.c
 * Author: Patrick
 *
 * Created on May 15, 2014
 * a simple program that buffers user serial input then sends it out 
 * sequentially all at once.
 */

#include <stdio.h>
#include <stdlib.h>

#include "rs232.h"
#include "interrupts.h"
#include "usart.h"
#include <p18f25k22.h>

#define INPUT_LENGTH 64	// size of input buffer

/***************USART set up *********************/
#pragma config FCMEN = OFF
#pragma config IESO = OFF
/****************************************************/

/***************Clocking set up *********************/
#pragma config WDTEN = OFF    // turn off watch dog timer
#pragma config FOSC = ECHP    // Ext. Clk, Hi Pwr
#pragma config PRICLKEN = OFF // disable primary clock


#pragma code high_vector=0x08

void interrupt_at_high_vector(void) {
    _asm GOTO rcISR _endasm
}
#pragma code

#pragma interrupt rcISR

char myInput2[INPUT_LENGTH];
int inputSpot2 = 0;

void rcISR(void) {
    unsigned char input;
    // Don't have to wait for data available if we are in ISR
    input = getc2USART();

    Write1USART(input);
    /*
    // Terminate string on enter or max size
    if (input >= ' ' && input <= 'z' || input == '\r' || input == '\n') {
        if (input == '\r' || input == '\n' || inputSpot2 == (INPUT_LENGTH - 1)) {
            myInput2[inputSpot2] = '\0';

            // Reset input, declare finished
            inputSpot2 = 0;
        } else {
            // Put character in the input buffer
            myInput2[inputSpot2] = input;
            inputSpot2++;

            // Print current character
            Write1USART(input);
        }
    } else {
        Write1USART(input);
    }
    */

    // Clear interrupt
    PIR3bits.RC2IF = 0;
}
 
/****************************************************/
//int inputFinished;
//char myInput;
//int i;

void main() {
    char myInput[INPUT_LENGTH];
    int inputFinished;
    int i;

    rs232Setup1(); // sets pc RX=C7, tx=C6
    rs232Setup2(); // sets dlp rx=b7, tx=b6
    while (1) {
        readBytesUntil(&myInput, '\r', INPUT_LENGTH);

        inputFinished = 0;
        i = 0;
        while (!inputFinished) {
            if (myInput[i] != '\0') {
                while(Busy2USART());
                Write2USART(myInput[i]);
                i++;
            } else {
                inputFinished = 1;

            }
        }
    }
    return;
}