#include "config.h"
#include "bcm2836.h"
#if CACHE_DEVICE

#include "idevice.h"
#include "stream.h"
#include "interrupt.h"
#include "systimer.h"
#include "cache.h"
#include "cachedevice.h"
#include "memaux.h"



u32 caCacheDevice::guid = (u32) ioCtrlRequest::Cache + BASE_HANDLE;
u32 caCacheDevice::isOpen = 0;

bool caCacheDevice::IsValidHandle(u32 handle) {
    bool res = false;
    if ((handle & ioCtrlRequest::maskIoCtrl) == ioCtrlRequest::Cache &&
            (handle & ioCtrlRequest::maskHandle) > BASE_HANDLE)
        res = true;
    return res;
}

u32 caCacheDevice::Open(caCacheDeviceConfigure *setup,
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

u32 caCacheDevice::Close(caDevicePort *port) {
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

u32 caCacheDevice::Write(caDevicePort *port) {
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

u32 caCacheDevice::Read(caDevicePort *port) {
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
            // INSERTHER FUNCTION READ
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

u32 caCacheDevice::IoCtrl(caDevicePort *port,
        caCacheDeviceCtrl *in) {
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
            case caCacheDeviceCtrl::IoCtrlDirect::cacheInvalidate:
                break;
            case caCacheDeviceCtrl::IoCtrlDirect::cacheStart:
                res = caCache::Start();
                break;
            case caCacheDeviceCtrl::IoCtrlDirect::cacheStop:
                res = caCache::Stop();
                break;
        }
    }
    port->tLastCmd = caDeviceAction::caActionIoCtrl;
    port->tLast = caSysTimer::GetCount();
    //TOUT();
    return res;
}

u32 caCacheDevice::IoctlReq(ioCtrlFunction request,
        u32 *p1, u32 *p2) {
    u32 res = deviceError::no_error;
    //TIN();
    switch (request) {
        case ioCtrlFunction::caOpenDevice:
            res = Open((caCacheDeviceConfigure *) p1, (caDevicePort *) p2);
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
            res = IoCtrl((caDevicePort *) p1, (caCacheDeviceCtrl *) p2);
            break;
        default:
            break;
    }
    //TOUT();
    return res;
}


#endif
