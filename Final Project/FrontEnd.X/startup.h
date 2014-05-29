/* 
 * File:   startup.h
 * Author: castia
 *
 * Created on May 25, 2014, 8:00 PM
 */

#ifndef STARTUP_H
#define	STARTUP_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#include "globals.h"

    void initClock(void);
    void initSPI1(void);

#ifdef	__cplusplus
}
#endif

#endif	/* STARTUP_H */

