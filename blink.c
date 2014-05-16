#include <msp430.h>
#include <stdio.h>
#include <math.h>
#include <string.h>



#define SHORT 50
#define ANTSWINT 100
#define ANTSWEXT 101


//prototypes
void port_init(void);
void uart_init(void);
void setup_interrupts(void);
void initLCD(void);
void writecom(unsigned char d, unsigned char mode);
void write_string_to_LCD(char *strdata, unsigned char len);
void write_int_to_LCD(unsigned int x);
void write_float_to_LCD(float x);
void short_dly(unsigned long dly);
unsigned char light_pressed(void);
unsigned char ping_pressed(void);
void ping_reader(void);
void send_buffer(unsigned char num_bytes);
unsigned char  antsw_pressed(void);
void toggle_antenna_switch(void);
void update_lcd_line2(void);
void enter_run_mode(void);
unsigned char run_pressed(void);
void exit_run_mode(void);
unsigned char stop_pressed(void);
unsigned char rfoff_pressed(void);
void turn_rf_off(void);
void read_block(void);
unsigned char rdblk_pressed(void);
void set_output_mode(void);






//globals
char keep_local;//if high, keep data returned to demo board (from reader) local instead of forwarding to host PC via USB.  if low, send to host PC.
char big_buffer_tx[1000];
char big_buffer_rx[1000];
unsigned int rx_index;
unsigned char currentantswstate;
unsigned char currentbaudrate;
void single_slot_inventory();
unsigned char run_mode;
unsigned char blkaddr;


//**********************************************************************************
//USCI A0 Interrupt Handler - data from PC is sent to RFID2 module
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR_HOOK(void)
{
	while (!(IFG2 & UCA1TXIFG));    // USCI_A1 TX buffer ready?
	UCA1TXBUF = UCA0RXBUF; //receive byte from A0, send it to A1 (receive from USB, send to RFID)
}


//**********************************************************************************
//USCI A1 Interrupt Handler - data from RFID2 module is sent to USB interface)
#pragma vector=USCIAB1RX_VECTOR
__interrupt void USCI1RX_ISR_HOOK(void)
{
	if(keep_local==1) //if high, keep data returned to demo board (from reader) local instead of forwarding to host PC via USB.  if low, send to host PC.
	{
		big_buffer_rx[rx_index++] = UCA1RXBUF;
	}
	else//send to PC via USB
	{
		while (!(IFG2 & UCA0TXIFG));    // USCI_A0 TX buffer ready?
		UCA0TXBUF = UCA1RXBUF; //receive byte from A1 and send to A0 (receive from RFID, send to USB)
	}
}




//**********************************************************************************************
//**********************************************************************************************
//**********************************************************************************************
int main(void)
{
	volatile unsigned char data, pos;

	WDTCTL = WDTPW + WDTHOLD;             // Stop watchdog timer


	//if (CALBC1_12MHZ==0xFF || CALDCO_12MHZ == 0xFF)
	 //   while(1);                               // If calibration constants erased do not load - trap CPU
	BCSCTL1 = CALBC1_12MHZ;	// Set DCO
	DCOCTL = CALDCO_12MHZ;


	//**********************************************
	//DO NOT CHANGE THE ORDER OF THE FOLLOWING CODE!
	port_init();

	initLCD();
	P4OUT |= BIT1; //LCD backlight on
	writecom(0x01, 0);//mode=0 instruction/command,    mode=1 data  clear display
	writecom(0x02, 0);//mode=0 instruction/command,    mode=1 data  position cursor home home
	write_string_to_LCD("DLP Design      ", 16);
	writecom(0xC0, 0);//mode=0 instruction/command,    mode=1 data  position cursor to start of second row
	write_string_to_LCD("DLP-RFID2 Demo  ", 16);

	keep_local=0;//if high, keep data returned to demo board (from reader) local instead of forwarding to host PC via USB.  if low, send to host PC.
	rx_index=0;//init the receive buffer index
	run_mode=0;//disable inventories
	blkaddr=0;
	currentantswstate = ANTSWINT;//init to internal

	/*	Code showing how to use J2, "SEL"  User Defined Jumper
	//pullup resistor enabled for P6.7 in port_init();
	data = P6IN;	    //read port 6
	data = data & 0x80;	//mask off other port bits
	if(data>0)          //if P6.7 is high (default)
	*/

	uart_init();

	setup_interrupts();
	//ping reader twice to sync and stop transmitting 'D's
	short_dly(50000);//pause for remainder of packet
	ping_reader();//ping for presence of reader and update display
	short_dly(50000);//pause for remainder of packet
	ping_reader();//ping for presence of reader and update display
	short_dly(50000);//pause for remainder of packet
	//**********************************************

	set_output_mode();


	for (;;)
	{
		if(light_pressed())
			P4OUT ^= 0x02; //toggle P4.1
		if(ping_pressed())
			ping_reader();//ping for presence of reader and update display
		if(antsw_pressed())
			toggle_antenna_switch();//toggle between internal and external antenna
		if(run_pressed())
			enter_run_mode();//setup for reading UIDs and set run flag for continuous inventories
		if(stop_pressed())
			exit_run_mode();//setup for reading UIDs and set run flag for continuous inventories
		if(rfoff_pressed())
			turn_rf_off();
		if(rdblk_pressed())
			read_block();

		if(run_mode==1)
			single_slot_inventory();
	}
}

