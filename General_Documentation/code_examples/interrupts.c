/**********************************************************************
* 2009 Microchip Technology Inc.
*
* FileName:        	interrupts.c
* Dependencies:    Header (.h) files if applicable, see below
* Processor:       	PIC18F
* Compiler:        	MCC18 v3.30 or higher
*
* SOFTWARE LICENSE AGREEMENT:
* Microchip Technology Incorporated ("Microchip") retains all 
* ownership and intellectual property rights in the code accompanying
* this message and in all derivatives hereto.  You may use this code,
* and any derivatives created by any person or entity by or on your 
* behalf, exclusively with Microchip's proprietary products.  Your 
* acceptance and/or use of this code constitutes agreement to the 
* terms and conditions of this notice.
*
* CODE ACCOMPANYING THIS MESSAGE IS SUPPLIED BY MICROCHIP "AS IS". NO 
* WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT 
* NOT LIMITED TO, IMPLIED WARRANTIES OF NON-INFRINGEMENT, 
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS 
* CODE, ITS INTERACTION WITH MICROCHIP'S PRODUCTS, COMBINATION WITH 
* ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
*
* YOU ACKNOWLEDGE AND AGREE THAT, IN NO EVENT, SHALL MICROCHIP BE 
* LIABLE, WHETHER IN CONTRACT, WARRANTY, TORT (INCLUDING NEGLIGENCE OR
* BREACH OF STATUTORY DUTY), STRICT LIABILITY, INDEMNITY, 
* CONTRIBUTION, OR OTHERWISE, FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
* EXEMPLARY, INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, FOR COST OR 
* EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE CODE, HOWSOEVER 
* CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE
* DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT ALLOWABLE BY LAW, 
* MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY RELATED TO THIS
* CODE, SHALL NOT EXCEED THE PRICE YOU PAID DIRECTLY TO MICROCHIP 
* SPECIFICALLY TO HAVE THIS CODE DEVELOPED.
*
* You agree that you are solely responsible for testing the code and 
* determining its suitability.  Microchip has no obligation to modify,
* test, certify, or support the code.
*
* REVISION HISTORY:
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Author        	Date      	Comments on this revision
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Harsha.J.M	04/04/09	First release of source file
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*
* ADDITIONAL NOTES:
* Code uses the Peripheral library support available with MCC18 Compiler
* Code Tested on:
* PicDem2+ demo board with PIC18F4685 controller
* The Processor starts with the External Crystal (8 Mhz).
*
* Uses INT0/INT1/INT2/RBx  pins to cause interrupts
**********************************************************************/

#define USE_OR_MASKS
#include <p18cxxx.h>
#include "portb.h"

//-------------------------------Configuration setting ----------------------------------------------
/**
	* Oscillator is configured as HS
	* Fail safe monitor is enabled
	* watch dog timer is disabled
	* Extended instruction mode is disabled
	* oscillator switch over is enabled	
*/
#if defined(__18F4685)		//If the selected device if PIC18F4685, then apply below settings else user will have to set
#pragma config OSC=HS, FCMEN=ON, WDT=OFF, IESO=ON, XINST=OFF, LVP=OFF 
#endif


unsigned char PORTResult[5]={0,0,0,0,0};	
 

void main(void)
{

unsigned char config=0;
	ADCON1 = 0xFF;

//**************** configure INT0 with pullups enabled, falling edge *********************************
		config = PORTB_CHANGE_INT_ON | FALLING_EDGE_INT | PORTB_PULLUPS_ON;	
	    OpenRB0INT(config );					//configures INT0 & enables it

//*************** configure INT1 with pullups enabled, falling edge **********************************
		config=0;
		config = PORTB_CHANGE_INT_ON | FALLING_EDGE_INT | PORTB_PULLUPS_ON;	
        OpenRB1INT( config);					//configures and enables INT1
  
//*************** configure INT2 with pullups enabled, falling edge **********************************
		config=0;
		config = PORTB_CHANGE_INT_ON | FALLING_EDGE_INT | PORTB_PULLUPS_ON;	
        OpenRB2INT( config);					//configures and enables INT2

//************** configure Change Notification in PORTB  with pullups enabled, falling edge **************
		config=0;
		config = PORTB_CHANGE_INT_ON | PORTB_PULLUPS_ON;	
        OpenPORTB(config);					//configures and enables change notification in PORTB

//**************** Check for interrupts ***********************************************************
	while(1)
	{
	      if( INTCONbits.INT0IF == 1)			//Check for INT0
		  {
			PORTResult[0]=1;					//Ser flag to indicate occurence of INT0
	        INTCONbits.INT0IF = 0;		  		//Clear INT0
		  }
		  
	      if(INTCON3bits.INT1IF == 1)			//Check for INT1
		  {
			PORTResult[1]=1;					//Ser flag to indicate occurence of INT1
	        INTCON3bits.INT1IF = 0;		  	//Clear INT1
		  }

	      if(INTCON3bits.INT2IF == 1)			//Check for INT2
		  {
			PORTResult[2]=1;					//Ser flag to indicate occurence of INT2
	        INTCON3bits.INT2IF = 0;		  	//Clear INT2		  
		  }

	      if(INTCONbits.RBIF == 1)				//Check for Change Notification interrupt
		  {
			PORTResult[4]=1;					//Ser flag to indicate occurence of Change Notification interrupt
	        INTCONbits.RBIF = 0;		  		//Clear Change Notification interrupt  
		  }
	}
	
	      CloseRB0INT();          //disable INT0
	      CloseRB1INT();          //disable INT1 
	      CloseRB2INT();          //disable INT2		  
	      ClosePORTB();           //disable Change notification	

}



