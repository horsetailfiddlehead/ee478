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

char i2cAddr;

/*
 * Initialization function for the i2c module
 */
void i2CSetup() {
    // set the interpic i2c address
#ifdef FRONT_NOT_BACK
    i2cAddr = 0xFE; // 0b1111111
#else
    i2cAddr = 0x00; // 0b0000000
#endif


    // setup D0, D1 as inputs
    TRISDbits.TRISD0 = 1;
    TRISDbits.TRISD1 = 1;

    // setup associated ANSEL bits as digital
    ANSELDbits.ANSD0 = 0;
    ANSELDbits.ANSD1 = 0;

    // enable the interrupt priority bits
    INTCONbits.GIEH = 1; // global int enable
    INTCONbits.PEIE = 1; // peripheral int enable
    IPR3bits.SSP2IP = 1; // MSSP interrupt high priority
    PIE3bits.SSP2IE = 1; // MSSP interrupt enable

    // configure MSSP2 for i2c communication
    SSP2CON1 |= SSPDIS; // disable module
    SSP2STAT |= SLEW_OFF;
    SSP2CON1 |= 0b00001000; // set mode to master mode
    SSP2CON2 |= 0b00000000; // disable general call interrupt
    SSP2CON3 |= 0b01000011; // enable stop int, disable start int, addr/data hold

    //      CloseI2C2();
    //    OpenI2C2(MASTER, SLEW_OFF);
    SSP2ADD = 0x31; // set baud rate to 100kHz
    SSP2CON1 |= SSPEN; // enable module

    TRISAbits.RA0 = 0;
    ANSELAbits.ANSA0 = 0;
}

/* send bytes as the master. Returns a negative number
 * if error occurs during writing, otherwise exits with 0.
 */
int sendBytes(char *data, int numBytes) {
    int i = 0;
    signed char status = 0;
    // check if in a data sequence (stop bit sent last)
    // switch to master mode?

    SSP2CON2bits.SEN = 1; // send start bit
    while (SSP2CON2bits.SEN == 1); // or use IdleI2C2()

    status = WriteI2C2(i2cAddr & 0b11111110); // send address with write
    if (status < 0) {// if collision, revert to slave, reset data sequence flag to transfer ok
        return status;
    } else { // if no collision,
        for (i = 0; i < numBytes; i++) { //for num bytes
            status = WriteI2C2(data[i]);
            if (status < 0) {// check for ack or WCOL
                return status; // if nack or wcol, break
            }
        }
    }
    SSP2CON2bits.PEN = 1; // send stop
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

void i2cISR() {
    // ccheck if address
    // dump buffer
    // not address
    //store into global variable/buffer
    // set i2c process flag
    // clear interrupt flag
}