//*******************************************************************************************************
void short_dly(unsigned long dly)
{
	volatile unsigned long i;            // volatile to prevent optimization

	i = dly;                          // SW Delay
	do i--;
	while (i != 0);
}



//**************************************************************************************************
void ping_reader()//ping for presence of reader and update display
{
	unsigned char pos;

	writecom(0x02, 0);//mode=0 instruction/command,    mode=1 data  home
	write_string_to_LCD("                ", 16);

	for(pos=0; pos<240; pos++)
		big_buffer_rx[pos]=0;

	rx_index=0;//prepare receive index
	keep_local=1;//if high, keep data returned to demo board (from reader) local instead of forwarding to host PC via USB.  if low, send to host PC.

	pos=0;
	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x31;//1
	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x38;//8
	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x33;//3
	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x34;//4

	big_buffer_tx[pos++] = 0x46;//F
	big_buffer_tx[pos++] = 0x46;//F

	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x30;//0

	send_buffer(pos);

	short_dly(50000);//pause for remainder of packet
	//while(rx_index< 30);

	keep_local=0;//if high, keep data returned to demo board (from reader) local instead of forwarding to host PC via USB.  if low, send to host PC.

	//send received data to LCD display
	writecom(0x02, 0);//mode=0 instruction/command,    mode=1 data  home
	for(pos=18; pos<30; pos++)
		writecom(big_buffer_rx[pos], 1);//mode=0 instruction/command,    mode=1 data  C

	rx_index=0;//prepare receive index

	update_lcd_line2();//show line 2
}


//**************************************************************************************************
void turn_rf_off()
{
	unsigned char pos;

	rx_index=0;//prepare receive index
	keep_local=1;//if high, keep data returned to demo board (from reader) local instead of forwarding to host PC via USB.  if low, send to host PC.

	pos=0;
	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x31;//1
	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x41;//A
	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x33;//3
	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x34;//4

	big_buffer_tx[pos++] = 0x31;//1
	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x31;//1

	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x30;//0
	send_buffer(pos);
	while(rx_index< 15);
	short_dly(20000);

	keep_local=0;//if high, keep data returned to demo board (from reader) local instead of forwarding to host PC via USB.  if low, send to host PC.
	rx_index=0;//prepare receive index
}



//**************************************************************************************************
void toggle_antenna_switch()//toggle between internal and external antenna
{
	unsigned char pos;

	if(currentantswstate == ANTSWINT)
		currentantswstate = ANTSWEXT;
	else
		currentantswstate = ANTSWINT;

	rx_index=0;//prepare receive index
	keep_local=1;//if high, keep data returned to demo board (from reader) local instead of forwarding to host PC via USB.  if low, send to host PC.

	pos=0;
	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x31;//1
	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x38;//8
	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x33;//3
	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x34;//4


	big_buffer_tx[pos++] = 0x32;//2

	if(currentantswstate == ANTSWINT)
		big_buffer_tx[pos++] = 0x41;//A Internal antenna
	else
		big_buffer_tx[pos++] = 0x42;//B External antenna

	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x30;//0
	send_buffer(pos);
	while(rx_index< 15);
	short_dly(20000);

	keep_local=0;//if high, keep data returned to demo board (from reader) local instead of forwarding to host PC via USB.  if low, send to host PC.
	rx_index=0;//prepare receive index

	update_lcd_line2();//show line 2
}


