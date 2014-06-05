/* 
 * File:   rfidReader.h
 * Author: Patrick
 *
 * Created on May 29, 2014, 12:45 PM
 */

#ifndef RFIDREADER_H
#define	RFIDREADER_H

#ifdef	__cplusplus
extern "C" {
#endif
    
    /********ReaderData sizes************************/
#define UID_SIZE 24
#define MAX_UIDS 5  // number of UIDs stored
#define RFID_BLOCK 10

void resetRFID(void);
void RFIDSetup(void);
void pingRFID(void);
void inventoryRFID(void);
void quietRFID(char* uid);
void sendToRFID2(char* myInput);
void writeRFID(char* uid, char block, unsigned int data);
void readRFID(char* uid, char block);

//void processRFIDCmd(void);

typedef struct {
    // Read UIDs, length can be optimized
    // Currently can read only 3 UIDs before we get errors based on the size
    // of the array
    char readUID[MAX_UIDS][UID_SIZE];

    short lineFeeds;
    short configFlag;

    // Current spot in the array processing for input from RFID
    int inputSpot2;

    // The UID that we are reading. (first, second, etc)
    int numUID;

    // Weather or not we are in a block of square brackets
    short nextBlock;

    // If the user input was an inventory command
    short invCom;

    // If the user input was a single block read command
    short readFlag_1;

    // Are the UIDs available to read?
    int availableUIDs;
    
    // Read mode set?
    short readMode;

    short writeFlag_1;
} RFIDDriver;
extern RFIDDriver readerData;

#ifdef	__cplusplus
}
#endif

#endif	/* RFIDREADER_H */

