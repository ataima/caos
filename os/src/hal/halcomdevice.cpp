
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
    IOpen=caHalComDevice::Open;
    IClose=caHalComDevice::Close;
    IWrite=caHalComDevice::Write;
    IRead=caHalComDevice::Read;
    IFlush=caHalComDevice::Flush;
    IIoCtrl=caHalComDevice::IoCtrl;
    IGetOpenFlag=caHalComDevice::GetOpenFlag;
    IGetDeviceLog=caHalComDevice::GetDeviceLog;
    ItoString=caHalComDevice::toString;
    IIrqService1=caHalComDevice::IrqService1;
    IIrqService2=caHalComDevice::IrqService2;
    IIrqService3=caHalComDevice::IrqService3;
    IIrqService4=caHalComDevice::IrqService4;
    IIrqService5=caHalComDevice::IrqService5;
    IIrqService6=caHalComDevice::IrqService6;
    IIrqService7=caHalComDevice::IrqService7;
    IIrqService8=caHalComDevice::IrqService8;
}

u32 caHalComDevice::Open(IDevice * instance,caIDeviceConfigure * in,
        caDeviceHandle *port) {
    u32 res = deviceError::no_error;
    caHalComDevice* dev=static_cast<caHalComDevice*>(instance);
    caSysLog *pLog=dev->GetDeviceLog(dev);
    PLOG(pLog, device) << " in : isOpen = " << dev->isOpen << caEnd::endl;
    caComDeviceConfigure *setup = static_cast<caComDeviceConfigure *> (in);
    res = dev->link->hll_config(setup->speed, setup->stop, setup->parity, setup->data);
    if (res == deviceError::no_error) {
        dev->Rx.Init(dev->RxBuffer, QUEUESIZE);
        dev->Tx.Init(dev->TxBuffer, QUEUESIZE);
        dev->isOpen++;
        dev->signalTx = dev->signalRx = 0;
        port->handle = caHalDeviceRules::addHandle(dev->handle_guid, dev->mask_guid);
        port->status = caDeviceHandle::statusHandle::Open;
        port->tStart = dev->link->hll_tick();
        port->tLast = port->tStart;
        port->tStop = 0;
        port->tLastCmd = caDeviceAction::caActionOpen;
        dev->link->hll_get_errors(port->rdError, port->wrError);
        res = dev->link->hll_en_int_rx();
        PLOG(pLog, device) << " handle = " << port->handle << caEnd::endl;
    } else {
        res = deviceError::error_configure_serial_port;
        PLOG(pLog, error) << " deviceError::error_configure_serial_port" << caEnd::endl;
    }
    PLOG(pLog, device) << " out : res = " << res << caEnd::endl;
    return res;
}

u32 caHalComDevice::Close(IDevice * instance,caDeviceHandle *port) {
    u32 res = deviceError::no_error;
    caHalComDevice* dev=static_cast<caHalComDevice*>(instance);   
    caSysLog *pLog=dev->GetDeviceLog(dev);
    PLOG(pLog, device) << " in : isOpen = " << dev->isOpen << caEnd::endl;
    dev->isOpen = 0;
    port->status = caDeviceHandle::statusHandle::Close;
    port->tStop = dev->link->hll_tick();
    port->tLast = port->tStop;
    port->tLastCmd = caDeviceAction::caActionClose;
    dev->link->hll_get_errors(port->rdError, port->wrError);
    port->handle = dev->mask_guid | BASE_HANDLE;
    res = dev->link->hll_stop();
    dev->link->hll_lnk_obj = nullptr;
    PLOG(pLog, device) << " out : res = " << res << caEnd::endl;
    return res;
}

