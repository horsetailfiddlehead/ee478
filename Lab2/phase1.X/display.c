#include "globals.h"

void displayFrontPanel(Global* globalData) {
    char myDisplay[] = "Enter Command\n\rs (0-100%), i, d\n\rSet Point (%), Increment, Decrement\n\r";

    if (*globalData->displayFlag == 1) {
        putc1USART(0x0C);
        puts1USART(myDisplay);
        *globalData->displayFlag = 0;
    }

}



