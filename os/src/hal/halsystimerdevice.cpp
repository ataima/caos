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
#include "halsystimerdevice.h"
#include "memaux.h"

caHalSysTimerDevice::caHalSysTimerDevice(hal_llc_sys_time *st, u32 mask) {
    isOpen = 0;
    link = st;
    handle_guid = BASE_HANDLE;
    mask_guid = (mask & ioCtrlRequest::maskIoCtrl);
    link->hll_lnk_obj = (void *) this;
    signal_1 = signal_2 = 0;
}

u32 caHalSysTimerDevice::Open(caIDeviceConfigure * setup,
        caDeviceHandle *port) {
    u32 res = deviceError::no_error;
    LOG(caLog, device) << " in : isOpen = " << isOpen << caEnd::endl;
    caSysTimerConfigure *conf = (caSysTimerConfigure *) (setup);
    if (conf != NULL) {
        if (link->hll_config(conf->tick_ps, conf->clock_ps, conf->prescaler_ps, conf->irq_ps)) {
            isOpen++;
            port->handle = caHalDeviceRules::addHandle(handle_guid, mask_guid);
            port->status = caDeviceHandle::statusHandle::Open;
            port->tStart = link->hll_tick();
            port->tLast = port->tStart;
            port->tStop = port->wrError = port->rdError = 0;
            port->tLastCmd = caDeviceAction::caActionOpen;
            res = link->hll_start();
            LOG(caLog, device) << " handle = " << port->handle << caEnd::endl;
        } else {
            res = deviceError::error_systimer_configure_error;
            LOG(caLog, error) << " deviceError::error_systimer_configure_error" << caEnd::endl;
        }
    } else {
        res = deviceError::error_systimer_configure_not_valid;
        LOG(caLog, error) << " deviceError::error_systimer_configure_not_valid" << caEnd::endl;
    }
    LOG(caLog, device) << " out : res = " << res << caEnd::endl;
    return res;
}

u32 caHalSysTimerDevice::Close(caDeviceHandle *port) {
    u32 res = deviceError::no_error;
    LOG(caLog, device) << " in : isOpen = " << isOpen << caEnd::endl;
    isOpen = 0;
    port->status = caDeviceHandle::statusHandle::Close;
    port->tStop = link->hll_tick();
    port->tLast = port->tStop;
    port->tLastCmd = caDeviceAction::caActionClose;
    port->handle = mask_guid | BASE_HANDLE;
    port->wrError = port->rdError = 0;
    res = link->hll_stop();
    link->hll_lnk_obj = NULL;
    LOG(caLog, device) << " out : res = " << res << caEnd::endl;
    return res;
}

#define WRITE_TIMER_SIZEOF (u32)(4*sizeof(u32))

u32 caHalSysTimerDevice::Write(caDeviceHandle *port) {
    u32 res = deviceError::no_error;
    LOG(caLog, device) << " in : isOpen = " << isOpen << caEnd::endl;
    if (port->wrSize == WRITE_TIMER_SIZEOF) {
        u32 * ptr = (u32*) port->wrBuff;
        res = hal_llc_time_1.hll_settime(ptr[0], ptr[1], ptr[2], ptr[3]);
        port->writed += WRITE_TIMER_SIZEOF;
        port->wrBuff += WRITE_TIMER_SIZEOF;
        port->wrSize -= WRITE_TIMER_SIZEOF;
        port->tLast = hal_llc_time_1.hll_tick();
        port->tLastCmd = caDeviceAction::caActionWrite;
        port->wrError = port->rdError = 0;
    } else {
        res = deviceError::error_invalid_handle_port_wr_size;
        LOG(caLog, error) << " deviceError::error_invalid_handle_port_wr_size:"
                << port->wrSize << caEnd::endl;
    }
    LOG(caLog, device) << " out : res = " << res << caEnd::endl;
    return res;
}

#define READ_SIZEOF (u32)(sizeof(u32))

