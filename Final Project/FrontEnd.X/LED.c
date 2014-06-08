#include "globals.h"


/*
 * Port RC0 = Red
 * Port RC1 = Green
 */

unsigned int card;
unsigned int status;

/*
 * Setup the LED Driver
 */
void LEDinit(void) {
    TRISB = 0xF0; // set pins 0:3 as outputs, 4:7 as inputs
    ANSELB = 0x00; // disable analog input

    WPUB |= 0xF0; // enable internal pullup on card inputs
    INTCON2bits.RBPU = 0;

    LATB = 0x00; // clear existing mismatch conditions

    IOCB = 0xF0; // enable IOC interrupts on pins B4:B7
    INTCONbits.RBIE = 1; // enable PortB interrupts
    INTCON2bits.RBIP = 1;
    INTCONbits.GIE = 1;
}

void LEDColor(void) {
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


void LEDSelect(void) {
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