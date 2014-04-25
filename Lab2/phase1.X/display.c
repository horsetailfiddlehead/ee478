#include "globals.h"

int displayFrontPanel(Global* globalData) {
    char myDisplay[] = "Enter Speed\n\r";

    if (*globalData->displayFlag == 1) {
        puts1USART(myDisplay);
        *globalData->displayFlag = 0;
    }
    return 0;

}



