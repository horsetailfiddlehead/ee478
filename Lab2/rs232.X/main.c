/*
 * File:   main.c
 * Author: rynmcd
 *
 * Created on April 21, 2014, 4:10 PM
 */

#include <stdlib.h>
#include <stdio.h>
#include <portb.h>
#include <p18f25k22.h>
//#include <sw_uart.h>
#include <usart.h>
#include <timers.h>
#include <delays.h>
#include "SRAM.h"
#pragma setup FOSC = ECHP
#pragma setup WDTEN = OFF
#pragma setup PBADEN = OFF
#pragma setup PRICLKEN = OFF
#pragma setup FCMEN = OFF
#pragma setup IESO = OFF





/*
#define SWTXDpin 6
#define SWRXDpin 7
*/

#define CLOCK 20000000
#define BAUD 9600
#define IN_BUF_SZ 64

/*
 *
 */

/*
void DelayTXBitUART();
void DelayRXHalfBitUART();
void DelayRXBitUART();
*/

void readBytesUntil(char* myStorage, char stopChar, int size);

void rcISR(void);

// Input buffer
char myInput[IN_BUF_SZ];
// Input characters
int inputSpot = 0;
// Finish input is ready
short inputFinished = 0;

// Interrupt bit: PIE1bits.RC1IE
#pragma code high_vector=0x08
void interrupt_at_high_vector(void) {
    _asm GOTO rcISR _endasm
}
#pragma code

#pragma interrupt rcISR
void rcISR(void) {
    unsigned char input;

    // Don't have to wait for data available if we are in ISR
    input = getc1USART();

    // Terminate string on enter or max size
    if (input == '\r' || input == '\n' || inputSpot == (IN_BUF_SZ - 1)) {
        myInput[inputSpot] = '\0';

        // Reset input, declare finished
        inputSpot = 0;
        inputFinished = 1;
    } else {
        // Put character in the input buffer
        myInput[inputSpot] = input;
        inputSpot++;

        // Print current character
        putc1USART(input);
        inputFinished = 0;
    }

    // Clear interrupt
    PIR1bits.RCIF = 0;
}


void main(void)
{
    char myIntro[] = "\n\rEnter Error State(0-4):\n\r";
    char critical[] = "\n\rCritical!\n\r";
    char moderate[] = "\n\rModerate Warning.\n\r";
    char concern[] = "\n\rOf concern.\n\r";
    char good[] = "\n\rAll systems go.\n\r";
    char off[] = "\n\rMotor off.\n\r";
    int errorState = 0;
    int SRAM = 0;

    // Initialize SRAM
    SRAMsetUp();

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

    putc1USART(0x0C);
    puts1USART(myIntro);
    // Loop forever
    while(1) {
        if (inputFinished == 1) {
            errorState = (int)(myInput[0] - '0');
            switch(errorState) {
                case 0:
                    puts1USART(critical);
                break;

                case 1:
                    puts1USART(moderate);
                break;

                case 2:
                    puts1USART(concern);
                break;

                case 3:
                    puts1USART(good);
                break;

                case 4:
                    puts1USART(off);
                break;
                default:
                    putc1USART('E');
            }
            writeData(5, errorState);
            SRAM = readData(5);
            putc1USART(SRAM + '0');
            puts1USART(myIntro);
            inputFinished = 0;
            
        }
    }
}

// Precondition: USART 1 is open & configured
void readBytesUntil(char* myStorage, char stopChar, int size) {
    int i = 0;
    char message;

    while(!DataRdy1USART());
    message = getc1USART();
    putc1USART(message);
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


/*
    while(1) {
        while(!DataRdy1USART());
        message=getc1USART();
        putc1USART(message);
        Delay1KTCYx(4);
    }


    /* Write Cow
    while(1)
    {
        puts1USART(myWord);
    }
    */

/*
void DelayTXBitUART() {
    Delay1TCY(((((2*CLOCK) / (4*BAUD)) + 1) / 2) - 12);
}
void DelayRXHalfBitUART() {
    Delay1TCY(((((2*CLOCK) / (8*BAUD)) + 1) / 2) - 9);
}
void DelayRXBitUART() {
    Delay1TCY(((((2*CLOCK) / (4*BAUD)) + 1) / 2) - 14);
}
 */
