/*
 * File:   ioBuffer.c
 * Author: Patrick, Ryan
 *
 * Created on May 15, 2014
 * a simple program that buffers user serial input then sends it out
 * sequentially all at once.
 */

#include "globals.h"

/********Command constants***********************/
#define READER_INPUT_LENGTH 64
#define QUIET_LEN 37
#define END_COM "0000" // End of a command
#define STAY_QUIET "0112000304182202" // Beginning part of stay quiet, add 0000
#define AGC "0109000304F0000000"
#define REG_WRITE "010C00030410002101020000"
#define PING "0108000304FF0000"
#define INVENTORY "010B000304140601000000"
#define READ_SINGLE "0113000304182220"
#define WRITE_SINGLE "0117000304186221"
#define READ_SING_LEN 39
#define WR_SING_LEN 47

void sendToRFID(char* myString);
void setupRead(void);

// Set up on USART 2

// Send a ping commmand to the rfid

void pingRFID() {
    sendToRFID(PING);
}

// Send an inventory command to the RFID

void inventoryRFID() {
    // Set the inventory command flag for the interrupt
    readerData.invCom = 1;
    // If we have not set read mode yet, go to config mode
    if (readerData.readMode == 0) {
        // Config mode
        readerData.configFlag = 1;

        // Disable current flag
        readerData.invCom = 0;

        // Send read commands
        setupRead();

        // Turn off config
        readerData.configFlag = 0;

        // Turn back on inventory mode, indicate read set
        readerData.invCom = 1;
        readerData.readMode = 1;
    }
    // Send inventory command
    sendToRFID(INVENTORY);

    // Wait until interrupt finishes
    while (readerData.invCom == 1);
    readerData.availableUIDs = readerData.numUID;

    // Reset the UID counters
    readerData.numUID = 0;
    readerData.lineFeeds = 0;
}

// Send a quiet command to the given uid

void quietRFID(char* uid) {
    // Holds the command
    char quietCommand[QUIET_LEN]; // {STAY_QUIET, uid, END_COM};
    // Beginning part of command
    strcatpgm2ram(quietCommand, STAY_QUIET);

    // Concatenate the uid
    strcat(quietCommand, uid);

    // Add 0000 for the ending bits
    strcatpgm2ram(quietCommand, END_COM);

    // Do sendToRFID2 because it is already saved to ram
    sendToRFID2(quietCommand);
    return;
}

#define NAME_LEN 8
void char8RFID(char* uid, char block, char* myString1) {
    int i = 0;
    int myNumHigh = 0;
    int myNumLow = 0;
    char myString[9] = { 0 };
    strcpypgm2ram(myString, myString1);
    myNumHigh |= (int)myString[0] << 8;
    myNumHigh |= (int)myString[1];
    myNumLow |= (int)myString[2] << 8;
    myNumLow |= (int)myString[3];

    writeRFID(uid, block, myNumHigh, myNumLow); // name: first 4 chars left to right FI RE

    myNumHigh = 0;
    myNumLow = 0;

    myNumHigh |= (int)myString[4] << 8;
    myNumHigh |= (int)myString[5];
    myNumLow |= (int)myString[6] << 8;
    myNumLow |= (int)myString[7];

    writeRFID(uid, block+1, myNumHigh, myNumLow); // name: last 4 chars left to right  DU DE


}

void writeRFID(char* uid, char block, int highData, int lowData) {
    // Holds the command
    char writeCommand[WR_SING_LEN]; // {STAY_QUIET, uid, END_COM};
    char dataHex[5];
    if (readerData.availableUIDs > 0) {
        memset(writeCommand, '\0', WR_SING_LEN * sizeof (char));
        readerData.writeFlag_1 = 1;
        // If we have not set read mode yet, go to config mode
        if (readerData.readMode == 0) {
            // Config mode
            readerData.configFlag = 1;
            readerData.writeFlag_1 = 0;

            // Send read commands
            setupRead();

            // Turn off config
            readerData.configFlag = 0;
            readerData.writeFlag_1 = 1;
        }

        // Beginning part of command
        strcatpgm2ram(writeCommand, WRITE_SINGLE);

        // Concatenate the uid and block
        strcat(writeCommand, uid);

        sprintf(dataHex, "%02x", (int) block);
        strcat(writeCommand, dataHex);

        sprintf(dataHex, "%04x", highData);
        strcat(writeCommand, dataHex);

        sprintf(dataHex, "%04x", lowData);
        strcat(writeCommand, dataHex);

        // Add 0000 for the ending bits
        strcatpgm2ram(writeCommand, END_COM);

        // Do sendToRFID2 because it is already saved to ram
        sendToRFID2(writeCommand);

        // Wait until interrupt finishes
        while (readerData.writeFlag_1 == 1);
    }
    return;
}

