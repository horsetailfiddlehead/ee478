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
#define MAX_UIDS 3

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

/****************************************/
// These should be in a global structure

// User input
char myInput2[INPUT_LENGTH];

// Read UIDs, length can be optimized
// Currently can read only 3 UIDs before we get errors based on the size
// of the array
char readUID[MAX_UIDS][INPUT_LENGTH];

// Current spot in the array processing for input from RFID
int inputSpot2 = 0;

// The UID that we are reading. (first, second, etc)
int numUID = 0;

// Weather or not we are in a block of square brackets
short nextBlock = 0;

// If the user input was an inventory command
short invCom = 0;

/****************************************/

void rcISR(void) {
    // The input character from UART2 (the RFID reader)
    unsigned char input;

    // Don't have to wait for data available if we are in ISR
    input = getc2USART();

    // If we are processing an Inventory command
    if (invCom == 1) {

        // A 'D' character outside of square brackets indicates that the inventory
        // command has finished sending
        if (input == 'D' && nextBlock == 0) {
            // Reset the inventory command flag
            invCom = 0;

        // Begin reading what is inside a block of square brackets
        } else if (input == '[') {
            // Go to the beginning of the array, indicate that a block is being read
            inputSpot2 = 0;
            nextBlock = 1;

        // If we are at the end of a block of square brackets
        } else if(input == ']' && numUID < MAX_UIDS && nextBlock == 1) {
            // If there is a comma as the first character inside a block, then
            // discard what is read.  Otherwise, terminate the string and increment
            // the number of UIDs successfully read.
            if (readUID[numUID][0] != ',') {
                readUID[numUID][inputSpot2] = '\0';
                numUID++;
            }

            // Block of square brackets has be read, set the indicator to zero
            nextBlock = 0;

        // Put anything inside of a square bracket into the UID array
        } else if (nextBlock == 1 && inputSpot2 < INPUT_LENGTH && numUID < MAX_UIDS) {
            readUID[numUID][inputSpot2] = input;
            inputSpot2++;

        // If we are outside of a block, reset read position and ensure that the block
        // state indicator is zero.
        } else {
            inputSpot2 = 0;
            nextBlock = 0;
        }
        
    } else {
        // Echo back typed character
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

    // Controls the RESET for the RFID reader
    TRISBbits.RB5 = 0;
    ANSELBbits.ANSB5 = 0;

    // Set up UART to computer and RFID
    rs232Setup1(); // sets pc RX=C7, tx=C6
    rs232Setup2(); // sets dlp rx=b7, tx=b6

    // Start the RFID with a reset
    resetRFID();

    // Get RFID attention
    sendToRFID("0");

    while (1) {
        // Read user input from computer
        readBytesUntil(myInput2, '\r', INPUT_LENGTH);
        putc1USART('\r');
        putc1USART('\n');

        // Ping command
        if (strcmppgm2ram(myInput2, "ping") == 0) {
            sendToRFID(PING);

        // Inventory Command
        } else if (strcmppgm2ram(myInput2, "inventory") == 0) {
            // Set the inventory command flag for the interrupt
            invCom = 1;

            // Set the RFID reader to Read mode and send the Inventory command
            setupRead();
            sendToRFID(INVENTORY);

            // Wait until interrupt finishes
            while(invCom == 1);

            // Print all the UIDs
            for (i = 0; i < numUID; i++) {
                puts1USART(readUID[i]);
                putc1USART('\r');
                while(Busy1USART());
                putc1USART('\n');
            }

            // Reset the number of UIDs read
            numUID = 0;

        // Send the "Stay Quiet" command.
        // WARNING: THIS IS HARDCODED TO ONLY WORK WITH THE PROTOCARD
        } else if (strcmppgm2ram(myInput2, "quiet") == 0) {
            sendToRFID(STAY_QUIET);

        // Any errors will reset the RFID reader
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