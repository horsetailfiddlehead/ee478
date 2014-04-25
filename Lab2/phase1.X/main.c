/* 
 * File:   main.c
 * Author: castia
 *
 * Created on April 24, 2014, 4:22 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include "globals.h"
#include "i2c_local.h"
#include <i2c.h> // i2c library

/***************Clocking set up *********************/
#pragma config WDTEN = OFF    // turn off watch dog timer
#pragma config FOSC = ECHP    // Ext. Clk, Hi Pwr
#pragma config PRICLKEN = OFF // disable primary clock
/****************************************************/

/* Global Controls for I/O banks*/
#pragma config PBADEN = OFF   // turn off bank B ADCs

/*
 * 
 */
void main() {
   unsigned int setSpeed = 0x52;

    setupOutgoing();

    while (1) {

        runLocalI2C(&setSpeed);

    }
    return;
}

int startAndWrite(char *c) {
    char data = 0x9A;
    IdleI2C1();
    StartI2C1();
    WriteI2C1(*c); // just write a char
    WriteI2C1(data);
    StopI2C1();
}
