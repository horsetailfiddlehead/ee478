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

    /*
     * Configures the MSSP2 module for master/slave-idle at 100kHz
     */
    void i2CSetup();

    /*
     * Sends the first numBytes bytes of the char array
     */
    int sendBytes(char *data, int numBytes);

    void i2cISR();

#ifdef	__cplusplus
}
#endif

#endif	/* I2CCOMM_H */

