////////////////////////////////////////////////////////////////////////////////
//    Copyright (C) 2011  Angelo Coppi (angelogkcop at hotmail.com )
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
#include "bcm2836.h"
#include "idevice.h"
#include "stream.h"
#include "miniuart.h"
#include "interrupt.h"
#include "comdevice.h"
#include "systimer.h"
#include "sysirqctrl.h"
#include "memaux.h"
#include "softreq.h"

typedef struct device_pair {
    const char * name;
    ioCtrlRequest device;
} devicePair;

static devicePair allDevices[] = {
    {"MEMORY", ioCtrlRequest::Memory},
    {"SCHEDULER", ioCtrlRequest::Scheduler},
    {"EXCEPTION", ioCtrlRequest::Exception},
    {"TASK", ioCtrlRequest::Task},
    {"COM1", ioCtrlRequest::Com1},
    {"MEMPIPE", ioCtrlRequest::MemPipe},
    {"SYSTIMER", ioCtrlRequest::SysTimer},
    {"CACHE", ioCtrlRequest::Cache}
};

ioCtrlRequest caDevice::GetIoCtrlRequest(u32 handle) {
    u32 mask = ioCtrlRequest::maskIoCtrl;
    return (ioCtrlRequest) (handle & mask);
}

ioCtrlRequest caDevice::GetIoCtrlRequest(const char * name) {
    ioCtrlRequest res = ioCtrlRequest::no_ioctrl;
    u32 i;
    u32 mnDevices = (sizeof (allDevices) / sizeof (devicePair)) + 1;
    for (i = 0; i < mnDevices; i++) {
        if (caMemAux::StrICmp(allDevices[i].name, name) == 0) {
            res = allDevices[i].device;
            break;
        }
    }
    return res;
}

// return 0 OK else return caDevice::Error

deviceError caDevice::Open(const char * device,
        caIDeviceConfigure & in, caDevicePort & out) {
    //TIN();    
    u32 res = deviceError::no_error;
    if (device == NULL) {
        res = deviceError::error_unknow_device_name;
    } else {
        ioCtrlRequest req = GetIoCtrlRequest(device);
        switch (req) {
            case ioCtrlRequest::no_ioctrl:
                res = deviceError::error_unknow_device_name;
                break;
            case ioCtrlRequest::Memory:
            case ioCtrlRequest::Task:
            case ioCtrlRequest::Exception:
                break;
            case ioCtrlRequest::Scheduler:
                SchedulerOpenDevice(&in, &out, &res);
                break;
#if COM1_DEVICE                 
            case ioCtrlRequest::Com1:
                Com1OpenDevice(&in, &out, &res);
                break;
#endif                
#if MEM_PIPE_DEVICE                 
            case ioCtrlRequest::MemPipe:
                MemPipeOpenDevice(&in, &out, &res);
                break;
#endif      
#if SYS_TIMER_DEVICE                
            case ioCtrlRequest::SysTimer:
                SysTimerOpenDevice(&in, &out, &res);
                break;
#endif                
#if CACHE_DEVICE                
            case ioCtrlRequest::Cache:
                CacheOpenDevice(&in, &out, &res);
                break;
#endif                
            default:
                res = deviceError::error_unknow_device_ioctrl;
                break;
        }
    }
    //TOUT();
    return (deviceError) res;
}

bool caDevicePort::IsValidHandle(void) {
    ioCtrlRequest req;
    return caDevice::IsValidHandle(*this, req);
}

bool caDevice::IsValidHandle(caDevicePort & port, ioCtrlRequest & req) {
    bool res = false;
    req = GetIoCtrlRequest(port.handle);
    switch (req) {
        case ioCtrlRequest::Task:
        case ioCtrlRequest::Scheduler:
        case ioCtrlRequest::Memory:
        case ioCtrlRequest::Exception:
        case ioCtrlRequest::Com1:
        case ioCtrlRequest::MemPipe:
        case ioCtrlRequest::SysTimer:
        case ioCtrlRequest::Cache:
            res = ((port.handle & ioCtrlRequest::maskHandle) > BASE_HANDLE);
            break;
        default:
            break;
    }
    return res;
}

deviceError caDevice::Close(caDevicePort & port) {
    //TIN();
    u32 res = deviceError::no_error;
    ioCtrlRequest req;
    if (IsValidHandle(port, req)) {
        switch (req) {
            case 0:
                res = deviceError::error_unknow_device_name;
                break;
            case ioCtrlRequest::Task:
            case ioCtrlRequest::Memory:
            case ioCtrlRequest::Exception:
                break;
            case ioCtrlRequest::Scheduler:
                SchedulerCloseDevice(&port, &res);
                break;
#if COM1_DEVICE                 
            case ioCtrlRequest::Com1:
                Com1CloseDevice(&port, &res);
                break;
#endif
#if MEM_PIPE_DEVICE                 
            case ioCtrlRequest::MemPipe:
                MemPipeCloseDevice(&port, &res);
                break;
#endif
#if SYS_TIMER_DEVICE                 
            case ioCtrlRequest::SysTimer:
                SysTimerCloseDevice(&port, &res);
                break;
#endif
#if CACHE_DEVICE                 
            case ioCtrlRequest::Cache:
                CacheCloseDevice(&port, &res);
                break;
#endif
            default:
                res = deviceError::error_unknow_device_ioctrl;
                break;
        }
        //TOUT();
    } else
        res = deviceError::error_invalid_handle_port;
    return (deviceError) res;
}

