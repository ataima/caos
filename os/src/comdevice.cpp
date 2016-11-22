
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

#include "config.h"
#include "bcm2836.h"
#if COM1_DEVICE

#include "idevice.h"
#include "stream.h"
#include "miniuart.h"
#include "interrupt.h"
#include "comdevice.h"
#include "systimer.h"
#include "sysirqctrl.h"
#include "memaux.h"
#include "atomiclock.h"
#include "thread.h"
#include "scheduler.h"
#include "syslog.h"



u32 caComDevice::guid = (u32) ioCtrlRequest::Com1 + BASE_HANDLE;
u32 caComDevice::isOpen = 0;
u32 caComDevice::eOverrun = 0;
caCircularBuffer<u8> caComDevice::Rx;
caCircularBuffer<u8> caComDevice::Tx;
u8 caComDevice::RxBuffer[QUEUESIZE];
u8 caComDevice::TxBuffer[QUEUESIZE];
caAtomicLock caComDevice::RxLock;
caAtomicLock caComDevice::TxLock;
u32 caComDevice::signalRx = 0;
u32 caComDevice::signalTx = 0;
caSysLog Log;

bool caComDevice::IsValidHandle(u32 handle) {
    bool res = false;
    if ((handle & ioCtrlRequest::maskIoCtrl) == ioCtrlRequest::Com1 &&
            (handle & ioCtrlRequest::maskHandle) > BASE_HANDLE)
        res = true;
    return res;
}

u32 caComDevice::Open(caComDeviceConfigure *setup, caDevicePort *port) {
    u32 res = deviceError::no_error;
    //TIN();
    if (setup != NULL && isOpen == 0) {
        if (caMiniUart::Init(setup->speed, setup->stop, setup->parity, setup->data)) {
            caMiniUart::ClearFifos();
            caMiniUart::Enable(0, 0);
            caMiniUart::Enable(1, 1);
            Rx.Init(RxBuffer, QUEUESIZE);
            Tx.Init(TxBuffer, QUEUESIZE);
            isOpen++;
            if (port != NULL) {
                signalTx = signalRx = eOverrun = 0;
                caMemAux::MemZero((u32*) port, sizeof (caDevicePort) / sizeof (u32));
                port->handle = ++guid;
                port->status = caDevicePort::statusPort::Open;
                port->tStart = caSysTimer::GetCount();
                port->tLast = port->tStart;
                port->tStop = 0;
                port->tLastCmd = caDeviceAction::caActionOpen;
                if (!((caMiniUart::EnableIrqRx()
                        & caIrqCtrl::EnableIrqAux()) == 1)) {
                    res = deviceError::error_configure_irq_serial_port;
                }
            } else {
                res = deviceError::error_invalid_null_port;
            }
        } else {
            res = deviceError::error_configure_serial_port;
        }
    } else {
        isOpen++;
    }
    //TOUT();
    return res;
}

u32 caComDevice::Close(caDevicePort *port) {
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
        port->error = eOverrun;
        if (!caMiniUart::Stop()) {
            res = deviceError::error_stop_serial_port;
        }
    }
    //TOUT();
    return res;
}

u32 caComDevice::Write(caDevicePort *port) {
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
            if (TxLock.Get() != 0) {
                res = deviceError::error_device_is_busy;
            } else {
                u32 wSize, writed;
                u32 maxWr = Tx.Available();
                if (maxWr > 0) {
                    if (maxWr > port->wrSize)
                        wSize = port->wrSize;
                    else
                        wSize = maxWr;
                    if (TxLock.Lock()) {
                        if (!Tx.Insert(port->wrBuff, wSize, writed))
                            res = deviceError::error_write_less_data;
                        TxLock.UnLock();
                    } else {
                        res = deviceError::error_device_is_busy;
                    }
                    if (res == deviceError::no_error) {
                        port->writed += writed;
                        port->wrBuff += writed;
                        port->wrSize -= writed;
                        port->tLast = caSysTimer::GetCount();
                        port->tLastCmd = caDeviceAction::caActionWrite;
                        port->error = eOverrun;
                        caMiniUart::EnableIrqTx();
                    }
                } else {
                    res = deviceError::error_tx_queue_full;
                }
            }
        } else {
            res = deviceError::error_invalid_handle_port;
        }
    }
    //TOUT();
    return res;
}

