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

unsigned char data, addr, status;

/* method declarations*/
int setupOutgoing();
int sendSpeed(unsigned int *slaveAddr, unsigned int *speed);
int getRemoteData();

/* TX & RX interrupt handlers*/

/*
 * configures and starts outgoing data communication
 * The outgoing I2C uses MSSP1
 */
int setupOutgoing(){
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
 * send data to a slave device
 * takes the slave address, writes the given data byte
 */
int sendSpeed(unsigned int *slaveAddr, unsigned int *speed) {
    // get the data from global variable
    char newSpeed[2] = {*speed, '\0'};
    // wait until idle - not actually needed for single-master bus
    IdleI2C1();
    StartI2C1(); // send start
    data = SSP1BUF;
    do { // send address until ack'd
        status = WriteI2C1(*slaveAddr || 0x00);
        if (!status) { // write collision
            data = SSP1BUF;
            SSP1CON1bits.WCOL = 0;
        }
    } while (!status);

    while (putsI2C1(newSpeed) != 0); // 	send bytes
    StopI2C1(); // stop transmission

    return 1;
}

// read data
int getRemoteData() {
    // get and store data from remote
    // run any processes
}

