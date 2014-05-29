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

#define __KEYPAD_DEBUG 0

#include <stdio.h>
#include <stdlib.h>
#include <usart.h>
#include <delays.h>

#include "keypadDriver.h"

#ifdef __KEYPAD_DEBUG
#include "rs232.h"	// use serial for debugging
#include <p18f46k22.h>

///***************Clocking set up *********************/
//#pragma config WDTEN = OFF    // turn off watch dog timer
//#pragma config FOSC = ECHP    // Ext. Clk, Hi Pwr
//#pragma config PRICLKEN = OFF // disable primary clock
///****************************************************/
//#pragma config PBADEN = OFF 	// turn off the ADCs for whatever pins I'm using

/****************************************************/
#endif

int resetPins(int key);
int checkForInput(void);

//void main() {
//    char keyNum = 0;
//    keypadSetup();
//    rs232Setup1(); // setup the serial port
//
//    while (1) { // just loop for test
//        keyNum = (char) checkForInput() + '`';
//        Write1USART(keyNum);
//
//        Delay1KTCYx(1);
//    }
//
//}

/*
 * Polls the keypad for a key press. The key is stored in the global data struct given.
 */
void keypad(GlobalState *gData) {
    gData->keyPress = checkForInput();
    if (gData->keyPress >= 0) {
        gData->keyFlag = TRUE;
    }
}

// checks the keypad for key press. returns the first key press sensed.
// Returns the number of the key pressed (* = 14, # = 15)

int checkForInput() {
    char scan;

    PORTDbits.RD2 = 0; // check row1
    Delay10TCYx(10);
    scan = (PORTBbits.RB3 << 3 | PORTBbits.RB2 << 2 | PORTBbits.RB1 << 1 | PORTBbits.RB0);
    switch (scan) {
        case 0b00001110: // is on Bbits.RB3
            return resetPins(1);
        case 0b00001101: // on pin 5
            return resetPins(2);
        case 0b00001011: // on pin 6
            return resetPins(3);
        case 0b00000111: // on pin 7
            return resetPins(10);
    }

    PORTDbits.RD2 = 1; // check row2
    PORTDbits.RD3 = 0;
    Delay10TCYx(10);
    scan = (PORTBbits.RB3 << 3 | PORTBbits.RB2 << 2 | PORTBbits.RB1 << 1 | PORTBbits.RB0);
    switch (scan) {
        case 0b00001110: // is on Bbits.RB3
            return resetPins(4);
        case 0b00001101: // on pin 5
            return resetPins(5);
        case 0b00001011: // on pin 6
            return resetPins(6);
        case 0b00000111: // on pin 7
            return resetPins(11);
    }


    PORTDbits.RD3 = 1; // check row3
    PORTDbits.RD4 = 0;
    Delay10TCYx(10);
    scan = (PORTBbits.RB3 << 3 | PORTBbits.RB2 << 2 | PORTBbits.RB1 << 1 | PORTBbits.RB0);
    switch (scan) {
        case 0b00001110: // is on Bbits.RB3
            return resetPins(7);
        case 0b00001101: // on pin 5
            return resetPins(8);
        case 0b00001011: // on pin 6
            return resetPins(9);
        case 0b00000111: // on pin 7
            return resetPins(12);
    }

    PORTDbits.RD4 = 1; // check row4
    PORTDbits.RD5 = 0;
    Delay10TCYx(10);
    scan = (PORTBbits.RB3 << 3 | PORTBbits.RB2 << 2 | PORTBbits.RB1 << 1 | PORTBbits.RB0);
    switch (scan) {
        case 0b00001110: // is on Bbits.RB3
            return resetPins(14);
        case 0b00001101: // on pin 5
            return resetPins(0);
        case 0b00001011: // on pin 6
            return resetPins(15);
        case 0b00000111: // on pin 7
            return resetPins(13);
    }

    return resetPins(-1); // assume no key was pressed.
}


// resets the driving pins

int resetPins(int key) {
    PORTDbits.RD5 = 1; // set outputs HIGH
    PORTDbits.RD4 = 1;
    PORTDbits.RD3 = 1;
    PORTDbits.RD2 = 1;
    return key;
}

/* sets all pins to input or output and disables analog in;
 * sets initial port outputs to HIGH
 */
void keypadSetup() {
    // initialize pins 4-7 HIGH
    PORTDbits.RD2 = 1;
    PORTDbits.RD3 = 1;
    PORTDbits.RD4 = 1;
    PORTDbits.RD5 = 1;

    // pins 4-7 are toggled, pins 0-3 are monitored
    TRISDbits.RD5 = 0;
    TRISDbits.RD4 = 0;
    TRISDbits.RD3 = 0;
    TRISDbits.RD2 = 0;

    TRISBbits.RB0 = 1;
    TRISBbits.RB1 = 1;
    TRISBbits.RB2 = 1;
    TRISBbits.RB3 = 1;

    ANSELBbits.ANSB3 = 0; // disable analog input
    ANSELBbits.ANSB2 = 0;
    ANSELBbits.ANSB1 = 0;
    ANSELBbits.ANSB0 = 0;
    ANSELDbits.ANSD2 = 0;
    ANSELDbits.ANSD3 = 0;
    ANSELDbits.ANSD4 = 0;
    ANSELDbits.ANSD5 = 0;


    // enable weak pull ups on ports b0-b3
    WPUB = WPUB & 0b11111111;
    // enable pull ups on portB globally
    INTCON2 = INTCON2 & 0b01111111;

    return;
}
