/*
 * Created by: Patrick
 * June 2, 2014
 *
 * I2C system for the front and back ends. The system is configured as a
 * Master/slave-idle. The i2c uses MSSP2 for communication
 */

#include "globals.h"

/********Configuration Settings*******************************/
#define MASTER  0b00001000 //I2C  Master mode
#define SLAVE   0b00000110 // I2C slave mode, 7-bit address
#define SSPEN   0b00100000  /* Enable serial port and configures SCK, SDO, SDI*/
#define SSPDIS  0b11011111   // disable serial port
#define SLEW_OFF 0b10000000 /* Slew rate disabled for 100kHz mode */
#define BAUD    0x31;  // master address value for 100kHz baud rate
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
#define SUPPLY_CARD_DATA    0x04 // Sends requested block data
/*************************************************************/

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
    i2cData.destAddr = 0x00; // 0b0000000
    i2cData.myAddr = 0xFE;
#endif

    i2cData.inDataSequence = FALSE;
    i2cData.messageLength = 0;
    memset(i2cData.dataIn, '\0', sizeof (char) * MAX_IN_LENGTH);
    memset(i2cData.dataOut, '\0', sizeof (char) * MAX_OUT_LENGTH);
    i2cData.inLength = 0;
    i2cData.outLength = 0;

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
#if FRONT_NOT_BACK
    SSP2CON1 = MASTER; // set mode to master mode
    SSP2ADD = BAUD; // set baud rate to 100kHz
#else
    SSP2CON1 = SLAVE;
    SSP2ADD = i2cData.myAddr;
#endif
    SSP2CON2 = 0b00000000; // disable general call interrupt
    SSP2CON3 = 0b01100011; // enable stop int, enable start int, addr/data hold

    SSP2CON1 |= SSPEN; // enable module

}

/*
 * Process received commands
 */
void processI2C() {
#if FRONT_NOT_BACK // receives backends commands
    // switch on command
    // parse data into parts
    switch (i2cData.dataIn[0]) {
        case CARD_CHANGE:

            break;
        case CARD_UID:
            int newSlotNum = i2cData.dataIn[1]; // slot number
            // UID
            // move uid to slot
            break;
        case SUPPLY_CARD_DATA:

            break;
        case INVALID_COMMAND:

            break;
        case RECEIVE_ERROR:

            break;
        case END_OF_TRANSMISSION:

            break;
        default:
            i2cData.dataOut[0] = INVALID_COMMAND;
            i2cData.outLength = 1;
            break;
    }
#else
    switch (i2cData.dataOut[0]) {
        case REQUEST_CARD_UPDATE:

            break;
        case REQUEST_CARD_DATA:

            break;
        case WRITE_CARD_BLOCK:

            break;
        case WRITE_AFI:

            break;
        case WRITE_DSFID:

            break;
        case INVALID_COMMAND:

            break;
        case RECEIVE_ERROR:

            break;
        case END_OF_TRANSMISSION:

            break;
        default:
            i2cData.dataOut[0] = INVALID_COMMAND;
            i2cData.outLength = 1;
            break;
    }
#endif
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
    if (status < 0) {// if collision, revert to slave, reset data sequence flag to transfer ok
        sendStop();
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
