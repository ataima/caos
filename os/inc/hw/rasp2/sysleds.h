#ifndef _SYS_LEDS_HEADER_
#define _SYS_LEDS_HEADER_


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
    static void LedOn(u32 led);
    static void LedOff(u32 led);
};

#endif

#endif