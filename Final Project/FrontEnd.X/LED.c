#include "globals.h"


/*
 * Port RC0 = Red
 * Port RC1 = Green
 */

unsigned int card;
unsigned int status;

void LEDinit (void) {
    // OUTPUT
    TRISBbits.RB0 = 0;
    TRISBbits.RB1 = 0;
    TRISBbits.RB2 = 0;
    TRISBbits.RB3 = 0;
    
    ANSELBbits.ANSB0 = 0;
    ANSELBbits.ANSB1 = 0;
    ANSELBbits.ANSB2 = 0;
    ANSELBbits.ANSB3 = 0;
    ANSELCbits.ANSC2 = 0;
    ANSELCbits.ANSC3 = 0;



    TRISAbits.RA7 = 1;
}

void LEDColor (void) {
    switch (status) {
        case 0:
            // Yellow: Card registered, but not read.
            PORTCbits.RC1 = 1;
            PORTCbits.RC0 = 1;
            break;
        case 1:
            // Green: Card successfully read.
            PORTCbits.RC1 = 1; //green
            PORTCbits.RC0 = 0; //red

            break;
        case 2:
            // Red: Error, card not read.
            PORTCbits.RC1 = 0;
            PORTCbits.RC0 = 1;
            break;
        default:
            // : Loading
            PORTCbits.RC1 = 0;
            PORTCbits.RC0 = 0;
            break;
    }
}

/* Alternate set up to reduce number of pins:
 * 2 pins to select the LED
 * 2 pins to select the LED color
 */


void LEDSelect (void) {
    switch (card) {
        case 0:
            // Select Card Reader 1
            PORTCbits.RC3 = 0;
            PORTCbits.RC2 = 0;
            LEDColor();
            break;
        case 1:
            // Select Card Reader 2
            PORTCbits.RC3 = 0;
            PORTCbits.RC2 = 1;
            LEDColor();
            break;
        case 2:
            // Select Card Reader 3
            PORTCbits.RC3 = 1;
            PORTCbits.RC2 = 0;
            LEDColor();
            break;
        case 3:
            // Select Card Reader 4
            PORTCbits.RC3 = 1;
            PORTCbits.RC2 = 1;
            LEDColor();
            break;
        default:
            break;
    }

}