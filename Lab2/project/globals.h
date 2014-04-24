/* 
 * File:   globals.h
 * Author: castia
 *
 * Created on April 23, 2014, 9:30 PM
 *
 * global header file for lab2 project. Put any externs used by other files here
 *
 */

#ifndef GLOBALS_H
#define	GLOBALS_H

#ifdef	__cplusplus
extern "C" {
#endif

    /* includes for the whole project*/
#include <stdio.h>
#include <stdlib.h>
#include <p18f25k22.h>
#include <delays.h>
    
    /* DEFINES for the whole project */
#define USE_AND_OR  // allows use of compiler libs


    /***************Clocking set up *********************/
#pragma WDTEN = OFF;    // turn off watch dog timer
#pragma FOSC = ECHP;    // Ext. Clk, Hi Pwr
    //#pragma PRICLKEN = OFF; // disable primary clock
    /****************************************************/

    /* Global Controls for I/O banks*/
    #pragma PBADEN = OFF;   // turn off bank B ADCs

    /* Global variables */
    extern unsigned int setSpeed;   // the user defined motor speed
    extern unsigned int motorSpeed; // The actual speed of the motor
    extern unsigned int errorState; // The current motor state
	
	extern unsigned int WE /* synthesis LOC="P21" */;
	extern unsigned int store /* synthesis LOC="P20" */;
	extern unsigned int OE /* synthesis LOC="P19" */;


#ifdef	__cplusplus
}
#endif

#endif	/* GLOBALS_H */

