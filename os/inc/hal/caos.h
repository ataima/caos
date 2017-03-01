#ifndef DEVICE_H
#define DEVICE_H

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


#include "hal.h"
#include "halsystimerdevice.h"
#include "halcomdevice.h"
#include "haljobdevice.h"
#include "halmemdevice.h"
#include "halpipedevice.h"

class caOS {
private:

    /* table pof all caos devices*/
    typedef struct device_pair {
        const char * name; /// name of device
        ioCtrlRequest mask; /// base mask of device's handle
        IDevice *device; /// device object
    } devicePair;

    static devicePair allDevices[];

    static caHalJobDevice scheduler;

    static caHalSysTimerDevice timer1; // already present for task scheduler...  

    static caHalMemDevice memory;
    
    static caHalPipeDevice pipe;
    
#if SYS_TIMER_2_DEVICE 
    static caHalSysTimerDevice timer2;
#endif

#if SYS_TIMER_3_DEVICE 
    static caHalSysTimerDevice timer3;
#endif

#if SYS_TIMER_4_DEVICE 
    static caHalSysTimerDevice timer4;
#endif

#if SYS_TIMER_5_DEVICE 
    static caHalSysTimerDevice timer5;
#endif

#if SYS_TIMER_6_DEVICE 
    static caHalSysTimerDevice timer6;
#endif

#if SYS_TIMER_7_DEVICE 
    static caHalSysTimerDevice timer7;
#endif

#if SYS_TIMER_8_DEVICE 
    static caHalSysTimerDevice timer8;
#endif


    static caHalComDevice com1;

#if COM2_DEVICE
    static caHalComDevice com2;
#endif
#if COM3_DEVICE
    static caHalComDevice com3;
#endif
#if COM4_DEVICE
    static caHalComDevice com4;
#endif
#if COM5_DEVICE
    static caHalComDevice com5;
#endif
#if COM6_DEVICE
    static caHalComDevice com6;
#endif
#if COM7_DEVICE
    static caHalComDevice com7;
#endif
#if COM8_DEVICE
    static caHalComDevice com8;
#endif
private:
    static bool GetDevice(const char * name, s32 & offset);
    static bool GetDevice(caDeviceHandle & port, s32 & offset);
public:
    static deviceError Init(void);
    //  methos available only in supervisor mode device to device or user to device by software isr
    static deviceError Open(const char * device, caIDeviceConfigure & in, caDeviceHandle & out);
    static deviceError Close(caDeviceHandle & port);
    static deviceError Write(caDeviceHandle & port);
    static deviceError Write(caDeviceHandle & port, caStringStream<s8> &ss);
    static deviceError Read(caDeviceHandle & port);
    static deviceError Read(caDeviceHandle & port, caTokenizeSStream<s8> &in);
    static deviceError IoCtrl(caDeviceHandle & port, caIDeviceCtrl &in);
    // method avaiable only in supervisor mode device to device or  isr to device

    static inline caHalJobDevice & getScheduler(void) {
        return scheduler;
    }

    static inline caHalComDevice & getCom1(void) {
        return com1;
    }

#if COM2_DEVICE

    static inline caHalComDevice & getCom2(void) {
        return com2;
    }
#endif
#if COM3_DEVICE

    static inline caHalComDevice & getCom3(void) {
        return com3;
    }
#endif
#if COM4_DEVICE

    static inline caHalComDevice & getCom4(void) {
        return com4;
    }
#endif
#if COM5_DEVICE

    static inline caHalComDevice & getCom5(void) {
        return com5;
    }
#endif
#if COM6_DEVICE

    static inline caHalComDevice & getCom6(void) {
        return com6;
    }
#endif
#if COM7_DEVICE

    static inline caHalComDevice & getCom7(void) {
        return com7;
    }
#endif
#if COM8_DEVICE

    static inline caHalComDevice & getCom8(void) {
        return com8;
    }
#endif

    static inline caHalSysTimerDevice & getTimer1(void) {
        return timer1;
    }

#if SYS_TIMER_2_DEVICE

    static inline caHalSysTimerDevice & getTimer2(void) {
        return timer2;
    }
#endif
#if SYS_TIMER_3_DEVICE

    static inline caHalSysTimerDevice & getTimer3(void) {
        return timer3;
    }
#endif
#if SYS_TIMER_4_DEVICE

    static inline caHalSysTimerDevice & getTimer4(void) {
        return timer4;
    }
#endif
#if SYS_TIMER_5_DEVICE

    static inline caHalSysTimerDevice & getTimer5(void) {
        return timer5;
    }
#endif
#if SYS_TIMER_6_DEVICE

    static inline caHalSysTimerDevice & getTimer6(void) {
        return timer6;
    }
#endif
#if SYS_TIMER_7_DEVICE

    static inline caHalSysTimerDevice & getTimer7(void) {
        return timer7;
    }
#endif
#if SYS_TIMER_8_DEVICE

    static inline caHalSysTimerDevice & getTimer8(void) {
        return timer8;
    }
#endif

};

#endif /* DEVICE_H */