u32 caHalComDevice::Write(IDevice * instance,caDeviceHandle *port) {
    u32 res = deviceError::no_error;
    caHalComDevice* dev=static_cast<caHalComDevice*>(instance);  
    caSysLog *pLog=dev->GetDeviceLog(dev);
    PLOG(pLog, device) << " in : isOpen = " << dev->isOpen << caEnd::endl;
    if (port->wrSize != 0) {
        if (dev->TxLock.Get() != 0) {
            res = deviceError::error_device_is_busy;
            PLOG(pLog, error) << " deviceError::error_device_is_busy" << caEnd::endl;
        } else {
            u32 wSize, writed = 0;
            u32 maxWr = dev->Tx.Available();
            if (maxWr > 0) {
                if (maxWr > port->wrSize)
                    wSize = port->wrSize;
                else
                    wSize = maxWr;
                if (dev->TxLock.Lock()) {
                    if (!dev->Tx.Push(port->wrBuff, wSize, writed)) {
                        res = deviceError::error_write_less_data;
                        PLOG(pLog, error) << " deviceError::error_write_less_data" << caEnd::endl;
                    }
                    dev->TxLock.UnLock();
                } else {
                    res = deviceError::error_device_is_busy;
                    PLOG(pLog, error) << " deviceError::error_device_is_busy" << caEnd::endl;
                }
                if (res == deviceError::no_error) {
                    port->writed += writed;
                    port->wrBuff += writed;
                    port->wrSize -= writed;
                    port->tLast = dev->link->hll_tick();
                    port->tLastCmd = caDeviceAction::caActionWrite;
                    dev->link->hll_get_errors(port->rdError, port->wrError);
                    res = dev->link->hll_start_tx();
                }
            }
        }
    } else {
        res = deviceError::error_invalid_handle_port_wr_size;
        PLOG(pLog, error) << " deviceError::error_invalid_handle_port_wr_size:"
                << port->wrSize << caEnd::endl;
    }
    PLOG(pLog, device) << " out : res = " << res << caEnd::endl;
    return res;
}

u32 caHalComDevice::Read(IDevice * instance,caDeviceHandle *port) {
    u32 res = deviceError::no_error;
    caHalComDevice* dev=static_cast<caHalComDevice*>(instance); 
    caSysLog *pLog=dev->GetDeviceLog(dev);
    PLOG(pLog, device) << " in : isOpen = " << dev->isOpen << caEnd::endl;
    if (port->rdSize != 0) {
        if (dev->RxLock.Get() != 0) {
            res = deviceError::error_device_is_busy;
            PLOG(pLog, error) << " deviceError::error_device_is_busy" << caEnd::endl;
        } else {
            s_t rSize, pSize = 0;
            if (dev->Rx.Empty() == false) {
                if (dev->Rx.Size() < port->rdSize)
                    rSize = dev->Rx.Size();
                else
                    rSize = port->rdSize;
                if (dev->RxLock.Lock()) {
                    if (!dev->Rx.Pop(port->rdBuff, rSize, pSize)) {
                        res = deviceError::error_read_less_data;
                        PLOG(pLog, error) << " deviceError::error_read_less_data" << caEnd::endl;
                    }
                    dev->RxLock.UnLock();
                } else {
                    res = deviceError::error_device_is_busy;
                    PLOG(pLog, error) << " deviceError::error_device_is_busy" << caEnd::endl;
                }
                if (res == deviceError::no_error) {
                    port->readed += pSize;
                    port->rdBuff += pSize;
                    port->rdSize -= pSize;
                    port->tLast = dev->link->hll_tick();
                    port->tLastCmd = caDeviceAction::caActionRead;
                    dev->link->hll_get_errors(port->rdError, port->wrError);
                }
            } else {
                res = deviceError::error_rx_queue_empty;
                PLOG(pLog, error) << " deviceError::error_rx_queue_empty" << caEnd::endl;
            }
        }
    } else {
        res = deviceError::error_invalid_handle_port_rd_size;
        PLOG(pLog, error) << " deviceError::error_invalid_handle_port_rd_size:"
                << port->rdSize << caEnd::endl;
    }
    PLOG(pLog, device) << " out : res = " << res << caEnd::endl;
    return res;
}

u32 caHalComDevice::Flush(IDevice * instance,caDeviceHandle *port) {
    u32 res = deviceError::no_error;
    caHalComDevice* dev=static_cast<caHalComDevice*>(instance);
    port->tLast = dev->link->hll_tick();
    port->wrError = port->rdError = 0;
    port->tLastCmd = caDeviceAction::caActionFlush;
    dev->Rx.Clean();
    dev->Tx.Clean();
    return res;
}

