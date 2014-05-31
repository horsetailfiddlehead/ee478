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



void main() {
    GlobalState globalData;
    systemSetup(&globalData);

    // lcd test code
    printMainMenu(&globalData);

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
    keypadSetup(); // configure keypad
    setupPWM();

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