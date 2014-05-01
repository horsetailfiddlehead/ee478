#include "globals.h"
#include "string.h"
int stringToNum(char* myStr);

void dataProcess(Global* globalData) {
    unsigned int userSetPoint = 0;
    // myCommand key
    // 1 = set point
    // 2 = increment
    // 3 = decrement
    // 4 = error
    if (*globalData->processFlag == 1) {
        *globalData->processFlag = 0;
        // Normal state
        // if (*globalData->errorState == 4) {
        // Only for command "s "
        if (globalData->myInput[0] == 's' && globalData->myInput[1] == ' ') {
            *globalData->myCommand = 1;
            // Get data
            userSetPoint = 2 * stringToNum((char*) &globalData->myInput[2]);
            if (userSetPoint <= 202 && userSetPoint >= 0) { // Only allow inputs of 0 to 101% (101% is for testing purposes)
                *globalData->setSpeed = userSetPoint;
            } else {
                *globalData->myCommand = 4;
            }
        } else if (globalData->myInput[0] == 'i') {
            *globalData->myCommand = 2;
        } else if (globalData->myInput[0] == 'd') {
            *globalData->myCommand = 3;
        } else {
            *globalData->myCommand = 4;
        }
        // States where speed should be set to 0 (crit, warning, off)
        //} else if (*globalData->errorState != 2) {
        //*globalData->setSpeed = 0;
        //*globalData->controllerSpeed = 0;
        //writeData(1, 0);
        // Attention 
        // } else {
        //*globalData->setSpeed /= 2;
        //*globalData->controllerSpeed /= 2;
        //writeData(1, *globalData->setSpeed);
        //}
        *globalData->SRAMflag = 1;
    }
}

int stringToNum(char* myStr) {
    int i = 0;
    int sum = 0;
    while (myStr[i] != '\0') {
        sum *= 10;
        sum += myStr[i] - '0';
        i++;
    }
    return sum;
}