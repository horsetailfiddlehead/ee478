/*
 * Created by: Patrick
 * June 2, 2014
 *
 * I2C system for the front and back ends. The system is configured as a
 * Master/slave-idle. The i2c uses MSSP2 for communication
 */

#include "globals.h"

#define MASTER  0b00001000 //I2C  Master mode
#define SLAVE   0b00000110 // I2C slave mode, 7-bit address
#define SSPEN   0b00100000  /* Enable serial port and configures SCK, SDO, SDI*/
#define SSPDIS  0b11011111   // disable serial port
#define SLEW_OFF 0b10000000 /* Slew rate disabled for 100kHz mode */
#define BAUD 0x31;  // master address value for 100kHz baud rate

//char i2cAddr;
//Boolean inDataSequence; // 1 if someone is transmitting data
I2cDataStruct i2cData;

/*******Private prototypes*********************/
void switchToSlave(void);
void switchToMaster(void);
void sendStop(void);
void sendStart(void);
/***********************************************/

/*
 * Initialization function for the i2c module
 */
void i2CSetup() {
    // setup the struct
#if FRONT_NOT_BACK
    i2cData.destAddr = 0xFE; // 8-bit address, 7-bit address is 0x7F
    i2cData.myAddr = 0x00;
#else
    i2cAddr = 0x00; // 0b0000000
    i2cData.myAddr = 0xFE;
#endif

    i2cData.inDataSequence = FALSE;
    i2cData.messageLength = 0;

    // setup D0, D1 as inputs
    TRISDbits.TRISD0 = 1;
    TRISDbits.TRISD1 = 1;

    // setup associated ANSEL bits as digital
    ANSELDbits.ANSD0 = 0;
    ANSELDbits.ANSD1 = 0;

    // enable the interrupt priority bits
    INTCONbits.GIEH = 1; // global int enable
    INTCONbits.PEIE = 1; // peripheral int enable
    IPR3bits.SSP2IP = 1; // MSSP interrupt high priority
    PIE3bits.SSP2IE = 1; // MSSP interrupt enable

    // configure MSSP2 for i2c communication
    SSP2CON1 &= SSPDIS; // disable module
    SSP2STAT |= SLEW_OFF;
    SSP2CON1 = MASTER; // set mode to master mode
    SSP2CON2 = 0b00000000; // disable general call interrupt
    SSP2CON3 = 0b01100011; // enable stop int, enable start int, addr/data hold

    //      CloseI2C2();
    //    OpenI2C2(MASTER, SLEW_OFF);
    SSP2ADD = BAUD; // set baud rate to 100kHz
    SSP2CON1 |= SSPEN; // enable module

}

/* send bytes as the master.  checks the status of the bus before entering
 * master mode. Returns a negative number if error occurs during writing,
 * otherwise exits with 0. Exit state: slave mode.
 */
int sendBytes(char *data, int numBytes) {
    int i = 0;
    signed char status = 0;

    // enter masater mode if no data is being sent (stop bit last seen)
    if (!i2cData.inDataSequence)
        switchToMaster(); 
    else
        return -1;

    sendStart();

    status = WriteI2C2(i2cData.destAddr & 0b11111110); // send address with write
    if (status > 0) {// if collision, revert to slave, reset data sequence flag to transfer ok
        sendStop(); //TODO: fix conditional
        switchToSlave();
        return status;
    } else { // if no collision,
        for (i = 0; i < numBytes; i++) { //write out Numbytes of data
            status = WriteI2C2(data[i]);
            if (status < 0) {// if nack or wcol, break
                sendStop();
                switchToSlave();
                return status; 
            }
        }
    }
    sendStop();
    switchToSlave(); // switch back to slave mode
}

/*
 * Sets module to master mode. Note this clears various flags in the module
 */
void switchToMaster() {
    // set address reg to baud rate?
    // switch out of RCEN mode?
    SSP2CON1 &= SSPDIS; // diable module
    SSP2CON1 = MASTER; // change mode
    SSP2ADD = BAUD; // Set baud rate
    SSP2CON1 |= SSPEN; // enable module
}

/*
 * Sets module to slave mode. Note this clears various flags in the module
 */
void switchToSlave() {
    SSP2CON1 &= SSPDIS; // diable module
    SSP2CON1 = SLAVE; // change mode
    SSP2ADD = i2cData.myAddr; // update address buffer
    SSP2CON1 |= SSPEN; // enable module
}

/*
 * Sends a stop bit. Module must be in Master mode
 */
void sendStop() {
    SSP2CON2bits.PEN = 1; // send stop
    while (SSP2CON2bits.PEN == 1);
    i2cData.inDataSequence = FALSE;
}

/*
 * Sends a start bit. Module must be in Master mode
 */
void sendStart() {
    SSP2CON2bits.SEN = 1; // send start bit
    i2cData.inDataSequence = TRUE;
    while (SSP2CON2bits.SEN == 1); // or use IdleI2C2()
}

/*ISR for received data in slave mode
 *
 */
void i2cISR() {
    // if start bit, inDataSequence = TRUE
    // if stopbit, inDataSequence = false;
    // ccheck if address
    // dump buffer
    // not address
    //store into global variable/buffer
    // set i2c process flag
    // clear interrupt flag
}