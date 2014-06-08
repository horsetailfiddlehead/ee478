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
#include <pwm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "usart.h"
#include "delays.h"
#include <adc.h>
#include <timers.h>
#include "i2c.h"
#include "LED.h"
    
    // our headers
#include "rs232.h"
#include "keypadDriver.h"
#include "LCD.h"
#include "startup.h"
#include "interrupts.h"
#include "rfidReader.h"
#include "motorDriver.h"
#include "game.h"
#include "i2cComm.h"
#include "xbee.h"

#define FRONT_NOT_BACK 0 // is this the Front end or backend (should move to globals)

#define CARDSLOT_1 0b00010000
#define CARDSLOT_2 0b00100000
#define CARDSLOT_3 0b01000000
#define CARDSLOT_4 0b10000000
#define CARDBLOCKSIZE 4 // in bytes


    enum _myBool {
        FALSE = 0, TRUE = 1
    };
    typedef enum _myBool Boolean;

    typedef struct globaldata {
        short displayPage;
        Boolean keyFlag;
        Boolean displayedKey;
        Boolean goBack;
        Boolean xbeeFlag;
        Boolean firstTime;
        int keyPress;
        int keyStatus;
        int cursorPos;
        int mainMenuSpots[3]; // Find better way to do this
        int status;
        int mode;
	int cardSelect[4];
	int selectMove[4][3];
        int game;

        /* card block data read in*/
        unsigned char dataBlockNum;
        unsigned char dataSlotNum;
        unsigned char dataBlock[4]; // 32 bits of data
        
        Boolean getInventory;

        Boolean sendI2C; // i2c command prepped for transmission
        Boolean gotI2C; // i2c command was received

        Boolean updateLEDFlag;
        char lastCards;
        char readCard;  // flag indicating which card slot needs to be read
    } GlobalState;
    extern GlobalState globalData;

#ifdef	__cplusplus
}
#endif

#endif	/* GLOBALS_H */

