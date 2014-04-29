/*
 * File:   i2c_local.c
 * Author: Patrick Ma
 *
 * Created on April 23, 2014, 8:20 PM
 *
 * Controls the I2C behavior for the local control node.
 * Outgoing communication occurs on the I2C1 channel.
 * Incoming communication occurs on the I2C2 channel.
 */

/************ I2C set globals ************************/
#include "globals.h"
#include "i2c_local.h"
#include <i2c.h> // i2c library

/* Local defines */
#define BAUD_RATE   100000 // The transmit baud rate
#define BD_RT   ((20000000 / (4 * BAUD_RATE)) -1)// calculated SSPADD

/*****************************************************/


/* local variables*/
extern Global ourGlobal;
unsigned char i2cData, addr1;
int status, index;

/* method declarations*/
// See header file

/* TX & RX interrupt handlers*/

/*
 * configures and starts outgoing data communication
 * The outgoing I2C uses MSSP1
 */
int setupOutgoing() {
    // set pins RC14, RC15 as inputs
    TRISCbits.TRISC3 = 1; // SCL1
    ANSELCbits.ANSC3 = 0;

    TRISCbits.TRISC4 = 1; // SDA1
    ANSELCbits.ANSC4 = 0;

    // configure i2c1 for master mode @ 100 kHz
    CloseI2C1();
    OpenI2C1(MASTER, SLEW_OFF);
    SSPADD = BD_RT;

    return (1);
}

/*
 * configures and starts incoming data communication
 * The incoming I2C uses MSSP2
 */
int setupIncoming() {

    int SLAVE_ADDRESS = 0x0; // the slave address
    // set pins  as inputs
    TRISBbits.TRISB1 = 1; // SCL2
    ANSELBbits.ANSB1 = 0;

    TRISBbits.TRISB2 = 1; // SDA2
    ANSELBbits.ANSB2 = 0;
        // Enable Priority
    RCONbits.IPEN = 1;
    // High priority receive interrupt
    IPR3bits.SSP2IP = 1;
    // Enable all high priority interrupts
    INTCONbits.GIEH = 1;
    INTCONbits.PEIE = 1;
    IPR3bits.SSP2IP = 1;
    PIE3bits.SSP2IE = 1;


    // configure MSSP2 for i2c slave operation.
    CloseI2C2();
    OpenI2C2(SLAVE_7, SLEW_OFF);
    SSPADD = SLAVE_ADDRESS;
    return (1);
}

/* Task to send the current setpoint to a remote node*/
void runLocalI2C(unsigned int *setSpeed) {
    addr1 = 0x29;
    sendSpeed(&addr1, setSpeed);
    //    Delay10TCYx(20);
}

/* Task to read from the bus*/
void runRemoteI2C(unsigned int *setSpeed) {
    receiveData();
}

/*
 * send data to a slave device
 * takes the slave address, writes the given data byte
 */
int sendSpeed(unsigned int *slaveAddr, unsigned int *speed) {
    // get the data from global variable
    char newSpeed[3] = {*speed, 'X', '\0'};

    // wait until idle - not actually needed for single-master bus
    IdleI2C1();
    StartI2C1(); // send start
    i2cData = SSP1BUF;

    WriteI2C1(*slaveAddr & 0xFE);
    //    do { // send address until ack'd
    //        status = WriteI2C1(*slaveAddr || 0x00);
    //        if (!(0 == status)) { // write collision
    //            data = SSP1BUF;
    //            SSP1CON1bits.WCOL = 0;
    //        }
    //    } while (!(0 == status));

    // 	send bytes
    WriteI2C1(newSpeed[0]); // TODO: send multiple bytes

    StopI2C1(); // stop transmission

    return 1;
}

// read data

int receiveData() {
    // get here after interrupt
    // dumb remote just waits for a call
    while (0 == DataRdyI2C2());
    //AckI2C2(); // do we need to ack?
    i2cData = SSPBUF; // clear buffer
    while (0 == DataRdyI2C2()); // wait for next byte
    *ourGlobal.setSpeed = getcI2C2(); // store buffer value to memory
    Delay10TCYx(1);

}


