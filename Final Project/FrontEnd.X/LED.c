#include "globals.h"


/*
 * Port RC0 = Red
 * Port RC1 = Green
 */

LEDDriver ledData;

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

    // initialize ledData
    memset(ledData.ledStatus, 0, sizeof(char) * NUM_SLOTS);
}


/*
 * Primary led driverr
 */
void updateLEDs() {
    char status = 0;
    char card = 0;
    for (card = 0; card < NUM_SLOTS; card++) {
        status = ledData.ledStatus[card];
        LEDSelect(card, status);
    }
}


/*
 * updates the LEDs with the appropriate status
 */
void LEDColor(char status) {
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


void LEDSelect(char card, char status) {
    switch (card) {
        case 0:
            // Select Card Reader 1
            PORTCbits.RC3 = 0;
            PORTCbits.RC2 = 0;
            LEDColor(status);
            break;
        case 1:
            // Select Card Reader 2
            PORTCbits.RC3 = 0;
            PORTCbits.RC2 = 1;
            LEDColor(status);
            break;
        case 2:
            // Select Card Reader 3
            PORTCbits.RC3 = 1;
            PORTCbits.RC2 = 0;
            LEDColor(status);
            break;
        case 3:
            // Select Card Reader 4
            PORTCbits.RC3 = 1;
            PORTCbits.RC2 = 1;
            LEDColor(status);
            break;
        default:
            break;
    }
}