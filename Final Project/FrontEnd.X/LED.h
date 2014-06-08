/*
 * Status LED control header file
 * June 8, 2014
 */

#ifndef LED_H
#define	LED_H

#ifdef	__cplusplus
extern "C" {
#endif

#define NUM_SLOTS 4 // Number of card slots on board

    typedef struct {
        char ledStatus[NUM_SLOTS]; // each char represents a slot
    }LEDDriverStruct;

    extern LEDDriverStruct ledData;

    void LEDSetup(void);
    void updateLEDs(void);

#ifdef	__cplusplus
}
#endif

#endif	/* LED_H */

