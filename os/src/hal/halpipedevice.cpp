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
#include "memory.h"
#include "halpipedevice.h"
#include "memaux.h"

caHalPipeDevice::caHalPipeDevice(hal_llc_mem_io *iface, u32 mask) {
    isOpen = 0;
    link = iface;
    handle_guid = BASE_HANDLE;
    mask_guid = (mask & ioCtrlRequest::maskIoCtrl);
}

u32 caHalPipeDevice::Open(caIDeviceConfigure * /*setup*/, caDeviceHandle *port) {
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

u32 caHalPipeDevice::Close(caDeviceHandle *port) {
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

u32 caHalPipeDevice::Write(caDeviceHandle *port) {
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

u32 caHalPipeDevice::Read(caDeviceHandle *port) {
    u32 res = deviceError::no_error;
    LOG(caLog, device) << " in : isOpen = " << isOpen << caEnd::endl;
    if (port->rdSize != 0) {
        u32 pSize = port->rdSize;
        port->readed += pSize;
        port->rdBuff += pSize;
        port->rdSize -= pSize;
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

u32 caHalPipeDevice::Flush(caDeviceHandle *port) {
    u32 res = deviceError::no_error;
    port->tLast = link->hll_tick();
    port->wrError = port->rdError = 0;
    port->tLastCmd = caDeviceAction::caActionFlush;
    return res;
}

u32 caHalPipeDevice::IoCtrl(caDeviceHandle *port,
        caIDeviceCtrl *inp) {
    u32 res = deviceError::no_error;
    port->tLast = link->hll_tick();
    LOG(caLog, device) << " in : isOpen = " << isOpen << caEnd::endl;
    caPipeDeviceCtrl *in = static_cast<caPipeDeviceCtrln *> (inp);
    switch (in->command) {
        case caPipeDeviceCtrl::IoMemCtrlDirect::memReallocate:
            break;
        default:
            LOG(caLog, error) << " deviceError::error_ioctrl_command_error"
                    << caEnd::endl;
            res = deviceError::error_ioctrl_command_error;
            break;
        case caPipeDeviceCtrl::IoMemCtrlDirect::memList:
            break;
        case caPipeDeviceCtrl::IoMemCtrlDirect::memAllocate:
            break;
        case caPipeDeviceCtrl::IoMemCtrlDirect::memFree:
            break;

    }
    port->tLastCmd = caDeviceAction::caActionIoCtrl;
    LOG(caLog, device) << " out : res = " << res << caEnd::endl;
    return res;
}

u32 caHalPipeDevice::IrqService1(u8 *, s_t size, s_t &) {
    LOG(caLog, irq_1) << " IrqService1 S=" << size << caEnd::endl;

    return 1;
}

u32 caHalPipeDevice::IrqService2(u8 *, s_t size, s_t &) {
    LOG(caLog, irq_2) << " IrqService2 S=" << size << caEnd::endl;

    return 1;
}

u32 caHalPipeDevice::IrqService3(u8 *, s_t, s_t &) {
    LOG(caLog, irq_3) << " IrqService3 " << caEnd::endl;

    return 1;
}

u32 caHalPipeDevice::IrqService4(u8 *, s_t, s_t &) {
    LOG(caLog, irq_4) << " IrqService4 " << caEnd::endl;

    return 1;
}

u32 caHalPipeDevice::IrqService5(u8 *, s_t, s_t &) {
    LOG(caLog, irq_5) << " IrqService5 " << caEnd::endl;

    return 1;
}

u32 caHalPipeDevice::IrqService6(u8 *, s_t, s_t &) {
    LOG(caLog, irq_6) << " IrqService6 " << caEnd::endl;

    return 1;
}

u32 caHalPipeDevice::IrqService7(u8 *, s_t, s_t &) {
    LOG(caLog, irq_7) << " IrqService7 " << caEnd::endl;

    return 1;
}

u32 caHalPipeDevice::IrqService8(u8 *, s_t, s_t &) {
    LOG(caLog, irq_8) << " IrqService8 " << caEnd::endl;
    return 1;
}



