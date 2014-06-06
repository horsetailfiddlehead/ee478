/*
 * Created by: Patrick
 * June 2, 2014
 *
 * I2C system for the front and back ends. The system is configured as a
 * Master/slave-idle. The i2c uses MSSP2 for communication
 */

#include "globals.h"
#include "i2c.h"

#define MASTER_FIRMW 0b00001011 //I2C Firmware Controlled Master mode (slave Idle)
#define SSPEN 0b00100000  /* Enable serial port and configures SCK, SDO, SDI*/
#define SSPDIS 0b0000000    // disable serial port
#define SLEW_OFF 0b10000000 /* Slew rate disabled for 100kHz mode */

/*
 * Initialization function for the i2c module
 */
void i2CSetup(){
    // setup D0, D1 as inputs
    TRISDbits.TRISD0 = 1;
    TRISDbits.TRISD1 = 1;

    // setup associated ANSEL bits as digital
    ANSELDbits.ANSD0 = 0;
    ANSELDbits.ANSD1 = 0;

    // enable the interrupt priority bits
    INTCONbits.GIEH = 1;    // global int enable
    INTCONbits.PEIE = 1;    // peripheral int enable
    IPR3bits.SSP2IP = 1;    // MSSP interrupt high priority
    PIE3bits.SSP2IE = 1;    // MSSP interrupt enable

    // configure MSSP2 for i2c communication
    SSP2CON1 |= SSPDIS;    // disable module
    SSP2STAT |= SLEW_OFF;
    SSP2CON1 |= 0b00001000; // set mode to master/slave-idle
    SSP2CON2 |= 0b10000000; // enable general call enable
    SSP2CON3 |= 0b00100011; // enable start int, addr/data hold

//      CloseI2C2();
//    OpenI2C2(MASTER, SLEW_OFF);
    SSP2ADD = 0x31; // set baud rate to 100kHz
    SSP2CON1 |= SSPEN;  // enable module

    TRISAbits.RA0 = 0;
    ANSELAbits.ANSA0 = 0;
}

// send bytes as the master
void sendBytes() {
    int i = 0;
    // switch to master mode?
//    for (i = 0; i < numBytes; i++) { //for num bytes
////        while ((SSP2CON2 & 0x1F) | SSP2STATbits.R_W);// check idle bus
//        // diable receiveEN
        SSP2CON2bits.SEN = 1;    // send start bit
        while (SSP2CON2bits.SEN == 1);
//    WriteI2C2('a');
        // check for ack or WCOL
        // if okay, load/send data
        // check ack
//        Delay10TCYx(4);
//    }
    SSP2CON2bits.PEN = 1;    // send stop
    while (SSP2CON2bits.PEN == 1);
//    // MAYBE? switch to slave mode

        // set address register
        // switch to RCEN idle
}

void switchToMaster() {
    // set address reg to baud rate?
    // switch out of RCEN mode?
}

//ISR for received data in slave mode
void i2cSlaveISR() {
    // ccheck if address
        // dump buffer
    // not address
        //store into global variable/buffer
        // set i2c process flag
    // clear interrupt flag
}