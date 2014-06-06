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

    /*
     * Configures the MSSP2 module for master/slave-idle at 100kHz
     */
    void i2CSetup();

    /*
     * Sends the first numBytes bytes of the char array
     */
    int sendBytes(char *data, int numBytes);

    void i2cISR();

    typedef struct i2cDataStruct {
        char inDataSequence;
        char destAddr;
        char myAddr;
        char i2cData[8]; // data from the i2c
        unsigned char messageLength;
    } I2cDataStruct;

#ifdef	__cplusplus
}
#endif

#endif	/* I2CCOMM_H */

