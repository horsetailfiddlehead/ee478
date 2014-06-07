/* 
 * File:   SchedMain.c
 * Author: Patrick
 *
 * Created on May 27, 2014, 4:35 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <p18f46k22.h>

// included files for each sw function
#include "globals.h"
#include "rs232.h"


// Function prototypes
void systemSetup(GlobalState *data);
void setupPWM(void);
void setXbeeNetwork(char* myNetwork);
void setupXbee(void);

void processUID(char* uid);


// PIC configuration settings
/***************Clocking set up *********************/
#pragma config WDTEN = OFF    // turn off watch dog timer
#pragma config FOSC = ECHPIO6    // Ext. Clk, Hi Pwr
#pragma config PRICLKEN = OFF // disable primary clock

/***************USART set up *********************/
#pragma config FCMEN = OFF
#pragma config IESO = OFF
/****************************************************/

/*
 * 
 */

RFIDDriver readerData;
GlobalState globalData;
extern I2cDataStruct i2cData;

#pragma code high_vector=0x08

void interrupt_at_high_vector(void) {
    _asm GOTO rcISR _endasm
}
#pragma code

#pragma interrupt rcISR

void rcISR(void) {
    // The input character from UART2 (the RFID reader)
    unsigned char input;

    /* Overrun errors
     * unsigned char temp;
    if (RCSTA1bits.OERR) {
        temp = RCREG;
        temp = RCREG;
        temp = RCREG;
        RCSTA1bits.CREN = 0;
        RCSTA1bits.CREN = 1;
        RCSTA1bits.OERR = 0;
    }
     */
    // Read fast by directly looking at RCREG
    if (PIR3bits.RC2IF) {
    input = RCREG2;
//        PORTAbits.RA0 = 1;
//         If we are processing an Inventory command
            if (readerData.invCom == 1 || readerData.readFlag_1 == 1 || readerData.writeFlag_1 == 1) {
                if (input == 'D' && readerData.nextBlock == 0) {
                    // Reset the inventory command flag
                    readerData.invCom = 0;
    
                    // Begin reading what is inside a block of square brackets
    
                } else if (input == '[') {
                    // Go to the beginning of the array, indicate that a block is being read
                    readerData.inputSpot2 = 0;
                    readerData.nextBlock = 1;
        //            PORTAbits.RA0 = 1;
    
                    // If we are at the end of a block of square brackets
                } else if (input == ']' && readerData.numUID < MAX_UIDS && readerData.nextBlock == 1) {
                    // If there is a comma as the first character inside a block, then
                    // discard what is read.  Otherwise, terminate the string and increment
                    // the number of UIDs successfully read.
                    if (readerData.readUID[readerData.numUID][0] != ',' && readerData.writeFlag_1 != 1) {
                        readerData.readUID[readerData.numUID][readerData.inputSpot2] = '\0';
                        readerData.numUID++;
                    }
    
                    // Block of square brackets has be read, set the indicator to zero
                    readerData.nextBlock = 0;
        //            PORTAbits.RA0 = 0;
    
                    // Disable read 1 flag
                    if (readerData.readFlag_1 == 1) {
                        readerData.readFlag_1 = 0;
                    }
    
                    // Disable write 1 flag
                    if (readerData.writeFlag_1 == 1) {
                        readerData.writeFlag_1 = 0;
                    }
    
                    // Put anything inside of a square bracket into the UID array
                } else if (readerData.nextBlock == 1 && readerData.inputSpot2 < UID_SIZE && readerData.numUID < MAX_UIDS) {
                    if (readerData.writeFlag_1 != 1) {
                        readerData.readUID[readerData.numUID][readerData.inputSpot2] = input;
                        readerData.inputSpot2++;
                    }
    
                    // If we are outside of a block, reset read position and ensure that the block
                    // state indicator is zero.
                } else {
                    readerData.inputSpot2 = 0;
                    readerData.nextBlock = 0;
        //            PORTAbits.RA0 = 0;
                    // Echo back typed character
                    //Write2USART(input);
                }
    
                // In config mode, count the line feeds
            } else if (readerData.configFlag == 1) {
                if (input == '\n') {
                    readerData.lineFeeds++;
                }
            } else {
                // Echo back typed character
                //Write2USART(input);
            }
        PIR3bits.RC2IF = 0;
    }

    if (PIR3bits.SSP2IF == 1) { // process i2c interrupt
        int temp = 0;
        static unsigned char byteNumber = 0;

        if (SSP2STATbits.P == 1) { // stop condition
            i2cData.inDataSequence = FALSE;
            i2cData.messageLength = byteNumber;
            byteNumber = 0;
        } else if ((SSP2STATbits.D_A == 0) && (SSP2STATbits.BF == 1)) { // check if address
            //            SSP2CON2bits.ACKDT = 0;
//            SSP2CON1bits.CKP = 1;
            temp = SSP2BUF; // get rid of address

        } else if ((SSP2STATbits.D_A == 1) && (SSP2STATbits.BF == 1)) { // check if data
            i2cData.i2cData[byteNumber++] = SSP2BUF;
//              SSP2CON2bits.ACKDT = 0;
//            SSP2CON1bits.CKP = 1;

        } else if (SSP2STATbits.S == 1 ) { // start condition
            i2cData.inDataSequence = TRUE;
        }
        PIR3bits.SSP2IF = 0; // clear the interrupt
    }

    //    PORTAbits.RA0 = 0;
    // Clear interrupts

    PIR3bits.TX2IF = 0;
}