u32 caHalSysTimerDevice::Read(caDeviceHandle *port) {
    u32 res = deviceError::no_error;
    LOG(caLog, device) << " in : isOpen = " << isOpen << caEnd::endl;
    if (port->rdSize != 0) {
        u32 *ptr = (u32 *) port->rdBuff;
        if (port->rdSize >= READ_SIZEOF) {
            *ptr++ = hal_llc_time_1.hll_day();
            port->rdSize -= READ_SIZEOF;
            port->readed += READ_SIZEOF;
        }
        if (port->rdSize >= READ_SIZEOF) {
            *ptr++ = hal_llc_time_1.hll_hour();
            port->rdSize -= READ_SIZEOF;
            port->readed += READ_SIZEOF;
        }
        if (port->rdSize >= READ_SIZEOF) {
            *ptr++ = hal_llc_time_1.hll_min();
            port->rdSize -= READ_SIZEOF;
            port->readed += READ_SIZEOF;
        }
        if (port->rdSize >= READ_SIZEOF) {
            *ptr++ = hal_llc_time_1.hll_sec();
            port->rdSize -= READ_SIZEOF;
            port->readed += READ_SIZEOF;
        }
        if (port->rdSize >= READ_SIZEOF) {
            *ptr++ = hal_llc_time_1.hll_ms();
            port->rdSize -= READ_SIZEOF;
            port->readed += READ_SIZEOF;
        }
        if (port->rdSize >= READ_SIZEOF) {
            *ptr++ = hal_llc_time_1.hll_us();
            port->rdSize -= READ_SIZEOF;
            port->readed += READ_SIZEOF;
        }
        if (port->rdSize >= READ_SIZEOF) {
            *ptr++ = hal_llc_time_1.hll_ns();
            port->rdSize -= READ_SIZEOF;
            port->readed += READ_SIZEOF;
        }
        if (port->rdSize >= READ_SIZEOF) {
            *ptr++ = hal_llc_time_1.hll_ps();
            port->rdSize -= READ_SIZEOF;
            port->readed += READ_SIZEOF;
        }
        port->tLast = hal_llc_time_1.hll_tick();
        port->tLastCmd = caDeviceAction::caActionRead;
        port->wrError = port->rdError = 0;
    } else {
        res = deviceError::error_invalid_handle_port_rd_size;
        LOG(caLog, error) << " deviceError::error_invalid_handle_port_rd_size:"
                << port->rdSize << caEnd::endl;
    }
    LOG(caLog, device) << " out : res = " << res << caEnd::endl;
    return res;
}

u32 caHalSysTimerDevice::Flush(caDeviceHandle *port) {
    u32 res = deviceError::no_error;
    port->tLast = link->hll_tick();
    port->wrError = port->rdError = 0;
    port->tLastCmd = caDeviceAction::caActionFlush;
    return res;
}

