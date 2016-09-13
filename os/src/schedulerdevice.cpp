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

#if SCHEDULER_DEVICE

#include "idevice.h"
#include "stream.h"
#include "interrupt.h"
#include "systimer.h"
#include "thread.h"
#include "scheduler.h"
#include "schedulerdevice.h"
#include "memaux.h"



u32 caSchedulerDevice::guid = (u32) ioCtrlRequest::Scheduler + BASE_HANDLE;
u32 caSchedulerDevice::isOpen = 0;

bool caSchedulerDevice::IsValidHandle(u32 handle) {
    bool res = false;
    if ((handle & ioCtrlRequest::maskIoCtrl) == ioCtrlRequest::Scheduler &&
            (handle & ioCtrlRequest::maskHandle) > BASE_HANDLE)
        res = true;
    return res;
}

u32 caSchedulerDevice::Open(caSchedulerDeviceConfigure *setup,
        caDevicePort *port) {
    u32 res = deviceError::no_error;
    //TIN();
    if (setup != NULL) {
        isOpen++;
        if (port != NULL) {
            caMemAux::MemZero((u32*) port, sizeof (caDevicePort) / sizeof (u32));
            port->handle = ++guid;
            port->status = caDevicePort::statusPort::Open;
            port->tStart = caSysTimer::GetCount();
            port->tLast = port->tStart;
            port->tStop = 0;
            port->tLastCmd = caDeviceAction::caActionOpen;
        } else {
            res = deviceError::error_invalid_null_port;
        }
    }
    //TOUT();
    return res;
}

u32 caSchedulerDevice::Close(caDevicePort *port) {
    u32 res = deviceError::no_error;
    //TIN();
    if (port == NULL) {
        res = deviceError::error_invalid_null_port;
    } else
        if (isOpen == 0) {
        res = deviceError::error_device_not_opened;
    } else
        if (!IsValidHandle(port->handle)) {
        res = deviceError::error_invalid_handle_port;
    } else {
        isOpen--;
        port->status = caDevicePort::statusPort::Close;
        port->tStop = caSysTimer::GetCount();
        port->tLast = port->tStop;
        port->tLastCmd = caDeviceAction::caActionClose;
        port->error = 0;
    }
    //TOUT();
    return res;
}

u32 caSchedulerDevice::Write(caDevicePort *port) {
    u32 res = deviceError::no_error;
    //TIN();
    if (port == NULL) {
        res = deviceError::error_invalid_null_port;
    } else
        if (isOpen == 0) {
        res = deviceError::error_device_not_opened;
    } else
        if (!IsValidHandle(port->handle)) {
        res = deviceError::error_invalid_handle_port;
    } else {
        if (port->wrBuff != NULL && port->wrSize > 0) {
            u32 writed = 0;
            // INSERT HERE FUNCTION WRITE
            // 
            port->writed += writed;
            port->wrBuff += writed;
            port->wrSize -= writed;
            port->tLast = caSysTimer::GetCount();
            port->tLastCmd = caDeviceAction::caActionWrite;
            port->error = 0;
        } else {
            res = deviceError::error_invalid_handle_port;
        }
    }
    //TOUT();
    return res;
}

u32 caSchedulerDevice::Read(caDevicePort *port) {
    u32 res = deviceError::no_error;
    //TIN();
    if (port == NULL) {
        res = deviceError::error_invalid_null_port;
    } else
        if (isOpen == 0) {
        res = deviceError::error_device_not_opened;
    } else
        if (!IsValidHandle(port->handle)) {
        res = deviceError::error_invalid_handle_port;
    } else {
        if (port->rdBuff != NULL && port->rdSize > 0) {
            u32 pSize = 0;
            // INSERT HERE FUNCTION READ
            port->readed += pSize;
            port->rdBuff += pSize;
            port->rdSize -= pSize;
            port->tLast = caSysTimer::GetCount();
            port->tLastCmd = caDeviceAction::caActionRead;
            port->error = 0;
        } else {
            res = deviceError::error_invalid_handle_port;
        }
    }
    //TOUT();
    return res;

}

u32 caSchedulerDevice::IoCtrl(caDevicePort *port,
        caSchedulerDeviceCtrl *in) {
    u32 res = deviceError::no_error;
    //TIN();
    if (port == NULL) {
        res = deviceError::error_invalid_null_port;
    } else
        if (isOpen == 0) {
        res = deviceError::error_device_not_opened;
    } else
        if (!IsValidHandle(port->handle)) {
        res = deviceError::error_invalid_handle_port;
    } else {
        switch (in->command) {
            case caSchedulerDeviceCtrl::IoCtrlDirect::schGetThid:
                in->io = caScheduler::GetCurrentTaskId();
                break;
        }
    }
    port->tLastCmd = caDeviceAction::caActionIoCtrl;
    port->tLast = caSysTimer::GetCount();
    //TOUT();
    return res;
}

u32 caSchedulerDevice::IoctlReq(ioCtrlFunction request,
        u32 *p1, u32 *p2) {
    u32 res = deviceError::no_error;
    caStringStream<s8> ss;
    //TIN();
    switch (request) {
        case ioCtrlFunction::caOpenDevice:
            res = Open((caSchedulerDeviceConfigure *) p1, (caDevicePort *) p2);
            break;
        case ioCtrlFunction::caCloseDevice:
            res = Close((caDevicePort *) p1);
            break;
        case ioCtrlFunction::caWriteDevice:
            res = Write((caDevicePort *) p1);
            break;
        case ioCtrlFunction::caReadDevice:
            res = Read((caDevicePort *) p1);
            break;
        case ioCtrlFunction::caIoCtrlDevice:
            res = IoCtrl((caDevicePort *) p1, (caSchedulerDeviceCtrl *) p2);
            break;
        case ioCtrlFunction::caSchedulerListAll:
            ss.Init((s8*) p1, (u32) p2);
            ss << " --- TASK ---\r\n";
            res = caScheduler::Dump(ss);
            break;
        case ioCtrlFunction::caIoSleep:
            res = caScheduler::SetSleepMode(*p1, (u32) p2);
            break;
        default:
            break;
    }
    //TOUT();
    return res;
}


#endif
