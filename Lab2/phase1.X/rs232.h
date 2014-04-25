/* 
 * File:   rs232.h
 * Author: castia
 *
 * Created on April 24, 2014, 10:10 PM
 */
void rs232Setup(void);
void readBytesUntil(char* myStorage, char stopChar, int size);
