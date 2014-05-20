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
#include <string.h>

#include "interrupts.h"
#include "rs232.h"
#include "usart.h"
#include "delays.h"
#include <p18f25k22.h>

#define INPUT_LENGTH 64	// size of input buffer

#define START 01
#define FLAGS 0304
#define READ_SINGLE_BLOCK 0
#define READ 0
#define WRITE 0

#define STAY_QUIET "01120003041822023FBD371B000007E00000"
#define AGC "0109000304F0000000"
#define REG_WRITE "010C00030410002101020000"
#define PING "0108000304FF0000"
#define INVENTORY "010B000304140601000000"

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
char readUID[INPUT_LENGTH] = { 0 };
int inputSpot2 = 0;
short nextBlock = 0;

void rcISR(void) {
    unsigned char input;
    // Don't have to wait for data available if we are in ISR
    input = getc2USART();

    if (strcmppgm2ram(myInput2, "inventory") == 0) {
        if (input == '[') {
            inputSpot2 = 0;
            nextBlock = 0;
        }

        if (readUID[inputSpot2-1] == '[' && input == ',') {
            nextBlock = 1;
        } else if (nextBlock == 0 && (input == '[' || readUID[0] == '[')) {
            readUID[inputSpot2] = input;
            if (input == '\n') {
                readUID[inputSpot2] == '\0';
                inputSpot2 = 0;
            }
            inputSpot2++;
        }
    } else {
     
        Write1USART(input);
    }

    // Clear interrupt
    PIR3bits.RC2IF = 0;
}

/****************************************************/
//int inputFinished;
//char myInput;
//int i;


void setupRead();
void sendToRFID(char* myString);
void resetRFID();

void main() {
    int i;
    
    TRISBbits.RB5 = 0;
    ANSELBbits.ANSB5 = 0;

    rs232Setup1(); // sets pc RX=C7, tx=C6
    rs232Setup2(); // sets dlp rx=b7, tx=b6

    resetRFID();

    // Get RFID attention
    sendToRFID("0");

    while (1) {
        readBytesUntil(myInput2, '\r', INPUT_LENGTH);
        putc1USART('\r');
        putc1USART('\n');
        i = 0;
        if (strcmppgm2ram(myInput2, "ping") == 0) {
            sendToRFID(PING);
        } else if (strcmppgm2ram(myInput2, "inventory") == 0) {
            setupRead();
            sendToRFID(INVENTORY);
            Delay10TCYx(100);
            puts1USART(readUID);
        } else if (strcmppgm2ram(myInput2, "quiet") == 0) {
            sendToRFID(STAY_QUIET);
        } else {
            resetRFID();
        }
    }
    return;
}

void sendToRFID(char* myString) {
    char myInput[INPUT_LENGTH];
    short inputFinished = 0;
    int i = 0;
    strcpypgm2ram(myInput, myString);
    while(!inputFinished) {
        if (myInput[i] != '\0') {
            while(Busy2USART());
            Write2USART(myInput[i]);
            i++;
        } else {
            inputFinished = 1;
        }
    }
    Delay10TCYx(100);
    return;
}

void setupRead() {
    sendToRFID(REG_WRITE);
    sendToRFID(AGC);
    return;
}

void resetRFID() {
    
    PORTBbits.RB5 = 1;
    PORTBbits.RB5 = 0;
    PORTBbits.RB5 = 1;

}