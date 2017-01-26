
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
#include "halcomdevice.h"

caHalComDevice::caHalComDevice(hal_llc_com_io *com, u32 mask) {
    isOpen = 0;
    signalRx = signalTx = 0;
    link = com;
    handle_guid = BASE_HANDLE;
    mask_guid = (mask & ioCtrlRequest::maskIoCtrl);
    link->hll_lnk_obj = (void *) this;
}

u32 caHalComDevice::Open(caIDeviceConfigure * in,
        caDeviceHandle *port) {
    u32 res = deviceError::no_error;
    LOG(caLog, device) << " in : isOpen = " << isOpen << caEnd::endl;
    caComDeviceConfigure *setup = static_cast<caComDeviceConfigure *> (in);
    res = link->hll_config(setup->speed, setup->stop, setup->parity, setup->data);
    if (res == deviceError::no_error) {
        Rx.Init(RxBuffer, QUEUESIZE);
        Tx.Init(TxBuffer, QUEUESIZE);
        isOpen++;
        signalTx = signalRx = 0;
        port->handle = caHalDeviceRules::addHandle(handle_guid, mask_guid);
        port->status = caDeviceHandle::statusHandle::Open;
        port->tStart = link->hll_tick();
        port->tLast = port->tStart;
        port->tStop = 0;
        port->tLastCmd = caDeviceAction::caActionOpen;
        link->hll_get_errors(port->rdError, port->wrError);
        res = link->hll_en_int_rx();
        LOG(caLog, device) << " handle = " << port->handle << caEnd::endl;
    } else {
        res = deviceError::error_configure_serial_port;
        LOG(caLog, error) << " deviceError::error_configure_serial_port" << caEnd::endl;
    }
    LOG(caLog, device) << " out : res = " << res << caEnd::endl;
    return res;
}

u32 caHalComDevice::Close(caDeviceHandle *port) {
    u32 res = deviceError::no_error;
    LOG(caLog, device) << " in : isOpen = " << isOpen << caEnd::endl;
    isOpen = 0;
    port->status = caDeviceHandle::statusHandle::Close;
    port->tStop = link->hll_tick();
    port->tLast = port->tStop;
    port->tLastCmd = caDeviceAction::caActionClose;
    link->hll_get_errors(port->rdError, port->wrError);
    port->handle = mask_guid | BASE_HANDLE;
    res = link->hll_stop();
    link->hll_lnk_obj = NULL;
    LOG(caLog, device) << " out : res = " << res << caEnd::endl;
    return res;
}

u32 caHalComDevice::Write(caDeviceHandle *port) {
    u32 res = deviceError::no_error;
    LOG(caLog, device) << " in : isOpen = " << isOpen << caEnd::endl;
    if (port->wrSize != 0) {
        if (TxLock.Get() != 0) {
            res = deviceError::error_device_is_busy;
            LOG(caLog, error) << " deviceError::error_device_is_busy" << caEnd::endl;
        } else {
            u32 wSize, writed = 0;
            u32 maxWr = Tx.Available();
            if (maxWr > 0) {
                if (maxWr > port->wrSize)
                    wSize = port->wrSize;
                else
                    wSize = maxWr;
                if (TxLock.Lock()) {
                    if (!Tx.Push(port->wrBuff, wSize, writed)) {
                        res = deviceError::error_write_less_data;
                        LOG(caLog, error) << " deviceError::error_write_less_data" << caEnd::endl;
                    }
                    TxLock.UnLock();
                } else {
                    res = deviceError::error_device_is_busy;
                    LOG(caLog, error) << " deviceError::error_device_is_busy" << caEnd::endl;
                }
                if (res == deviceError::no_error) {
                    port->writed += writed;
                    port->wrBuff += writed;
                    port->wrSize -= writed;
                    port->tLast = link->hll_tick();
                    port->tLastCmd = caDeviceAction::caActionWrite;
                    link->hll_get_errors(port->rdError, port->wrError);
                    res = link->hll_start_tx();
                }
            }
        }
    } else {
        res = deviceError::error_invalid_handle_port_wr_size;
        LOG(caLog, error) << " deviceError::error_invalid_handle_port_wr_size:"
                << port->wrSize << caEnd::endl;
    }
    LOG(caLog, device) << " out : res = " << res << caEnd::endl;
    return res;
}

