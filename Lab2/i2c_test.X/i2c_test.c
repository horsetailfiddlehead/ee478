/* 
 * File:   i2c_test.c
 * Author: pgma2010
 *
 * Created on April 22, 2014, 4:49 PM
 */

/*
 * Simple test file for i2c
 */

#include <stdio.h>
#include <stdlib.h>
#include <p18f25k22.h>
#include <delays.h>
#define USE_AND_OR  // allows use of compiler libs

/************ I2C set globals ************************/
#include <i2c.h> // i2c library

#define BAUD_RATE   100000 // The transmit baud rate
#define BD_RT   ((20000000 / (4 * BAUD_RATE)) -1)// calculated SSPADD

/*****************************************************/

/***************Clocking set up *********************/
#pragma WDTEN = OFF;    // turn off watch dog timer
#pragma FOSC = ECHP;    // Ext. Clk, Hi Pwr
//#pragma PRICLKEN = OFF; // disable primary clock
/****************************************************/

#pragma PBADEN = OFF;   // turn off bank B ADCs



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

