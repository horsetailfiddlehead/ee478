/* 
 * File:   globals.h
 * Author: Patrick
 *
 * Created on May 27, 2014, 4:44 PM
 */

#ifndef GLOBALS_H
#define	GLOBALS_H

#ifdef	__cplusplus
extern "C" {
#endif

    // microchip libraries
#include <p18f46k22.h>
#include <usart.h>
#include <spi.h>
#include <delays.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "usart.h"
#include "delays.h"

    // our headers
#include "rs232.h"
#include "keypadDriver.h"
#include "LCD.h"
#include "startup.h"
#include "interrupts.h"

    enum _myBool {
        FALSE = 0, TRUE = 1
    };
    typedef enum _myBool Boolean;

    typedef struct globaldata {
        short displayPage;
        Boolean keyFlag;
        int keyPress;
    } GlobalState;

#ifdef	__cplusplus
}
#endif

#endif	/* GLOBALS_H */