//**************************************************************************************************
void enter_run_mode()//setup for reading UIDs and set run flag for continuous inventories
{
	run_mode=1;
}

//**************************************************************************************************
void exit_run_mode()//setup for reading UIDs and set run flag for continuous inventories
{
	run_mode=0;
}


//**************************************************************************************************
void read_block()
{
	unsigned char pos, index;
	unsigned char uid_buf[20];


	rx_index=0;//prepare receive index
	keep_local=1;//if high, keep data returned to demo board (from reader) local instead of forwarding to host PC via USB.  if low, send to host PC.

	//send write request
	//set up for reading UIDs
	pos=0;
	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x31;//1
	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x43;//C
	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x33;//3
	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x34;//4

	big_buffer_tx[pos++] = 0x31;//1
	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x32;//2
	big_buffer_tx[pos++] = 0x31;//1
	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x31;//1

	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x30;//0  read user memory blocks

	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x30;//0
	send_buffer(pos);
	while(rx_index< 10);
	short_dly(20000);
	rx_index=0;//ignore return data

	for(pos=0; pos<200; pos++)
		big_buffer_rx[pos]=0;

	//send AGC Toggle
	pos=0;
	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x31;//1
	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x39;//9
	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x33;//3
	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x34;//4

	big_buffer_tx[pos++] = 0x46;//F
	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x30;//0

	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x30;//0
	send_buffer(pos);
	while(rx_index< 12);
	short_dly(20000);
	rx_index=0;//ignore return data

	for(pos=0; pos<200; pos++)
		big_buffer_rx[pos]=0;

	//send AM/PM toggle
	pos=0;
	big_buffer_tx[pos++] = 0x30;//0 
	big_buffer_tx[pos++] = 0x31;//1 
	big_buffer_tx[pos++] = 0x30;//0 
	big_buffer_tx[pos++] = 0x39;//9 
	big_buffer_tx[pos++] = 0x30;//0 
	big_buffer_tx[pos++] = 0x30;//0 
	big_buffer_tx[pos++] = 0x30;//0 
	big_buffer_tx[pos++] = 0x33;//3 
	big_buffer_tx[pos++] = 0x30;//0 
	big_buffer_tx[pos++] = 0x34;//4 
                                    
	big_buffer_tx[pos++] = 0x46;//F 
	big_buffer_tx[pos++] = 0x31;//1 
	big_buffer_tx[pos++] = 0x46;//F 
	big_buffer_tx[pos++] = 0x46;//F 
                                    
	big_buffer_tx[pos++] = 0x30;//0 
	big_buffer_tx[pos++] = 0x30;//0 
	big_buffer_tx[pos++] = 0x30;//0 
	big_buffer_tx[pos++] = 0x30;//0 
	send_buffer(pos);
	while(rx_index< 12);
	short_dly(9000);


	rx_index=0;//ignore return data
	while(rx_index>0)
	{
		rx_index=0;//ignore return data
		short_dly(9000);
	}

	for(pos=0; pos<200; pos++)
		big_buffer_rx[pos]=0;

	//build and send the Read block command
	pos=0;
	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x31;//1
	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x42;//B
	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x33;//3
	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x34;//4

	big_buffer_tx[pos++] = 0x31;//1
	big_buffer_tx[pos++] = 0x38;//8
	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x32;//2
	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x30;//addr msb
	big_buffer_tx[pos++] = 0x30+blkaddr;//addr lsb

	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x30;//0

	send_buffer(pos);
	while(rx_index< 12);
	short_dly(30000);

	writecom(0x02, 0);//mode=0 instruction/command,    mode=1 data  home
	if(rx_index < 41) //if empty packet [] or multiple tags in fiend
	{
		write_string_to_LCD("NO TAG FOUND    ", 16);
	}
	else
	{
		index=48;
		for(pos=0; pos<8; pos+=2)
		{
			uid_buf[pos] = big_buffer_rx[index];
			uid_buf[pos+1] = big_buffer_rx[index+1];
			index-=2;
		}

		writecom(0x02, 0);//mode=0 instruction/command,    mode=1 data  home
		write_string_to_LCD("                ", 16);
		writecom(0x02, 0);//mode=0 instruction/command,    mode=1 data  home
		write_string_to_LCD("BLK ", 4);

		write_int_to_LCD(blkaddr);

		write_string_to_LCD(": ", 2);

		//send received data to LCD display
		for(pos=0; pos<8; pos++)
			writecom(uid_buf[pos], 1);//mode=0 instruction/command,    mode=1 data  C
	}

	blkaddr++;
	if(blkaddr>7) blkaddr=0;

	keep_local=0;//if high, keep data returned to demo board (from reader) local instead of forwarding to host PC via USB.  if low, send to host PC.
	rx_index=0;//prepare receive index
	update_lcd_line2();//show line 2
}


