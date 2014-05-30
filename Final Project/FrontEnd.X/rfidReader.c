/*
 * File:   ioBuffer.c
 * Author: Patrick, Ryan
 *
 * Created on May 15, 2014
 * a simple program that buffers user serial input then sends it out
 * sequentially all at once.
 */

#include "globals.h"

#define READER_INPUT_LENGTH 64	// size of input buffer
#define UID_SIZE 24
#define MAX_UIDS 3  // UID storage size

/********Command constants***********************/
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

///***************USART set up *********************/
//#pragma config FCMEN = OFF
//#pragma config IESO = OFF
///****************************************************/
//
///***************Clocking set up *********************/
//#pragma config WDTEN = OFF    // turn off watch dog timer
//#pragma config FOSC = ECHP    // Ext. Clk, Hi Pwr
//#pragma config PRICLKEN = OFF // disable primary clock
//

/***************Interrupts*****************************/

#pragma code high_vector=0x08

void interrupt_at_high_vector(void) {
    _asm GOTO rcISR _endasm
}
#pragma code

#pragma interrupt rcISR

/****************************************/
// These should be in a global structure

typedef struct {
    // User input
    char userInput2[READER_INPUT_LENGTH];

    // Read UIDs, length can be optimized
    // Currently can read only 3 UIDs before we get errors based on the size
    // of the array
    char readUID[MAX_UIDS][UID_SIZE];

    // Current spot in the array processing for input from RFID
    int inputSpot2;

    // The UID that we are reading. (first, second, etc)
    int numUID;

    // Weather or not we are in a block of square brackets
    short nextBlock;

    // If the user input was an inventory command
    short invCom;
} RFIDDriver;
static RFIDDriver readerData;

/****************************************/

void rcISR(void) {
    // The input character from UART2 (the RFID reader)
    unsigned char input;

    // Don't have to wait for data available if we are in ISR
    input = getc2USART();

    // If we are processing an Inventory command
    if (readerData.invCom == 1) {

        // A 'D' character outside of square brackets indicates that the inventory
        // command has finished sending
        if (input == 'D' && readerData.nextBlock == 0) {
            // Reset the inventory command flag
            readerData.invCom = 0;

            // Begin reading what is inside a block of square brackets
        } else if (input == '[') {
            // Go to the beginning of the array, indicate that a block is being read
            readerData.inputSpot2 = 0;
            readerData.nextBlock = 1;

            // If we are at the end of a block of square brackets
        } else if (input == ']' && readerData.numUID < MAX_UIDS && readerData.nextBlock == 1) {
            // If there is a comma as the first character inside a block, then
            // discard what is read.  Otherwise, terminate the string and increment
            // the number of UIDs successfully read.
            if (readerData.readUID[readerData.numUID][0] != ',') {
                readerData.readUID[readerData.numUID][readerData.inputSpot2] = '\0';
                readerData.numUID++;
            }

            // Block of square brackets has be read, set the indicator to zero
            readerData.nextBlock = 0;

            // Put anything inside of a square bracket into the UID array
        } else if (readerData.nextBlock == 1 && readerData.inputSpot2 < READER_INPUT_LENGTH && readerData.numUID < MAX_UIDS) {
            readerData.readUID[readerData.numUID][readerData.inputSpot2] = input;
            readerData.inputSpot2++;

            // If we are outside of a block, reset read position and ensure that the block
            // state indicator is zero.
        } else {
            readerData.inputSpot2 = 0;
            readerData.nextBlock = 0;
        }

    } else {
        // Echo back typed character
        Write1USART(input);
    }

    // Clear interrupt
    PIR3bits.RC2IF = 0;
}

/****************************************************/

void sendToRFID(char* myString);
void setupRead(void);

void processRFIDCmd() {
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
        readBytesUntil(readerData.userInput2, '\r', READER_INPUT_LENGTH);
        putc1USART('\r');
        putc1USART('\n');

        // Ping command
        if (strcmppgm2ram(readerData.userInput2, "ping") == 0) {
            sendToRFID(PING);

            // Inventory Command
        } else if (strcmppgm2ram(readerData.userInput2, "inventory") == 0) {
            // Set the inventory command flag for the interrupt
            readerData.invCom = 1;

            // Set the RFID reader to Read mode and send the Inventory command
            setupRead();
            sendToRFID(INVENTORY);

            // Wait until interrupt finishes
            while (readerData.invCom == 1);

            // Print all the UIDs
            for (i = 0; i < readerData.numUID; i++) {
                puts1USART(readerData.readUID[i]);
                putc1USART('\r');
                while (Busy1USART());
                putc1USART('\n');
            }

            // Reset the number of UIDs read
            readerData.numUID = 0;

            // Send the "Stay Quiet" command.
            // WARNING: THIS IS HARDCODED TO ONLY WORK WITH THE PROTOCARD
        } else if (strcmppgm2ram(readerData.userInput2, "quiet") == 0) {
            sendToRFID(STAY_QUIET);

            // Any errors will reset the RFID reader
        } else {
            resetRFID();
        }
    }
    return;
}

void sendToRFID(char* myString) {
    char myInput[READER_INPUT_LENGTH];
    short inputFinished = 0;
    int i = 0;
    strcpypgm2ram(myInput, myString);
    while (!inputFinished) {
        if (myInput[i] != '\0') {
            while (Busy2USART());
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

// initialize the RFID reader and variables

void RFIDSetup() {
    // initialize local vars
    readerData.inputSpot2 = 0;
    readerData.numUID = 0;
    readerData.nextBlock = 0;
    readerData.invCom = 0;

    setupRead();
}