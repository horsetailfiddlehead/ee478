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
#include <p18f46k22.h>

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

#define UID_LEN 25
/****************************************/
// These should be in a global structure
typedef struct testStruct {
    char myInput2[INPUT_LENGTH];
    char readUID[MAX_UIDS][UID_LEN];
    int inputSpot2;
    int numUID;
    short nextBlock;
    short invCom;
} Sloww;

Sloww butt;
// User input


// Read UIDs, length can be optimized
// Currently can read only 3 UIDs before we get errors based on the size
// of the array


// Current spot in the array processing for input from RFID


// The UID that we are reading. (first, second, etc)


// Weather or not we are in a block of square brackets


// If the user input was an inventory command

/****************************************/

void rcISR(void) {
    // The input character from UART2 (the RFID reader)
    unsigned char input;
    PORTAbits.RA0 = 1;

    // Don't have to wait for data available if we are in ISR
    input = RCREG;

    // If we are processing an Inventory command
    // time 32.2 us
    if (butt.invCom == 1) {
        //Write2USART(input);
        // A 'D' character outside of square brackets indicates that the inventory
        // command has finished sending
        if (input == 'D' && butt.nextBlock == 0) {
            // Reset the inventory command flag
            butt.invCom = 0;

        // Begin reading what is inside a block of square brackets
            // 18 us
        } else if (input == '[') {
            // Go to the beginning of the array, indicate that a block is being read
            butt.inputSpot2 = 0;
            butt.nextBlock = 1;

        // If we are at the end of a block of square brackets
            // 40.5 us
        } else if(input == ']' && butt.numUID < MAX_UIDS && butt.nextBlock == 1) {
            // If there is a comma as the first character inside a block, then
            // discard what is read.  Otherwise, terminate the string and increment
            // the number of UIDs successfully read.
            if (butt.readUID[butt.numUID][0] != ',') {
                butt.readUID[butt.numUID][butt.inputSpot2] = '\0';
                butt.numUID++;
            }

            // Block of square brackets has be read, set the indicator to zero
            butt.nextBlock = 0;

        // Put anything inside of a square bracket into the UID array
        } else if (butt.nextBlock == 1 && butt.inputSpot2 < UID_LEN && butt.numUID < MAX_UIDS) {
            butt.readUID[butt.numUID][butt.inputSpot2] = input;
            butt.inputSpot2++;

        // If we are outside of a block, reset read position and ensure that the block
        // state indicator is zero.
        } else {
            butt.inputSpot2 = 0;
            butt.nextBlock = 0;
        }
        
    } else {
        // Ping - 13.4 us
        // Echo back typed character
        // Write2USART(input);
    }

    // Clear interrupt
    PIR1bits.RC1IF = 0;
    PORTAbits.RA0 = 0;
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
    ANSELAbits.ANSA0 = 0;
    TRISAbits.RA0 = 0;
    PORTAbits.RA0 = 1;
    // Controls the RESET for the RFID reader
    //TRISBbits.RB5 = 0;
    //ANSELBbits.ANSB5 = 0;
    butt.inputSpot2 = 0;
    butt.numUID = 0;
    butt.nextBlock = 0;
    butt.invCom = 0;
    // Set up UART to computer and RFID
    rs232Setup1(); // sets pc RX=C7, tx=C6
    rs232Setup2(); // sets dlp rx=b7, tx=b6

    // Start the RFID with a reset
    //resetRFID();

    // Get RFID attention
    sendToRFID("\n");

    while (1) {
        // Read user input from computer
        readBytesUntil(butt.myInput2, '\r', INPUT_LENGTH);
        putc2USART('\r');
        putc2USART('\n');

        // Ping command
        if (strcmppgm2ram(butt.myInput2, "ping") == 0) {
            sendToRFID(PING);

        // Inventory Command
        } else if (strcmppgm2ram(butt.myInput2, "inventory") == 0) {
            // Set the inventory command flag for the interrupt
            butt.invCom = 1;

            // Set the RFID reader to Read mode and send the Inventory command
            //setupRead();
            sendToRFID(INVENTORY);

            // Wait until interrupt finishes
            while(butt.invCom == 1);

            // Print all the UIDs
            for (i = 0; i < butt.numUID; i++) {
                puts2USART(butt.readUID[i]);
                putc2USART('\r');
                while(Busy2USART());
                putc2USART('\n');
            }

            // Reset the number of UIDs read
            butt.numUID = 0;

        // Send the "Stay Quiet" command.
        // WARNING: THIS IS HARDCODED TO ONLY WORK WITH THE PROTOCARD
        } else if (strcmppgm2ram(butt.myInput2, "quiet") == 0) {
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
            while(Busy1USART());
            Write1USART(myInput[i]);
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