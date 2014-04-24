#include "GLOBALS_H"
#include <p18f25k22.h>

unsigned short OE;
unsigned short WE;
unsigned short store;

void setUp () {
	// Write Enable
	TRISBbits.TRISB4 = 1;
	
	// Output Enable
	TRISBbits.TRISB5 = 1;
	
	// Store
	TRISBbits.TRISB3 = 1;
	
	setUpIn();
}

void setUpOut () {
	// Set data outputs
	TRISAbits.TRISA0 = 0;
	TRISAbits.TRISA1 = 0;
	TRISAbits.TRISA2 = 0;
	TRISAbits.TRISA3 = 0;
	TRISAbits.TRISA4 = 0;
	TRISAbits.TRISA5 = 0;
	TRISCbits.TRISC0 = 0;
	TRISCbits.TRISC1 = 0;
}

void setUpIn () {
	// Set data inputs
	TRISAbits.TRISA0 = 1;
	TRISAbits.TRISA1 = 1;
	TRISAbits.TRISA2 = 1;
	TRISAbits.TRISA3 = 1;
	TRISAbits.TRISA4 = 1;
	TRISAbits.TRISA5 = 1;
	TRISCbits.TRISC0 = 1;
	TRISCbits.TRISC1 = 1;
}

// Reading current speed
void readSpeed (int adx) {
	setUp();
	setUpOut();
	
	// Setting up Address
	PORTAbits.PORTA0 = (adx >> 0) & 0x01;
	PORTAbits.PORTA1 = (adx >> 1) & 0x01;
	PORTAbits.PORTA2 = (adx >> 2) & 0x01;
	PORTAbits.PORTA3 = (adx >> 3) & 0x01;
	PORTAbits.PORTA4 = (adx >> 4) & 0x01;
	PORTAbits.PORTA5 = (adx >> 5) & 0x01;
	PORTAbits.PORTC0 = (adx >> 6) & 0x01;
	PORTAbits.PORTC1 = (adx >> 7) & 0x01;

	TRISBbits.TRISB3 = 0;
	TRISBbits.TRISB3 = 1;
	
	setUpIn();
	
	TRISBbits.TRISB5 = 0; // OE

}

// Writing current speed
void writeSpeed(int adx) {
	setUpIn();
}


// Writing current error state
void writeState(int adx) {
	setUpOut();
}

// Reading current error state
void readState(int adx) {
	setUpIn();
}