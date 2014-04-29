#include "globals.h"
#include "string.h"
int stringToNum(char* myStr);

void dataProcess(Global* globalData) {
    char myTest[] = "setpoint 50";
    char* curPtr;
    // myCommand key
    // 1 = set point
    // 2 = increment
    // 3 = decrement
    // 4 = error
    if (*globalData->processFlag == 1) {
        if (globalData->myInput[0] == 's') {
            *globalData->myCommand = 1;
            *globalData->myOp = 2*stringToNum((char*) &globalData->myInput[2]);
            *globalData->processFlag == 0;
        } else if (globalData->myInput[0] == 'i') {
            *globalData->myCommand = 2;
        } else if (globalData->myInput[0] == 'd') {
            *globalData->myCommand = 3;
        } else {
            *globalData->myCommand = 4;
        }
        *globalData->processFlag = 0;
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