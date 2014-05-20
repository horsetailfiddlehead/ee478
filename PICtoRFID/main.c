/* 
 * File:   main.c
 * Author: castia
 *
 * Created on April 24, 2014, 4:22 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <i2c.h> 
#include "SRAM.h"
#include "rs232.h"
#include "globals.h"

/***************USART set up *********************/
#pragma config FCMEN = OFF
#pragma config IESO = OFF
/****************************************************/

/***************Clocking set up *********************/
#pragma config WDTEN = OFF    // turn off watch dog timer
#pragma config FOSC = ECHP    // Ext. Clk, Hi Pwr
#pragma config PRICLKEN = OFF // disable primary clock
/****************************************************/
#define IN_BUF_SZ 64
#define DEF_PWM 0

int myCommand = 0;

Global ourGlobal

void main() {
    unsigned int temp = 0;

    char errorMsg[] = "Error: Input again.\n\r";

    // I2c Setup
    setupOutgoing();
    setupIncoming();

    // Rs232 setup and interrupt
    rs232Setup();

    // Enable SRAM pins correctly
    // SRAMsetUp();

    while (1) {
        displayFrontPanel(&ourGlobal);
        dataProcess(&ourGlobal);
        if (*ourGlobal.SRAMflag == 1) {
            *ourGlobal.SRAMflag = 0;
            writeData(0, *ourGlobal.myCommand);
            switch (*ourGlobal.myCommand) {
                case 1:

                    break;
                case 2:

                    break;
                case 3:

                    break;
                case 4:

                    break;
            }
        }
        Delay1KTCYx(1);

    }
}