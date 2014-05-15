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
#include "interrupts.h

#define INPUT_LENGTH 50	// size of input buffer

/***************USART set up *********************/
#pragma config FCMEN = OFF
#pragma config IESO = OFF
/****************************************************/

/***************Clocking set up *********************/
#pragma config WDTEN = OFF    // turn off watch dog timer
#pragma config FOSC = ECHP    // Ext. Clk, Hi Pwr
#pragma config PRICLKEN = OFF // disable primary clock

/*
#pragma code high_vector=0x08

void interrupt_at_high_vector(void) {
    _asm GOTO rcISR _endasm
}
#pragma code

#pragma interrupt rcISR

void rcISR(void) {
    unsigned char input;
    // Don't have to wait for data available if we are in ISR
    input = getc1USART();

    // Terminate string on enter or max size
    if (input >= ' ' && input <= 'z' || input == '\r' || input == '\n') {
        if (input == '\r' || input == '\n' || inputSpot == (IN_BUF_SZ - 1)) {
            myInput[inputSpot] = '\0';

            // Reset input, declare finished
            inputSpot = 0;
            inputFinished = 1;
            *ourGlobal.processFlag = 1;
        } else {
            // Put character in the input buffer
            myInput[inputSpot] = input;
            inputSpot++;

            // Print current character
            putc1USART(input);
            inputFinished = 0;
        }
    } else {
        putc1USART(input);
    }

    // Clear interrupt
    PIR1bits.RCIF = 0;
}
*/
/****************************************************/

int main() {
char myInput[INPUT_LENGTH] = \0;
int inputFinished;

rs232Setup1(); // sets pc RX=C7, tx=C6
rs232Setup2();	// sets dlp rx=b7, tx=b6
while(1) {
readBytesUntil(&myInput, '\n', INPUT_LENGTH);

inputFinished = 0;
int i = 0;
while(!inputFinished) {
	if (myInput[i] != '\0') {
		puts2USART(myInput[i]);
	} else {
	inputFinished = 1;
	i++;
	}
}

return EXIT_SUCCESS;
}