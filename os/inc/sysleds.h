#ifndef _SYS_LEDS_HEADER_
#define _SYS_LEDS_HEADER_

#include "bcm2836.h"

#if HAVE_LEDS

class caSysLed {
public:
    static void LedsOff(void);
    static void LedsOn(void);
    static void LedRedOff(void);
    static void LedRedOn(void);
    static void LedGreenOff(void);
    static void LedGreenOn(void);
};

#endif

#endif