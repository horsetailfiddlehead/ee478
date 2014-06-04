#include "startup.h"

// sets the external clock pin as input

void initClock() {
    TRISAbits.RA7 = 1;
}

// sets up SPI port for Display communication

void initSPI1() {
    // Could use the C18 function, but this gives more control
    SSP1STAT = 0b01000000;
    SSP1CON1 = 0b00000000;
    SSP1CON1bits.SSPEN = 1;
    SSP1CON2 = 0b00000000;

    TRISCbits.RC4 = 0; // A0
    //    TRISCbits.RC1 = 0; // RST
    TRISBbits.RB4 = 0; // CS
    TRISCbits.RC3 = 0; // SCK1
    TRISCbits.RC5 = 0; // SDO1
    ANSELBbits.ANSB4 = 0;
    ANSELCbits.ANSC3 = 0;
    ANSELCbits.ANSC4 = 0;
    ANSELCbits.ANSC5 = 0;
}
