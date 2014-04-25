/* 
 * File:   globals.h
 * Author: castia
 *
 * Created on April 23, 2014, 9:30 PM
 *
 * global header file for lab2 project. Put any externs used by other files here
 *
 */

#ifndef GLOBALS_H
#define	GLOBALS_H

    /* includes for the whole project*/
#include <stdio.h>
#include <stdlib.h>
#include <usart.h>
#include <i2c.h>
#include <p18f25k22.h>
#include <delays.h>
#include "SRAM.h"
#include "rs232.h"
#include "string.h"
#include "interrupts.h"

    /* DEFINES for the whole project */
#define USE_AND_OR  // allows use of compiler libs

typedef struct globalStruct {
    unsigned int* setSpeed; // the user defined motor speed
    unsigned int* motorSpeed; // The actual speed of the motor
    unsigned int* errorState; // The current motor state is off
    unsigned char* myInput;
    int* inputSpot;
    short* inputFinished;
    short* displayFlag;
    short* SRAMflag;
    short* i2cFlag;
    short* processFlag;
} Global;

int displayFrontPanel(Global* globalData);
int dataProcess(Global* globalData);

#endif	/* GLOBALS_H */
