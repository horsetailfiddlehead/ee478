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
void setXbeeNetwork(void);
void setupXbee(void);


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

#pragma code high_vector=0x08

void interrupt_at_high_vector(void) {
    _asm GOTO rcISR _endasm
}
#pragma code

#pragma interrupt rcISR
void rcISR(void) {
    // The input character from UART2 (the RFID reader)
    unsigned char input;
    unsigned char temp;
    /*
    if (RCSTA1bits.OERR) {
        temp = RCREG;
        temp = RCREG;
        temp = RCREG;
        RCSTA1bits.CREN = 0;
        RCSTA1bits.CREN = 1;
        RCSTA1bits.OERR = 0;
    }
    */
    // Don't have to wait for data available if we are in ISR
    input = RCREG;
    PORTAbits.RA0 = 1;
    // Clear interrupt
    PIR1bits.RC1IF = 0;
    // If we are processing an Inventory command
    if (readerData.invCom == 1) {

        if (input == 'D' && readerData.nextBlock == 0) {
            // Reset the inventory command flag
            readerData.invCom = 0;

            // Begin reading what is inside a block of square brackets

        } else if (input == '[') {
            // Go to the beginning of the array, indicate that a block is being read
            readerData.inputSpot2 = 0;
            readerData.nextBlock = 1;
            PORTAbits.RA0 = 1;

            // If we are at the end of a block of square brackets
        } else if (input == ']' && readerData.numUID < MAX_UIDS && readerData.nextBlock == 1) {
            // If there is a comma as the first character inside a block, then
            // discard what is read.  Otherwise, terminate the string and increment
            // the number of UIDs successfully read.
            if (readerData.readUID[readerData.numUID][0] != ',') {
                readerData.readUID[readerData.numUID][readerData.inputSpot2] = '\0';
                readerData.numUID++;
            }

            // Block of square brackets has be read, set the indicator to zero
            readerData.nextBlock = 0;
            PORTAbits.RA0 = 0;

            // Put anything inside of a square bracket into the UID array
        } else if (readerData.nextBlock == 1 && readerData.inputSpot2 < READER_INPUT_LENGTH && readerData.numUID < MAX_UIDS) {
            readerData.readUID[readerData.numUID][readerData.inputSpot2] = input;
            readerData.inputSpot2++;

            // If we are outside of a block, reset read position and ensure that the block
            // state indicator is zero.
        } else {
            readerData.inputSpot2 = 0;
            readerData.nextBlock = 0;
            PORTAbits.RA0 = 0;
            // Echo back typed character
            //Write2USART(input);
        }
    } else {
        // Echo back typed character
        //Write2USART(input);
    }
    PORTAbits.RA0 = 0;
}




void main() {
    short flag = 0;
    int i = 0;
    GlobalState globalData;
    systemSetup(&globalData);


    // lcd test code
    printMainMenu(&globalData);
/*
    while (1) {
        if (flag == 0) {
            setXbeeNetwork();
            flag = 1;
        }
    }
  */
    
    while (1) {
        if (!globalData.keyFlag) {
            keypad(&globalData);
        }

        if (globalData.keyFlag && !globalData.displayedKey) { // TODO this goes into a display function
            globalData.keyFlag = FALSE;
            globalData.displayedKey = TRUE;

            processDisplay(&globalData);

        }
        if( globalData.getInventory == TRUE) {
            // get the inventory of cards
            strcpypgm2ram(readerData.userInput2, "inventory"); // this is not "safe" or good pointer use
            processRFIDCmd();
            // Wait until interrupt finishes
            while (readerData.invCom == 1);
            // Print all the UIDs
            for (i = 0; i < readerData.numUID; i++) {
                puts2USART(readerData.readUID[i]);
                putc2USART('\r');
                while (Busy2USART());
                putc2USART('\n');
            }
            readerData.availableUIDs = 1;

            // Reset the number of UIDs read
            readerData.numUID = 0;

            while (readerData.availableUIDs == 0);
            puts2USART(readerData.readUID[0]);
            printrs(0, 24, BLACK, RED, readerData.readUID[0], 1); // print first UID
            //prints(0, 32, BLACK, RED, readerData.readUID[1], 1); // print first UID
            prints(0, H - 8, BLACK, RED, "Press B to go back.", 1);
            globalData.getInventory = FALSE;
        }

    }
    return;
    

}

void systemSetup(GlobalState *data) {
    initSPI1();
    initLCD();
    rs232Setup2(); // configure USART2
    rs232Setup1(); // configure USART1
    keypadSetup(); // configure keypad
    setupPWM();
    //setupXbee();
    RFIDSetup();

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

void setXbeeNetwork(void) {
    /*
    // Tx set low
    TXSTA1bits.TXEN1 = 0;
    PORTCbits.RC6 = 0;

    //Reset Pin- configure these to be outputs
    //PORTBbits.RB7 = 0;
    PORTAbits.RA1 = 0;
    // Delay 10 Instruction cycles, pulse must be at least 200ns;
    Delay10TCY();
    //PORTBbits.RB7 = 1;
    PORTAbits.RA1 = 1;
    Delay10TCY();
    // Reenable Tx
    TXSTA1bits.TXEN1 = 0;
    */
    //putrs2USART("ATID\r\n");
    putrs2USART("+++\r\n");
    while(Busy2USART());
    Delay10KTCYx(1);
    putc1USART('+');
    while(Busy1USART());
    Delay10KTCYx(1);
    putc1USART('+');
    while(Busy1USART());
    Delay10KTCYx(1);
    putc1USART('+');
    while(Busy1USART());
    Delay10KTCYx(1);
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
    //puts1USART((const far rom char*)"ATWR,AR,CN\r");
    putrs2USART("ATCN\r\n");
    while(Busy2USART());
    putrs1USART("ATCN\r");
    while(Busy1USART());
}