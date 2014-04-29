/* 
 * File:   i2c_remote.h
 * Author: castia
 *
 * Created on April 25, 2014, 4:46 AM
 */

#ifndef I2C_REMOTE_H
#define	I2C_REMOTE_H

void runRemoteI2C(unsigned int *setSpeed);
void runReceiveI2C(unsigned int *setSpeed);
int setupOutgoing();
int setupIncoming();
int sendSpeed(unsigned int *slaveAddr, unsigned int *speed);
int getRemoteData();

#endif	/* I2C_REMOTE_H */

