/* 
 * File:   SchedMain.c
 * Author: Patrick
 *
 * Created on May 27, 2014, 4:35 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <p18f46k22.h>

// included files for each sw function
#include "globals.h"
#include "rs232.h"


// Function prototypes
void systemSetup(void);

// PIC configuration settings
/***************Clocking set up *********************/
#pragma config WDTEN = OFF    // turn off watch dog timer
#pragma config FOSC = ECHPIO6    // Ext. Clk, Hi Pwr
#pragma config PRICLKEN = OFF // disable primary clock

/***************USART set up *********************/
#pragma config FCMEN = OFF
#pragma config IESO = OFF
/****************************************************/


/*
 * 
 */
void main() {
    int keypress = -1;
    systemSetup();

    while(1) {
        keypress = checkForInput();
        putc2USART(keypress);

        Delay1KTCYx(20);
    }

    return;
}

void systemSetup() {
    rs232Setup2();
    keypadSetup();

    return;
}