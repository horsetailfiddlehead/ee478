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
	if(*gData->actualSpeed == 0) {
	// Check if motor is off
		*gData->errorState = 3;
	} else if (*gData->actualSpeed < computeLow(gData, 5) || 
			*gData->actualSpeed > computeHigh(gData, 5)){
	// Level 0 - Severe
		*gData->errorState = 0;
	} else if (*gData->actualSpeed < computeLow(gData, 2) || 
				*gData->actualSpeed > computeHigh(gData, 2)){
	// Level 1 - Moderate
		*gData->errorState = 1;
	} else if (*gData->actualSpeed < computeLow(gData, 1) || 
				*gData->actualSpeed > computeHigh(gData, 1)){
	// Level 2 - Warning
		*gData->errorState = 2;
	} else {
	// System is normal
		*gData->errorState = 4;
	}
}

int computeLow(Global* gData, int range) {
	return ((*gData->setSpeed) - 2*range);
}

int computeHigh(Global* gData, int range) {
	return ((*gData->setSpeed) + 2*range);
}