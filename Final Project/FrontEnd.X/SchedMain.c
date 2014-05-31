/* 
 * File:   SchedMain.c
 * Author: Patrick
 *
 * Created on May 27, 2014, 4:35 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <p18f46k22.h>

// included files for each sw function
#include "globals.h"
#include "rs232.h"


// Function prototypes
void systemSetup(GlobalState *data);
void setupPWM(void);
void setXbeeNetwork(void);
void setupXbee(void);


// PIC configuration settings
/***************Clocking set up *********************/
#pragma config WDTEN = OFF    // turn off watch dog timer
#pragma config FOSC = ECHPIO6    // Ext. Clk, Hi Pwr
#pragma config PRICLKEN = OFF // disable primary clock

/***************USART set up *********************/
#pragma config FCMEN = OFF
#pragma config IESO = OFF
/****************************************************/

/*
 * 
 */


void rcISR(void) {
    // The input character from UART2 (the RFID reader)
    unsigned char input;

    // Don't have to wait for data available if we are in ISR
    input = getc1USART();
    
    putc2USART(input);

    // Clear interrupt
    PIR1bits.RC1IF = 0;
}


void main() {
    short flag = 0;
    GlobalState globalData;
    systemSetup(&globalData);


    // lcd test code
    printMainMenu(&globalData);
/*
    while (1) {
        if (flag == 0) {
            setXbeeNetwork();
            flag = 1;
        }
    }
  */
    
    while (1) {
        if (!globalData.keyFlag) {
            keypad(&globalData);
        }

        if (globalData.keyFlag && !globalData.displayedKey) { // TODO this goes into a display function
            globalData.keyFlag = FALSE;
            globalData.displayedKey = TRUE;

            processDisplay(&globalData);

        }
    }
    return;
    

}

void systemSetup(GlobalState *data) {
    initSPI1();
    initLCD();
    rs232Setup2(); // configure USART2
    rs232Setup1(); // configure USART1
    keypadSetup(); // configure keypad
    setupPWM();
    //setupXbee();

    data->displayPage = 0;
    data->keyFlag = FALSE;
    data->displayedKey = FALSE;
    data->keyPress = -1;
    data->cursorPos = 0;
    // Find better way to do this
    data->mainMenuSpots[0] = 40;
    data->mainMenuSpots[1] = 80;
    data->mainMenuSpots[2] = 120;

    return;
}

void setupPWM(void) {
    // configure PWM
    TRISBbits.RB5 = 1; // disable PWM output
    CCPTMRS0 = 0b01000000; // set CCP3 to use Timer 4
    T4CON = 0b00000111; // set timer prescale 1:16, turn on timer4
    PR4 = 0xFF; // PR = 77 for 4kHz (cool sound). set to 0xff because will be
                // set based on the key pressed


    CCP3CON = 0b00011100; // set LSB of duty yle, select pwm mode
    CCPR4L = 0x3E; // set MSB of duty cycle
    PIR5 = 0b00000000; // clear timer interrupt flag
    TRISBbits.RB5 = 1; //disable PWM output
    SetDCPWM4(50); // Square wave
}

void setupXbee(void) {
    TRISAbits.RA1 = 0;
    ANSELAbits.ANSA1 = 0;
    PORTAbits.RA1 = 1;
}

void setXbeeNetwork(void) {
    /*
    // Tx set low
    TXSTA1bits.TXEN1 = 0;
    PORTCbits.RC6 = 0;

    //Reset Pin- configure these to be outputs
    //PORTBbits.RB7 = 0;
    PORTAbits.RA1 = 0;
    // Delay 10 Instruction cycles, pulse must be at least 200ns;
    Delay10TCY();
    //PORTBbits.RB7 = 1;
    PORTAbits.RA1 = 1;
    Delay10TCY();
    // Reenable Tx
    TXSTA1bits.TXEN1 = 0;
    */
    //putrs2USART("ATID\r\n");
    putrs2USART("+++\r\n");
    while(Busy2USART());
    Delay10KTCYx(1);
    putc1USART('+');
    while(Busy1USART());
    Delay10KTCYx(1);
    putc1USART('+');
    while(Busy1USART());
    Delay10KTCYx(1);
    putc1USART('+');
    while(Busy1USART());
    Delay10KTCYx(1);
    // Config Commands
    //Carriage return
    // Config Commands
    //Carriage return
    // etc

    // Exit
    // ATWR,AC,CN - Write changes to nonVolatile memory
            // ATAC - Apply changes
            // ATCN - Exit config mode
    //Carriage return
    //puts1USART((const far rom char*)"ATWR,AR,CN\r");
    putrs2USART("ATCN\r\n");
    while(Busy2USART());
    putrs1USART("ATCN\r");
    while(Busy1USART());
}