/* 
 * File:   rfidReader.h
 * Author: Patrick
 *
 * Created on May 29, 2014, 12:45 PM
 */

#ifndef RFIDREADER_H
#define	RFIDREADER_H

#ifdef	__cplusplus
extern "C" {
#endif

void resetRFID(void);
void RFIDSetup(void);
void processRFIDCmd(void);

#ifdef	__cplusplus
}
#endif

#endif	/* RFIDREADER_H */

