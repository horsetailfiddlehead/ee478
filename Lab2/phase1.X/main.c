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
#include "SRAM.h"
#include "rs232.h"
#include <i2c.h> // i2c library

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
/***********************Global Setup*****************/
unsigned int setSpeed = 0;
unsigned int motorSpeed = 0; // The actual speed of the motor
unsigned int errorState = 5; // The current motor state is off
char myInput[IN_BUF_SZ];
int inputSpot = 0;
short inputFinished = 0;

short displayFlag = 1;
short SRAMflag = 0;
short i2cFlag = 0;
short processFlag = 0;
int myCommand = 0;
int myOp = 0;

Global ourGlobal = {&setSpeed, &motorSpeed, &errorState, &myInput, &inputSpot, &inputFinished, &displayFlag, &SRAMflag, &i2cFlag, &processFlag, &myCommand, &myOp};

/****************************************************/

/****************Interrupt Stuff*********************/

/* Global Controls for I/O banks*/
#pragma config PBADEN = OFF   // turn off bank B ADCs

#pragma code high_vector=0x08
void interrupt_at_high_vector(void) {
    _asm GOTO rcISR _endasm
}
#pragma code

#pragma interrupt rcISR
void rcISR(void) {
    unsigned char input;
    // Don't have to wait for data available if we are in ISR
    input = getc1USART();

    // Terminate string on enter or max size
    if (input >= ' ' && input <= 'z' || input == '\r' || input == '\n') {
        if (input == '\r' || input == '\n' || inputSpot == (IN_BUF_SZ - 1)) {
            myInput[inputSpot] = '\0';

            // Reset input, declare finished
            inputSpot = 0;
            inputFinished = 1;
            *ourGlobal.processFlag = 1;
        } else {
            // Put character in the input buffer
            myInput[inputSpot] = input;
            inputSpot++;

            // Print current character
            putc1USART(input);
            inputFinished = 0;
        }
    } else {
        putc1USART(input);
    }

    // Clear interrupt
    PIR1bits.RCIF = 0;
}
/****************************************************/

#define LOCAL 0
/*
 * 
 */
void main() {
    /*
     // pwm GO!
     Disable CCp4
     * select timer2
     * load reg p4 with timer value
     * configure ccp4con
     * load ccpr4L reg, dc4b[1:0](ccp4con) with duty cycle
     * configure timver
     *  clar TMRIF interrup flag
     *  congiure T2CKPS prescale bits
     * set TIMR2EN
     * enambe PWM ->clearing TRIS
     *
     */

     // I2c
    setupOutgoing();

    // Rs232 setup and interrupt
    rs232Setup();

    // Enable SRAM pins correctly
    SRAMsetUp();

    while (1) {
        displayFrontPanel(&ourGlobal);
        dataProcess(&ourGlobal);
        if (*ourGlobal.SRAMflag == 1) {
            writeData(0, *ourGlobal.myCommand);
            writeData(1, *ourGlobal.myOp);
            *ourGlobal.SRAMflag = 0;
            *ourGlobal.i2cFlag = 1;
        }
        if (*ourGlobal.i2cFlag == 1) {
            ourGlobal.setSpeed = readData(1);
            runLocalI2C(ourGlobal.setSpeed);
            *ourGlobal.i2cFlag = 0;
            *ourGlobal.displayFlag = 1;
        }
        runLocalI2C(ourGlobal.setSpeed);
        Delay1KTCYx(1);
    }
}

int startAndWrite(char *c) {
    char data = 0x9A;
    IdleI2C1();
    StartI2C1();
    WriteI2C1(*c); // just write a char
    WriteI2C1(data);
    StopI2C1();
}
