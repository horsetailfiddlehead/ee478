#include "globals.h"
#include "rs232.h"

// setup for USART1
void rs232Setup1() {
    //  Set RX as input, TX as output
    TRISCbits.TRISC7 = 1;
    TRISCbits.TRISC6 = 0;
    // Enable digital for all c pins
    ANSELC=0x00;

    //  Configure UART, 9600 baud with 20MHz clock.
    Open1USART(USART_TX_INT_OFF & USART_RX_INT_ON & USART_ASYNCH_MODE & USART_EIGHT_BIT & USART_BRGH_HIGH, 129);

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
    TRISCbits.TRISB7 = 1;
    TRISCbits.TRISB6 = 0;
    // Enable digital for all c pins
    ANSELC=0x00;

    //  Configure UART, 9600 baud with 20MHz clock.
    Open2USART(USART_TX_INT_OFF & USART_RX_INT_ON & USART_ASYNCH_MODE & USART_EIGHT_BIT & USART_BRGH_HIGH, 129);

    // Enable Priority
    RCONbits.IPEN = 1;
    // High priority receive interrupt
    IPR1bits.RCIP = 1;
    // Enable all high priority interrupts
    INTCONbits.GIEH = 1;
	

}

// Precondition: USART 1 is open & configured
void readBytesUntil(char* myStorage, char stopChar, int size) {
    int i = 0;
    char message;

    while(!DataRdy1USART());
    message = getc1USART();
    putc1USART(message);	//echo char back
    while(message != stopChar && i < (size - 1)) {
        myStorage[i] = message;
        i++;
        while(!DataRdy1USART());
        message = getc1USART();
        putc1USART(message);
    }

    myStorage[i] = '\0';
    i = 0;
}