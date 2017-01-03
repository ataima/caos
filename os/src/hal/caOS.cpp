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
#include "bcm2836.h"

#include "miniuart.h"
#include "interrupt.h"
#include "systimer.h"
#include "sysirqctrl.h"
#include "memaux.h"

#include "thread.h"
#include "scheduler.h"
#include "schedulerdevice.h"
#include "memory.h"
#include "systimerdevice.h"
#include "caos.h"


#if COM1_DEVICE
caHalComDevice caOS::com1(&hal_ll_com1,ioCtrlRequest::Com1);
#endif

#if COM2_DEVICE
caHalComDevice caOS::com2(&hal_ll_com2,ioCtrlRequest::Com2);
#endif

#if COM3_DEVICE
caHalComDevice caOS::com3(&hal_ll_com3,ioCtrlRequest::Com3);
#endif

#if COM4_DEVICE
caHalComDevice caOS::com4(&hal_ll_com4,ioCtrlRequest::Com4);
#endif

#if COM5_DEVICE
caHalComDevice caOS::com5(&hal_ll_com5,ioCtrlRequest::Com5);
#endif

#if COM6_DEVICE
caHalComDevice caOS::com6(&hal_ll_com6,ioCtrlRequest::Com6);
#endif

#if COM7_DEVICE
caHalComDevice caOS::com7(&hal_ll_com7,ioCtrlRequest::Com7);
#endif

#if COM8_DEVICE
caHalComDevice caOS::com8(&hal_ll_com8,ioCtrlRequest::Com8);
#endif


// TO DO HAVE TO DESIGN A DEVICE MANAGER TO INSERT REMOVE DEVICES
// AT MOMENT A SIMPLE ARRAY ....


caOS::devicePair caOS::allDevices[] = {
    {"MEMORY", ioCtrlRequest::Memory, NULL},
    {"SCHEDULER", ioCtrlRequest::Scheduler, NULL},
    {"TASK", ioCtrlRequest::Task, NULL},
    {"MEMPIPE", ioCtrlRequest::MemPipe, NULL},
    {"SYSTIMER", ioCtrlRequest::SysTimer, NULL},
    {"CACHE", ioCtrlRequest::Cache, NULL},
#if COM1_DEVICE   
    {"TTY1", ioCtrlRequest::Com1, &com1},
#endif
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
    u32 mnDevices = (sizeof (allDevices) / sizeof (devicePair)) + 1;
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
