
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

#include "halcomdevice.h"
#include "comdevice.h"
#include "thread.h"
#include "scheduler.h"

u32 caHalComDevice::guid = (u32) ioCtrlRequest::Com1 + BASE_HANDLE;

caHalComDevice::caHalComDevice(const hal_ll_com_io *com) {
    isOpen = eOverrun = 0;
    signalRx = signalTx = 0;
    link = com;
}

u32 caHalComDevice::addHandle(void) {
    u32 handle = (ioCtrlRequest::maskHandle & guid);
    handle++;
    guid = (u32) (ioCtrlRequest)::Com1 + handle;
    return guid;
}

u32 caHalComDevice::Open(caIDeviceConfigure * in,
        caDeviceHandle *port) {
    u32 res = deviceError::no_error;
    if (isOpen == 0) {
        caComDeviceConfigure *setup = static_cast<caComDeviceConfigure *> (in);
        res = link->hll_config(setup);
        if (res == deviceError::no_error) {
            Rx.Init(RxBuffer, QUEUESIZE);
            Tx.Init(TxBuffer, QUEUESIZE);
            isOpen++;
            signalTx = signalRx = eOverrun = 0;
            port->handle = addHandle();
            ;
            port->status = caDeviceHandle::statusHandle::Open;
            port->tStart = link->hll_time();
            port->tStop = 0;
            port->tLastCmd = caDeviceAction::caActionOpen;
            res = link->hll_en_int();
        } else {
            res = deviceError::error_configure_serial_port;
        }
    }
    return res;
}

u32 caHalComDevice::Close(caDeviceHandle *port) {
    u32 res = deviceError::no_error;
    isOpen = 0;
    // TO DO UNLINK HARDAWARE ISR
    port->status = caDeviceHandle::statusHandle::Close;
    port->tStop = link->hll_time();
    port->tLast = port->tStop;
    port->tLastCmd = caDeviceAction::caActionClose;
    port->error = eOverrun;
    res = link->hll_stop();
    return res;
}

u32 caHalComDevice::Write(caDeviceHandle *port) {
    u32 res = deviceError::no_error;
    if (port->wrSize > 0) {
        if (TxLock.Get() != 0) {
            res = deviceError::error_device_is_busy;
        } else {
            u32 wSize, writed = 0;
            u32 maxWr = Tx.Available();
            if (maxWr > 0) {
                if (maxWr > port->wrSize)
                    wSize = port->wrSize;
                else
                    wSize = maxWr;
                if (TxLock.Lock()) {
                    if (!Tx.Push(port->wrBuff, wSize, writed))
                        res = deviceError::error_write_less_data;
                    TxLock.UnLock();
                } else {
                    res = deviceError::error_device_is_busy;
                }
                if (res == deviceError::no_error) {
                    port->writed += writed;
                    port->wrBuff += writed;
                    port->wrSize -= writed;
                    port->tLast = link->hll_time();
                    port->tLastCmd = caDeviceAction::caActionWrite;
                    port->error = eOverrun;
                    res = link->hll_start_tx();
                }
            }
        }
    } else {
        res = deviceError::error_invalid_handle_port;
    }
    return res;
}

u32 caHalComDevice::Read(caDeviceHandle *port) {
    u32 res = deviceError::no_error;
    if (port->rdSize > 0) {
        if (RxLock.Get() != 0) {
            res = deviceError::error_device_is_busy;
        } else {
            u32 rSize, pSize = 0;
            if (Rx.Empty() == false) {
                if (Rx.Size() < port->rdSize)
                    rSize = Rx.Size();
                else
                    rSize = port->rdSize;
                if (RxLock.Lock()) {
                    if (!Rx.Pop(port->rdBuff, rSize, pSize))
                        res = deviceError::error_read_less_data;
                    RxLock.UnLock();
                } else {
                    res = deviceError::error_device_is_busy;
                }
                if (res == deviceError::no_error) {
                    port->readed += pSize;
                    port->rdBuff += pSize;
                    port->rdSize -= pSize;
                    port->tLast = link->hll_time();
                    port->tLastCmd = caDeviceAction::caActionRead;
                    port->error = eOverrun;
                }
            } else {
                res = deviceError::error_rx_queue_empty;
            }
        }
    } else {
        res = deviceError::error_invalid_handle_port;
    }
    return res;
}