u32 caHalComDevice::IoCtrl(IDevice * instance,caDeviceHandle *port, caIDeviceCtrl *inp) {
    u32 res = deviceError::no_error;
    caHalComDevice* dev=static_cast<caHalComDevice*>(instance);
    caSysLog *pLog=dev->GetDeviceLog(dev);
    port->tLast = dev->link->hll_tick();
    dev->link->hll_get_errors(port->rdError, port->wrError);
    PLOG(pLog, device) << " in : isOpen = " << dev->isOpen << caEnd::endl;
    caComDeviceCtrl *in = static_cast<caComDeviceCtrl *> (inp);
    switch (in->command) {
        default:
            PLOG(pLog, error) << " deviceError::error_ioctrl_command_error"
                    << caEnd::endl;
            res = deviceError::error_ioctrl_command_error;
            break;
        case caComDeviceCtrl::IoComCtrlDirect::comFlush:
            PLOG(pLog, info) << " caComDeviceCtrl::IoCtrlDirect::comFlush"
                    << caEnd::endl;
            res = Flush(dev,port);
            break;
        case caComDeviceCtrl::IoComCtrlDirect::comStart:
            PLOG(pLog, info) << " caComDeviceCtrl::IoCtrlDirect::comStart"
                    << caEnd::endl;
            res = dev->link->hll_enable(1, 1);
            break;
        case caComDeviceCtrl::IoComCtrlDirect::comStop:
            PLOG(pLog, info) << " caComDeviceCtrl::IoCtrlDirect::comStop"
                    << caEnd::endl;
            res = dev->link->hll_enable(0, 0);
            break;
        case caComDeviceCtrl::IoComCtrlDirect::comListHardware:
            PLOG(pLog, info) << " caComDeviceCtrl::IoCtrlDirect::comListHardware"
                    << caEnd::endl;
            if (in->ss != nullptr) {
                in->ss->Clear();
                u32 size = dev->link->hll_dump(in->ss);
                in->params[0] = size;
                if (size == 0) {
                    res = deviceError::error_com_dump;
                    PLOG(pLog, error) << " deviceError::error_com_dump"
                            << caEnd::endl;
                }
            } else {
                res = deviceError::error_invalid_null_destination;
                PLOG(pLog, error) << " deviceError::error_invalid_null_destination"
                        << caEnd::endl;
            }
            break;
        case caComDeviceCtrl::IoComCtrlDirect::comStatusBuffer:
            PLOG(pLog, info) << " caComDeviceCtrl::IoCtrlDirect::comStatusBuffer"
                    << caEnd::endl;
            in->params[0] = dev->Rx.Size();
            in->params[1] = dev->Tx.Size();
            break;
        case caComDeviceCtrl::IoComCtrlDirect::comAddSignalRx:
            PLOG(pLog, info) << " caComDeviceCtrl::IoCtrlDirect::comAddSignalRx"
                    << caEnd::endl;
            if (hal_llc_scheduler.hll_scheduler_valid_handle(dev->signalRx)) {
                res = deviceError::error_signal_already_set;
                PLOG(pLog, error) << " deviceError::error_signal_already_set"
                        << caEnd::endl;
            } else {
                if (hal_llc_scheduler.hll_scheduler_valid_handle(in->params[0])) {
                    dev->signalRx = in->params[0];
                } else {
                    res = deviceError::error_invalid_handle_port;
                    PLOG(pLog, error) << " deviceError::error_invalid_handle_port"
                            << caEnd::endl;
                }
            }
            break;
        case caComDeviceCtrl::IoComCtrlDirect::comAddSignalTx:
            PLOG(pLog, info) << " caComDeviceCtrl::IoCtrlDirect::comAddSignalTx"
                    << caEnd::endl;
            if (hal_llc_scheduler.hll_scheduler_valid_handle(dev->signalTx)) {
                res = deviceError::error_signal_already_set;
                PLOG(pLog, error) << " deviceError::error_signal_already_set"
                        << caEnd::endl;
            } else {
                if (hal_llc_scheduler.hll_scheduler_valid_handle(in->params[1])) {
                    dev->signalTx = in->params[1];
                } else {
                    res = deviceError::error_invalid_handle_port;
                    PLOG(pLog, error) << " deviceError::error_invalid_handle_port"
                            << caEnd::endl;
                }
            }
            break;
        case caComDeviceCtrl::IoComCtrlDirect::comRemoveSignalRx:
            PLOG(pLog, info) << " : caComDeviceCtrl::IoCtrlDirect::comRemoveSignalRx"
                    << caEnd::endl;
            if (hal_llc_scheduler.hll_scheduler_valid_handle(dev->signalRx)) {
                dev->signalRx = 0;
            } else {
                res = deviceError::error_invalid_handle_port;
                PLOG(pLog, error) << " deviceError::error_invalid_handle_port"
                        << caEnd::endl;
            }
            break;
        case caComDeviceCtrl::IoComCtrlDirect::comRemoveSignalTx:
            PLOG(pLog, info) << " caComDeviceCtrl::IoCtrlDirect::comRemoveSignalTx"
                    << caEnd::endl;
            if (hal_llc_scheduler.hll_scheduler_valid_handle(dev->signalTx)) {
                dev->signalTx = 0;
            } else {
                res = deviceError::error_invalid_handle_port;
                PLOG(pLog, error) << " deviceError::error_invalid_handle_port"
                        << caEnd::endl;
            }
            break;
        case caComDeviceCtrl::IoComCtrlDirect::comGetSignalRx:
            PLOG(pLog, info) << " caComDeviceCtrl::IoCtrlDirect::comGetSignalRx"
                    << caEnd::endl;
            in->params[0] = dev->signalRx;
            break;
        case caComDeviceCtrl::IoComCtrlDirect::comGetSignalTx:
            PLOG(pLog, info) << " caComDeviceCtrl::IoCtrlDirect::comGetSignalTx"
                    << caEnd::endl;
            in->params[0] = dev->signalTx;
            break;
    }
    port->tLastCmd = caDeviceAction::caActionIoCtrl;
    PLOG(pLog, device) << " out : res = " << res << caEnd::endl;
    return res;
}

