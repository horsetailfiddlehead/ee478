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
#if FRONT_NOT_BACK

#else
    unsigned char input;
    /*
     * RFID interrupt
     */

    if (PIR1bits.RC1IF) {
        input = RCREG1; // Read fast by directly looking at RCREG
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

                // If we are at the end of a block of square brackets
            } else if (input == ']' && readerData.numUID < MAX_UIDS && readerData.nextBlock == 1) {
                // If there is a comma as the first character inside a block, then
                // discard what is read.  Otherwise, terminate the string and increment
                // the number of UIDs successfully read.
                if (readerData.readFlag_1 == 1) {
                    readerData.readData[readerData.inputSpot2] = '\0';
                    readerData.availableData++;
                } else if (readerData.readUID[readerData.numUID][0] != ',' && readerData.writeFlag_1 != 1) {
                    readerData.readUID[readerData.numUID][readerData.inputSpot2] = '\0';
                    readerData.numUID++;
                }

                // Block of square brackets has be read, set the indicator to zero
                readerData.nextBlock = 0;

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
                if (readerData.readFlag_1 == 1) {
                    readerData.readData[readerData.inputSpot2] = input;
                    readerData.inputSpot2++;
                } else if (readerData.writeFlag_1 != 1) {
                    readerData.readUID[readerData.numUID][readerData.inputSpot2] = input;
                    readerData.inputSpot2++;
                }

                // If we are outside of a block, reset read position and ensure that the block
                // state indicator is zero.
            } else {
                readerData.inputSpot2 = 0;
                readerData.nextBlock = 0;
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
        PIR1bits.RC1IF = 0;
    }/*
         * LED status interrupt
         * Card slots are:
         * 0b00010000 = slot 1
         * 0b00100000 = slot2
         * 0b01000000 = slot3
         * 0b10000000 = slot 4
         */
    else if (INTCONbits.RBIF == 1) {
        char presentCards = PORTB;

        globalData.lastCards ^= presentCards; // find which port is triggered

        // set corresponding led status based on current port status
        // tell rfid moudule to read the card (if needed)
        if (globalData.lastCards & CARDSLOT_1) { //first slot
            if (!(presentCards & CARDSLOT_1)) { // card is placed
                ledData.ledStatus[0] = 0;
                globalData.readCard |= CARDSLOT_1;
            } else {
                ledData.ledStatus[0] = 3;
            }
        } else if (globalData.lastCards & CARDSLOT_2) { //second slot
            if (!(presentCards & CARDSLOT_2)) { // card is placed
                ledData.ledStatus[1] = 0;
                globalData.readCard |= CARDSLOT_2;
            } else {
                ledData.ledStatus[1] = 3;
            }
        } else if (globalData.lastCards & CARDSLOT_3) { //third slot
            if (!(presentCards & CARDSLOT_3)) { // card is placed
                ledData.ledStatus[2] = 0;
                globalData.readCard |= CARDSLOT_3;
            } else {
                ledData.ledStatus[2] = 3;
            }
        } else if (globalData.lastCards & CARDSLOT_4) { // fourth slot
            if (!(presentCards & CARDSLOT_4)) { // card is placed
                ledData.ledStatus[3] = 0;
                globalData.readCard |= CARDSLOT_4;
            } else {
                ledData.ledStatus[3] = 3;
            }
        }
        globalData.updateLEDFlag = TRUE; // tell led driver to update leds
        globalData.lastCards = presentCards;
        PORTB = presentCards;
        INTCONbits.RBIF = 0; // reset int flag
    }
