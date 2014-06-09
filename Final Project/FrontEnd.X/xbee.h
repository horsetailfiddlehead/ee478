/* 
 * File:   xbee.h
 * Author: castia
 *
 * Created on June 8, 2014, 2:20 PM
 */

#ifndef XBEE_H
#define	XBEE_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "globals.h"
    
#define FIND_ID "1234"
#define PLAYER_ID "5555"
#define HOST_ID "6666"
#define SEARCH_NET "4444"
#define PLAY_NET "8765"

void hostGame(void);
void findGame(void);
void generateNetwork(char* newNetwork);
void generateID(char* otherID);
void setupHost(void);
void setupClient(void);

void resetXbee(void);
void setupXbee(void);
void setXbeeNetwork(char* myNetwork, char* myID, char* myDL);


#ifdef	__cplusplus
}
#endif

#endif	/* XBEE_H */

