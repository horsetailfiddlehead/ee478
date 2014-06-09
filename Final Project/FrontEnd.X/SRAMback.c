//#include "GLOBALS_H"
#include "globals.h"
#include "SRAM.h"
#include <delays.h>

/*
 * SRAM for the backend
 */

#if !FRONT_NOT_BACK

unsigned short OE;
unsigned short WE;
unsigned short store;

void SRAMsetUp() {
    // Write Enable
    TRISCbits.TRISC3 = 0;

    // Output Enable
    TRISCbits.TRISC2 = 0;

    // Store
    TRISCbits.TRISC1 = 0;

    // Default Values
    PORTCbits.RC3 = 1;
    PORTCbits.RC2 = 1;
    PORTCbits.RC1 = 1;

    // Set digitalOut
    ANSELAbits.ANSA0 = 0;
    ANSELAbits.ANSA1 = 0;
    ANSELAbits.ANSA2 = 0;
    ANSELAbits.ANSA5 = 0;
    ANSELEbits.ANSE0 = 0;
    ANSELEbits.ANSE1 = 0;
    ANSELEbits.ANSE2 = 0;

    ANSELCbits.ANSC3 = 0;
    ANSELCbits.ANSC2 = 0;
    //ANSELCbits.ANSC1 = 0; NOT NEEDED FOR PORT C

    setUpIn();
}

void setUpOut() {
    // Set data outputs
    TRISAbits.TRISA0 = 0;
    TRISAbits.TRISA1 = 0;
    TRISAbits.TRISA2 = 0;
    TRISAbits.TRISA4 = 0;
    TRISAbits.TRISA5 = 0;
    TRISAbits.TRISA6 = 0;
    TRISE &= 0b11111000;
    TRISCbits.TRISC0 = 0;
}

void setUpIn() {
    // Set data inputs
    TRISAbits.TRISA0 = 1;
    TRISAbits.TRISA1 = 1;
    TRISAbits.TRISA2 = 1;
    TRISAbits.TRISA4 = 1;
    TRISAbits.TRISA5 = 1;
    TRISAbits.TRISA6 = 1;
    TRISE = 0xFF;
    TRISCbits.TRISC0 = 1;
}

// Reading data

int readData(int adx) {
    int myRead = 0;
    SRAMsetUp();
    setUpOut();

    // Setting up Address
    PORTAbits.RA0 = (adx >> 0) & 0x01;
    PORTAbits.RA1 = (adx >> 1) & 0x01;
    PORTAbits.RA2 = (adx >> 2) & 0x01;
    PORTAbits.RA4 = (adx >> 3) & 0x01;
    PORTAbits.RA5 = (adx >> 4) & 0x01;
    LATEbits.LATE0 = (adx >> 5) & 0x01; // problem?
    LATEbits.LATE1 = (adx >> 6) & 0x01; // problem?
    LATEbits.LATE2 = (adx >> 7) & 0x01; // problem?
    PORTAbits.RA6 = (adx >> 8) & 0x01;
    PORTCbits.RC0 = (adx >> 9) & 0x01;

    // Store in MAR
    PORTCbits.RC1 = 0;
    PORTCbits.RC1 = 1;

    // I/O are inputs
    setUpIn();

    // Output Enable
    PORTCbits.RC2 = 0;
    Delay10TCYx(5);
    // Get the first 3 bits of Port A, bits 4-5 of port A, and the first 3 bits of port E
    myRead = (PORTA & 0x07) | ((PORTA & 0x18) >> 1) | ((PORTE & 0x07) << 5);
            //    Delay10TCYx(5);
            // Output Enable
            PORTCbits.RC2 = 1;

    return myRead;

}



// Writing data

void writeData(int adx, int data) {
    SRAMsetUp();
    setUpOut();

    // Setting up Address
    PORTAbits.RA0 = (adx >> 0) & 0x01;
    PORTAbits.RA1 = (adx >> 1) & 0x01;
    PORTAbits.RA2 = (adx >> 2) & 0x01;
    PORTAbits.RA4 = (adx >> 3) & 0x01;
    PORTAbits.RA5 = (adx >> 4) & 0x01;
    LATEbits.LATE0 = (adx >> 5) & 0x01; // problem?
    LATEbits.LATE1 = (adx >> 6) & 0x01; // problem?
    LATEbits.LATE2 = (adx >> 7) & 0x01; // problem?
    PORTAbits.RA6 = (adx >> 8) & 0x01;
    PORTCbits.RC0 = (adx >> 9) & 0x01;

    // Store in MAR
    PORTCbits.RC1 = 0;
    PORTCbits.RC1 = 1;

    // Send Data
    PORTAbits.RA0 = (data >> 0) & 0x01;
    PORTAbits.RA1 = (data >> 1) & 0x01;
    PORTAbits.RA2 = (data >> 2) & 0x01;
    PORTAbits.RA4 = (data >> 3) & 0x01;
    PORTAbits.RA5 = (data >> 4) & 0x01;
    LATEbits.LATE0 = (data >> 5) & 0x01; // problem?
    LATEbits.LATE1 = (data >> 6) & 0x01; // problem?
    LATEbits.LATE2 = (data >> 7) & 0x01; // problem?

    // Write Enable
    PORTCbits.RC3 = 0;
    //Delay10TCYx(10);
    PORTCbits.RC3 = 1;
}

#endif
