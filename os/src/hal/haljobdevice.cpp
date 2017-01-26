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
#include "haljobdevice.h"
#include "memaux.h"
#include "scheduler.h"


caHalJobDevice::caHalJobDevice(hal_llc_scheduler_io *iface, u32 mask) {
    isOpen = 0;
    link = iface;
    handle_guid = BASE_HANDLE;
    mask_guid = (mask & ioCtrlRequest::maskIoCtrl);
}

u32 caHalJobDevice::Open(caIDeviceConfigure * /*setup*/, caDeviceHandle *port) {
    u32 res = deviceError::no_error;
    LOG(caLog, device) << " in : isOpen = " << isOpen << caEnd::endl;
    //caJobConfigure *conf = (caJobConfigure *) (setup);
    isOpen++;
    port->handle = caHalDeviceRules::addHandle(handle_guid, mask_guid);
    port->status = caDeviceHandle::statusHandle::Open;
    port->tStart = link->hll_tick();
    port->tLast = port->tStart;
    port->tStop = port->wrError = port->rdError = 0;
    port->tLastCmd = caDeviceAction::caActionOpen;
    LOG(caLog, device) << " handle = " << port->handle << caEnd::endl;
    LOG(caLog, device) << " out : res = " << res << caEnd::endl;
    return res;
}

u32 caHalJobDevice::Close(caDeviceHandle *port) {
    u32 res = deviceError::no_error;
    LOG(caLog, device) << " in : isOpen = " << isOpen << caEnd::endl;
    isOpen = 0;
    port->status = caDeviceHandle::statusHandle::Close;
    port->tStop = link->hll_tick();
    port->tLast = port->tStop;
    port->tLastCmd = caDeviceAction::caActionClose;
    port->handle = mask_guid | BASE_HANDLE;
    port->wrError = port->rdError = 0;
    LOG(caLog, device) << " out : res = " << res << caEnd::endl;
    return res;
}

u32 caHalJobDevice::Write(caDeviceHandle *port) {
    u32 res = deviceError::no_error;
    LOG(caLog, device) << " in : isOpen = " << isOpen << caEnd::endl;
    if (port->wrSize != 0) {
        u32 size = port->wrSize;
        //u32 * ptr = (u32*) port->wrBuff;
        // TODO INSERT WRITE DATA 
        port->writed += size;
        port->wrBuff += size;
        port->wrSize -= size;
        port->tLast = link->hll_tick();
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

u32 caHalJobDevice::Read(caDeviceHandle *port) {
    u32 res = deviceError::no_error;
    LOG(caLog, device) << " in : isOpen = " << isOpen << caEnd::endl;
    if (port->rdSize != 0) {
        //u32 *ptr = (u32 *) port->rdBuff;
        //TODO ADD READ HERE
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

u32 caHalJobDevice::Flush(caDeviceHandle *port) {
    u32 res = deviceError::no_error;
    port->tLast = link->hll_tick();
    port->wrError = port->rdError = 0;
    port->tLastCmd = caDeviceAction::caActionFlush;
    return res;
}

u32 caHalJobDevice::IoCtrl(caDeviceHandle *port,
        caIDeviceCtrl *inp) {
    u32 res = deviceError::no_error;
    port->tLast = link->hll_tick();
    LOG(caLog, device) << " in : isOpen = " << isOpen << caEnd::endl;
    caJobDeviceCtrl *in = static_cast<caJobDeviceCtrl *> (inp);
    switch (in->command) {
        default:
            LOG(caLog, error) << " deviceError::error_ioctrl_command_error" << caEnd::endl;
            res = deviceError::error_ioctrl_command_error;
            break;
        case caJobDeviceCtrl::IoCtrlDirect::jobGetThid:
            in->param_1 = caScheduler::GetCurrentTaskId();
            break;
        case caJobDeviceCtrl::IoCtrlDirect::jobLogCreate:
            if (!caLog.IsValid()) {
                if (caLog.Init(in->param_1, (deviceloglevels) in->param_2) != TRUE)
                    res = deviceError::error_cannot_create_log;
            } else {
                res = deviceError::error_log_already_set;
            }
            break;
        case caJobDeviceCtrl::IoCtrlDirect::jobDestroy:
            if (caLog.IsValid()) {
                if (caLog.Destroy() != TRUE)
                    res = deviceError::error_cannot_destroy_log;
            } else {
                res = deviceError::error_log_not_set;
            }
            break;
        case caJobDeviceCtrl::IoCtrlDirect::jobLogStart:
            if (caLog.IsValid()) {
                caLog.Enable();
            } else {
                res = deviceError::error_log_already_set;
            }
            break;
        case caJobDeviceCtrl::IoCtrlDirect::jobLogStop:
            if (caLog.IsValid()) {
                caLog.Disable();
            } else {
                res = deviceError::error_log_not_set;
            }
            break;
        case caJobDeviceCtrl::IoCtrlDirect::jobLogGet:
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

u32 caHalJobDevice::IoctlReq(ioCtrlFunction request,
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
            res = IoCtrl((caDeviceHandle *) p1, (caJobDeviceCtrl *) p2);
            break;
        default:
            break;
    }
    //
    return res;
}

u32 caHalJobDevice::IrqService1(u8 *, s_t size, s_t &) {
    LOG(caLog, irq_1) << " IrqService1 S=" << size << caEnd::endl;
    return 1;
}

u32 caHalJobDevice::IrqService2(u8 *, s_t size, s_t &) {
    LOG(caLog, irq_2) << " IrqService2 S=" << size << caEnd::endl;
    return 1;
}

u32 caHalJobDevice::IrqService3(u8 *, s_t, s_t &) {
    LOG(caLog, irq_3) << " IrqService3 " << caEnd::endl;
    return 1;
}

u32 caHalJobDevice::IrqService4(u8 *, s_t, s_t &) {
    LOG(caLog, irq_4) << " IrqService4 " << caEnd::endl;
    return 1;
}

u32 caHalJobDevice::IrqService5(u8 *, s_t, s_t &) {
    LOG(caLog, irq_5) << " IrqService5 " << caEnd::endl;
    return 1;
}

u32 caHalJobDevice::IrqService6(u8 *, s_t, s_t &) {
    LOG(caLog, irq_6) << " IrqService6 " << caEnd::endl;
    return 1;
}

u32 caHalJobDevice::IrqService7(u8 *, s_t, s_t &) {
    LOG(caLog, irq_7) << " IrqService7 " << caEnd::endl;
    return 1;
}

u32 caHalJobDevice::IrqService8(u8 *, s_t, s_t &) {
    LOG(caLog, irq_8) << " IrqService8 " << caEnd::endl;
    return 1;
}



