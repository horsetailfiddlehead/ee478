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

    /***************Clocking set up *********************/
#pragma config WDTEN = OFF    // turn off watch dog timer
#pragma config FOSC = ECHP    // Ext. Clk, Hi Pwr
    //#pragma PRICLKEN = OFF // disable primary clock
    /****************************************************/

    /* Global Controls for I/O banks*/
#pragma config PBADEN = OFF   // turn off bank B ADCs

/*
 * 
 */
void main() {
    setupOutgoing();


    return;
}

