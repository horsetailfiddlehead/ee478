/* 
 * File:   i2c_local.h
 * Author: Patrick
 *
 * Created on April 23, 2014, 8:20 PM
 */

#ifndef I2C_LOCAL_H
#define	I2C_LOCAL_H

void runLocalI2C(unsigned int *setSpeed);
void runReceiveI2C(unsigned int *setSpeed);
int setupOutgoing();
int setupIncoming();
int sendSpeed(unsigned int *slaveAddr, unsigned int *speed);
int getRemoteData();



#endif	/* I2C_LOCAL_H */

