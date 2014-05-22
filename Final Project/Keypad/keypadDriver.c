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

#ifdef __KEYPAD_DEBUG
#include "rs232.c"	// use serial for debugging
#endif

/***************Clocking set up *********************/
#pragma config WDTEN = OFF;    // turn off watch dog timer
#pragma config FOSC = ECHP;    // Ext. Clk, Hi Pwr
#pragma config PRICLKEN = OFF; // disable primary clock
/****************************************************/
#pragma config ICSP = ON;			// turn on the In-circuit programmer
#pragma config PBADEN = OFF; 	// turn off the ADCs for whatever pins I'm using

/****************************************************/

/* Notes: don't know which pins are going to be used. I'm just going to use the
 * pin numbers in the schematic above.
 */

int checkForInput();
int resetPins();
void keypadSetup();

#ifdef __KEYPAD_DEBUG
void main(){
	int keyNum = 0;
	keypadSetup();
rs232Setup(); // setup the serial port

	while(1) { // just loop for test
		keyNum = checkForInput();
		putc1USART(keyNum);

		Delay10CYx(10);
	}

}
#endif

// checks the keypad for key press. returns the first key press sensed.
// Returns the number of the key pressed (* = 14, # = 15)
int checkForInput(){
	int key = -1;

	
	PORTpin0 = 1;	// set outputs HIGH
	PORTpin1 = 1; 
	PORTpin2 = 1;
	PORTpin3 = 1;

	if( PORTpin4 == 1 || PORTpin5 == 1 || PORTpin6 == 1 || PORTpin7 == 1) {
		PORTpin1 = 0; // leave pin0 high
		PORTpin2 = 0;
		PORTpin3 = 0;
		char scan = PORTpin4 << 3 || PORTpin5 << 2 || PORTpin6 << 1 || PORTpin7;
		switch (scan) {
			case 16: // is on pin4
				return resetPins(1);
			case 8: // on pin 5
				return resetPins(2);
			case 2:  // on pin 6
				return resetPins(3);
			case 1:  // on pin 7
				return resetPins(10);
			default :
				return resetPins(-2); // should never get here
		}

		PORTpin0 = 0; // check column2
		PORTpin1 = 1;
		scan = PORTpin4 << 3 || PORTpin5 << 2 || PORTpin6 << 1 || PORTpin7;
		switch (scan) {
			case 16: // is on pin4
				return resetPins(4);
			case 8: // on pin 5
				return resetPins(5);
			case 2:  // on pin 6
				return resetPins(6);
			case 1:  // on pin 7
				return resetPins(11);
			default :
				return resetPins(-2); // should never get here
		}

		PORTpin1 = 0; // check column3
		PORTpin2 = 1;
		scan = PORTpin4 << 3 || PORTpin5 << 2 || PORTpin6 << 1 || PORTpin7;
		switch (scan) {
			case 16: // is on pin4
				return resetPins(7);
			case 8: // on pin 5
				return resetPins(8);
			case 2:  // on pin 6
				return resetPins(9);
			case 1:  // on pin 7
				return resetPins(12);
			default :
				return resetPins(-2); // should never get here
		}

		PORTpin2 = 0; // check column4
		PORTpin3 = 1;
		scan = PORTpin4 << 3 || PORTpin5 << 2 || PORTpin6 << 1 || PORTpin7;
		switch (scan) {
			case 16: // is on pin4
				return resetPins(14);
			case 8: // on pin 5
				return resetPins(0);
			case 2:  // on pin 6
				return resetPins(15);
			case 1:  // on pin 7
				return resetPins(13);
			default :
				return resetPins(-2); // should never get here
		}
	}

	return resetPins(-1);  // assume no key was pressed.
}


// resets the driving pins
int resetPins(int key) {
	PORTpin0 = 0;	// set outputs LOW
	PORTpin1 = 0; 
	PORTpin2 = 0;
	PORTpin3 = 0;
	return key;
}


// sets all pins to input or output and disables analog in; sets initial port outputs to LOW. May want to consider a better starting point, like all outputs high?
void keypadSetup() {
	// pins 0-3 are toggled, pins 4-7 are monitored
	TRISpin0 = 0;
	TRISpin1 = 0;
	TRISpin2 = 0;
	TRISpin3 = 0;
	TRISpin4 = 1;
	TRISpin5 = 1;
	TRISpin6 = 1;
	TRISpin7 = 1;

	ANSELpin0 = 0;	// disable analog input
	ANSELpin1 = 0; 
	ANSELpin2 = 0;
	ANSELpin3 = 0;
	ANSELpin4 = 0;
	ANSELpin5 = 0;
	ANSELpin6 = 0;
	ANSELpin7 = 0;


	PORTpin0 = 0;	// disable set outputs
	PORTpin1 = 0; 
	PORTpin2 = 0;
	PORTpin3 = 0;
	PORTpin4 = 0;
	PORTpin5 = 0;
	PORTpin6 = 0;
	PORTpin7 = 0;

	return;
}
