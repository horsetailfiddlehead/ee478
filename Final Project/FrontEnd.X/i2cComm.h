/* 
 * File:   i2cComm.h
 * Author: Patrick
 *
 * Created on June 5, 2014, 2:22 AM
 */

#ifndef I2CCOMM_H
#define	I2CCOMM_H

#ifdef	__cplusplus
extern "C" {
#endif
#include "globals.h"

#define MAX_IN_LENGTH 8
#define MAX_OUT_LENGTH 8

    /*
     * Configures the MSSP2 module for master/slave-idle at 100kHz
     */
    void i2CSetup();

    /*
     * Sends the first numBytes bytes of the char array
     */
    int sendBytes(char *data, int numBytes);

    /*
     * Processes received data and commands
     */
    void processI2C();

    typedef struct i2cDataStruct {
        char inDataSequence;
        char destAddr;
        char myAddr;
        char dataOut[MAX_OUT_LENGTH]; // data from the i2c
        unsigned char outLength;
        char dataIn[MAX_IN_LENGTH];
        unsigned char inLength;
        int transmissionNum; // the curent transmission length
    } I2cDataStruct;

    /*************Inter-PIC Commands******************************/
#define INVALID_COMMAND     0xFF //received command is not recognized
#define RECEIVE_ERROR       0xFE //received command cannot be fulfilled
#define END_OF_TRANSMISSION 0xFD //
    /*----------------Front End SendS-----------------------------*/
#define REQUEST_CARD_UPDATE 0x02 // Ask for cards present
#define REQUEST_CARD_DATA   0x04 //  Requests data from a block
#define WRITE_CARD_BLOCK    0xFC // Write 32-bit data to card block
#define WRITE_AFI           0xFB // Write data to Attribute Family Identifier
#define WRITE_DSFID         0xFA // Write data to Data Structure Format ID
    /*------------------Back End Sends----------------------------*/
#define CARD_CHANGE         0x01 // Indicates cards in play changed
#define CARD_UID            0x02 // Sending slot# + UID
#define CARD_DATA_BLOCK    0x04 // Sends requested block data
    /*************************************************************/

#ifdef	__cplusplus
}
#endif

#endif	/* I2CCOMM_H */

