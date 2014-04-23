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

/************ I2C set globals ************************/
#include <i2c.h> // i2c library

#define BAUD_RATE   100 // The transmit baud rate
#define SSP_ADDRESS   ((20000000 / (4 * BAUD_RATE)) -1)// calculated SSPADD

/*****************************************************/

/***************Clocking set up *********************/
#pragma WDTEN = OFF;    // turn off watch dog timer
#pragma FOSC = ECHP;    // Ext. Clk, Hi Pwr
#pragma PRICLKEN = OFF; // disable primary clock
/****************************************************/

#pragma PBADEN = OFF;   // turn off bank B ADCs



/*
 * 
 */
int main(int argc, char** argv) {

    // set the MSSP1 SCL1 for output?
    ANSELCbits.ANSC3 = 0;
    TRISCbits.TRISC3 = 0;


    // configure i2c1 for master mode @ 100 kHz
    OpenI2C1(MASTER, SLEW_OFF);
    SSPADD = SSP_ADDRESS;

/* enable module1 as master
 * set tris as input
 * set baud rate
 *
 * loop{
 * send start
 * wait
 *
 * send stop
 * wait

  */
    return (1);
}

