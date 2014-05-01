/*
 * Created by: Patrick Ma
 * Date: April 29, 2014
 * 
 * Control.c implements the local side of the controller controller.
 * It  adjusts the controller speed value sent to the remote node according to the
 * difference between the expected set speed and the actual controller speed.
 */

#include <stdlib.h>
#include <stdio.h>
#include "globals.h"
//#include "control.h"

#define TEST_CONTROL 0

#ifdef TEST_CONTROL
Global gData;
int i = 0;
#endif

void controller(Global *gData) {
    
    // Add the difference between the desired and measured speeds
    // back to the controller speed sent to the remote
    if (*gData->setSpeed == 0) {    // zero case
        *gData->controllerSpeed = 0;
    } else if (*gData->setSpeed > *gData->actualSpeed) {
        *gData->controllerSpeed += *gData->setSpeed - *gData->actualSpeed;
    } else if (*gData->setSpeed < *gData->actualSpeed) {
        *gData->controllerSpeed -= *gData->actualSpeed - *gData->setSpeed;
    }
    
    // store the updated speed in SRAM
    *gData->processFlag = 1;
    
    //*gData->controllerSpeed = *gData->setSpeed;
    return;
}

//#ifdef TEST_CONTROL
//// a quick test program for the control logic
////
//// 4/29/2014
//void printData() {
//	printf("%d) set: %f controller: %f actual: %f\n", i, gData.setSpeed,
//			gData.controllerSpeed, gData.actualSpeed);
//}
//
//void main () {
//	gData.setSpeed = 10;
//	gData.actualSpeed = 2;
//
////	for (int i = 0; i < 10; i++) {
//	while(gData.setSpeed != gData.actualSpeed){
//		i++;
//		printData();
//		control(&gData);
//		gData.actualSpeed = (0.75) * (gData.controllerSpeed); // model motor response
//		printData();
//	}
//
//	return;
//}
//#endif