deviceError caDevice::Write(caDevicePort & port) {
    //TIN();
    u32 res = deviceError::no_error;
    ioCtrlRequest req;
    if (IsValidHandle(port, req)) {
        switch (req) {
            case 0:
                res = deviceError::error_unknow_device_name;
                break;
            case ioCtrlRequest::Task:
            case ioCtrlRequest::Memory:
            case ioCtrlRequest::Exception:
                break;
            case ioCtrlRequest::Scheduler:
                SchedulerWriteDevice(&port, &res);
                break;
#if COM1_DEVICE                 
            case ioCtrlRequest::Com1:
                Com1WriteDevice(&port, &res);
                break;
#endif
#if MEM_PIPE_DEVICE                 
            case ioCtrlRequest::MemPipe:
                MemPipeWriteDevice(&port, &res);
                break;
#endif
#if SYS_TIMER_DEVICE                 
            case ioCtrlRequest::SysTimer:
                SysTimerWriteDevice(&port, &res);
                break;

#endif
#if CACHE_DEVICE                 
            case ioCtrlRequest::Cache:
                CacheWriteDevice(&port, &res);
                break;
#endif
            default:
                res = deviceError::error_unknow_device_ioctrl;
                break;
        }
        //TOUT();
    } else
        res = deviceError::error_invalid_handle_port;
    return (deviceError) res;
}

deviceError caDevice::Read(caDevicePort & port) {
    //TIN();
    u32 res = deviceError::no_error;
    ioCtrlRequest req;
    if (IsValidHandle(port, req)) {
        switch (req) {
            case 0:
                res = deviceError::error_unknow_device_name;
                break;
            case ioCtrlRequest::Task:
            case ioCtrlRequest::Memory:
            case ioCtrlRequest::Exception:
                break;
            case ioCtrlRequest::Scheduler:
                SchedulerReadDevice(&port, &res);
                break;
#if COM1_DEVICE                                 
            case ioCtrlRequest::Com1:
                Com1ReadDevice(&port, &res);
                break;
#endif                
#if MEM_PIPE_DEVICE                 
            case ioCtrlRequest::MemPipe:
                MemPipeReadDevice(&port, &res);
                break;
#endif
#if SYS_TIMER_DEVICE                 
            case ioCtrlRequest::SysTimer:
                SysTimerReadDevice(&port, &res);
                break;
#endif
#if CACHE_DEVICE                 
            case ioCtrlRequest::Cache:
                CacheReadDevice(&port, &res);
                break;
#endif                
            default:
                res = deviceError::error_unknow_device_ioctrl;
                break;
        }
        //TOUT();
    } else
        res = deviceError::error_invalid_handle_port;
    return (deviceError) res;

}

deviceError caDevice::IoCtrl(caDevicePort & port, caIDeviceCtrl & in) {
    //TIN();
    u32 res = deviceError::no_error;
    ioCtrlRequest req;
    if (IsValidHandle(port, req)) {
        switch (req) {
            case 0:
                res = deviceError::error_unknow_device_name;
                break;
            case ioCtrlRequest::Scheduler:
                SchedulerIoCtrlDevice(&port, &in, &res);
                break;
            case ioCtrlRequest::Task:
            case ioCtrlRequest::Memory:
            case ioCtrlRequest::Exception:
                break;
#if COM1_DEVICE                   
            case ioCtrlRequest::Com1:
                Com1IoCtrlDevice(&port, &in, &res);
                break;
#endif                
#if MEM_PIPE_DEVICE                   
            case ioCtrlRequest::MemPipe:
                MemPipeIoCtrlDevice(&port, &in, &res);
                break;
#endif                
#if SYS_TIMER_DEVICE                   
            case ioCtrlRequest::SysTimer:
                SysTimerIoCtrlDevice(&port, &in, &res);
                break;
#endif                
#if CACHE_DEVICE                   
            case ioCtrlRequest::Cache:
                CacheIoCtrlDevice(&port, &in, &res);
                break;
#endif                
            default:
                res = deviceError::error_unknow_device_ioctrl;
                break;
        }
        //TOUT();
    } else
        res = deviceError::error_invalid_handle_port;
    return (deviceError) res;
}

void DumpDevicePort(caDevicePort & port, caStringStream<s8> & ss) {
    ss << caStringFormat::hex;
    ss << "HANDLE    =" << port.handle << caEnd::endl;
    ss << "STATUS    =" << port.status << caEnd::endl;
    ss << "ERROR     =" << port.error << caEnd::endl;
    ss << "TIME START=" << port.tStart << caEnd::endl;
    ss << "TIME STOP =" << port.tStop << caEnd::endl;
    ss << "TIME LAST =" << port.tLast << caEnd::endl;
    ss << "LAST CMD  =" << port.tLastCmd << caEnd::endl;
    ss << "WR BUFF   =" << (u32) port.wrBuff << caEnd::endl;
    ss << "WR SIZE   =" << port.wrSize << caEnd::endl;
    ss << "WRITED    =" << port.writed << caEnd::endl;
    ss << "RD BUFF   =" << (u32) port.rdBuff << caEnd::endl;
    ss << "RD SIZE   =" << port.rdSize << caEnd::endl;
    ss << "READED    =" << port.readed << caEnd::endl;
    ss.Str();
}