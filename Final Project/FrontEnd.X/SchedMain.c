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
    int keypress = -1;
    GlobalState globalData;
    // LCD menu
    int menuSpots[3] = {40, 80, 120};
    int cursorPos = 0;

    systemSetup(&globalData);

    // lcd test code
    clean(BLUE);
    drawBoxFill(0, 0, 20, V - 1, CYAN);
    drawBox(0, 0, 20, V - 1, 2, WHITE);
    prints(35, 7, WHITE, CYAN, (const rom far char*) "Main Menu", 1);
    prints(35, menuSpots[0], WHITE, BLUE, (const rom far char*) "Single Player", 1);
    prints(35, menuSpots[1], WHITE, BLUE, (const rom far char*) "Multiplayer", 1);
    prints(35, menuSpots[2], WHITE, BLUE, (const rom far char*) "Build Cards", 1);

    while (1) {
        globalData.keyPress = checkForInput();
        if (globalData.keyPress >= 0) {
            globalData.keyFlag = TRUE;
        }

        if (globalData.keyFlag) { // TODO this goes into a display function
            globalData.keyFlag = FALSE;
            putc2USART(globalData.keyPress + '0');

            prints(35, 125, WHITE, BLUE, (const rom far char*) " ", 1);
            integerprint(35, 125, WHITE, RED, globalData.keyPress, 1);
        }

        // Clear cursor
        //        prints(25,menuSpots[cursorPos], WHITE, BLUE, (const rom far char*)">", 1);
        //        Delay10KTCYx(50000*1);
        //        prints(25,menuSpots[cursorPos], WHITE, BLUE, (const rom far char*)" ", 1);
        //        cursorPos = (cursorPos + 1) % 3;

        Delay1KTCYx(20);
    }

    return;
}

void systemSetup(GlobalState *data) {
    initSPI1();
    initLCD();
    rs232Setup2(); // configure USART2
    keypadSetup(); // configure keypad

    data->displayPage = 0;
    data->keyFlag = FALSE;
    data->keyPress = -1;

    return;
}