#endif
    if (PIR3bits.SSP2IF == 1) { // process i2c interrupt
        int temp = 0;
        static unsigned char byteNumber = 0;

        //        PORTAbits.AN0 = 1;

        if (SSP2STATbits.P == 1) { // stop condition
            i2cData.inDataSequence = FALSE;
            i2cData.inLength = byteNumber;
            byteNumber = 0;
            if (!i2cData.transmitting) {
                globalData.gotI2C = 1; // alert the scheduler
            }
        } else if ((SSP2STATbits.D_A == 0) && (SSP2STATbits.BF == 1)) { // check if address
            //            SSP2CON2bits.ACKDT = 0;
            //            SSP2CON1bits.CKP = 1;
            //            i2cData.dataOut[byteNumber++] = SSP2BUF;
            temp = SSP2BUF; // get rid of address
        } else if ((SSP2STATbits.D_A == 1) && (SSP2STATbits.BF == 1)) { // check if data
            i2cData.dataIn[byteNumber++] = SSP2BUF;
            //            SSP2CON2bits.ACKDT = 0;
            //            SSP2CON1bits.CKP = 1;

        } else if (SSP2STATbits.S == 1) { // start condition
            i2cData.inDataSequence = TRUE;
        }
        PIR3bits.SSP2IF = 0; // clear the interrupt

        //        PORTAbits.AN0 = 1;

    }

    // Clear interrupts

    //    PIR1bits.TX1IF = 0;
    PIR1bits.RC1IF = 0;
    PIR3bits.TX2IF = 0;
    PIR3bits.RC2IF = 0;
}