void readRFID(char* uid, char block) {
    // Holds the command
    char readCommand[READ_SING_LEN]; // {STAY_QUIET, uid, END_COM};
    char blockHex[3];
    if (readerData.availableUIDs > 0) {
        memset(readCommand, '\0', READ_SING_LEN * sizeof (char));
        readerData.readFlag_1 = 1;
        // If we have not set read mode yet, go to config mode
        if (readerData.readMode == 0) {
            // Config mode
            readerData.configFlag = 1;

            // Disable current flag
            readerData.readFlag_1 = 0;

            // Send read commands
            setupRead();

            // Turn off config
            readerData.configFlag = 0;

            readerData.readFlag_1 = 1;
        }

        // Beginning part of command
        strcatpgm2ram(readCommand, READ_SINGLE);

        // Concatenate the uid and block
        strcat(readCommand, uid);

        sprintf(blockHex,"%02x",(int)block);
        strcat(readCommand, blockHex);

        // Add 0000 for the ending bits
        strcatpgm2ram(readCommand, END_COM);

        // Do sendToRFID2 because it is already saved to ram
        sendToRFID2(readCommand);

        // Wait until interrupt finishes
        while (readerData.readFlag_1 == 1);
    }
    return;
}

/* // Depreciated
void processRFIDCmd() {
    int i;
    //    // Controls the RESET for the RFID reader
    //    TRISBbits.RB5 = 0;
    //    ANSELBbits.ANSB5 = 0;

    // Set up UART to computer and RFID
    //    rs232Setup1(); // sets pc RX=C7, tx=C6
    //    rs232Setup2(); // sets dlp rx=b7, tx=b6

    // Start the RFID with a reset
    //resetRFID();

    // Get RFID attention
    //sendToRFID("0");

    //    while (1) {
    //        // Read user input from computer
    //        readBytesUntil(readerData.userInput2, '\r', READER_INPUT_LENGTH);
    //        putc1USART('\r');
    //        putc1USART('\n');

    // Ping command
    sendToRFID("\n");
    if (strcmppgm2ram(readerData.userInput2, "ping") == 0) {
        sendToRFID(PING);
        // Inventory Command
    } else if (strcmppgm2ram(readerData.userInput2, "inventory") == 0) {
        // Set the inventory command flag for the interrupt
        readerData.invCom = 1;

        // Set the RFID reader to Read mode and send the Inventory command
        if (readerData.readMode == 0) {
            readerData.configFlag = 1;
            readerData.invCom = 0;
            setupRead();
            readerData.configFlag = 0;
            readerData.invCom = 1;
            readerData.readMode = 1;
        }
        sendToRFID(INVENTORY);

        // Wait until interrupt finishes
        while (readerData.invCom == 1);
        // Print all the UIDs
        for (i = 0; i < readerData.numUID; i++) {
            puts2USART(readerData.readUID[i]);
            putc2USART('\r');
            while (Busy2USART());
            putc2USART('\n');
        }
        readerData.availableUIDs = readerData.numUID;

        // Reset the number of UIDs read
        readerData.numUID = 0;
        readerData.lineFeeds = 0;

        // Send the "Stay Quiet" command.
        // WARNING: THIS IS HARDCODED TO ONLY WORK WITH THE PROTOCARD
    } else if (strcmppgm2ram(readerData.userInput2, "quiet") == 0) {
        sendToRFID(STAY_QUIET);

        // Any errors will reset the RFID reader
    } else {
        //resetRFID();
    }
    //    }
    return;
}
 */

// Sends the string to the DLP RFID 2

void sendToRFID(char* myString) {
    // Copy string into an input array
    char myInput[READER_INPUT_LENGTH];
    // Says whether the input has finished sending or not
    short inputFinished = 0;
    int i = 0;

    // Copy string to ram so it can be read correctly
    strcpypgm2ram(myInput, myString);

    // Send character by character
    while (!inputFinished) {
        if (myInput[i] != '\0') {
            while (Busy1USART());
            Write1USART(myInput[i]);
            i++;
        } else {
            inputFinished = 1;
        }
    }

    // Short delay, try removing
    Delay10TCYx(100);
    return;
}

// Use this for when you need to send a string that isn't sent
// in double quotes (not this: "mystring!")
//
// see sendToRFID for more comments

void sendToRFID2(char* myInput) {
    short inputFinished = 0;
    int i = 0;
    while (!inputFinished) {
        if (myInput[i] != '\0') {
            while (Busy1USART());
            Write1USART(myInput[i]);
            i++;
        } else {
            inputFinished = 1;
        }
    }
    Delay10TCYx(100);
    return;
}

// Set up read commands

void setupRead() {
    // Set to register write mode
    sendToRFID(REG_WRITE);

    // Needs to read two line breaks from dlp before
    // continuing.
    while (readerData.lineFeeds < 2);

    // Reset line feeds for next command
    readerData.lineFeeds = 0;

    // Set AGC mode
    sendToRFID(AGC);

    // We need to read only one line feed when setting AGC
    while (readerData.lineFeeds < 1);

    // Reset line feeds
    readerData.lineFeeds = 0;

    // Send new line to clear out any junk
    // Try removing
    sendToRFID("\n");
    return;
}

// Depreciated - we don't use them anymore

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
    readerData.readMode = 0;
    readerData.lineFeeds = 0;
    readerData.configFlag = 0;
    readerData.availableUIDs = FALSE;
    memset(readerData.readUID, '\0', MAX_UIDS * UID_SIZE * sizeof (char));
    memset(readerData.readData, '\0', 16*sizeof(char));
    readerData.readFlag_1 = 0;
    readerData.writeFlag_1 = 0;
    readerData.availableData = 0;

#if !FRONT_NOT_BACK
    // Get RFID attention if not already
    sendToRFID("\n");
#endif
}
