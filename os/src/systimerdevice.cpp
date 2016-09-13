#include "config.h"
#include "bcm2836.h"
#if SYS_TIMER_DEVICE

#include "idevice.h"
#include "stream.h"
#include "interrupt.h"
#include "systimer.h"
#include "systimerdevice.h"
#include "memaux.h"
#include "miniuart.h"



u32 caSysTimerDevice::guid = (u32) ioCtrlRequest::SysTimer + BASE_HANDLE;
u32 caSysTimerDevice::isOpen = 0;

u32 caSysTimerDevice::Flush(caDevicePort *port) {
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
        //Rx.Clean();
        //Tx.Clean();
        port->tLast = caSysTimer::GetCount();
        port->error = 0;
    }
    //TOUT();
    return res;
}

bool caSysTimerDevice::IsValidHandle(u32 handle) {
    bool res = false;
    if ((handle & ioCtrlRequest::maskIoCtrl) == ioCtrlRequest::SysTimer &&
            (handle & ioCtrlRequest::maskHandle) > BASE_HANDLE)
        res = true;
    return res;
}

u32 caSysTimerDevice::Open(caSysTimerDeviceConfigure *setup,
        caDevicePort *port) {
    u32 res = deviceError::no_error;
    //TIN();
    if (setup != NULL && isOpen == 0) {
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
    } else {
        isOpen++;
    }
    TOUT();
    return res;
}

u32 caSysTimerDevice::Close(caDevicePort *port) {
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

u32 caSysTimerDevice::Write(caDevicePort *port) {
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
            sysTimerSet *req = (sysTimerSet*) port->wrBuff;
            u32 writed = sizeof (sysTimerSet);
            caSysTimer::SetTime(req);
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

u32 caSysTimerDevice::Read(caDevicePort *port) {
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
            u32 pSize;
            if (port->rdSize >= sizeof (sysTimerStatus))
                pSize = sizeof (sysTimerStatus);
            else
                pSize = port->rdSize;
            caSysTimer::GetStatus((sysTimerStatus *) port->rdBuff, pSize);
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

u32 caSysTimerDevice::IoCtrl(caDevicePort *port,
        caSysTimerDeviceCtrl *in) {
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
            case caSysTimerDeviceCtrl::IoCtrlDirect::sysTimerFlush:
                res = Flush(port);
                break;
            case caSysTimerDeviceCtrl::IoCtrlDirect::sysTimerListHardware:
                if (in->ss != NULL) {
                    in->ss->Clear();
                    *in->ss << " --- SYS TIMER LIST ---" << caEnd::endl;
                    res = caSysTimer::Dump(*in->ss);
                } else
                    res = deviceError::error_invalid_null_destination;
                break;
        }
    }
    port->tLastCmd = caDeviceAction::caActionIoCtrl;
    //TOUT();
    return res;
}

u32 caSysTimerDevice::IoctlReq(ioCtrlFunction request,
        u32 *p1, u32 *p2) {
    u32 res = deviceError::no_error;
    //TIN();
    switch (request) {
        case ioCtrlFunction::caOpenDevice:
            res = Open((caSysTimerDeviceConfigure *) p1, (caDevicePort *) p2);
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
            res = IoCtrl((caDevicePort *) p1, (caSysTimerDeviceCtrl *) p2);
            break;
        default:
            break;
    }
    //TOUT();
    return res;
}


#endif