u32 caComDevice::Read(caDevicePort *port) {
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
            if (RxLock.Get() != 0) {
                res = deviceError::error_device_is_busy;
            } else {
                u32 rSize, pSize;
                if (Rx.Empty() == false) {
                    if (Rx.Size() < port->rdSize)
                        rSize = Rx.Size();
                    else
                        rSize = port->rdSize;
                    if (RxLock.Lock()) {
                        if (!Rx.Remove(port->rdBuff, rSize, pSize))
                            res = deviceError::error_read_less_data;
                        RxLock.UnLock();
                    } else {
                        res = deviceError::error_device_is_busy;
                    }
                    if (res == deviceError::no_error) {
                        port->readed += pSize;
                        port->rdBuff += pSize;
                        port->rdSize -= pSize;
                        port->tLast = caSysTimer::GetCount();
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
    }
    //TOUT();
    return res;
}

u32 caComDevice::Flush(caDevicePort *port) {
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
        Rx.Clean();
        Tx.Clean();
    }
    //TOUT();
    return res;
}

u32 caComDevice::IoCtrl(caDevicePort *port, caComDeviceCtrl *in) {
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
        port->tLast = caSysTimer::GetCount();
        port->error = eOverrun;

        switch (in->command) {
            case caComDeviceCtrl::IoCtrlDirect::comFlush:
                res = Flush(port);
                break;
            case caComDeviceCtrl::IoCtrlDirect::comStart:
                res = caMiniUart::Enable(1, 1);
                break;
            case caComDeviceCtrl::IoCtrlDirect::comStop:
                res = caMiniUart::Enable(0, 0);
                break;
            case caComDeviceCtrl::IoCtrlDirect::comListHardware:
                if (in->ss != NULL) {
                    in->ss->Clear();
                    *in->ss << " --- COM1 LIST ---" << caEnd::endl;
                    res = caMiniUart::Dump(*in->ss);
                } else
                    res = deviceError::error_invalid_null_destination;
                break;
            case caComDeviceCtrl::IoCtrlDirect::comStatusBuffer:
                in->st_rx = Rx.Size();
                in->st_tx = Tx.Size();
                break;
            case caComDeviceCtrl::IoCtrlDirect::comAddSignalRx:
                if (caScheduler::IsValidContext(signalRx)) {
                    res = deviceError::error_signal_already_set;
                } else {
                    if (caScheduler::IsValidContext(in->st_rx)) {
                        signalRx = in->st_rx;
                    } else
                        res = deviceError::error_invalid_handle_port;
                }
                break;
            case caComDeviceCtrl::IoCtrlDirect::comAddSignalTx:
                if (caScheduler::IsValidContext(signalTx)) {
                    res = deviceError::error_signal_already_set;
                } else {
                    if (caScheduler::IsValidContext(in->st_tx)) {
                        signalTx = in->st_tx;
                    } else
                        res = deviceError::error_invalid_handle_port;
                }
                break;
            case caComDeviceCtrl::IoCtrlDirect::comRemoveSignalRx:
                if (caScheduler::IsValidContext(signalRx)) {
                    signalRx = 0;
                } else {
                    res = deviceError::error_invalid_handle_port;
                }
                break;
            case caComDeviceCtrl::IoCtrlDirect::comRemoveSignalTx:
                if (caScheduler::IsValidContext(signalTx)) {
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
    }
    port->tLastCmd = caDeviceAction::caActionIoCtrl;
    //TOUT();
    return res;
}

u32 caComDevice::IoctlReq(ioCtrlFunction request, u32 *p1, u32 *p2) {
    u32 res = deviceError::no_error;
    ;
    //TIN();
    switch (request) {
        case ioCtrlFunction::caOpenDevice:
            res = Open((caComDeviceConfigure *) p1, (caDevicePort *) p2);
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
            res = IoCtrl((caDevicePort *) p1, (caComDeviceCtrl *) p2);
            break;
        default:
            break;
    }
    //TOUT();
    return res;
}

// ISR Interrupt service routine from caIrqCtrl::SelectServiceIrq()

void caComDevice::IrqService(void) {
    muIirReg iir;
    muLsrReg lsr;
    muStatReg stat;
    u8 c = 0;
    u32 symbol;
    iir.asReg = caMiniUart::GetIIR();
    stat.asReg = caMiniUart::GetStat();
    if (iir.asRdBit.pending == 0) {
        u32 irq = iir.asRdBit.irqid;
        if (irq == 1) // TX
        {
            //if TX.empty is false there are some char in queue to transmit
            if (Tx.Empty() == false) {
                symbol = 7 - stat.asBit.txfifo;
                while (symbol != 0) {
                    lsr.asReg = caMiniUart::GetLsr();
                    if (lsr.asBit.overrun)eOverrun++;
                    if (lsr.asBit.txempty == 0)break;
                    if (Tx.Empty() == false) {
                        size_t removed = 0;
                        Tx.Remove(&c, 1, removed);
                        caMiniUart::SetIO(c);
                    } else {
                        caMiniUart::DisableIrqTx();
                        break;
                    }
                    symbol--;
                }
            } else {
                caMiniUart::DisableIrqTx();
            }
            if (signalTx != 0)
                caScheduler::WakeUp(signalTx);
        } else
            if (irq == 2) // RX
        {
            symbol = stat.asBit.rxfifo;
            while (symbol != 0) {
                lsr.asReg = caMiniUart::GetLsr();
                if (lsr.asBit.overrun)eOverrun++;
                if (lsr.asBit.rxready == 0) break;
                c = caMiniUart::GetIO();
                size_t inserted = 0;
                Rx.Insert(&c, 1, inserted);
                symbol--;
            }
            if (signalRx != 0)
                caScheduler::WakeUp(signalRx);
        }
    }
}
#endif