void main() {
    int i = 0;
    int j = 0;
    systemSetup(&globalData);

    TRISAbits.RA0 = 0;
    ANSELAbits.ANSA0 = 0;
    TRISAbits.RA1 = 0;
    ANSELAbits.ANSA1 = 0;
    // lcd test code
    printMainMenu(&globalData);

    while (1) {
#if FRONT_NOT_BACK
        char test[2] ={'2', '\0'};
        sendBytes(test, 2);
#else
        PORTAbits.AN1 = i2cData.inDataSequence;
#endif
        if (!globalData.keyFlag) {
            keypad(&globalData);
        }

        if (globalData.keyFlag && !globalData.displayedKey) { // TODO this goes into a display function
            globalData.keyFlag = FALSE;
            globalData.displayedKey = TRUE;

            processDisplay(&globalData);

        }
        // Doing an inventory command from the Build card menu
        if (globalData.getInventory == TRUE) {
            // get the inventory of cards
            inventoryRFID();

            // Print out each on to the LCD
            for (i = 0; i < readerData.availableUIDs; i++) {
                if (readerData.readUID[i][0] != ',') {
                    // Get rid of commas
                    processUID(readerData.readUID[i]);
                    printrs(0, 24 + 8 * i, BLACK, RED, readerData.readUID[i], 1); // print first UID
                }
            }
            // Tell UID to be quiet - Works but needs to have at least one uid in this state
            // quietRFID(readerData.readUID[0]);
            writeRFID(readerData.readUID[0], 0x01, 0xFACE, 0xDEAD);
            readRFID(readerData.readUID[0], 0x01);
            // Print out block on to the LCD
            for (j = 0; j < readerData.availableUIDs; j++) {
                if (readerData.readUID[j][0] != ',') {
                    // Get rid of commas
                    printrs(0, 24 + 8 * i + 8 * j, BLACK, RED, readerData.readUID[j], 1); // print first UID
                }
            }
            prints(0, H - 8, BLACK, RED, "Press B to go back.", 1);
            // Turn off inventory flag

            globalData.getInventory = FALSE;
        }
        if (globalData.xbeeFlag == TRUE) {

            setXbeeNetwork("4567");
            globalData.xbeeFlag = FALSE;
        }



    }
    return;


}

// Reads the UID up until the comma

void processUID(char* uid) {
    int i = 0;
    while (uid[i] != ',') {

        i++;
    }
    uid[i] = '\0';
}

void systemSetup(GlobalState *data) {
    initSPI1();
    initLCD();
    rs232Setup2(); // configure USART2
    rs232Setup1(); // configure USART1
    keypadSetup(); // configure keypad
    setupPWM();
    setupXbee();
    RFIDSetup();
    i2CSetup();

    data->displayPage = 0;
    data->keyFlag = FALSE;
    data->displayedKey = FALSE;
    data->keyPress = -1;
    data->cursorPos = 0;
    // Find better way to do this
    data->mainMenuSpots[0] = 40;
    data->mainMenuSpots[1] = 80;
    data->mainMenuSpots[2] = 120;
    data->getInventory = FALSE;
    data->xbeeFlag = FALSE;
    // Game Related Globals
    data->keyStatus = -1;
    memset(data->selectMove, 0, sizeof (int) * 4 * 3);
    data->selectMove[0][1] = 10;
    data->cardSelect[0] = 1;
    data->cardSelect[1] = 0;
    data->cardSelect[2] = 0;
    data->cardSelect[3] = 0;
    data->firstTime = TRUE;

    OpenTimer0(TIMER_INT_OFF & T0_SOURCE_INT & T0_PS_1_32);

    return;
}

void setupPWM(void) {
    // configure PWM

    TRISBbits.RB5 = 1; // disable PWM output
    CCPTMRS0 = 0b01000000; // set CCP3 to use Timer 4
    T4CON = 0b00000111; // set timer prescale 1:16, turn on timer4
    PR4 = 0xFF; // PR = 77 for 4kHz (cool sound). set to 0xff because will be
    // set based on the key pressed


    CCP3CON = 0b00011100; // set LSB of duty yle, select pwm mode
    CCPR4L = 0x3E; // set MSB of duty cycle
    PIR5 = 0b00000000; // clear timer interrupt flag
    TRISBbits.RB5 = 1; //disable PWM output
    SetDCPWM4(50); // Square wave
}

void setupXbee(void) {

    TRISAbits.RA1 = 0;
    ANSELAbits.ANSA1 = 0;
    PORTAbits.RA1 = 1;
}

void setXbeeNetwork(char* myNetwork) {
    // Tx set low
    TXSTA1bits.TXEN1 = 0;
    PORTCbits.RC6 = 0;

    //Reset Pin- configure these to be outputs
    //PORTBbits.RB7 = 0;
    PORTAbits.RA1 = 0;
    // Delay 10 Instruction cycles, pulse must be at least 200ns;
    Delay10TCYx(5);
    //PORTBbits.RB7 = 1;
    PORTAbits.RA1 = 1;
    while (!DataRdy1USART());
    while (!DataRdy1USART());
    // Reenable Tx
    TXSTA1bits.TXEN1 = 1;
    putrs1USART("ATID");
    while (Busy1USART());
    putrs1USART(myNetwork);
    while (Busy1USART());
    // Config Commands
    //Carriage return
    // Config Commands
    //Carriage return
    // etc
    // Exit

    // ATWR,AC,CN - Write changes to nonVolatile memory
    // ATAC - Apply changes
    // ATCN - Exit config mode
    //Carriage return
    putrs1USART("WR,AC,ID\r");
    while (Busy1USART());
    while (!DataRdy1USART());
    //
    //    putrs1USART("ATID\r");
    //    while(Busy1USART());
    //    while (!DataRdy1USART());
}