u32 caHalComDevice::IrqService1(IDevice * instance,u8 * txbuff, s_t size, s_t & writed) {
    caHalComDevice* dev=static_cast<caHalComDevice*>(instance);
    caSysLog *pLog=dev->GetDeviceLog(dev);
    PLOG(pLog, irq_1) << " IrqService1 S=" << size << caEnd::endl;
    dev->Tx.Pop(txbuff, size, writed);
    if (dev->signalTx != 0)
        dev->link->hll_wakeup_1(dev->signalTx);
    return size - writed;
}

u32 caHalComDevice::IrqService2(IDevice * instance,u8 * rxbuff, s_t size, s_t & readed) {
    caHalComDevice* dev=static_cast<caHalComDevice*>(instance);
    caSysLog *pLog=dev->GetDeviceLog(dev);
    PLOG(pLog, irq_2) << " IrqService2 S=" << size << caEnd::endl;
    dev->Rx.Push(rxbuff, size, readed);
    if (dev->signalRx != 0)
        dev->link->hll_wakeup_2(dev->signalRx);
    return size - readed;
}

u32 caHalComDevice::IrqService3(IDevice * instance,u8 *, s_t, s_t &) {
    caHalComDevice* dev=static_cast<caHalComDevice*>(instance);
    caSysLog *pLog=dev->GetDeviceLog(dev);
    PLOG(pLog, irq_3) << " IrqService3 " << caEnd::endl;
    return 1;
}

u32 caHalComDevice::IrqService4(IDevice * instance,u8 *, s_t, s_t &) {
    caHalComDevice* dev=static_cast<caHalComDevice*>(instance);
    caSysLog *pLog=dev->GetDeviceLog(dev);
    PLOG(pLog, irq_4) << " IrqService4 " << caEnd::endl;
    return 1;
}

u32 caHalComDevice::IrqService5(IDevice * instance,u8 *, s_t, s_t &) {
    caHalComDevice* dev=static_cast<caHalComDevice*>(instance);
    caSysLog *pLog=dev->GetDeviceLog(dev);
    PLOG(pLog, irq_5) << " IrqService5 " << caEnd::endl;
    return 1;
}

u32 caHalComDevice::IrqService6(IDevice * instance,u8 *, s_t, s_t &) {
    caHalComDevice* dev=static_cast<caHalComDevice*>(instance);
    caSysLog *pLog=dev->GetDeviceLog(dev);
    PLOG(pLog, irq_6) << " IrqService6 " << caEnd::endl;
    return 1;
}

u32 caHalComDevice::IrqService7(IDevice * instance,u8 *, s_t, s_t &) {
    caHalComDevice* dev=static_cast<caHalComDevice*>(instance);
    caSysLog *pLog=dev->GetDeviceLog(dev);
    PLOG(pLog, irq_7) << " IrqService7 " << caEnd::endl;
    return 1;
}

u32 caHalComDevice::IrqService8(IDevice * instance,u8 *, s_t, s_t &) {
    caHalComDevice* dev=static_cast<caHalComDevice*>(instance);
    caSysLog *pLog=dev->GetDeviceLog(dev);
    PLOG(pLog, irq_8) << " IrqService8 " << caEnd::endl;
    return 1;
}