//**************************************************************************************************
void single_slot_inventory()//setup for reading UIDs and set run flag for continuous inventories
{
	unsigned char pos, index;
	unsigned char uid_buf[20];

	//writecom(0x02, 0);//mode=0 instruction/command,    mode=1 data  home
	//write_string_to_LCD("run mode...     ", 16);

	rx_index=0;//prepare receive index
	keep_local=1;//if high, keep data returned to demo board (from reader) local instead of forwarding to host PC via USB.  if low, send to host PC.

	//send write request
	//set up for reading UIDs
	pos=0;
	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x31;//1
	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x43;//C
	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x33;//3
	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x34;//4

	big_buffer_tx[pos++] = 0x31;//1
	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x32;//2
	big_buffer_tx[pos++] = 0x31;//1
	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x31;//1

	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x32;//2  read UIDs

	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x30;//0
	send_buffer(pos);
	while(rx_index< 10);
	short_dly(20000);
	rx_index=0;//ignore return data

	for(pos=0; pos<200; pos++)
		big_buffer_rx[pos]=0;



	//send AGC Toggle
	pos=0;
	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x31;//1
	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x39;//9
	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x33;//3
	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x34;//4

	big_buffer_tx[pos++] = 0x46;//F
	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x30;//0

	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x30;//0
	send_buffer(pos);
	while(rx_index< 12);
	short_dly(20000);
	rx_index=0;//ignore return data

	for(pos=0; pos<200; pos++)
		big_buffer_rx[pos]=0;

	//send AM/PM toggle
	pos=0;
	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x31;//1
	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x39;//9
	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x33;//3
	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x34;//4

	big_buffer_tx[pos++] = 0x46;//F
	big_buffer_tx[pos++] = 0x31;//1
	big_buffer_tx[pos++] = 0x46;//F
	big_buffer_tx[pos++] = 0x46;//F

	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x30;//0
	send_buffer(pos);
	while(rx_index< 12);
	short_dly(9000);


	rx_index=0;//ignore return data
	while(rx_index>0)
	{
		rx_index=0;//ignore return data
		short_dly(9000);
	}

	for(pos=0; pos<200; pos++)
		big_buffer_rx[pos]=0;

	//send single slot inventory request
	pos=0;
	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x31;//1
	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x38;//8
	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x33;//3
	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x34;//4

	big_buffer_tx[pos++] = 0x31;//1
	big_buffer_tx[pos++] = 0x34;//4
	big_buffer_tx[pos++] = 0x32;//2
	big_buffer_tx[pos++] = 0x36;//6
	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x31;//1

	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x30;//0
	big_buffer_tx[pos++] = 0x30;//0
	send_buffer(pos);

	while(rx_index < 52);//wait for inventory results
	short_dly(9000);//pause for remainder of packet

	writecom(0x02, 0);//mode=0 instruction/command,    mode=1 data  home
	if(rx_index < 62) //if empty packet [] or multiple tags in fiend
	{
		write_string_to_LCD("NO TAG FOUND    ", 16);
	}
	else
	{
		index=65;
		for(pos=0; pos<16; pos+=2)
		{
			uid_buf[pos] = big_buffer_rx[index];
			uid_buf[pos+1] = big_buffer_rx[index+1];
			index-=2;
		}

		//send received data to LCD display
		for(pos=0; pos<16; pos++)
			writecom(uid_buf[pos], 1);//mode=0 instruction/command,    mode=1 data  C
	}

	keep_local=0;//if high, keep data returned to demo board (from reader) local instead of forwarding to host PC via USB.  if low, send to host PC.
	rx_index=0;//prepare receive index
	update_lcd_line2();//show line 2
}






