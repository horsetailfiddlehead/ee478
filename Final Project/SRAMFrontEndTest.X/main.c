/* 
 * File:   main.c
 * Author: castia
 *
 * Created on June 5, 2014, 11:18 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <p18f46k22.h>
#include "SRAM.h"
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
int main() {
    int data1 = 0;
    int data2 = 0;
    SRAMsetUp();
    writeData(5, 0xFF);
    writeData(8, 0xAA);
    data1 = readData(5);
    data2 = readData(8);
    return 0;
}

