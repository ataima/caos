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
        sysTimerFlush = IO_CTRL_UID(id_device::id_SysTimer),
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
    static u32 Open(IDevice * instance,caIDeviceConfigure *conf, caDeviceHandle *port);
    static u32 Close(IDevice * instance,caDeviceHandle *port);
    static u32 Write(IDevice * instance,caDeviceHandle *port);
    static u32 Read(IDevice * instance,caDeviceHandle *port);
    static u32 IoCtrl(IDevice * instance,caDeviceHandle *port, caIDeviceCtrl *in);
    static u32 Flush(IDevice * instance,caDeviceHandle *port);
    static u32 IrqService1(IDevice * instance,u8 * buff, s_t size, s_t & iosize);
    static u32 IrqService2(IDevice * instance,u8 * buff, s_t size, s_t & iosize);
    static u32 IrqService3(IDevice * instance,u8 * buff, s_t size, s_t & iosize);
    static u32 IrqService4(IDevice * instance,u8 * buff, s_t size, s_t & iosize);
    static u32 IrqService5(IDevice * instance,u8 * buff, s_t size, s_t & iosize);
    static u32 IrqService6(IDevice * instance,u8 * buff, s_t size, s_t & iosize);
    static u32 IrqService7(IDevice * instance,u8 * buff, s_t size, s_t & iosize);
    static u32 IrqService8(IDevice * instance,u8 * buff, s_t size, s_t & iosize);

    static inline caSysLog * GetDeviceLog(IDevice * instance) {
        caHalSysTimerDevice* dev=static_cast<caHalSysTimerDevice*>(instance);
        return &dev->caLog;
    }

    static inline u32 GetOpenFlag(IDevice * instance) {
        caHalSysTimerDevice* dev=static_cast<caHalSysTimerDevice*>(instance);
        return dev->isOpen;
    }

    static inline const char * toString(void) {
        return "Hal Sysytem Timer driver ('SysTimer') :";
    }

};



#endif  //_HAL_SYS_TIMER_DEVICE_H