//**************************************************************************************************
void update_lcd_line2()//show line 2
{
	//B:115200 ANT:INT

	writecom(0xC0, 0);//mode=0 instruction/command,    mode=1 data  position cursor to start of second row
	write_string_to_LCD("B:115200        ", 16);

	writecom(0xC9, 0);//mode=0 instruction/command,    mode=1 data  set position on second row
	if(currentantswstate == ANTSWINT)//internal
		write_string_to_LCD("ANT:INT", 7);
	else
		write_string_to_LCD("ANT:EXT", 7);
}



//**************************************************************************************************
unsigned char antsw_pressed()
{
	unsigned char data;

	data = P5IN;	//read port 5
	data &= 0x08;	//mask off other port bits
	if(!data)//if P5.3 is low then button is pressed
	{
		while(!data)//wait for button release
		{
			data = P5IN;	//read port
			data &= 0x08;	//mask off other port bits
		}

	 	return 1;
	}

	return 0;//button not pressed
}





//**************************************************************************************************
unsigned char stop_pressed()
{
	unsigned char data;

	data = P5IN;	//read port 5
	data &= 0x01;	//mask off other port bits
	if(!data)//if P5.0 is low then button is pressed
	{
		while(!data)//wait for button release
		{
			data = P5IN;	//read port
			data &= 0x01;	//mask off other port bits
		}

	 	return 1;
	}

	return 0;//button not pressed
}

//**************************************************************************************************
unsigned char rdblk_pressed()
{
	unsigned char data;

	data = P5IN;	//read port 5
	data &= 0x10;	//mask off other port bits
	if(!data)//if P5.4 is low then button is pressed
	{
		while(!data)//wait for button release
		{
			data = P5IN;	//read port
			data &= 0x10;	//mask off other port bits
		}

	 	return 1;
	}

	return 0;//button not pressed
}



//**************************************************************************************************
unsigned char rfoff_pressed()
{
	unsigned char data;

	data = P5IN;	//read port 5
	data &= 0x02;	//mask off other port bits
	if(!data)//if P5.1 is low then button is pressed
	{
		while(!data)//wait for button release
		{
			data = P5IN;	//read port
			data &= 0x02;	//mask off other port bits
		}

	 	return 1;
	}

	return 0;//button not pressed
}


//**************************************************************************************************
unsigned char run_pressed()
{
	unsigned char data;

	data = P4IN;	//read port 4
	data &= 0x80;	//mask off other port bits
	if(!data)//if P4.7 is low then button is pressed
	{
		while(!data)//wait for button release
		{
			data = P4IN;	//read port
			data &= 0x80;	//mask off other port bits
		}

	 	return 1;
	}

	return 0;//button not pressed
}



//**************************************************************************************************
unsigned char light_pressed()
{
	unsigned char data;

	data = P5IN;	//read port 5
	data &= 0x20;	//mask off other port bits
	if(!data)//if P5.5 is low then button is pressed
	{
		while(!data)//wait for button release
		{
			data = P5IN;	//read port
			data &= 0x20;	//mask off other port bits
		}

	 	return 1;
	}

	return 0;//button not pressed
}




//**************************************************************************************************
unsigned char ping_pressed()
{
	unsigned char data;

	data = P5IN;	//read port 5
	data &= 0x40;	//mask off other port bits
	if(!data)//if P5.6 is low then button is pressed
	{
		while(!data)//wait for button release
		{
			data = P5IN;	//read port
			data &= 0x40;	//mask off other port bits
		}

	 	return 1;
	}

	return 0;//button not pressed
}



//********************************************************************************************************
void setup_interrupts()
{
	  /*
	   * IFG2, Interrupt Flag Register 2
	   *
	   * ~UCB0TXIFG -- No interrupt pending
	   * ~UCB0RXIFG -- No interrupt pending
	   * ~UCA0TXIFG -- No interrupt pending
	   * UCA0RXIFG -- Interrupt pending
	   *
	   * Note: ~<BIT> indicates that <BIT> has value zero
	   */
	  IFG2 &= ~(UCA0RXIFG);

	  /*
	   * IE2, Interrupt Enable Register 2
	   *
	   * ~UCB0TXIE -- Interrupt disabled
	   * ~UCB0RXIE -- Interrupt disabled
	   * ~UCA0TXIE -- Interrupt disabled
	   * UCA0RXIE -- Interrupt enabled
	   *
	   * Note: ~<BIT> indicates that <BIT> has value zero
	   */
	  IE2 |= UCA0RXIE;

	  /*
	   * SR, Status Register
	   *
	   * ~SCG1 -- Disable System clock generator 1
	   * ~SCG0 -- Disable System clock generator 0
	   * ~OSCOFF -- Oscillator On
	   * ~CPUOFF -- CPU On
	   * GIE -- General interrupt enable
	   *
	   * Note: ~<BIT> indicates that <BIT> has value zero
	   */
	  __bis_SR_register(GIE);
}







