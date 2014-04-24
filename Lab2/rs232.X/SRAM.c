//#include "GLOBALS_H"
#include <p18f25k22.h>
#include "SRAM.h"
#include <delays.h>

unsigned short OE;
unsigned short WE;
unsigned short store;

void SRAMsetUp() {
    // Write Enable
    TRISBbits.TRISB4 = 0;

    // Output Enable
    TRISBbits.TRISB5 = 0;

    // Store
    TRISBbits.TRISB3 = 0;

    // Default Values
    PORTBbits.RB3 = 1;
    PORTBbits.RB4 = 1;
    PORTBbits.RB5 = 1;


    ANSELA = 0x00;
    ANSELBbits.ANSB3 = 0;
    ANSELBbits.ANSB4 = 0;
    ANSELBbits.ANSB5 = 0;

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
    TRISCbits.TRISC0 = 0;
    TRISCbits.TRISC1 = 0;
}

void setUpIn () {
    // Set data inputs
    TRISAbits.TRISA0 = 1;
    TRISAbits.TRISA1 = 1;
    TRISAbits.TRISA2 = 1;
    TRISAbits.TRISA3 = 1;
    TRISAbits.TRISA4 = 1;
    TRISAbits.TRISA5 = 1;
    TRISCbits.TRISC0 = 1;
    TRISCbits.TRISC1 = 1;
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
    PORTCbits.RC0 = (adx >> 6) & 0x01;
    PORTCbits.RC1 = (adx >> 7) & 0x01;

    // Store in MAR
    PORTBbits.RB3 = 0;
    PORTBbits.RB3 = 1;

    // I/O are inputs
    setUpIn();

    // Output Enable
    PORTBbits.RB5 = 0;

    // Get the first 6 bits of Port A and the first 2 bits of port C shifted
    myRead = (PORTA & 0x3F) | ((PORTC << 6) & 0xC0);
    
    // Output Enable
    PORTBbits.RB5 = 1;

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
    PORTCbits.RC0 = (adx >> 6) & 0x01;
    PORTCbits.RC1 = (adx >> 7) & 0x01;

    // Store in MAR
    PORTBbits.RB3 = 0;
    PORTBbits.RB3 = 1;

    // Send Data
    PORTAbits.RA0 = (data >> 0) & 0x01;
    PORTAbits.RA1 = (data >> 1) & 0x01;
    PORTAbits.RA2 = (data >> 2) & 0x01;
    PORTAbits.RA3 = (data >> 3) & 0x01;
    PORTAbits.RA4 = (data >> 4) & 0x01;
    PORTAbits.RA5 = (data >> 5) & 0x01;
    PORTCbits.RC0 = (data >> 6) & 0x01;
    PORTCbits.RC1 = (data >> 7) & 0x01;

    // Write Enable
    PORTBbits.RB4 = 0;
    //Delay10TCYx(10);
    PORTBbits.RB4 = 1;
}

