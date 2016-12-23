////////////////////////////////////////////////////////////////////////////////
//    Copyright (C) 2016  Angelo Coppi (angelogkcop at hotmail.com )
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.
////////////////////////////////////////////////////////////////////////////////
// Author : Angelo Coppi (coppi dot angelo at virgilio dot it )
// History:        
////////////////////////////////////////////////////////////////////////////////

#include "config.h"
#include "caos_c_types.h"
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