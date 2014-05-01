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

#include <stdlib.h>
#include <stdio.h>
#include "globals.h"

// Compare actual speed to set speed to see if error exists
void errorCheck(Global* gData) {	
	if(actualSpeed == 0) {
	// Check if motor is off
		*gData->errorState = 3;
	} else if ((float)*gData->actualSpeed < computeLow(gData, 0.05) || 
				(float)	*gData->actualSpeed > computeHigh(gData, 0.05)){
	// Level 0 - Severe
		*gData->errorState = 0;
	} else if ((float)*gData->actualSpeed < computeLow(gData, 0.02) || 
				(float)*gData->actualSpeed > computeHigh(gData, 0.02)){
	// Level 1 - Moderate
		*gData->errorState = 1;
	} else if ((float)*gData->actualSpeed < computeLow(gData, 0.01) || 
				(float)*gData->actualSpeed > computeHigh(gData, 0.01)){
	// Level 2 - Warning
		*gData->errorState = 2
	} else {
	// System is normal
		*gData->errorState = 4;
	}
}

float computeLow(Global* gData, float range) {
	return ((*gData->setSpeed) * (1.0 - range));
}

float computeHigh(Global* gData, float range) {
	return ((*gData->setSpeed) * (1.0 + range)); 
}