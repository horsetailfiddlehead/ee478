/*
 * Created by: Patrick
 * June 2, 2014
 *
 * I2C system for the front and back ends. The system is configured as a
 * Master/slave-idle. The i2c uses MSSP1 for communication
 */

#include "globals.h"

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
    TRISDbits.TRISD1 = 0;

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
    SSP2CON1 |= 0b00001011; // set mode to master/slave-idle
    SSP2CON2 |= 0b10000000; // enable general call enable
    SSP2CON3 |= 0b00100011; // enable start int, addr/data hold

    SSP2ADD = 0x31; // set baud rate to 100kHz
    SSP2CON1 |= SSPEN;  // enable module


}
