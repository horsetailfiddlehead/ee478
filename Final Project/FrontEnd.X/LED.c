#include "globals.h"

/*
 * Prototypes
 */
void LEDSelect(char card, char status);
void LEDColor(char status);


/*
 * Port RB1 = Red
 * Port RB0 = Green
 */

LEDDriverStruct ledData;

/*
 * Setup the LED Driver
 */
void LEDSetup(void) {
    TRISB = 0x30; //0xF0; // set pins 0:3 as outputs, 4:7 as inputs
    ANSELB = 0x00; // disable analog input

    WPUB |= 0x30; //0xF0; // enable internal pullup on card inputs
    INTCON2bits.RBPU = 0;

    // initialize ledData
    memset(ledData.ledStatus, 0x03, sizeof(char) * NUM_SLOTS);
    updateLEDs();
    
    PORTB = 0;
    LATB = PORTB ; // clear existing mismatch conditions

    IOCB = 0x30; //0xF0; // enable IOC interrupts on pins B4:B7
    INTCONbits.RBIE = 1; // enable PortB interrupts
    INTCON2bits.RBIP = 1; // set priority level to high
    INTCONbits.GIE = 1; // enable general purpose interrupts

    
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
            PORTBbits.RB0 = 1;
            PORTBbits.RB1 = 1;
            break;
        case 1:
            // Green: Card successfully read.
            PORTBbits.RB0 = 1; //green
            PORTBbits.RB1 = 0; //red

            break;
        case 2:
            // Red: Error, card not read.
            PORTBbits.RB0 = 0;
            PORTBbits.RB1 = 1;
            break;
        default:
            // : Loading
            PORTBbits.RB0 = 0;
            PORTBbits.RB1 = 0;
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
            PORTBbits.RB3 = 0;
            PORTBbits.RB2 = 0;
            LEDColor(status);
            break;
        case 1:
            // Select Card Reader 2
            PORTBbits.RB3 = 0;
            PORTBbits.RB2 = 1;
            LEDColor(status);
            break;
        case 2:
            // Select Card Reader 3
            PORTBbits.RB3 = 1;
            PORTBbits.RB2 = 0;
            LEDColor(status);
            break;
        case 3:
            // Select Card Reader 4
            PORTBbits.RB3 = 1;
            PORTBbits.RB2 = 1;
            LEDColor(status);
            break;
        default:
            break;
    }
}