/* 
 * File:   keypadDriver.h
 * Author: ma
 *
 * Created on May 23, 2014, 9:10 PM
 */

#ifndef KEYPADDRIVER_H
#define	KEYPADDRIVER_H

#ifdef	__cplusplus
extern "C" {
#endif

    // prototypes
    /*
     * keypadSetup() initializes the GPIO pins used by the keypad module.
     * Pins B0-B3 are used as digital inputs with internal weak pull up to Vcc
     * Pins C0-C3 are used as digital outputs
     */
    void keypadSetup();

    /*
     * checkForInput() scans the keypad and returns a numeric value of the key
     * which is pressed, or -1 otherwise. The codding is:
     *  numbers = corresponding number
     * A = 10
     * B = 11
     * C = 12
     * D = 13
     * * = 14
     * # = 15
     */
    int checkForInput();


#ifdef	__cplusplus
}
#endif

#endif	/* KEYPADDRIVER_H */

