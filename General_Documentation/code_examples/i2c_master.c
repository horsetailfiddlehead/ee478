/**********************************************************************
* 2009 Microchip Technology Inc.
*
* FileName:        	i2c_master.c
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
* SCA & SCL must be connected to the corresponding in slave
*Note: If not using PicDem2+ demo board, then SCA & SCL lines have to be pulled up by connecting 4.7KOhms resister to Vdd.
**********************************************************************/

#define USE_OR_MASKS
#include <p18cxxx.h>
#include "i2c.h"

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


unsigned char I2C_Send[21] = "MICROCHIP:I2C_MASTER" ;
unsigned char I2C_Recv[21];

//************ I2C MASTER ****************************
void main(void)
{

unsigned char sync_mode=0, slew=0, add1,w,data,status,length;

for(w=0;w<20;w++)
I2C_Recv[w]=0;

add1=0xA2;		//address of the device (slave) under communication

	CloseI2C();	//close i2c if was operating earlier

//------------------------INITIALISE THE I2C MODULE FOR MASTER MODE WITH 100KHz ---------------------------
	sync_mode = MASTER;
	slew = SLEW_OFF;
  
	OpenI2C(sync_mode,slew);

SSPADD=0x0A;			 //400kHz Baud clock(9) @8MHz

//check for bus idle condition in multi master communication
	IdleI2C();
//--------------------START I2C---------------
	StartI2C();
	
//**************write the address of the device for communication************
		data = SSPBUF;		//read any previous stored content in buffer to clear buffer full status
	do
	{
	status = WriteI2C( add1 | 0x00 );	//write the address of slave
		if(status == -1)		//check if bus collision happened
		{
			data = SSPBUF;		//upon bus collision detection clear the buffer, 
			SSPCON1bits.WCOL=0;	// clear the bus collision status bit
		}
	}
	while(status!=0);		//write untill successful communication
//R/W BIT IS '0' FOR FURTHER WRITE TO SLAVE	
	
//***********WRITE THE THE DATA TO BE SENT FOR SLAVE****************
while(putsI2C(I2C_Send)!=0);	//write string of data to be transmitted to slave

//-------------TERMINATE COMMUNICATION FROM MASTER SIDE---------------
	IdleI2C();
		
//-----------------RESTART I2C COMMUNICATION---------------------------------------
	RestartI2C();
	IdleI2C();	
//**************write the address of the device for communication************
		data = SSPBUF;		//read any previous stored content in buffer to clear buffer full status

//R/W BIT IS '1' FOR READ FROM SLAVE		
add1 = 0xA2;
	do
	{
	status = WriteI2C( add1 | 0x01 );  //write the address of slave
		if(status == -1)		//check if bus collision happened
		{
			data = SSPBUF;		//upon bus collision detection clear the buffer, 
			SSPCON1bits.WCOL=0;	// clear the bus collision status bit
		}
	}
	while(status!=0);			//write untill successful communication
	
//******************* Recieve data from slave ******************************
	while( getsI2C(I2C_Recv,20) );		//recieve data string of lenght 20 from slave 
	I2C_Recv[20] = '\0' ;
		
		NotAckI2C();					//send the end of transmission signal through nack
		while( SSPCON2bits.ACKEN!=0);		//wait till ack sequence is complete	
		
//********************* close I2C *****************************************		
CloseI2C();								//close I2C module

while(1);						//End of program
}

