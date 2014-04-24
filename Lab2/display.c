#include <p18F25K22.h>
#include <delays.h>
#include <portb.h>
#include <usart.h>
#include <stdio.h>
#include <stdlib.h>
#include "GLOBALS_H"
 
/*  PIC Configuratings */
#pragma config OSC = HS
#pragma config WDT = OFF
#pragma config PBADEN = OFF
#pragma config LVP = OFF
#pragma config PWRT = ON
#pragma config DEBUG= OFF
 
void main(void)
{
    //  Set all of PORTC as outputs. TX and RX must be set as outputs first
    TRISC=0x00;
     
    //  Configure UART
    OpenUSART(USART_TX_INT_OFF & USART_RX_INT_OFF & USART_ASYNCH_MODE &USART_EIGHT_BIT & USART_CONT_RX & USART_BRGH_HIGH, 10);
        Delay1KTCYx(4);
        while(1)
        {
            while(!DataRdyUSART());
            putsUSART("Enter motor speed:		");
			setSpeed=ReadUSART();
            Delay1KTCYx(4);
			
			// Display to terminal the speed of the motor
			sprintf("Entered Motor Speed:		%s",setSpeed);
			sprintf("Actual Motor Speed:		%s",motorSpeed);	
			
			// Display current state of system
				switch(errorState) {
				case 0:
					putrs1USART((const far rom char *)"\nCritical!!!\n");
					break;
				case 1:
					putrs1USART((const far rom char *)"\nModerate Warning.\n");
					break;
				case 2:
					putrs1USART((const far rom char *)"\nOf concern.\n");
					break;
				case 3:
					putrs1USART((const far rom char *)"\nAll systems running.\n");
					break;
				case 4:
					putrs1USART((const far rom char *)"\nMotor currently turned off.\n");
					break;
			}		
			Delay1KTCYx(4);
        }
}