u32 caHalSysTimerDevice::IoCtrl(caDeviceHandle *port,
        caIDeviceCtrl *inp) {
    u32 res = deviceError::no_error;
    port->tLast = link->hll_tick();
    LOG(caLog, device) << " in : isOpen = " << isOpen << caEnd::endl;
    caSysTimerDeviceCtrl *in = static_cast<caSysTimerDeviceCtrl *> (inp);
    switch (in->command) {
        default:
            LOG(caLog, error) << " deviceError::error_ioctrl_command_error" << caEnd::endl;
            res = deviceError::error_ioctrl_command_error;
            break;
        case caSysTimerDeviceCtrl::IoSysTimerCtrlDirect::sysTimerFlush:
            LOG(caLog, info) << " caSysTimerDeviceCtrl::IoCtrlDirect::sysTimerFlush"
                    << caEnd::endl;
            res = Flush(port);
            break;
        case caSysTimerDeviceCtrl::IoSysTimerCtrlDirect::sysTimerStart:
            LOG(caLog, info) << " caSysTimerDeviceCtrl::IoCtrlDirect::sysTimerStart"
                    << caEnd::endl;
            res = hal_llc_time_1.hll_start();
            break;
        case caSysTimerDeviceCtrl::IoSysTimerCtrlDirect::sysTimerStop:
            LOG(caLog, info) << " caSysTimerDeviceCtrl::IoCtrlDirect::sysTimerStop"
                    << caEnd::endl;
            res = hal_llc_time_1.hll_stop();
            break;
        case caSysTimerDeviceCtrl::IoSysTimerCtrlDirect::sysTimerListHardware:
            LOG(caLog, info) << " caSysTimerDeviceCtrl::IoCtrlDirect::sysTimerListHardware"
                    << caEnd::endl;
            if (in->ss != NULL) {
                in->ss->Clear();
                u32 size = hal_llc_time_1.hll_dump(in->ss);
                in->params[0] = size;
                if (size == 0) {
                    res = deviceError::error_systimer_dump;
                    LOG(caLog, error) << " deviceError::error_systimer_dump"
                            << caEnd::endl;
                }
            } else {
                res = deviceError::error_invalid_null_destination;
                LOG(caLog, error) << " deviceError::error_invalid_null_destination"
                        << caEnd::endl;
            }
            break;
        case caSysTimerDeviceCtrl::IoSysTimerCtrlDirect::sysTimerAddSignal_1:
            LOG(caLog, info) << " caSysTimerDeviceCtrl::IoCtrlDirect::sysTimerAddSignal_1"
                    << caEnd::endl;
            if (hal_llc_scheduler.hll_scheduler_valid_handle(signal_1)) {
                res = deviceError::error_signal_already_set;
                LOG(caLog, error) << " deviceError::error_signal_already_set"
                        << caEnd::endl;
            } else {
                if (hal_llc_scheduler.hll_scheduler_valid_handle(in->params[0])) {
                    signal_1 = in->params[0];
                } else {
                    res = deviceError::error_invalid_handle_port;
                    LOG(caLog, error) << " deviceError::error_invalid_handle_port"
                            << caEnd::endl;
                }
            }
            break;
        case caSysTimerDeviceCtrl::IoSysTimerCtrlDirect::sysTimerAddSignal_2:
            LOG(caLog, info) << " caSysTimerDeviceCtrl::IoCtrlDirect::sysTimerAddSignal_2"
                    << caEnd::endl;
            if (hal_llc_scheduler.hll_scheduler_valid_handle(signal_2)) {
                res = deviceError::error_signal_already_set;
                LOG(caLog, error) << " deviceError::error_signal_already_set" << caEnd::endl;
            } else {
                if (hal_llc_scheduler.hll_scheduler_valid_handle(in->params[1])) {
                    signal_2 = in->params[1];
                } else {
                    res = deviceError::error_invalid_handle_port;
                    LOG(caLog, error) << " deviceError::error_invalid_handle_port"
                            << caEnd::endl;
                }
            }
            break;
        case caSysTimerDeviceCtrl::IoSysTimerCtrlDirect::sysTimerRemoveSignal_1:
            LOG(caLog, info) << " : caSysTimerDeviceCtrl::IoCtrlDirect::sysTimerRemoveSignal_1"
                    << caEnd::endl;
            if (hal_llc_scheduler.hll_scheduler_valid_handle(signal_1)) {
                signal_1 = 0;
            } else {
                res = deviceError::error_invalid_handle_port;
                LOG(caLog, error) << " deviceError::error_invalid_handle_port"
                        << caEnd::endl;
            }
            break;
        case caSysTimerDeviceCtrl::IoSysTimerCtrlDirect::sysTimerRemoveSignal_2:
            LOG(caLog, info) << " caSysTimerDeviceCtrl::IoCtrlDirect::sysTimerRemoveSignal_2"
                    << caEnd::endl;
            if (hal_llc_scheduler.hll_scheduler_valid_handle(signal_2)) {
                signal_2 = 0;
            } else {
                res = deviceError::error_invalid_handle_port;
                LOG(caLog, error) << " deviceError::error_invalid_handle_port"
                        << caEnd::endl;
            }
            break;
        case caSysTimerDeviceCtrl::IoSysTimerCtrlDirect::sysTimerGetSignal_1:
            LOG(caLog, info) << " caSysTimerDeviceCtrl::IoCtrlDirect::sysTimerGetSignal_1"
                    << caEnd::endl;
            in->params[0] = signal_1;
            break;
        case caSysTimerDeviceCtrl::IoSysTimerCtrlDirect::sysTimerGetSignal_2:
            LOG(caLog, info) << " caSysTimerDeviceCtrl::IoCtrlDirect::comGetSignalTx"
                    << caEnd::endl;
            in->params[0] = signal_2;
            break;
    }
    port->tLastCmd = caDeviceAction::caActionIoCtrl;
    LOG(caLog, device) << " out : res = " << res << caEnd::endl;
    return res;
}

u32 caHalSysTimerDevice::IrqService1(u8 *, s_t size, s_t &iosize) {
    LOG(caLog, irq_1) << " IrqService1 S=" << size << caEnd::endl;
    iosize = 0;
    if (signal_1 != 0)
        link->hll_wakeup_1(signal_1);
    return 1;
}

u32 caHalSysTimerDevice::IrqService2(u8 *, s_t size, s_t &iosize) {
    LOG(caLog, irq_2) << " IrqService2 S=" << size << caEnd::endl;
    iosize = 0;
    if (signal_2 != 0)
        link->hll_wakeup_2(signal_2);
    return 1;
}

u32 caHalSysTimerDevice::IrqService3(u8 *, s_t, s_t &) {
    LOG(caLog, irq_3) << " IrqService3 " << caEnd::endl;
    return 1;
}

u32 caHalSysTimerDevice::IrqService4(u8 *, s_t, s_t &) {
    LOG(caLog, irq_4) << " IrqService4 " << caEnd::endl;
    return 1;
}

u32 caHalSysTimerDevice::IrqService5(u8 *, s_t, s_t &) {
    LOG(caLog, irq_5) << " IrqService5 " << caEnd::endl;
    return 1;
}

u32 caHalSysTimerDevice::IrqService6(u8 *, s_t, s_t &) {
    LOG(caLog, irq_6) << " IrqService6 " << caEnd::endl;
    return 1;
}

u32 caHalSysTimerDevice::IrqService7(u8 *, s_t, s_t &) {
    LOG(caLog, irq_7) << " IrqService7 " << caEnd::endl;
    return 1;
}

u32 caHalSysTimerDevice::IrqService8(u8 *, s_t, s_t &) {
    LOG(caLog, irq_8) << " IrqService8 " << caEnd::endl;
    return 1;
}