void main() {
    int i = 0;
    int j = 0;
    char test[2] = {'2', '\0'};

    systemSetup(&globalData);

#if FRONT_NOT_BACK
    TRISAbits.RA0 = 0;
    ANSELAbits.ANSA0 = 0;

    printMainMenu(&globalData);
#else
    //    TRISDbits.RD4 = 0;
    //    ANSELDbits.ANSD4 = 0;
    //    PORTDbits.RD4 = 1;
#endif



    while (1) {
#if FRONT_NOT_BACK


        // get the updated cards
        if (globalData.runGetUpdatedCards == 1) {
            getUpdatedCards();
            globalData.runGetUpdatedCards = FALSE;
        }
#else
        // move the reader -- if we had one -- to the proper location


        /*
         * run LED driver
         */
        if (globalData.updateLEDFlag) {
            globalData.updateLEDFlag = FALSE;
            updateLEDs();
        }
#endif

        // process pending i2c tasks
        if (globalData.sendI2C == 1) {
            sendBytes(i2cData.dataOut, i2cData.outLength);
            globalData.sendI2C = FALSE;
        }
        if (globalData.gotI2C == 1) {
            processI2C();
            globalData.gotI2C = 0;
        }

#if FRONT_NOT_BACK
        if (!globalData.keyFlag) {
            // Check keystroke
            keypad(&globalData);
        }

        if (globalData.keyFlag && !globalData.displayedKey || globalData.myRequestStatus != 0) {
            globalData.keyFlag = FALSE;
            globalData.displayedKey = TRUE;

            switch (globalData.displayPage) {
                case 0: // main menu
                    processPrintCursor(&globalData, 3, BLUE, WHITE);
                    if (globalData.newDisplay == 1) {
                        globalData.newDisplay = 0;
                        switch (globalData.cursorPos) {
                            case 0: // go to singleplayer
                                globalData.displayPage = 1;
                                printSelectGame(&globalData);
                                break;
                            case 1: // multi
                                globalData.displayPage = 2;
                                printSelectGame(&globalData);
                                break;
                            case 2: // build
                                globalData.displayPage = 3;
                                printBuild(&globalData);
                                break;
                            case 0xFF: // back
                                globalData.displayPage = 0;
                                break;
                            default:
                                globalData.displayPage = 0;
                                printMainMenu(&globalData);
                                break;
                        }
                    }
                    break;
                case 1: // display sp game menu
                    processPrintCursor(&globalData, 4, GREEN, BLACK);
                    if (globalData.newDisplay == 1) {
                        globalData.newDisplay = 0;
                        switch (globalData.cursorPos) {
                            case 0: // go to monster game
                                globalData.displayPage = 4;
                                singlePlayer(&globalData);
                                break;
                            case 1: // go to clue
                                globalData.displayPage = 5;
                                clean(BLACK);
                                prints(0, 35, WHITE, BLACK, "In Progress", 1);
                                break;
                            case 2: // go to error
                                globalData.displayPage = 6;
                                printBSOD();
                                break;
                            case 3: // go to error
                                clean(BLACK);
                                prints(0, 35, WHITE, BLACK, "In Progress", 1);
                                globalData.displayPage = 7;
                                break;
                            default:
                                globalData.displayPage = 0;
                                printMainMenu(&globalData);
                                break;
                        }
                        globalData.cursorPos = 0;
                    }
                    break;
                case 2: // display mp game menu
                    processPrintCursor(&globalData, 4, GREEN, BLACK);
                    if (globalData.newDisplay == 1) {
                        globalData.newDisplay = 0;
                        switch (globalData.cursorPos) {
                            case 0: // go to monster game
                                globalData.displayPage = 4;
                                printSelectGame(&globalData);
                                break;
                            case 1: // go to clue
                                globalData.displayPage = 5;
                                clean(BLACK);
                                prints(0, 35, WHITE, BLACK, "In Progress", 1);
                                break;
                            case 2: // go to error
                                globalData.displayPage = 6;
                                printBSOD();
                                break;
                            case 3: // go to error
                                clean(BLACK);
                                prints(0, 35, WHITE, BLACK, "In Progress", 1);
                                globalData.displayPage = 7;
                                break;
                            default:
                                globalData.displayPage = 0;
                                printMainMenu(&globalData);
                                break;
                        }
                        globalData.cursorPos = 0;
                    }
                    break;
                case 3: // build cards
                    processPrintCursor(&globalData, 4, RED, BLACK);
                    if (globalData.newDisplay == 1) {
                        globalData.newDisplay = 0;
                        switch (globalData.cursorPos) {
                            case 0: // go to monster game
                                buildCard(&globalData, 0);
                                prints(140, 35, BLACK, RED, "DONE", 1);
                                globalData.displayPage = 0;
                                printMainMenu(&globalData);
                                break;
                            case 1: // go to clue
                                buildCard(&globalData, 1);
                                prints(140, 35, BLACK, RED, "DONE", 1);
                                globalData.displayPage = 0;
                                printMainMenu(&globalData);
                                break;
                            case 2: // go to error
                                buildCard(&globalData, 2);
                                prints(140, 35, BLACK, RED, "DONE", 1);
                                globalData.displayPage = 0;
                                printMainMenu(&globalData);
                                break;
                            case 3: // go to error
                                buildCard(&globalData, 3);
                                clean(BLACK);
                                prints(140, 35, BLACK, RED, "DONE", 1);
                                globalData.displayPage = 0;
                                printMainMenu(&globalData);
                                break;
                            default:
                                globalData.displayPage = 0;
                                printMainMenu(&globalData);
                                break;
                        }
                        globalData.cursorPos = 0;
                    }
                    break;
                case 4: // singleplayer monster
                    singlePlayer(&globalData);
                    break;
                case 5: // clue sp
                    break;
                case 6: // bsod
                    break;
                case 7: // in progress, b goes back
                    if (globalData.keyPress == 0x0B) {
                        globalData.displayPage = 0;
                        printMainMenu(&globalData);
                    }
                    break;
            }
        }


        //        if (!globalData.keyFlag) {
        //            keypad(&globalData);
        //        }

        //        mainMenu(&globalData);

        //
        //        if (globalData.keyFlag && !globalData.displayedKey) { // TODO this goes into a display function
        //            globalData.keyFlag = FALSE;
        //            globalData.displayedKey = TRUE;
        //
        ////            processDisplay(globalData);
        //
        //        }


#else
        //Doing an inventory command from the Build card menu
        if (globalData.readCard != 0) {

            //            globalData.getInventory = TRUE;
            //        }
            //        if (globalData.getInventory == TRUE) {
            // get the inventory of cards
            inventoryRFID();

            // Print out each on to the LCD
            for (i = 0; i < readerData.availableUIDs; i++) {
                if (readerData.readUID[i][0] != ',') {
                    // Get rid of commas
                    processUID(readerData.readUID[i]);
                    //                    printrs(0, 24 + 8 * i, BLACK, RED, readerData.readUID[i], 1); // print first UID
                }
            }

            // got inventory, tell frontend
            i2cData.dataOut[0] = CARD_CHANGE;
            i2cData.outLength = 1;
            globalData.sendI2C = TRUE;
            i2cData.transmissionNum = 0; // begin counting slots

            // Tell UID to be quiet - Works but needs to have at least one uid in this state
            // quietRFID(readerData.readUID[0]);

            //            if (readerData.availableUIDs > 0) {
            //
            //                // block 0 high bits being 0x0000 indicates factory card, not custom
            //                // block 0 high bits being 0x0001 indicates custom card
            //                // block 0 low bits indicate the game the card is for. 0x0001 is the monster game
            //                writeRFID(readerData.readUID[0], 0x00, 0x0000, 0x0001); // 7654 3210
            //                writeRFID(readerData.readUID[0], 0x01, 0x0010, 0x0001); // hex 7-5 are for level 0x001 is level 1
            //                // hex 4 is for type 0 1 2 3
            //                // 0x0 fire, 0x1 water, 0x3 earth
            //                // last 4 are monster ID
            //
            //                //writes 8 chars in 2 addresses of memory (0x02 and 0x03 here)
            //                char8RFID(readerData.readUID[0], 0x02, "FIREDUDE");
            //                writeRFID(readerData.readUID[0], 0x04, 0x0003, 0x0201); // Move list by id, has moves 03, 02, and 01
            //                readRFID(readerData.readUID[0], 0x00);
            //                printrs(0, 32, BLACK, RED, readerData.readData, 1); // print 1st block
            //                readRFID(readerData.readUID[0], 0x01);
            //                printrs(0, 40, BLACK, RED, readerData.readData, 1); // print 2nd block
            //                readRFID(readerData.readUID[0], 0x02);
            //                printrs(0, 48, BLACK, RED, readerData.readData, 1); // print 3rd block
            //                readRFID(readerData.readUID[0], 0x03);
            //                printrs(0, 56, BLACK, RED, readerData.readData, 1); // print 4th block
            //                readRFID(readerData.readUID[0], 0x04);
            //                printrs(0, 64, BLACK, RED, readerData.readData, 1); // print 5th block
            //            }
            //
            //
            //            readRFID(readerData.readUID[0], 0x01);
            //            // Print out block on to the LCD
            //            for (j = 0; j < readerData.availableUIDs; j++) {
            //                if (readerData.readUID[j][0] != ',') {
            //                    // Get rid of commas
            //                    printrs(0, 24 + 8 * i + 8 * j, BLACK, RED, readerData.readUID[j], 1); // print first UID
            //                }
            //            }
            //
            //            prints(0, H - 8, BLACK, RED, "Press B to go back.", 1);
            //            // Turn off inventory flag

            //            globalData.getInventory = FALSE;
            globalData.readCard = 0;
        }
#endif
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

    rs232Setup2(); // configure USART2
    rs232Setup1(); // configure USART1
    i2CSetup();
    RFIDSetup();

#if FRONT_NOT_BACK
    initSPI1();
    initLCD();
    keypadSetup(); // configure keypad
    setupPWM();
#else
    LEDSetup();
#endif
    data->myRequestStatus = 0;
    data->displayPage = 0;
    data->keyFlag = FALSE;
    data->displayedKey = FALSE;
    data->keyPress = -1;
    data->cursorPos = 0;
    // Select Game Menu
    data->mode = -1;
    data->game = -1;
    // Find better way to do this
    data->mainMenuSpots[0] = 40;
    data->mainMenuSpots[1] = 80;
    data->mainMenuSpots[2] = 120;
    data->getInventory = FALSE;
    data->xbeeFlag = FALSE;
    data->goBack = FALSE;
    // Game Related Globals
    data->keyStatus = -1;
    memset(data->selectMove, 0, sizeof (int) * 4 * 3);
    data->selectMove[0][1] = 10;
    data->cardSelect[0] = 1;
    data->cardSelect[1] = 0;
    data->cardSelect[2] = 0;
    data->cardSelect[3] = 0;
    data->firstTime = TRUE;
    data->updateLEDFlag = TRUE;
    data->lastCards = 0;
    data->readCard = 0;
    data->dataBlockNum = 0;
    data->dataSlotNum = 0;
    memset(data->dataBlock, 0, sizeof (char) * CARDBLOCKSIZE);
    data->runGetUpdatedCards = FALSE;
    data->gotI2C = 0;
    data->sendI2C = 0;
    data->displayPage = 0;
    data->newDisplay = 0;
    data->newGame = 1;
    data->newKeyboard = 1;
    data->doneKeyboard = 0;
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

