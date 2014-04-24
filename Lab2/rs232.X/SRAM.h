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




#ifdef	__cplusplus
}
#endif

#endif	/* SRAM_H */

void SRAMsetUp();
void setUpIn();
void setUpOut();
int readData(int adx);
void writeData(int adx, int data);
