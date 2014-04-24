/*
 * File:   i2c_local.h
 * Author: Patrick
 *
 * Created on April 23, 2014, 8:20 PM
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

/* method declarations*/

/* TX & RX interrupt handlers*/

// send data
int sendSpeed(unsigned int *speed) {
    // get the data from global variable
    // send data
    // check for valid recieve
    

    return transmitGood;
}

// read data
int getRemoteData() {
    // get and store data from remote
    // run any processes
}


/*
 *
 */
int main(int argc, char** argv) {
    char test = "I";

    // set the MSSP1 SCL1 for output?


    // set pins RC14, RC15 as inputs
    TRISCbits.TRISC3 = 1; // SCL1
    ANSELCbits.ANSC3 = 0;

    TRISCbits.TRISC4 = 1; // SDA1
    ANSELCbits.ANSC4 = 0;

    // configure i2c1 for master mode @ 100 kHz
    CloseI2C1();
    OpenI2C1(MASTER, SLEW_OFF);
    SSPADD = BD_RT;

    while(1) {
//        IdleI2C1();
//        WriteI2C1(test);    // just write a char
        Delay10TCYx(40);
        StartI2C1();
        Delay10TCYx(20);
        WriteI2C1(test);

    }

    return (1);
}