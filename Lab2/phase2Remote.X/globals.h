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
#include <pwm.h>
#include "SRAM.h"
#include "adc.h"

#include "string.h"
#include "interrupts.h"

    /* DEFINES for the whole project */
#define USE_AND_OR  // allows use of compiler libs

typedef struct globalStruct {
    // System variables
    unsigned int* setSpeed; // the user defined motor speed
    unsigned int* actualSpeed; // Motor speed given from the remote (was motorSpeed)
    unsigned int* controllerSpeed; // Motor speed send to the remote node by the local
    unsigned int* errorState; // The current motor state is off

    // User Input
    unsigned char* myInput;
    int* inputSpot;
    short* inputFinished;

    // Flags
    short* displayFlag;
    short* SRAMflag;
    short* i2cFlag;
    short* processFlag;
    int* myCommand;
    int* myOp;

} Global;

void displayFrontPanel(Global* globalData);
void dataProcess(Global* globalData);

#endif	/* GLOBALS_H */
