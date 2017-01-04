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



#if SYS_TIMER_DEVICE


#include "systimerdevice.h"
#include "memaux.h"




u32 caSysTimerDevice::guid = (u32) ioCtrlRequest::SysTimer + BASE_HANDLE;
u32 caSysTimerDevice::isOpen = 0;

u32 caSysTimerDevice::Flush(caDeviceHandle *port) {
    u32 res = deviceError::no_error;
    if (port == NULL) {
        res = deviceError::error_invalid_null_port;
    } else
        if (isOpen == 0) {
        res = deviceError::error_device_not_opened;
    } else
        if (!IsValidHandle(port->handle)) {
        res = deviceError::error_invalid_handle_port;
    } else {
        //Rx.Clean();
        //Tx.Clean();
        port->tLast = hal_ll_time.hll_tick();
        port->wrError = port->rdError = 0;
    }
    return res;
}

bool caSysTimerDevice::IsValidHandle(u32 handle) {
    bool res = false;
    if ((handle & ioCtrlRequest::maskIoCtrl) == ioCtrlRequest::SysTimer &&
            (handle & ioCtrlRequest::maskHandle) > BASE_HANDLE)
        res = true;
    return res;
}

u32 caSysTimerDevice::Open(caIDeviceConfigure * setup,
        caDeviceHandle *port) {
    u32 res = deviceError::no_error;
    //TIN();
    if (setup != NULL) {
        if (isOpen == 0) {
            isOpen++;
            if (port != NULL) {
                caMemAux<u32>::MemSet((u32*) port, 0, sizeof (caDeviceHandle));
                port->handle = ++guid;
                port->status = caDeviceHandle::statusHandle::Open;
                port->tStart = hal_ll_time.hll_tick();
                port->tLast = port->tStart;
                port->tStop = 0;
                port->tLastCmd = caDeviceAction::caActionOpen;
            } else {
                res = deviceError::error_invalid_null_port;
            }
        } else {
            isOpen++;
        }
    } else
        if (setup == NULL)
        res = deviceError::error_device_config_param;
    else
        if (port == NULL)
        res = deviceError::error_invalid_null_port;

    return res;
}

u32 caSysTimerDevice::Close(caDeviceHandle *port) {
    u32 res = deviceError::no_error;
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
        port->status = caDeviceHandle::statusHandle::Close;
        port->tStop = hal_ll_time.hll_tick();
        port->tLast = port->tStop;
        port->tLastCmd = caDeviceAction::caActionClose;
        port->wrError = port->rdError = 0;
    }
    return res;
}

u32 caSysTimerDevice::Write(caDeviceHandle *port) {
    u32 res = deviceError::no_error;
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
            u32 * ptr = (u32*) port->wrBuff;
            u32 writed = 4 * sizeof (u32);
            res = hal_ll_time.hll_settime(ptr[0], ptr[1], ptr[2], ptr[3]);
            port->writed += writed;
            port->wrBuff += writed;
            port->wrSize -= writed;
            port->tLast = hal_ll_time.hll_tick();
            port->tLastCmd = caDeviceAction::caActionWrite;
            port->wrError = port->rdError = 0;
        } else {
            res = deviceError::error_invalid_handle_port;
        }
    }
    return res;
}

u32 caSysTimerDevice::Read(caDeviceHandle *port) {
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
            u32 *ptr=(u32 *)port->rdBuff;
            if(port->rdSize>=sizeof(u32))
            {
                *ptr++=hal_ll_time.hll_day();
                port->rdSize-=sizeof(u32);
                port->readed +=sizeof(u32);
            }
            if(port->rdSize>=sizeof(u32))
            {
                *ptr++=hal_ll_time.hll_hour();
                port->rdSize-=sizeof(u32);
                port->readed +=sizeof(u32);
            }
            if(port->rdSize>=sizeof(u32))
            {
                *ptr++=hal_ll_time.hll_min();
                port->rdSize-=sizeof(u32);
                port->readed +=sizeof(u32);
            }
            if(port->rdSize>=sizeof(u32))
            {
                *ptr++=hal_ll_time.hll_sec();
                port->rdSize-=sizeof(u32);
                port->readed +=sizeof(u32);
            }
            port->tLast = hal_ll_time.hll_tick();
            port->tLastCmd = caDeviceAction::caActionRead;
            port->wrError = port->rdError = 0;
        } else {
            res = deviceError::error_invalid_handle_port;
        }
    }
    //
    return res;

}

u32 caSysTimerDevice::IoCtrl(caDeviceHandle *port,
        caIDeviceCtrl *inp) {
    u32 res = deviceError::no_error;
    //TIN();
    if (port == NULL) {
        res = deviceError::error_invalid_null_port;
    } else
        if (inp == NULL) {
        res = deviceError::error_device_config_param;
    } else
        if (isOpen == 0) {
        res = deviceError::error_device_not_opened;
    } else
        if (!IsValidHandle(port->handle)) {
        res = deviceError::error_invalid_handle_port;
    } else {
        caSysTimerDeviceCtrl *in = static_cast<caSysTimerDeviceCtrl *> (inp);
        switch (in->command) {
            case caSysTimerDeviceCtrl::IoCtrlDirect::sysTimerFlush:
                res = Flush(port);
                break;
            case caSysTimerDeviceCtrl::IoCtrlDirect::sysTimerListHardware:
                if (in->ss != NULL) {
                    in->ss->Clear();                   
                    res = hal_ll_time.hll_dump(in->ss);
                } else
                    res = deviceError::error_invalid_null_destination;
                break;
        }
    }
    port->tLastCmd = caDeviceAction::caActionIoCtrl;
    //
    return res;
}

u32 caSysTimerDevice::IoctlReq(ioCtrlFunction request,
        u32 *p1, u32 *p2) {
    u32 res = deviceError::no_error;
    //TIN();
    switch (request) {
        case ioCtrlFunction::caOpenDevice:
            res = Open((caIDeviceConfigure *) p1, (caDeviceHandle *) p2);
            break;
        case ioCtrlFunction::caCloseDevice:
            res = Close((caDeviceHandle *) p1);
            break;
        case ioCtrlFunction::caWriteDevice:
            res = Write((caDeviceHandle *) p1);
            break;
        case ioCtrlFunction::caReadDevice:
            res = Read((caDeviceHandle *) p1);
            break;
        case ioCtrlFunction::caIoCtrlDevice:
            res = IoCtrl((caDeviceHandle *) p1, (caSysTimerDeviceCtrl *) p2);
            break;
        default:
            break;
    }
    //
    return res;
}


#endif