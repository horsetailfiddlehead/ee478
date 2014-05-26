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

void main(void ) {

}


/*
 * Port RC0 = Red
 * Port RC1 = Green
 * Port RC2 = Blue
 */

void LEDColor (int status) {
    while(1) {
        switch (status) {
            case 0:
                // Yellow: Card registered, but not read.
                PORTCbits.RC2 = 0;
                PORTCbits.RC1 = 1;
                PORTCbits.RC0 = 1;
                break;
            case 1:
                // Green: Card successfully read.
                PORTCbits.RC2 = 0;
                PORTCbits.RC1 = 1;
                PORTCbits.RC0 = 0;

                break;
            case 2:
                // Red: Error, card not read.
                PORTCbits.RC2 = 0;
                PORTCbits.RC1 = 0;
                PORTCbits.RC0 = 1;
                break;
            default:
                // Blue: Loading
                PORTCbits.RC2 = 1;
                PORTCbits.RC1 = 0;
                PORTCbits.RC0 = 0;
                break;
        }
    }
}

/* Alternate set up to reduce number of pins:
 * 2 pins to select the LED
 * 3 pins to select the LED color
 */


void LEDSelect (int card, int status) {
    while (1) {
        switch (card) {
            case 0:
                // Select Card Reader 1
                PORTCbits.RC1 = 0;
                PORTCbits.RC0 = 0;
                LEDColor(status);
                break;
            case 1:
                // Select Card Reader 2
                PORTCbits.RC1 = 0;
                PORTCbits.RC0 = 1;
                LEDColor(status);
                break;
            case 2:
                // Select Card Reader 3
                PORTCbits.RC1 = 1;
                PORTCbits.RC0 = 0;
                LEDColor(status);
                break;
            default:
                // Select Card Reader 4
                PORTCbits.RC1 = 1;
                PORTCbits.RC0 = 1;
                LEDColor(status);
                break;
        }
    }
}