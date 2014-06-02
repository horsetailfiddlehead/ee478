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
#define READER_INPUT_LENGTH 64	// size of input buffer
#define UID_SIZE 24
#define MAX_UIDS 5  // UID storage size

void resetRFID(void);
void RFIDSetup(void);
void processRFIDCmd(void);

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

    // Are the UIDs available to read?
    int availableUIDs;
} RFIDDriver;
extern RFIDDriver readerData;

#ifdef	__cplusplus
}
#endif

#endif	/* RFIDREADER_H */