//********************************************************************************************************
void port_init()
{
	   /* Port 1 Output Register */
	    P1OUT = 0;

	    /* Port 1 Direction Register */
	    P1DIR = BIT0 + BIT1 + BIT2 + BIT3 + BIT4 + BIT5 + BIT6 + BIT7;

	    /* Port 1 Interrupt Edge Select Register */
	    P1IES = 0;

	    /* Port 1 Interrupt Flag Register */
	    P1IFG = 0;

	    /* Port 2 Output Register */
	    P2OUT = 0;

	    /* Port 2 Direction Register */
	    P2DIR = BIT0 + BIT1 + BIT2 + BIT3 + BIT4 + BIT5 + BIT6 + BIT7;

	    /* Port 2 Interrupt Edge Select Register */
	    P2IES = 0;

	    /* Port 2 Interrupt Flag Register */
	    P2IFG = 0;

	    /* Port 3 Output Register */
	    P3OUT = 0;

	    /* Port 3 Port Select Register */
	    P3SEL = BIT4 + BIT5 + BIT6 + BIT7;

	    /* Port 3 Direction Register */
	    P3DIR = BIT0 + BIT1 + BIT2 + BIT3;

	    /* Port 4 Output Register */
	    P4OUT = 0;

	    /* Port 4 Direction Register */
	    P4DIR = BIT0 + BIT1 + BIT2 + BIT3 + BIT4 + BIT5 + BIT6;

	    /* Port 5 Output Register */
	    P5OUT = 0;

	    /* Port 5 Direction Register */
	    P5DIR = BIT7;

	    /* Port 6 Output Register */
	    P6OUT = 0;

	    /* Port 6 Direction Register */
	    P6DIR = BIT0 + BIT1 + BIT2 + BIT3 + BIT4 + BIT5 + BIT6;

	    //pull P6.7 high using internal pullup resistor
	    P6DIR &= ~BIT7;
	    P6OUT |= BIT7;//set internal resistor to pull up
	    P6REN |= BIT7;//enable pullup resistor
}







//***************************************************************************
void initLCD()
{
	short_dly(SHORT);
	writecom(0x30,0); //wake up
	short_dly(SHORT);
	writecom(0x30,0); //wake up
	short_dly(SHORT);
	writecom(0x30,0); //wake up
	short_dly(SHORT);
	writecom(0x39,0); //function set
	short_dly(SHORT);
	writecom(0x14,0); //internal osc frequency
	short_dly(SHORT);
	writecom(0x56,0); //power control
	short_dly(SHORT);
	writecom(0x6D,0); //follower control
	short_dly(SHORT);
	writecom(0x79,0); //contrast  was 70,0
	short_dly(SHORT);
	writecom(0x0C,0); //display on
	short_dly(SHORT);
	writecom(0x06,0); //entry mode
	short_dly(SHORT);
	writecom(0x01,0); //clear
	short_dly(SHORT);
}




//******************************************************************************
void writecom(unsigned char d, unsigned char mode)
{
	unsigned char serialcounter;

	P4OUT &= ~BIT5;//CSB low
	if(mode==0) P4OUT &= ~BIT6;//RS=0 instruction/command
	if(mode==1) P4OUT |= BIT6;//RS-1 data

	short_dly(SHORT);

	for(serialcounter = 1; serialcounter <= 8; serialcounter++) //send 8 bits
	{
		if((d&0x80)==0x80) //get only the MSB
			P4OUT |= BIT3;//SI high
		   else
			P4OUT &= ~BIT3;//SI low
		short_dly(SHORT);

		d=(d<<1); //shift data byte left
		P4OUT &= ~BIT4;//clock low
		short_dly(SHORT);
		P4OUT |= BIT4;//clock high (idle)
		short_dly(SHORT);
		P4OUT &= ~BIT4;//clock low
	}

	short_dly(SHORT);
	P4OUT |= BIT5;//CSB high (idle)
}




