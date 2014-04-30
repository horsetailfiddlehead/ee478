/*
 * Created by: Alyanna Castillo
 * Date: April 29, 2014
 * 
 * Control.c implements the local side of the controller controller.
 * It  adjusts the controller speed value sent to the remote node according to the
 * difference between the expected set speed and the actual controller speed.
 */

#include <stdlib.h>
#include <stdio.h>
#include "globals.h"
#include "control.h"

// Compare actual speed to set speed



void errorCheck(Global* gData) {	
	if(actualSpeed == 0) {
	// Check if motor is off
		errorState = 3;
	} else if (){
		errorState = 0;
	} else if (){
		errorState = 1;
	} else if (){
		errorState = 2
	} else {
	// System is normal
		errorState = 4;
	}
}

void computeRange(Global* gData, float range) {
	*gData->motorHigh = (*gData->setSpeed) * (1.0 + range); 
	*gData->motorLow = (*gData->setSpeed) * (1.0 - range);
}