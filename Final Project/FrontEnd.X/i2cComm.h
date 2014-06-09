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

#ifdef	__cplusplus
}
#endif

#endif	/* I2CCOMM_H */

