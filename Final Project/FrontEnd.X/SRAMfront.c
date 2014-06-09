//#include "GLOBALS_H"
#include <p18f46k22.h>
#include "SRAM.h"
#include <delays.h>

/*
 * This is used only for the Frontend
 */

#if FRONT_NOT_BACK
unsigned short OE;
unsigned short WE;
unsigned short store;

void SRAMsetUp() {
    // Write Enable
    TRISCbits.TRISC2 = 0;

    // Output Enable
    TRISCbits.TRISC1 = 0;
    // Store
    TRISCbits.TRISC0 = 0;

    // Default Values
    PORTCbits.RC2 = 1;
    PORTCbits.RC1 = 1;
    PORTCbits.RC0 = 1;

    // Set digitalOut
    ANSELAbits.ANSA0 = 0;
    ANSELAbits.ANSA1 = 0;
    ANSELAbits.ANSA2 = 0;
    ANSELAbits.ANSA3 = 0;
    ANSELAbits.ANSA5 = 0;
    ANSELEbits.ANSE0 = 0;
    ANSELEbits.ANSE1 = 0;
    ANSELEbits.ANSE2 = 0;

    ANSELCbits.ANSC2 = 0;
    //ANSELCbits.ANSC1 = 0; NOT NEEDED FOR PORT C
    //ANSELCbits.ANSC0 = 0;

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
    TRISAbits.TRISA6 = 0;
    TRISE &= 0b11111000;
}

void setUpIn () {
    // Set data inputs
    TRISA = 0xFF;
    TRISE = 0xFF;
}

// Reading data
int readData (int adx) {
    int myRead = 0;
    SRAMsetUp();
    setUpOut();

    // Setting up Address
    PORTAbits.RA0 = (adx >> 0) & 0x01;
    PORTAbits.RA1 = (adx >> 1) & 0x01;
    PORTAbits.RA2 = (adx >> 2) & 0x01;
    PORTAbits.RA3 = (adx >> 3) & 0x01;
    PORTAbits.RA4 = (adx >> 4) & 0x01;
    PORTAbits.RA5 = (adx >> 5) & 0x01;
    LATEbits.LATE0 = (adx >> 6) & 0x01; // problem?
    LATEbits.LATE1 = (adx >> 7) & 0x01; // problem?
    LATEbits.LATE2 = (adx >> 8) & 0x01; // problem?
    PORTAbits.RA6 = (adx >> 9) & 0x01;

    // Store in MAR
    PORTCbits.RC0 = 0;
    PORTCbits.RC0 = 1;

    // I/O are inputs
    setUpIn();

    // Output Enable
    PORTCbits.RC1 = 0;
    Delay10TCYx(5);
    // Get the first 6 bits of Port A and the first 2 bits of port E
    myRead = (PORTA & 0x3F) | ((PORTE << 6) & 0xC0);
//    Delay10TCYx(5);
    // Output Enable
    PORTCbits.RC1 = 1;

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
    PORTAbits.RA3 = (adx >> 3) & 0x01;
    PORTAbits.RA4 = (adx >> 4) & 0x01;
    PORTAbits.RA5 = (adx >> 5) & 0x01;
    LATEbits.LATE0 = (adx >> 6) & 0x01; // problem?
    LATEbits.LATE1 = (adx >> 7) & 0x01; // problem?
    LATEbits.LATE2 = (adx >> 8) & 0x01; // problem?
    PORTAbits.RA6 = (adx >> 9) & 0x01;

    // Store in MAR
    PORTCbits.RC0 = 0;
    PORTCbits.RC0 = 1;

    // Send Data
    PORTAbits.RA0 = (data >> 0) & 0x01;
    PORTAbits.RA1 = (data >> 1) & 0x01;
    PORTAbits.RA2 = (data >> 2) & 0x01;
    PORTAbits.RA3 = (data >> 3) & 0x01;
    PORTAbits.RA4 = (data >> 4) & 0x01;
    PORTAbits.RA5 = (data >> 5) & 0x01;
    LATEbits.LATE0 = (data >> 6) & 0x01; // problem?
    LATEbits.LATE1 = (data >> 7) & 0x01; // problem?

    // Write Enable
    PORTCbits.RC2 = 0;
    //Delay10TCYx(10);
    PORTCbits.RC2 = 1;
}
#endif
