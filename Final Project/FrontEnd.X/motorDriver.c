/*
 *  software for controlling the motor actuator
 */

#include "globals.h"

void motorSetup() {
    // setup inputs & outputs
    PORTAbits.VREFP = 1; // port A3 (vref plus)
    PORTDbits.AN27 = 1; // analog input for motor feedback
    PORTDbits.RD6 = 0; // Fin
    PORTDbits.RD5 = 0; // Rin

// setup port A3, D7 for analog input, others as no analog
    ANSELAbits.ANSA3 = 1;
    ANSELDbits.ANSD7 = 1;
    ANSELDbits.ANSD6 = 0;
    ANSELDbits.ANSD5 = 0;
}