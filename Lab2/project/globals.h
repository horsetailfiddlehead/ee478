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

    extern unsigned int setSpeed;   // the user defined motor speed
    extern unsigned int motorSpeed; // The actual speed of the motor
    extern unsigned int errorState; // The current motor state


#ifdef	__cplusplus
}
#endif

#endif	/* GLOBALS_H */

