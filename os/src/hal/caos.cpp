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
#include "memaux.h"
#include "scheduler.h"
#include "memory.h"
#include "caos.h"

caHalJobDevice caOS::scheduler(&hal_llc_scheduler, ioCtrlRequest::Task);

caHalSysTimerDevice caOS::timer1(&hal_llc_time_1, ioCtrlRequest::SysTimer1);

caHalMemDevice caOS::memory(&hal_llc_mem,ioCtrlRequest::Memory);

caHalPipeDevice caOS::pipe(&hal_llc_mem,ioCtrlRequest::Pipe);

#if SYS_TIMER_2_DEVICE
caHalSysTimerDevice caOS::timer2(&hal_llc_time_2, ioCtrlRequest::SysTimer2);
#endif

#if SYS_TIMER_3_DEVICE
caHalSysTimerDevice caOS::timer3(&hal_llc_time_3, ioCtrlRequest::SysTimer3);
#endif

#if SYS_TIMER_4_DEVICE
caHalSysTimerDevice caOS::timer4(&hal_llc_time_4, ioCtrlRequest::SysTimer4);
#endif

#if SYS_TIMER_5_DEVICE
caHalSysTimerDevice caOS::timer5(&hal_llc_time_5, ioCtrlRequest::SysTimer5);
#endif

#if SYS_TIMER_6_DEVICE
caHalSysTimerDevice caOS::timer6(&hal_llc_time_6, ioCtrlRequest::SysTimer6);
#endif

#if SYS_TIMER_7_DEVICE
caHalSysTimerDevice caOS::timer7(&hal_llc_time_7, ioCtrlRequest::SysTimer7);
#endif

#if SYS_TIMER_8_DEVICE
caHalSysTimerDevice caOS::timer8(&hal_llc_time_8, ioCtrlRequest::SysTimer8);
#endif

caHalComDevice caOS::com1(&hal_llc_com1, ioCtrlRequest::Com1);

#if COM2_DEVICE
caHalComDevice caOS::com2(&hal_llc_com2, ioCtrlRequest::Com2);
#endif

#if COM3_DEVICE
caHalComDevice caOS::com3(&hal_llc_com3, ioCtrlRequest::Com3);
#endif

#if COM4_DEVICE
caHalComDevice caOS::com4(&hal_llc_com4, ioCtrlRequest::Com4);
#endif

#if COM5_DEVICE
caHalComDevice caOS::com5(&hal_llc_com5, ioCtrlRequest::Com5);
#endif

#if COM6_DEVICE
caHalComDevice caOS::com6(&hal_llc_com6, ioCtrlRequest::Com6);
#endif

#if COM7_DEVICE
caHalComDevice caOS::com7(&hal_llc_com7, ioCtrlRequest::Com7);
#endif

#if COM8_DEVICE
caHalComDevice caOS::com8(&hal_llc_com8, ioCtrlRequest::Com8);
#endif


caOS::devicePair caOS::allDevices[] = {
    {"MEMORY", ioCtrlRequest::Memory, &memory},
    {"TASK", ioCtrlRequest::Task, &scheduler},
    {"PIPE", ioCtrlRequest::Pipe, &pipe},
    {"SYS_TIMER_1", ioCtrlRequest::SysTimer1, &timer1},

#if SYS_TIMER_2_DEVICE    
    {"SYS_TIMER_2", ioCtrlRequest::SysTimer2, &timer2},
#endif    
#if SYS_TIMER_3_DEVICE    
    {"SYS_TIMER_3", ioCtrlRequest::SysTimer3, &timer3},
#endif    
#if SYS_TIMER_4_DEVICE    
    {"SYS_TIMER_4", ioCtrlRequest::SysTimer4, &timer4},
#endif    
#if SYS_TIMER_5_DEVICE    
    {"SYS_TIMER_5", ioCtrlRequest::SysTimer5, &timer5},
#endif    
#if SYS_TIMER_6_DEVICE    
    {"SYS_TIMER_6", ioCtrlRequest::SysTimer6, &timer6},
#endif    
#if SYS_TIMER_7_DEVICE    
    {"SYS_TIMER_7", ioCtrlRequest::SysTimer7, &timer7},
#endif    
#if SYS_TIMER_8_DEVICE    
    {"SYS_TIMER_8", ioCtrlRequest::SysTimer8, &timer8},
#endif    
    {"CACHE", ioCtrlRequest::Cache, NULL},

    {"TTY1", ioCtrlRequest::Com1, &com1},

#if COM2_DEVICE    
    {"TTY2", ioCtrlRequest::Com2, &com2},
#endif
#if COM3_DEVICE    
    {"TTY3", ioCtrlRequest::Com3, &com3},
#endif
#if COM4_DEVICE    
    {"TTY4", ioCtrlRequest::Com4, &com4},
#endif
#if COM5_DEVICE    
    {"TTY5", ioCtrlRequest::Com5, &com5},
#endif
#if COM6_DEVICE
    {"TTY6", ioCtrlRequest::Com6, &com6},
#endif
#if COM7_DEVICE    
    {"TTY7", ioCtrlRequest::Com7, &com7},
#endif
#if COM8_DEVICE    
    {"TTY8", ioCtrlRequest::Com8, &com8},
#endif
};

