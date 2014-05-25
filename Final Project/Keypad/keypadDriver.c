/*
 * Created: Patrick Ma
 * Date: May 21, 2014
 *
 * keypadDriver.c
 *
 * The software driver for the keypad. The current option uses 8 pins and polls
 * the matrix looking for key presses.
 *
 * The keypad has a layout like this:
 *
 *    1|  2|  3|  A| pin
 *  ----------------- 4
 *    4|  5|  6|  B|
 *  ----------------- 5
 *    7|  8|  9|  C|
 *  ----------------- 6
 *    *|  0|  #|  D|
 *  ----------------- 7
 *     |   |   |   |
 * pin 0   1   2   3
 *
 */

#define __KEYPAD_DEBUG 1

#include <stdio.h>
#include <stdlib.h>
#include <usart.h>
#include <delays.h>

#include "keypadDriver.h"

#ifdef __KEYPAD_DEBUG
#include "rs232.h"	// use serial for debugging
#include <p18f25k22.h>
#endif

/***************Clocking set up *********************/
#pragma config WDTEN = OFF    // turn off watch dog timer
#pragma config FOSC = ECHP    // Ext. Clk, Hi Pwr
#pragma config PRICLKEN = OFF // disable primary clock
/****************************************************/
#pragma config PBADEN = OFF 	// turn off the ADCs for whatever pins I'm using

/****************************************************/


//int checkForInput(void);

int resetPins(int key);
void keypadSetup();
int checkForInput();

//void keypadSetup(void);

#ifdef __KEYPAD_DEBUG

void main() {
    char keyNum = 0;
    keypadSetup();
    rs232Setup1(); // setup the serial port

    while (1) { // just loop for test
        keyNum = (char) checkForInput() + '`';
        Write1USART(keyNum);

        Delay10KTCYx(1);
    }

}
#endif

// checks the keypad for key press. returns the first key press sensed.
// Returns the number of the key pressed (* = 14, # = 15)

int checkForInput() {
    int key = -1;
    char scan;

    PORTCbits.RC0 = 0; // set outputs LOW
    //    PORTCbits.RC2 = 1;
    //    PORTCbits.RC1 = 1;
    //    PORTCbits.RC0 = 1;

    //    if (PORTBbits.RB0 == 1 || PORTBbits.RB1 == 1 || PORTBbits.RB2 == 1 || PORTBbits.RB3 == 1) {
    //        PORTCbits.RC2 = 0; // leave Cbits.RC3 high
    //        PORTCbits.RC1 = 0;
    //        PORTCbits.RC0 = 0;
    Delay10TCYx(10);
    scan = (PORTBbits.RB0 << 3 | PORTBbits.RB1 << 2 | PORTBbits.RB2 << 1 | PORTBbits.RB3);
    //        Write1USART(scan);
    switch (scan) {
        case 0b00001110: // is on Bbits.RB3
            return resetPins(1);
        case 0b00001101: // on pin 5
            return resetPins(2);
        case 0b00001011: // on pin 6
            return resetPins(3);
        case 0b00000111: // on pin 7
            return resetPins(10);
            //         default:
            //             key = -2; // return resetPins(-2); // should never get here
    }

    PORTCbits.RC0 = 1; // check column2
    PORTCbits.RC1 = 0;
    Delay10TCYx(10);
    scan = (PORTBbits.RB0 << 3 | PORTBbits.RB1 << 2 | PORTBbits.RB2 << 1 | PORTBbits.RB3);
    Write1USART(scan);
    switch (scan) {
        case 0b00001110: // is on Bbits.RB3
            return resetPins(4);
        case 0b00001101: // on pin 5
            return resetPins(5);
        case 0b00001011: // on pin 6
            return resetPins(6);
        case 0b00000111: // on pin 7
            return resetPins(11);
            //         default:
            //             key = -2; // return resetPins(-2); // should never get here
    }


    PORTCbits.RC1 = 1; // check column3
    PORTCbits.RC2 = 0;
    Delay10TCYx(10);


    PORTCbits.RC2 = 1; // check column4
    PORTCbits.RC3 = 0;
    Delay10TCYx(10);
    

    return resetPins(key); // assume no key was pressed.
}


// resets the driving pins

int resetPins(int key) {
    PORTCbits.RC3 = 1; // set outputs HIGH
    PORTCbits.RC2 = 1;
    PORTCbits.RC1 = 1;
    PORTCbits.RC0 = 1;
    return key;
}


// sets all pins to input or output and disables analog in; sets initial port outputs to LOW. May want to consider a better starting point, like all outputs high?

void keypadSetup() {
    //    PORTBbits.RB3 = 0; // disable set outputs
    //    PORTBbits.RB2 = 0;
    //    PORTBbits.RB1 = 0;
    //    PORTBbits.RB0 = 0;
    PORTCbits.RC0 = 1;
    PORTCbits.RC1 = 1;
    PORTCbits.RC2 = 1;
    PORTCbits.RC3 = 1;

    // pins 4-7 are toggled, pins 0-3 are monitored
    TRISCbits.RC3 = 0;
    TRISCbits.RC2 = 0;
    TRISCbits.RC1 = 0;
    TRISCbits.RC0 = 0;
    TRISBbits.RB0 = 1;
    TRISBbits.RB1 = 1;
    TRISBbits.RB2 = 1;
    TRISBbits.RB3 = 1;

    ANSELBbits.ANSB3 = 0; // disable analog input
    ANSELBbits.ANSB2 = 0;
    ANSELBbits.ANSB1 = 0;
    ANSELBbits.ANSB0 = 0;
    //        ANSELCbits.ANSC0 = 0;
    //        ANSELCbits.ANSC1 = 0;
    ANSELCbits.ANSC2 = 0;
    ANSELCbits.ANSC3 = 0;

    // enable weak pull ups on ports b0-b3
    WPUB = WPUB & 0b11111111;
    // enable pull ups on portB globally
    INTCON2 = INTCON2 & 0b01111111;



    return;
}
