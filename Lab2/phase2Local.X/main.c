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
#include "adc.h"

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
#define DEF_PWM 20
/***********************Global Setup*****************/
// System variables
unsigned int setSpeed = 0; // the user defined motor speed
unsigned int actualSpeed = 0; // Motor speed given from the remote (was motorSpeed)
unsigned int controllerSpeed = DEF_PWM; // Motor speed send to the remote node by the local
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

#pragma code low_vector=0x18

void interrupt_at_low_vector(void) {
    _asm GOTO i2cISR _endasm
}
#pragma code

#pragma interrupt i2cISR

void i2cISR(void) {
    unsigned int temp = 0;
    static int byteNum = 0;
    if (SSP2STATbits.D_A == 0 && SSP2STATbits.BF == 1) {
        temp = SSP2BUF;
    }
    if (SSP2STATbits.D_A == 1 && SSP2STATbits.BF == 1) {
//        if (2 > byteNum) {    // send two bytes for speed
//            *ourGlobal.actualSpeed = (*ourGlobal.actualSpeed << 8) | SSP2BUF;
//        }
//        byteNum++;
        *ourGlobal.actualSpeed =  SSP2BUF;
    }
    *ourGlobal.displayFlag = 1;
    PIR3bits.SSP2IF = 0;
}

/****************************************************/

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

}

/*
 * 
 */
void main() {

    unsigned int temp;
    // I2c Setup
    char errorMsg[] = "Error: Input again.\n\r";

    setupOutgoing();
    setupIncoming();

    // Rs232 setup and interrupt
    rs232Setup();

    // Enable SRAM pins correctly
    SRAMsetUp();

    // adc
    //    ADCON0 = 0b00111000;
    //    ADCON1 = 0;
    //    ADCON2 = 0b101110000;
    //    TRISCbits.RC2 = 1;
    //    TRISCbits.RC5 = 0;
    //    ANSELCbits.ANSC5 = 0;
    //    ANSELCbits.ANSC2 = 1; //set as input
    OpenADC(ADC_FOSC_64 & ADC_RIGHT_JUST & ADC_12_TAD,
            ADC_CH14 & ADC_INT_OFF, 15);

    // setup PWM (debug purposes only)
    setupPWM();

    // Set Default PWM
    SetDCPWM4(5 * (*ourGlobal.controllerSpeed));
    writeData(1, *ourGlobal.controllerSpeed);

    while (1) {
        displayFrontPanel(&ourGlobal);
        dataProcess(&ourGlobal);
        if (*ourGlobal.SRAMflag == 1) {
            int tempA;
            writeData(0, *ourGlobal.myCommand);
            switch (*ourGlobal.myCommand) {
                case 1:
                    writeData(1, *ourGlobal.controllerSpeed);
                    *ourGlobal.i2cFlag = 1;
                    break;
                case 2:
                    writeData(1, readData(1) + 1);
                    *ourGlobal.i2cFlag = 1;
                    break;
                case 3:
                    writeData(1, readData(1) - 1);
                    *ourGlobal.i2cFlag = 1;
                    break;
                case 4:
                    puts1USART(errorMsg);
                    *ourGlobal.displayFlag = 1;
                    break;
            }
            *ourGlobal.SRAMflag = 0;

        }
        if (*ourGlobal.i2cFlag == 1) {
            *ourGlobal.setSpeed = readData(1);
            SetDCPWM4(5 * (*ourGlobal.setSpeed));
            runLocalI2C(ourGlobal.setSpeed);
            *ourGlobal.i2cFlag = 0;
            *ourGlobal.displayFlag = 1;
            //
            //            Delay10KTCYx(5);
            //            PORTCbits.RC5 = 1;
            //            ConvertADC();
            //            while (BusyADC());
            //            temp = ReadADC();
            //            PORTCbits.RC5 = 0;
            //            *ourGlobal.actualSpeed = temp;

        }
        Delay1KTCYx(1);

    }
}

// as the name says

int readADC() {
    ADCON0bits.GO_DONE = 1;
    while (ADCON0bits.GO_DONE);
    ADCON0bits.ADON = 0;
    return ADRESL;
}