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

//#include "display.h"

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
#define DEF_PWM 00
/***********************Global Setup*****************/
// System variables
unsigned int setSpeed = DEF_PWM; // the user defined motor speed (from local node)
unsigned int actualSpeed = 0; // Motor speed given from the remote (was motorSpeed)
unsigned int controllerSpeed = DEF_PWM; // PWM motor speed
unsigned int errorState = 4; // The current motor state is off

// User Input
unsigned char myInput[IN_BUF_SZ];
int inputSpot = 0;
short inputFinished = 0;

// Flags
short displayFlag = 1;
short SRAMflag = 0;
short i2cFlag = 0;
short processFlag = 0;
int myCommand = 0;
int myOp = 0;

Global ourGlobal = {&setSpeed, &actualSpeed, &controllerSpeed, &errorState, &myInput, &inputSpot, &inputFinished, &displayFlag, &SRAMflag, &i2cFlag, &processFlag, &myCommand, &myOp};

/****************************************************/

/****************Interrupt Stuff*********************/

/* Global Controls for I/O banks*/
#pragma config PBADEN = OFF   // turn off bank B ADCs

#pragma code high_vector=0x08

void interrupt_at_high_vector(void) {
    _asm GOTO i2cISR _endasm
}
#pragma code

#pragma interrupt i2cISR

void i2cISR(void) {
    int temp = 0;
    if (SSP2STATbits.D_A == 0 && SSP2STATbits.BF == 1) {
        temp = SSP2BUF; // get rid of address
    }
    if (SSP2STATbits.D_A == 1 && SSP2STATbits.BF == 1) {
        *ourGlobal.setSpeed = SSP2BUF; // new motor set speed
    }

    PIR3bits.SSP2IF = 0;
    *ourGlobal.SRAMflag = 1;
}
///****************************************************/

int readADC();

/* setsup the pwm to output the voltage
 */
void setupPWM() {
    // configure PWM
    TRISBbits.RB0 = 1; // disable PWM output
    CCPTMRS1 = 0b00000001; // set C4TSEL = 0b01
    PR4 = 0xF9; // PR = 2 for 20kHz
    T4CON = 0b00000100; // set timer prescale 1:1, turn on timer4

    CCP4CON = 0b00011100; // set LSB of duty cyle, select pwm mode
    CCPR4L = 0x3E; // set MSB of duty cycle
    PIR5 = 0b00000000; // clear timer interrupt flag
    TRISBbits.RB0 = 0; //enable PWM output
    // Rs232 setup and interrupt

}

/*
 *
 */
void main() {
    unsigned int temp = 0;
    unsigned int correct = 0;
    unsigned int delay = 0;

    // setup i2c
    setupIncoming();
    setupOutgoing();

    //setup PWM
    setupPWM();

    // Setup ADC
    TRISCbits.RC2 = 1;
    TRISCbits.RC5 = 0;
    ANSELCbits.ANSC5 = 0;
    ANSELCbits.ANSC2 = 1; //set as input
    OpenADC(ADC_FOSC_64 & ADC_RIGHT_JUST & ADC_12_TAD,
            ADC_CH14 & ADC_INT_OFF, 15);

    // Enable SRAM pins correctly
    SRAMsetUp();

    // Set Default PWM
    SetDCPWM4(5 * (*ourGlobal.controllerSpeed));
    writeData(1, *ourGlobal.controllerSpeed);

    while (1) {
        // Store a set speed
        if (*ourGlobal.SRAMflag == 1) {
            writeData(1, *ourGlobal.setSpeed);
            *ourGlobal.processFlag = 1;
            *ourGlobal.SRAMflag = 0;
        }

        // Measure Motor
        Delay10KTCYx(8);
        PORTCbits.RC5 = 1; // conversion indicator
        ConvertADC();
        while (BusyADC());
        temp = ReadADC();
        temp = ((50 * (temp)) >> 8) + 1;
        PORTCbits.RC5 = 0; // conversion indicator

        // only makes sense to update ADC if significantly different than
        // prior reading
        if ((temp - 1 > *ourGlobal.actualSpeed) || (*ourGlobal.actualSpeed > temp + 1)) {
            correct = temp;
            *ourGlobal.actualSpeed = correct;
            *ourGlobal.i2cFlag = 1;
        }

        // Get error state
        errorCheck(&ourGlobal);

        // enable auto correct
        //if (*ourGlobal.actualSpeed < (*ourGlobal.setSpeed - 1) ||
        //(*ourGlobal.setSpeed + 1) < *ourGlobal.actualSpeed) {
        controller(&ourGlobal); // compute the control function and adjust pwm
        //}

        
        // Process updated controller speed
        if (*ourGlobal.processFlag == 1) {
            *ourGlobal.processFlag = 0;
            writeData(2, *ourGlobal.controllerSpeed); // update the motor output
        }
        SetDCPWM4(5 * readData(2));

        // Transfer actual speed to local node
        if (*ourGlobal.i2cFlag == 1) {
            *ourGlobal.i2cFlag = 0;
            // Adjust actual speed for tranmittal as 8 bits. We can't go above
            // 65535 in th emath or it breaks 16 bit max int.
            // So 100 * 65 is the max doing it a conventional way
            //correct = 50 * ((*ourGlobal.actualSpeed) >> 8); //correct = 2 * (2*((50 * *ourGlobal.actualSpeed) / 1023));

            Delay1KTCYx(1);
            runLocalI2C(ourGlobal.actualSpeed);
            // TODO: Calculate and send error
        }

        Delay1KTCYx(1);
    }
}

int readADC() {
    ADCON0bits.GO_DONE = 1;
    while (ADCON0bits.GO_DONE);
    ADCON0bits.ADON = 0;
    return ADRESH;
}