u32 caHalComDevice::Read(caDeviceHandle *port) {
    u32 res = deviceError::no_error;
    LOG(caLog, device) << " in : isOpen = " << isOpen << caEnd::endl;
    if (port->rdSize != 0) {
        if (RxLock.Get() != 0) {
            res = deviceError::error_device_is_busy;
            LOG(caLog, error) << " deviceError::error_device_is_busy" << caEnd::endl;
        } else {
            u32 rSize, pSize = 0;
            if (Rx.Empty() == false) {
                if (Rx.Size() < port->rdSize)
                    rSize = Rx.Size();
                else
                    rSize = port->rdSize;
                if (RxLock.Lock()) {
                    if (!Rx.Pop(port->rdBuff, rSize, pSize)) {
                        res = deviceError::error_read_less_data;
                        LOG(caLog, error) << " deviceError::error_read_less_data" << caEnd::endl;
                    }
                    RxLock.UnLock();
                } else {
                    res = deviceError::error_device_is_busy;
                    LOG(caLog, error) << " deviceError::error_device_is_busy" << caEnd::endl;
                }
                if (res == deviceError::no_error) {
                    port->readed += pSize;
                    port->rdBuff += pSize;
                    port->rdSize -= pSize;
                    port->tLast = link->hll_tick();
                    port->tLastCmd = caDeviceAction::caActionRead;
                    link->hll_get_errors(port->rdError, port->wrError);
                }
            } else {
                res = deviceError::error_rx_queue_empty;
                LOG(caLog, error) << " deviceError::error_rx_queue_empty" << caEnd::endl;
            }
        }
    } else {
        res = deviceError::error_invalid_handle_port_rd_size;
        LOG(caLog, error) << " deviceError::error_invalid_handle_port_rd_size:"
                << port->rdSize << caEnd::endl;
    }
    LOG(caLog, device) << " out : res = " << res << caEnd::endl;
    return res;
}

u32 caHalComDevice::Flush(caDeviceHandle *port) {
    u32 res = deviceError::no_error;
    port->tLast = link->hll_tick();
    port->wrError = port->rdError = 0;
    port->tLastCmd = caDeviceAction::caActionFlush;
    Rx.Clean();
    Tx.Clean();
    return res;
}

u32 caHalComDevice::IoCtrl(caDeviceHandle *port, caIDeviceCtrl *inp) {
    u32 res = deviceError::no_error;
    port->tLast = link->hll_tick();
    link->hll_get_errors(port->rdError, port->wrError);
    LOG(caLog, device) << " in : isOpen = " << isOpen << caEnd::endl;
    caComDeviceCtrl *in = static_cast<caComDeviceCtrl *> (inp);
    switch (in->command) {
        default:
            LOG(caLog, error) << " deviceError::error_ioctrl_command_error" << caEnd::endl;
            res = deviceError::error_ioctrl_command_error;
            break;
        case caComDeviceCtrl::IoCtrlDirect::comFlush:
            LOG(caLog, info) << " caComDeviceCtrl::IoCtrlDirect::comFlush" << caEnd::endl;
            res = Flush(port);
            break;
        case caComDeviceCtrl::IoCtrlDirect::comStart:
            LOG(caLog, info) << " caComDeviceCtrl::IoCtrlDirect::comStart" << caEnd::endl;
            res = link->hll_enable(1, 1);
            break;
        case caComDeviceCtrl::IoCtrlDirect::comStop:
            LOG(caLog, info) << " caComDeviceCtrl::IoCtrlDirect::comStop" << caEnd::endl;
            res = link->hll_enable(0, 0);
            break;
        case caComDeviceCtrl::IoCtrlDirect::comListHardware:
            LOG(caLog, info) << " caComDeviceCtrl::IoCtrlDirect::comListHardware" << caEnd::endl;
            if (in->ss != NULL) {
                in->ss->Clear();
                res = link->hll_dump(in->ss);
            } else {
                res = deviceError::error_invalid_null_destination;
                LOG(caLog, error) << " deviceError::error_invalid_null_destination" << caEnd::endl;
            }
            break;
        case caComDeviceCtrl::IoCtrlDirect::comStatusBuffer:
            LOG(caLog, info) << " caComDeviceCtrl::IoCtrlDirect::comStatusBuffer" << caEnd::endl;
            in->param_1 = Rx.Size();
            in->param_2 = Tx.Size();
            break;
        case caComDeviceCtrl::IoCtrlDirect::comAddSignalRx:
            LOG(caLog, info) << " caComDeviceCtrl::IoCtrlDirect::comAddSignalRx" << caEnd::endl;
            if (hal_llc_scheduler.hll_scheduler_valid_handle(signalRx)) {
                res = deviceError::error_signal_already_set;
                LOG(caLog, error) << " deviceError::error_signal_already_set" << caEnd::endl;
            } else {
                if (hal_llc_scheduler.hll_scheduler_valid_handle(in->param_1)) {
                    signalRx = in->param_1;
                } else {
                    res = deviceError::error_invalid_handle_port;
                    LOG(caLog, error) << " deviceError::error_invalid_handle_port" << caEnd::endl;
                }
            }
            break;
        case caComDeviceCtrl::IoCtrlDirect::comAddSignalTx:
            LOG(caLog, info) << " caComDeviceCtrl::IoCtrlDirect::comAddSignalTx" << caEnd::endl;
            if (hal_llc_scheduler.hll_scheduler_valid_handle(signalTx)) {
                res = deviceError::error_signal_already_set;
                LOG(caLog, error) << " deviceError::error_signal_already_set" << caEnd::endl;
            } else {
                if (hal_llc_scheduler.hll_scheduler_valid_handle(in->param_2)) {
                    signalTx = in->param_2;
                } else {
                    res = deviceError::error_invalid_handle_port;
                    LOG(caLog, error) << " deviceError::error_invalid_handle_port" << caEnd::endl;
                }
            }
            break;
        case caComDeviceCtrl::IoCtrlDirect::comRemoveSignalRx:
            LOG(caLog, info) << " : caComDeviceCtrl::IoCtrlDirect::comRemoveSignalRx" << caEnd::endl;
            if (hal_llc_scheduler.hll_scheduler_valid_handle(signalRx)) {
                signalRx = 0;
            } else {
                res = deviceError::error_invalid_handle_port;
                LOG(caLog, error) << " deviceError::error_invalid_handle_port" << caEnd::endl;
            }
            break;
        case caComDeviceCtrl::IoCtrlDirect::comRemoveSignalTx:
            LOG(caLog, info) << " caComDeviceCtrl::IoCtrlDirect::comRemoveSignalTx" << caEnd::endl;
            if (hal_llc_scheduler.hll_scheduler_valid_handle(signalTx)) {
                signalTx = 0;
            } else {
                res = deviceError::error_invalid_handle_port;
                LOG(caLog, error) << " deviceError::error_invalid_handle_port" << caEnd::endl;
            }
            break;
        case caComDeviceCtrl::IoCtrlDirect::comGetSignalRx:
            LOG(caLog, info) << " caComDeviceCtrl::IoCtrlDirect::comGetSignalRx" << caEnd::endl;
            in->param_1 = signalRx;
            break;
        case caComDeviceCtrl::IoCtrlDirect::comGetSignalTx:
            LOG(caLog, info) << " caComDeviceCtrl::IoCtrlDirect::comGetSignalTx" << caEnd::endl;
            in->param_1 = signalTx;
            break;
        case caComDeviceCtrl::IoCtrlDirect::comLogCreate:
            if (!caLog.IsValid()) {
                if (caLog.Init(in->param_1, (deviceloglevels) in->param_2) != TRUE)
                    res = deviceError::error_cannot_create_log;
            } else {
                res = deviceError::error_log_already_set;
            }
            break;
        case caComDeviceCtrl::IoCtrlDirect::comLogDestroy:
            if (caLog.IsValid()) {
                if (caLog.Destroy() != TRUE)
                    res = deviceError::error_cannot_destroy_log;
            } else {
                res = deviceError::error_log_not_set;
            }
            break;
        case caComDeviceCtrl::IoCtrlDirect::comLogStart:
            if (caLog.IsValid()) {
                caLog.Enable();
            } else {
                res = deviceError::error_log_already_set;
            }
            break;
        case caComDeviceCtrl::IoCtrlDirect::comLogStop:
            if (caLog.IsValid()) {
                caLog.Disable();
            } else {
                res = deviceError::error_log_not_set;
            }
            break;
        case caComDeviceCtrl::IoCtrlDirect::comLogGet:
            if (caLog.IsValid()) {
                if (in->ss != NULL) {
                    caLog.Stream((deviceloglevels) in->param_1).Str(*in->ss);
                } else
                    res = deviceError::error_invalid_null_destination;
            } else {
                res = deviceError::error_log_not_set;
            }
            break;
    }
    port->tLastCmd = caDeviceAction::caActionIoCtrl;
    LOG(caLog, device) << " out : res = " << res << caEnd::endl;
    return res;
}

