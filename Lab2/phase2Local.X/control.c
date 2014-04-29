/*
 * Created by: Patrick Ma
 * Date: April 29, 2014
 * 
 * Control.c implements the local side of the motor controller.
 * It  adjusts the motor speed value sent to the remote node according to the
 * difference between the expected set speed and the actual motor speed.
 */

#include <stdlib.h>
#include <stdio.h>
#include "globals.h"
#include "control.h"

#define TEST_CONTROL 0

#ifdef TEST_CONTROL
Global gData;
int i = 0;
#endif

void control(Global *gData) {
	float error = gData->setSpeed - gData->currentSpeed;

	gData->motorSpeed = error;

	// send the updated speed to the remote
	gData->i2CFlag = 1;
	return;
}

#ifdef TEST_CONTROL
// a quick test program for the control logic
//
// 4/29/2014
void printData() {
	printf("%d) set: %f motor: %f current: %f\n", i, gData.setSpeed,
			gData.motorSpeed, gData.currentSpeed);
}

void main () {
	gData.setSpeed = 10;
	gData.currentSpeed = 2;

//	for (int i = 0; i < 10; i++) {
	while(gData.setSpeed != gData.currentSpeed){
		i++;
		printData();
		control(&gData);
		gData.currentSpeed = gData.currentSpeed + (0.5) * (gData.motorSpeed);
		printData();
	}

	return;
}
#endif
