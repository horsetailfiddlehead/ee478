/* 
 * File:   i2c_local.h
 * Author: Patrick
 *
 * Created on April 23, 2014, 8:20 PM
 */

#ifndef I2C_LOCAL_H
#define	I2C_LOCAL_H

#ifdef	__cplusplus
extern "C" {
#endif

int setupOutgoing();
int sendSpeed(unsigned int *slaveAddr, unsigned int *speed);
int getRemoteData();


#ifdef	__cplusplus
}
#endif

#endif	/* I2C_LOCAL_H */

