/* 
 * File:   main.c
 * Author: castia
 *
 * Created on May 25, 2014, 7:52 PM
 */

#include "globals.h"
#include "LCD.h"
#include "startup.h"
#include <delays.h>

/***************USART set up *********************/
#pragma config FCMEN = OFF
#pragma config IESO = OFF
/****************************************************/
/***************Clocking set up *********************/
#pragma config WDTEN = OFF    // turn off watch dog timer
#pragma config FOSC = ECHP    // Ext. Clk, Hi Pwr
#pragma config PRICLKEN = OFF // disable primary clock
/****************************************************/
/*
 * 
 */

void initializePreiphs();

void main() {
    int cursorPos = 0;
    int menuSpots[3] = {40, 80, 120};
    initializePreiphs();
    clean(BLUE);
    box(0,0,20,127,CYAN);
    prints(35,7, WHITE, CYAN, (const rom far char*)"Main Menu", 1);
    prints(35,menuSpots[0], WHITE, BLUE, (const rom far char*)"Single Player", 1);
    prints(35,menuSpots[1], WHITE, BLUE, (const rom far char*)"Multiplayer", 1);
    prints(35,menuSpots[2], WHITE, BLUE, (const rom far char*)"Build Cards", 1);
    
    // Draw cursor
    while(1) {

        // Clear cursor
        prints(25,menuSpots[cursorPos], WHITE, BLUE, (const rom far char*)">", 1);
        Delay10KTCYx(50000*1);
        prints(25,menuSpots[cursorPos], WHITE, BLUE, (const rom far char*)" ", 1);
        cursorPos = (cursorPos + 1) % 3;
    }

}

void initializePreiphs() {
    initClock();
    initSPI(); // Must come before initLCD()
    initLCD();
}