bool caOS::GetDevice(const char * name, s32 & offset) {
    bool res = false;
    offset = -1;
    u32 i;
    u32 mnDevices = (sizeof (allDevices) / sizeof (devicePair)) ;
    for (i = 0; i < mnDevices; i++) {
        // TO DO BINARY SEARCH ON allDevices
        if (caStrAux::StrICmp(allDevices[i].name, name) == 0) {
            offset = (s32) i;
            res = true;
            break;
        }
    }
    return res;
}

bool caOS::GetDevice(caDeviceHandle & port, s32 & offset) {
    bool res = false;
    offset = -1;
    u32 mask = ioCtrlRequest::maskIoCtrl & port.handle;
    u32 base = ioCtrlRequest::maskHandle & port.handle;
    if (base > BASE_HANDLE) {
        u32 i;
        u32 mnDevices = (sizeof (allDevices) / sizeof (devicePair)) + 1;
        for (i = 0; i < mnDevices; i++) {
            // TO DO BINARY SEARCH ON allDevices
            if (allDevices[i].mask == mask) {
                offset = (s32) i;
                res = true;
                break;
            }
        }
    }
    return res;
}

// return 0 OK else return caDevice::Error

deviceError caOS::Open(const char * name,
        caIDeviceConfigure & in, caDeviceHandle & out) {
    u32 res = deviceError::no_error;
    s32 offset;
    if (name == NULL) {
        res = deviceError::error_unknow_device_name;
    } else {
        if (caOS::GetDevice(name, offset)) {
            res = caHalDeviceRules::Open(allDevices[offset].device, &in, &out, allDevices[offset].mask);
        } else {
            res = deviceError::error_unknow_device_name;
        }
    }
    return (deviceError) res;
}

deviceError caOS::Close(caDeviceHandle & port) {
    u32 res = deviceError::no_error;
    s32 offset;
    if (caOS::GetDevice(port, offset)) {
        res = caHalDeviceRules::Close(allDevices[offset].device, &port, allDevices[offset].mask);
    } else {
        res = deviceError::error_invalid_handle_port;
    }
    return (deviceError) res;
}

deviceError caOS::Write(caDeviceHandle & port) {
    u32 res = deviceError::no_error;
    s32 offset;
    if (caOS::GetDevice(port, offset)) {
        res = caHalDeviceRules::Write(allDevices[offset].device, &port, allDevices[offset].mask);
    } else {
        res = deviceError::error_invalid_handle_port;
    }
    return (deviceError) res;
}

deviceError caOS::Write(caDeviceHandle & port, caStringStream<s8> &ss) {
    port.wrBuff = reinterpret_cast<u8*> (ss.Str());
    port.wrSize = ss.Size();
    return caOS::Write(port);
}

deviceError caOS::Read(caDeviceHandle & port) {
    u32 res = deviceError::no_error;
    s32 offset;
    if (caOS::GetDevice(port, offset)) {
        res = caHalDeviceRules::Read(allDevices[offset].device, &port, allDevices[offset].mask);
    } else {
        res = deviceError::error_invalid_handle_port;
    }
    return (deviceError) res;
}

deviceError caOS::IoCtrl(caDeviceHandle & port, caIDeviceCtrl & in) {
    u32 res = deviceError::no_error;
    s32 offset;
    if (caOS::GetDevice(port, offset)) {
        res = caHalDeviceRules::IoCtrl(allDevices[offset].device, &port, &in, allDevices[offset].mask);
    } else {
        res = deviceError::error_invalid_handle_port;
    }
    return (deviceError) res;
}
