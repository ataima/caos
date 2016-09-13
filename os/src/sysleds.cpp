#include "config.h"
#include "bcm2836.h"
#include "gpio.h"
#include "sysleds.h"

#if HAVE_LEDS

void caSysLed::LedsOff(void) {
    caGpio::SetOut(47);
    caGpio::SetOut(35);
    caGpio::Clr(1, BIT(47 - 32) | BIT(35 - 32));
}

void caSysLed::LedsOn(void) {
    caGpio::SetOut(47);
    caGpio::SetOut(35);
    caGpio::Set(1, BIT(47 - 32) | BIT(35 - 32));
}

void caSysLed::LedRedOff(void) {
    caGpio::SetOut(35);
    caGpio::Clr(35);
}

void caSysLed::LedRedOn(void) {
    caGpio::SetOut(35);
    caGpio::Set(35);
}

void caSysLed::LedGreenOff(void) {
    caGpio::SetOut(47);
    caGpio::Clr(47);
}

void caSysLed::LedGreenOn(void) {
    caGpio::SetOut(47);
    caGpio::Set(47);
}

#endif