u32 caHalComDevice::Flush(caDeviceHandle *port) {
    u32 res = deviceError::no_error;
    port->tLast = link->hll_time();
    port->error = eOverrun;
    port->tLastCmd = caDeviceAction::caActionFlush;
    Rx.Clean();
    Tx.Clean();
    return res;
}

u32 caHalComDevice::IoCtrl(caDeviceHandle *port, caIDeviceCtrl *inp) {
    u32 res = deviceError::no_error;
    port->tLast = link->hll_time();
    port->error = eOverrun;
    caComDeviceCtrl *in = static_cast<caComDeviceCtrl *> (inp);
    switch (in->command) {
        case caComDeviceCtrl::IoCtrlDirect::comFlush:
            res = Flush(port);
            break;
        case caComDeviceCtrl::IoCtrlDirect::comStart:
            res = link->hll_enable(1, 1);
            break;
        case caComDeviceCtrl::IoCtrlDirect::comStop:
            res = link->hll_enable(0, 0);
            break;
        case caComDeviceCtrl::IoCtrlDirect::comListHardware:
            if (in->ss != NULL) {
                in->ss->Clear();
                *in->ss << " --- COM1 LIST ---" << caEnd::endl;
                res = link->hll_dump(in->ss);
            } else
                res = deviceError::error_invalid_null_destination;
            break;
        case caComDeviceCtrl::IoCtrlDirect::comStatusBuffer:
            in->st_rx = Rx.Size();
            in->st_tx = Tx.Size();
            break;
        case caComDeviceCtrl::IoCtrlDirect::comAddSignalRx:
            if (hal_ll_scheduler.hll_scheduler_valid_handle(signalRx)) {
                res = deviceError::error_signal_already_set;
            } else {
                if (hal_ll_scheduler.hll_scheduler_valid_handle(in->st_rx)) {
                    signalRx = in->st_rx;
                } else
                    res = deviceError::error_invalid_handle_port;
            }
            break;
        case caComDeviceCtrl::IoCtrlDirect::comAddSignalTx:
            if (hal_ll_scheduler.hll_scheduler_valid_handle(signalTx)) {
                res = deviceError::error_signal_already_set;
            } else {
                if (hal_ll_scheduler.hll_scheduler_valid_handle(in->st_tx)) {
                    signalTx = in->st_tx;
                } else
                    res = deviceError::error_invalid_handle_port;
            }
            break;
        case caComDeviceCtrl::IoCtrlDirect::comRemoveSignalRx:
            if (hal_ll_scheduler.hll_scheduler_valid_handle(signalRx)) {
                signalRx = 0;
            } else {
                res = deviceError::error_invalid_handle_port;
            }
            break;
        case caComDeviceCtrl::IoCtrlDirect::comRemoveSignalTx:
            if (hal_ll_scheduler.hll_scheduler_valid_handle(signalTx)) {
                signalTx = 0;
            } else {
                res = deviceError::error_invalid_handle_port;
            }
            break;
        case caComDeviceCtrl::IoCtrlDirect::comStartLog:
            if (caLog.isEnabled() == 0) {
                caLog.Enable();
                res = caLog.Init();
            } else {
                res = deviceError::error_log_already_set;
            }
            break;
        case caComDeviceCtrl::IoCtrlDirect::comStopLog:
            if (caLog.isEnabled() != 0) {
                caLog.Disable();
                res = caLog.Destroy();
            } else {
                res = deviceError::error_log_not_set;
            }
            break;
        case caComDeviceCtrl::IoCtrlDirect::comGetLog:
            if (caLog.isEnabled() != 0) {

            } else {
                res = deviceError::error_log_empthy;
            }
            break;
    }
    port->tLastCmd = caDeviceAction::caActionIoCtrl;
    return res;
}

u32 caHalComDevice::IoctlReq(ioCtrlFunction request, u32 *p1, u32 *p2) {
    u32 res = deviceError::no_error;
    ;
    switch (request) {
        case ioCtrlFunction::caOpenDevice:
            res = Open((caComDeviceConfigure *) p1, (caDeviceHandle *) p2);
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
            res = IoCtrl((caDeviceHandle *) p1, (caComDeviceCtrl *) p2);
            break;
        default:
            break;
    }
    return res;
}