u32 caHalComDevice::IoctlReq(ioCtrlFunction request, u32 *p1, u32 *p2) {
    u32 res = deviceError::no_error;
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

u32 caHalComDevice::IrqService1(u8 * txbuff, s_t size, s_t & writed) {
    LOG(caLog, irq_1) << " IrqService1 S=" << size << caEnd::endl;
    Tx.Pop(txbuff, size, writed);
    if (signalTx != 0)
        link->hll_wakeup_1(signalTx);
    return size - writed;
}

u32 caHalComDevice::IrqService2(u8 * rxbuff, s_t size, s_t & readed) {
    LOG(caLog, irq_2) << " IrqService2 S=" << size << caEnd::endl;
    Rx.Push(rxbuff, size, readed);
    if (signalRx != 0)
        link->hll_wakeup_2(signalRx);
    return size - readed;
}

u32 caHalComDevice::IrqService3(u8 *, s_t, s_t &) {
    LOG(caLog, irq_3) << " IrqService3 " << caEnd::endl;
    return 1;
}

u32 caHalComDevice::IrqService4(u8 *, s_t, s_t &) {
    LOG(caLog, irq_4) << " IrqService4 " << caEnd::endl;
    return 1;
}

u32 caHalComDevice::IrqService5(u8 *, s_t, s_t &) {
    LOG(caLog, irq_5) << " IrqService5 " << caEnd::endl;
    return 1;
}

u32 caHalComDevice::IrqService6(u8 *, s_t, s_t &) {
    LOG(caLog, irq_6) << " IrqService6 " << caEnd::endl;
    return 1;
}

u32 caHalComDevice::IrqService7(u8 *, s_t, s_t &) {
    LOG(caLog, irq_7) << " IrqService7 " << caEnd::endl;
    return 1;
}

u32 caHalComDevice::IrqService8(u8 *, s_t, s_t &) {
    LOG(caLog, irq_8) << " IrqService8 " << caEnd::endl;
    return 1;
}
