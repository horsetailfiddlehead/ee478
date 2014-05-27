#include <stdio.h>
#include <stdlib.h>
#include <usart.h>
#include <delays.h>

/***************Clocking set up *********************/
#pragma config WDTEN = OFF    // turn off watch dog timer
#pragma config FOSC = ECHP    // Ext. Clk, Hi Pwr
#pragma config PRICLKEN = OFF // disable primary clock
/****************************************************/
#pragma config PBADEN = OFF 	// turn off the ADCs for whatever pins I'm using

/****************************************************/

/*
 * Port RC0 = Red
 * Port RC1 = Green
 */

unsigned int card;
unsigned int status;

void LEDinit (void) {
    TRISCbits.RC0 = 0;
    TRISCbits.RC1 = 0;
    TRISCbits.RC2 = 0;
    TRISCbits.RC3 = 0;
    ANSELCbits.ANSC2 = 0;
    ANSELCbits.ANSC3 = 0;

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
            PORTCbits.RC1 = 1;
            PORTCbits.RC0 = 0;

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

// Testing purposes
void main(void) {
    while(1) {
        // Check Card 1
        status = 1;
        card = 1;
        LEDSelect();
        Delay10KTCYx(50000*1);

        // Check Card 2
        status = 2;
        card = 2;
        LEDSelect();
        Delay10KTCYx(50000*1);

        // Check Card 3
        status = 3;
        card = 3;
        LEDSelect();
        Delay10KTCYx(50000*1);

        // Check Card 4
        status = 4;
        card = 4;
        LEDSelect();
        Delay10KTCYx(50000*1);
    }
}