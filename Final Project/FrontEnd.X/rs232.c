//#include "rs232.h"
//#include <p18f25k22.h>
//#include <usart.h>
#include "globals.h"
// setup for USART1

void rs232Setup1() {
    //  Set RX as input, TX as output
    TRISCbits.TRISC7 = 1;
    TRISCbits.TRISC6 = 0;
    // Enable digital for all c pins
    ANSELCbits.ANSC6 = 0;
    ANSELCbits.ANSC7 = 0;

    //  Configure UART, 115200 baud with 20MHz clock.
    //Open1USART(USART_TX_INT_OFF & USART_RX_INT_ON & USART_ASYNCH_MODE & USART_EIGHT_BIT & USART_BRGH_HIGH, 10);
    //  Configure UART, 9600 baud with 20MHz clock.
    Open1USART(USART_TX_INT_OFF & USART_RX_INT_OFF & USART_ASYNCH_MODE & USART_EIGHT_BIT & USART_BRGH_HIGH,129);
    // Enable Priority
    RCONbits.IPEN = 1;
    // High priority receive interrupt
    IPR1bits.RCIP = 1;
    // Enable all high priority interrupts
    INTCONbits.GIEH = 1;
}

// setup for USART2

void rs232Setup2() {
    //  Set RX as input, TX as output
    TRISDbits.TRISD7 = 1;
    TRISDbits.TRISD6 = 0;
    ANSELDbits.ANSD6 = 0;
    ANSELDbits.ANSD7 = 0;

    //  Configure UART, 115200 baud with 20MHz clock.
    Open2USART(USART_TX_INT_OFF & USART_RX_INT_ON & USART_ASYNCH_MODE & USART_EIGHT_BIT & USART_BRGH_HIGH, 10);

    // Enable Priority
    RCONbits.IPEN = 1;
    // High priority receive interrupt
    IPR3bits.RC2IP = 1;
    // Enable all high priority interrupts
    INTCONbits.GIEH = 1;
}

// Precondition: USART 1 is open & configured

void readBytesUntil1USART(char* myStorage, char stopChar, int size) {
    int i = 0;
    char message;

    while (!DataRdy1USART());
    message = getc1USART();
    while (message != stopChar && i < (size - 1)) {
        myStorage[i] = message;
        i++;
        while (!DataRdy1USART());
        message = getc1USART();
    }

    myStorage[i] = '\0';
    i = 0;
}
void readBytesUntil2USART(char* myStorage, char stopChar, int size) {
    int i = 0;
    char message;

    while (!DataRdy2USART());
    message = getc2USART();
    while (message != stopChar && i < (size - 1)) {
        myStorage[i] = message;
        i++;
        while (!DataRdy2USART());
        message = getc2USART();
    }

    myStorage[i] = '\0';
    i = 0;
}


