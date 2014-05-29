#include "startup.h"

void initClock() {
    TRISAbits.RA7 = 1;
}

void initSPI() {
    OpenSPI1(SPI_FOSC_4, MODE_00, SMPEND);

    SSP1STAT = 0b01000000;

    SSP1CON1 = 0b00000000;
    SSP1CON1bits.SSPEN = 1;

    SSP1CON2 = 0b00000000;

    TRISCbits.RC0 = 0; // A0
    TRISCbits.RC1 = 0; // RST
    TRISCbits.RC2 = 0; // CS
    ANSELCbits.ANSC2 = 0;
    TRISCbits.RC3 = 0; // SCK1
    ANSELCbits.ANSC3 = 0;
    TRISCbits.RC4 = 0;
    ANSELCbits.ANSC4 = 0;
    TRISCbits.RC5 = 0;
    ANSELCbits.ANSC5 = 0;
}
