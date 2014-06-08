#include "globals.h"

char myATMY[5];

void hostGame() {
    char playerID[16] = { 0 };
    char newNetwork[5] = { 0 };
    setupHost();
    srand(ReadTimer0());
    // Broadcast current ID for players
    while(!DataRdy1USART()) {
        putrs1USART(myATMY);
        while(Busy1USART());
        putc1USART('\r');
        while(Busy1USART());
    }

    // Read player id
    readBytesUntil1USART(playerID, '\r', 16);

    // Talk directly to player
    setXbeeNetwork(SEARCH_NET, myATMY, playerID);

    // Create a private network
    generateNetwork(newNetwork);

    // Contact player with new network
    putrs1USART(newNetwork);
    while(Busy1USART());
    putc1USART('\r');
    while(Busy1USART());

    // Change networks and broadcast until response
    setXbeeNetwork(newNetwork, myATMY, playerID);
    while(!DataRdy1USART()) {
        putc1USART('1');
        while(Busy1USART());
    }

    // get response
    getc1USART();

    // send confirmation
    putc1USART('0');
    while(Busy1USART());
}

void findGame() {
    char hostID[16] = { 0 };
    char newNetwork[16] = { 0 };

    setupClient();

    while(!DataRdy1USART());
    // want between two \r's, so do this twice
    readBytesUntil1USART(hostID, '\r', 16);
    readBytesUntil1USART(hostID, '\r', 16);
    // Configure to send to host
    setXbeeNetwork(SEARCH_NET, myATMY, hostID);
    
    // change own id to avoid getting any more input
    generateID(hostID);

    //broadcast new id
    putrs1USART(myATMY);
    while(Busy1USART());
    putc1USART('\r');
    while(Busy1USART());

    // wait for network
    while(!DataRdy1USART());

    // read new network
    readBytesUntil1USART(newNetwork, '\r', 16);

    // Change the network
    setXbeeNetwork(newNetwork, myATMY, hostID);

    // Wait until contacted by host on new network
    while(!DataRdy1USART());
    getc1USART();

    // respond
    putc1USART('0');
    while(Busy1USART());

    // wait for confirmation
    while(!DataRdy1USART());
    getc1USART();
}

void generateNetwork(char* newNetwork) {
    int network = 0;
    while (network == 0 || (strcmppgm2ram(newNetwork, SEARCH_NET) != 0)) {
        network = rand() % 0xFFFE;
        sprintf(newNetwork, "%04x", network);
    }
}

void setupHost() {
    // set network to SEARCH_NET
    // set id to random
    // set ATDL to FIND_ID
    generateID("FFFE");
    setXbeeNetwork(SEARCH_NET, myATMY, FIND_ID);

}

void setupClient() {
    strcpypgm2ram(myATMY, FIND_ID);
    setXbeeNetwork(SEARCH_NET,myATMY, "0000");

    // set id to FIND_ID
    // set network to SEARCH_NET
}

void generateID(char* otherID) {
    int ID = 0;
    while (ID == 0 || (strcmppgm2ram(myATMY, otherID) != 0)) {
        ID = rand() % 0xFFFE;
        sprintf(myATMY, "%04x", ID);
    }
}

//void configureATDL(char* hostID) {
//    // Tx set low
//    TXSTA1bits.TXEN1 = 0;
//    PORTCbits.RC6 = 0;
//
//    resetXbee();
//    // Reenable Tx
//    TXSTA1bits.TXEN1 = 1;
//    putrs1USART("ATDL");
//    while (Busy1USART());
//    putrs1USART(hostID);
//    while (Busy1USART());
//
//    // ATWR,AC,CN - Write changes to nonVolatile memory
//    // ATAC - Apply changes
//    // ATCN - Exit config mode
//    //Carriage return
//    putrs1USART("WR,AC,CN\r");
//    while (Busy1USART());
//    while (!DataRdy1USART());
//
//}

void resetXbee(void) {
    //Reset Pin- configure these to be outputs
    PORTBbits.RB7 = 0;
    // Delay 10 Instruction cycles, pulse must be at least 200ns;
    Delay10TCYx(5);
    PORTBbits.RB7 = 1;
    // Two pulses/responses come in on startup
    while (!DataRdy1USART());
    while (!DataRdy1USART());
}

void setupXbee(void) {

    TRISBbits.RB7 = 0;
    PORTBbits.RB7 = 1;
}

void setXbeeNetwork(char* myNetwork, char* myID, char* myDL) {
    // Tx set low
    TXSTA1bits.TXEN1 = 0;
    PORTCbits.RC6 = 0;

    resetXbee();
    // Reenable Tx
    TXSTA1bits.TXEN1 = 1;
    putrs1USART("ATID");
    while (Busy1USART());
    putrs1USART(myNetwork);
    while (Busy1USART());
    putrs1USART(",MY");
    while (Busy1USART());
    putrs1USART(myID);
    while (Busy1USART());
    putrs1USART(",DL");
    while (Busy1USART());
    putrs1USART(myDL);
    while (Busy1USART());
    putrs1USART(",DH");
    while (Busy1USART());
    putrs1USART("0000");
    while (Busy1USART());

    // Config Commands
    //Carriage return
    // Config Commands
    //Carriage return
    // etc
    // Exit

    // ATWR,AC,CN - Write changes to nonVolatile memory
    // ATAC - Apply changes
    // ATCN - Exit config mode
    //Carriage return
    putrs1USART(",WR,AC,CN\r");
    while (Busy1USART());
    while (!DataRdy1USART());
    //
    //    putrs1USART("ATID\r");
    //    while(Busy1USART());
    //    while (!DataRdy1USART());
}