//**************************************************************************************************
void write_string_to_LCD(char *strdata, unsigned char len)
{
	unsigned char loop;

	for(loop=0; loop<len; loop++)
		writecom(strdata[loop], 1);//mode=0 instruction/command,    mode=1 data  A
}

//**************************************************************************************************
void write_int_to_LCD(unsigned int x)
{
	char buf[10];

	sprintf(buf,"%d", x) ;
	writecom(buf[0], 1);//mode=0 instruction/command,    mode=1 data
	//writecom(buf[1], 1);//mode=0 instruction/command,    mode=1 data
	//writecom(buf[2], 1);//mode=0 instruction/command,    mode=1 data
}


//**************************************************************************************************
void write_float_to_LCD(float x)
{
	char buf[10];

	sprintf(buf,"%f", x) ;
	writecom(buf[0], 1);//mode=0 instruction/command,    mode=1 data  A
	writecom(buf[1], 1);//mode=0 instruction/command,    mode=1 data  A
	writecom(buf[2], 1);//mode=0 instruction/command,    mode=1 data  A
}



//*****************************************************************************************************************
void uart_init()
{
    UCA0CTL1 |= UCSWRST;// Disable USCI

	UCA0CTL1 = UCSSEL_2 + UCSWRST;
	UCA0MCTL = UCBRF_0 + UCBRS_1;
	UCA0BR0=104;//correct for 115,200 baud
	UCA0BR1=0;//correct for 115,200 baud

    UCA0CTL1 &= ~UCSWRST;// Enable USCI

    UC0IE |= UCA0RXIE;//USCI_A0 Interrupt Enable Register

 //***************************************************************************************

    UCA1CTL1 |= UCSWRST;// Disable USCI

	UCA1CTL1 = UCSSEL_2 + UCSWRST;
	UCA1MCTL = UCBRF_0 + UCBRS_1;
	UCA1BR0 = 104;
	UCA1BR1 = 0;

    UCA1CTL1 &= ~UCSWRST;// Enable USCI

    UC1IE |= UCA1RXIE;//USCI_A1 Interrupt Enable Register
}



//******************************************************************************************************************************
void send_buffer(unsigned char num_bytes)
{
	unsigned char x;

	x=0;
	while(x < num_bytes)
	{
		while (!(IFG2&UCA1TXIFG));    // USCI_A1 TX buffer ready?
		UCA1TXBUF = big_buffer_tx[x];
		x++;
		short_dly(SHORT);
	}
}



//*****************************************************************************************************************
void set_output_mode()
{
	unsigned char pos;

		rx_index=0;//prepare receive index
		keep_local=1;//if high, keep data returned to demo board (from reader) local instead of forwarding to host PC via USB.  if low, send to host PC.
		pos=0;
		big_buffer_tx[pos++] = 0x30;//0
		big_buffer_tx[pos++] = 0x31;//1
		big_buffer_tx[pos++] = 0x30;//0
		big_buffer_tx[pos++] = 0x40;//9
		big_buffer_tx[pos++] = 0x30;//0
		big_buffer_tx[pos++] = 0x30;//0
		big_buffer_tx[pos++] = 0x30;//0
		big_buffer_tx[pos++] = 0x33;//3
		big_buffer_tx[pos++] = 0x30;//0
		big_buffer_tx[pos++] = 0x34;//4

		big_buffer_tx[pos++] = 0x32;//2 set mode to digital out on port pin P2.4
		big_buffer_tx[pos++] = 0x44;//C
		big_buffer_tx[pos++] = 0x30;//0
		big_buffer_tx[pos++] = 0x34;//4

		big_buffer_tx[pos++] = 0x30;//0
		big_buffer_tx[pos++] = 0x30;//0
		big_buffer_tx[pos++] = 0x30;//0
		big_buffer_tx[pos++] = 0x30;//0
	 	send_buffer(pos);
		//while(rx_index< 15);
		short_dly(20000);
		keep_local=0;//if high, keep data returned to demo board (from reader) local instead of forwarding to host PC via USB.  if low, send to host PC.
		rx_index=0;//prepare receive index
}








