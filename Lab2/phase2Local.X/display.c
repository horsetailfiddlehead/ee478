#include "globals.h"
void usartPrint(const char* myString);

void displayFrontPanel(Global* globalData) {
    char errorDisplay[64];

    if (*globalData->displayFlag == 1) {
        *globalData->displayFlag = 0;
        putc1USART(0x0C);
        if (*globalData->controllerSpeed % 2 == 0) {
            sprintf(errorDisplay, (char*) "Current Speed: %s.0%%\r\n", itoa(* (globalData->controllerSpeed) / 2, 16));
        } else {
            sprintf(errorDisplay, (char*) "Current Speed: %s.5%%\r\n", itoa(* (globalData->controllerSpeed) / 2, 16));
        }
        puts1USART(errorDisplay);
        if (*globalData->actualSpeed % 2 == 0) {
            sprintf(errorDisplay, (char*) "Measured Speed: %s.0%%\r\n", itoa(*(globalData->actualSpeed) / 2, 16));
        } else {
            sprintf(errorDisplay, (char*) "Measured Speed: %s.5%%\r\n", itoa(*(globalData->actualSpeed) / 2, 16));
        }
        puts1USART(errorDisplay);
        putrs1USART("Enter Command\r\n");
        putrs1USART("s (0-100%), i, d\r\nSet Point (%), Increment, Decrement\r\n");
        //        *globalData->displayFlag = 0;
        switch (*globalData->errorState) {
                // Level 0: Severe
            case 0:
                putrs1USART("\r\nWarning! Motor speed 1% out of requested speed.\r\n");
                break;
                // Level 1: Moderate
            case 1:
                putrs1USART("\r\nError! Motor speed 2% out of range. System paused.\r\n");
                break;
                // Level 2: Of Concern
            case 2:
                putrs1USART("\r\nSevere error! Motor speed 5% out of range. Shutting down.\r\n");
                break;
                // Level 3: Off
            case 3:
                putrs1USART("\r\nSystem currently off.\r\n");
                break;
                // Normal State
            default:
                putrs1USART("\r\nAll systems normal.\r\n");
                break;
        }
    }

}

void usartPrint(const char* myString) {
    int i = 0;
    while (myString[i] == '\0') {
        putc1USART(myString[i]);
    }

}



