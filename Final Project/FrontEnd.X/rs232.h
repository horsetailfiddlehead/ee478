/* 
 * File:   rs232.h
 * Author: castia
 *
 * Created on April 24, 2014, 10:10 PM
 */
void rs232Setup1(void);
void rs232Setup2(void);
void readBytesUntil1USART(char* myStorage, char stopChar, int size);
void readBytesUntil2USART(char* myStorage, char stopChar, int size);
