/*
 *  software for controlling the motor actuator
 */

#include "globals.h"

void adjustMotor(int direction);

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

    // setup ADC reader
    OpenADC(ADC_FOSC_2 & ADC_RIGHT_JUST & ADC_12_TAD,
            ADC_CH27 & ADC_INT_OFF,
            ADC_REF_VDD_VREFPLUS & ADC_REF_VDD_VSS);

    Delay10TCYx(5);
}

void move(int location) {
    static int loc;
    int posn = 0;
    ConvertADC();
    while (BusyADC());
    loc = ReadADC();
    while (0 != posn) {
        if (posn < 0) {
            adjustMotor(1);
        } else {
            adjustMotor(0);
        }
        Delay10TCY(2);
        ConvertADC;
        while (BusyADC());
        loc = ReadADC();
    }
    PORTDbits.RD6 = 1;  // lock the reader
    PORTDbits.RD7 = 1;
}

// moves the motor forward (1) or backwards(0)
void adjustMotor(int direction) {
    PORTDbits.RD6 = direction;
    PORTDbits.RD7 = !direction;
}
