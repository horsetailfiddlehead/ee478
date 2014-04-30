#include "globals.h"

void displayFrontPanel(Global* globalData) {
    char myDisplay[] = "Enter Command\n\rs (0-100%), i, d\n\rSet Point (%), Increment, Decrement\n\r";
	char errorDisplay[64];
	
    if (*globalData->displayFlag == 1) {
        putc1USART(0x0C);
        puts1USART(myDisplay);
        *globalData->displayFlag = 0;
		switch(*globalData->errorState) {
			// Level 0: Severe
			case 0:
				errorDisplay = "\nWarning! Motor speed 1% out of requested speed.\n";
				puts1USART(errorDisplay);
				break;
			// Level 1: Moderate
			case 1:
				errorDisplay = "\nError! Motor speed 2% out of range. System paused.\n";
				puts1USART(errorDisplay);
				break;
			// Level 2: Of Concern
			case 2:
				errorDisplay = "\nSevere error! Motor speed 5% out of range. Shutting down.\n";
				puts1USART(errorDisplay);
				break;
			// Level 3: Off
			case 3:
				errorDisplay = "\nSystem currently off.\n";
				puts1USART(errorDisplay);
				break;
			// Normal State
			default:
				errorDisplay = "\nAll systems normal.\n";
				puts1USART(errorDisplay);
				break;
		}
    }

}



