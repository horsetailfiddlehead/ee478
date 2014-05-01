/*
 * Created by: Alyanna Castillo
 * Date: April 29, 2014
 * 
 * errorCheck.c checks for errors within the monitor channel.
 * Errors are organized into three classes:
 * +/- 5% - Level 0 - Severe 
 * +/- 2% - Level 1 - Moderate 
 * +/- 1% - Level 2 - Of Concern 
 *
 * There is a separate state for when the motor is off and
 * when the system is normal.
 */

#include "globals.h"

int computeLow(Global* gData, int range);
int computeHigh(Global* gData, int range);

// Compare actual speed to set speed to see if error exists

void errorCheck(Global* gData) {
    static short lastState = 4;
    if (*gData->actualSpeed == 0 || CCP4CON == 0) {
        // Check if motor is off
        *gData->errorState = 3;
    } else if (*gData->actualSpeed < computeLow(gData, 5) ||
            *gData->actualSpeed > computeHigh(gData, 5)) {
        // Level 0 - Severe
        *gData->errorState = 0;
  //      *gData->setSpeed = 0;
//        writeData(1, 0);
//        CCP4CON = 0; // disable pwm output
    } else if (*gData->actualSpeed < computeLow(gData, 2) ||
            *gData->actualSpeed > computeHigh(gData, 2)) {
        // Level 1 - Warning
        *gData->errorState = 1;
        //*gData->setSpeed = 0;
        //writeData(1, 0);
    } else if (*gData->actualSpeed < computeLow(gData, 1) ||
            *gData->actualSpeed > computeHigh(gData, 1)) {
        // Level 2 - Attention
        *gData->errorState = 2;
        //*gData->setSpeed /= 2; // *ourGlobal.setSpeed / 2
        //writeData(1, *gData->setSpeed);
    } else {
        // System is normal
        *gData->errorState = 4;
    }

    if (lastState != *gData->errorState) { // send new state to local
        *gData->i2cFlag = 1;
    }
    lastState = *gData->errorState;
}

// returns low bound, with floor of zero
int computeLow(Global* gData, int range) {
    if (2 * range > *gData->setSpeed) {
        return 0;
    }
    return ((*gData->setSpeed) - 2 * range);
}

int computeHigh(Global* gData, int range) {
    return ((*gData->setSpeed) + 2 * range);
}