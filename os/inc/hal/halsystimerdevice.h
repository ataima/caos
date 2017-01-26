#ifndef _HAL_SYS_TIMER_DEVICE_H
#define _HAL_SYS_TIMER_DEVICE_H

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
#include "syslog.h"

struct caSysTimerDeviceCtrl
: public caIDeviceCtrl {
public:

    typedef enum tag_io_ctrl_systimer_specific_request {
        sysTimerFlush = 0x5000,
        sysTimerListHardware,
        sysTimerStart,
        sysTimerStop,
        sysTimerAddSignal_1,
        sysTimerAddSignal_2,
        sysTimerRemoveSignal_1,
        sysTimerRemoveSignal_2,
        sysTimerGetSignal_1,
        sysTimerGetSignal_2,
    } IoSysTimerCtrlDirect;   
};

struct caSysTimerConfigure
: public caIDeviceConfigure {
public:
    u32 tick_ps;
    u32 clock_ps;
    u32 prescaler_ps;
    u32 irq_ps;
    // add with high resolition timers...
};

class caHalSysTimerDevice
: public IDevice {
private:
    u32 mask_guid;
    u32 handle_guid;
    u32 isOpen;
    u32 signal_1;
    u32 signal_2;
    caSysLog caLog;
    hal_llc_sys_time *link;
public:
    caHalSysTimerDevice(hal_llc_sys_time *st, u32 mask_handle);
    u32 IoctlReq(ioCtrlFunction request, u32 *p1, u32 *p2);
    u32 Open(caIDeviceConfigure *conf, caDeviceHandle *port);
    u32 Close(caDeviceHandle *port);
    u32 Write(caDeviceHandle *port);
    u32 Read(caDeviceHandle *port);
    u32 IoCtrl(caDeviceHandle *port, caIDeviceCtrl *in);
    u32 Flush(caDeviceHandle *port);
    u32 IrqService1(u8 * buff, s_t size, s_t & iosize);
    u32 IrqService2(u8 * buff, s_t size, s_t & iosize);
    u32 IrqService3(u8 * buff, s_t size, s_t & iosize);
    u32 IrqService4(u8 * buff, s_t size, s_t & iosize);
    u32 IrqService5(u8 * buff, s_t size, s_t & iosize);
    u32 IrqService6(u8 * buff, s_t size, s_t & iosize);
    u32 IrqService7(u8 * buff, s_t size, s_t & iosize);
    u32 IrqService8(u8 * buff, s_t size, s_t & iosize);

    inline u32 GetOpenFlag(void) {
        return isOpen;
    }
};



#endif  //_HAL_SYS_TIMER_DEVICE_H

