/* 
 * File:   SRAM.h
 * Author: castia
 *
 * Created on April 24, 2014, 1:59 AM
 */

#ifndef SRAM_H
#define	SRAM_H

#ifdef	__cplusplus
extern "C" {
#endif

void SRAMsetUp(void);
void setUpIn(void);
void setUpOut(void);
int readData(int adx);
void writeData(int adx, int data);


#ifdef	__cplusplus
}
#endif

#endif	/* SRAM_H */


