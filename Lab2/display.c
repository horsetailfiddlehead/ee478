#include <p18F25K22.h>
#include <delays.h>
#include <portb.h>
#include <usart.h>
#include <stdio.h>
#include <stdlib.h>
 
/*  PIC Configuratings */
#pragma config OSC = HS
#pragma config WDT = OFF
#pragma config PBADEN = OFF
#pragma config LVP = OFF
#pragma config PWRT = ON
#pragma config DEBUG= OFF
 
/* UART Global variables */
int speed;                       //  Used for receiving commands from the computer
int error = 4; // Default set to '4' for no error mode
int aSpeed = 0; // Actual speed retrieved from motor

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
			speed=ReadUSART();
            Delay1KTCYx(4);
			
			putsUSART("Entered Motor Speed:		%s",speed);
			putsUSART("Actual Motor Speed:		%s",aSpeed);
			putsUSART("Current Warning Level:	%s", "NONE");
            Delay1KTCYx(4);
        }
}

void enterSpeed () {
	
}