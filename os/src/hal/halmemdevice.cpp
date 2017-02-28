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
#include "halmemdevice.h"
#include "memaux.h"

caHalMemDevice::caHalMemDevice(hal_llc_mem_io *iface, u32 mask) {
    isOpen = 0;
    link = iface;
    handle_guid = BASE_HANDLE;
    mask_guid = (mask & ioCtrlRequest::maskIoCtrl);
    IOpen=caHalMemDevice::Open;
    IClose=caHalMemDevice::Close;
    IWrite=caHalMemDevice::Write;
    IRead=caHalMemDevice::Read;
    IFlush=caHalMemDevice::Flush;
    IIoCtrl=caHalMemDevice::IoCtrl;
    IGetOpenFlag=caHalMemDevice::GetOpenFlag;
    IGetDeviceLog=caHalMemDevice::GetDeviceLog;
    ItoString=caHalMemDevice::toString;
    IIrqService1=caHalMemDevice::IrqService1;
    IIrqService2=caHalMemDevice::IrqService2;
    IIrqService3=caHalMemDevice::IrqService3;
    IIrqService4=caHalMemDevice::IrqService4;
    IIrqService5=caHalMemDevice::IrqService5;
    IIrqService6=caHalMemDevice::IrqService6;
    IIrqService7=caHalMemDevice::IrqService7;
    IIrqService8=caHalMemDevice::IrqService8;
}

u32 caHalMemDevice::Open(IDevice * instance,caIDeviceConfigure * /*setup*/, caDeviceHandle *port) {
    u32 res = deviceError::no_error;
    caHalMemDevice* dev=static_cast<caHalMemDevice*>(instance);
    caSysLog *pLog=dev->GetDeviceLog(dev);
    PLOG(pLog, device) << " in : isOpen = " << dev->isOpen << caEnd::endl;
    //caJobConfigure *conf = (caJobConfigure *) (setup);
    dev->isOpen++;
    port->handle = caHalDeviceRules::addHandle(dev->handle_guid, dev->mask_guid);
    port->status = caDeviceHandle::statusHandle::Open;
    port->tStart = dev->link->hll_tick();
    port->tLast = port->tStart;
    port->tStop = port->wrError = port->rdError = 0;
    port->tLastCmd = caDeviceAction::caActionOpen;
    PLOG(pLog, device) << " handle = " << port->handle << caEnd::endl;
    PLOG(pLog, device) << " out : res = " << res << caEnd::endl;
    return res;
}

u32 caHalMemDevice::Close(IDevice * instance,caDeviceHandle *port) {
    u32 res = deviceError::no_error;
    caHalMemDevice* dev=static_cast<caHalMemDevice*>(instance);
    caSysLog *pLog=dev->GetDeviceLog(dev);
    PLOG(pLog, device) << " in : isOpen = " << dev->isOpen << caEnd::endl;
    dev->isOpen = 0;
    port->status = caDeviceHandle::statusHandle::Close;
    port->tStop = dev->link->hll_tick();
    port->tLast = port->tStop;
    port->tLastCmd = caDeviceAction::caActionClose;
    port->handle = dev->mask_guid | BASE_HANDLE;
    port->wrError = port->rdError = 0;
    PLOG(pLog, device) << " out : res = " << res << caEnd::endl;
    return res;
}


/// Write to device memDevice -> allocate memory : in port->wrSize,out  port->wrBuff size

u32 caHalMemDevice::Write(IDevice * instance,caDeviceHandle *port) {
    u32 res = deviceError::no_error;
    caHalMemDevice* dev=static_cast<caHalMemDevice*>(instance);
    caSysLog *pLog=dev->GetDeviceLog(dev);
    PLOG(pLog, device) << " in : isOpen = " << dev->isOpen << caEnd::endl;
    if (port->wrSize != 0) {
        port->writed = 0;
        port->wrBuff = (u8*) caMemory::Allocate(port->wrSize);
        port->tLast = dev->link->hll_tick();
        port->tLastCmd = caDeviceAction::caActionWrite;
        port->wrError = port->rdError = 0;
    } else {
        res = deviceError::error_invalid_handle_port_wr_size;
        PLOG(pLog, error) << " deviceError::error_invalid_handle_port_wr_size:"
                << port->wrSize << caEnd::endl;
    }
    PLOG(pLog, device) << " out : res = " << res << caEnd::endl;
    return res;
}

u32 caHalMemDevice::Read(IDevice * instance,caDeviceHandle *port) {
    u32 res = deviceError::no_error;
    caHalMemDevice* dev=static_cast<caHalMemDevice*>(instance);
    caSysLog *pLog=dev->GetDeviceLog(dev);
    PLOG(pLog, device) << " in : isOpen = " << dev->isOpen << caEnd::endl;
    port->readed = caMemory::Free(port->rdBuff, &port->rdSize);
    port->rdBuff=nullptr;
    port->tLast = hal_llc_time_1.hll_tick();
    port->tLastCmd = caDeviceAction::caActionRead;
    port->wrError = port->rdError = 0;
    PLOG(pLog, device) << " out : res = " << res << caEnd::endl;
    return res;
}

u32 caHalMemDevice::Flush(IDevice * instance,caDeviceHandle *port) {
    u32 res = deviceError::no_error;
    caHalMemDevice* dev=static_cast<caHalMemDevice*>(instance);
    port->tLast = dev->link->hll_tick();
    port->wrError = port->rdError = 0;
    port->tLastCmd = caDeviceAction::caActionFlush;
    return res;
}

u32 caHalMemDevice::IoCtrl(IDevice * instance,caDeviceHandle *port,
        caIDeviceCtrl *inp) {
    u32 res = deviceError::no_error;
    caHalMemDevice* dev=static_cast<caHalMemDevice*>(instance);
    caSysLog *pLog=dev->GetDeviceLog(dev);
    port->tLast = dev->link->hll_tick();
    PLOG(pLog, device) << " in : isOpen = " << dev->isOpen << caEnd::endl;
    caMemoryDeviceCtrl *in = static_cast<caMemoryDeviceCtrl *> (inp);
    switch (in->command) {
        case caMemoryDeviceCtrl::IoMemCtrlDirect::memReallocate:
            //todo 
            break;
        default:
            PLOG(pLog, error) << " deviceError::error_ioctrl_command_error"
                    << caEnd::endl;
            res = deviceError::error_ioctrl_command_error;
            break;
        case caMemoryDeviceCtrl::IoMemCtrlDirect::memList:
            caMemory::List(*inp->ss);
            break;
        case caMemoryDeviceCtrl::IoMemCtrlDirect::memInfo:
            inp->params[0]=ptr_to_uint(caMemory::GetStartAddress());
            inp->params[1]=ptr_to_uint(caMemory::GetEndAddress());
            inp->params[2]=caMemory::GetAvailMemory();
            inp->params[3]=caMemory::GetTotalSize();
            break;
    }
    port->tLastCmd = caDeviceAction::caActionIoCtrl;
    PLOG(pLog, device) << " out : res = " << res << caEnd::endl;
    return res;
}

u32 caHalMemDevice::IrqService1(IDevice * instance,u8 *, s_t size, s_t &) {
    caHalMemDevice* dev=static_cast<caHalMemDevice*>(instance);
    caSysLog *pLog=dev->GetDeviceLog(dev);
    PLOG(pLog, irq_1) << " IrqService1 S=" << size << caEnd::endl;
    return 1;
}

u32 caHalMemDevice::IrqService2(IDevice * instance,u8 *, s_t size, s_t &) {
    caHalMemDevice* dev=static_cast<caHalMemDevice*>(instance);
    caSysLog *pLog=dev->GetDeviceLog(dev);
    PLOG(pLog, irq_2) << " IrqService2 S=" << size << caEnd::endl;
    return 1;
}

u32 caHalMemDevice::IrqService3(IDevice * instance,u8 *, s_t, s_t &) {
    caHalMemDevice* dev=static_cast<caHalMemDevice*>(instance);
    caSysLog *pLog=dev->GetDeviceLog(dev);
    PLOG(pLog, irq_3) << " IrqService3 " << caEnd::endl;
    return 1;
}

u32 caHalMemDevice::IrqService4(IDevice * instance,u8 *, s_t, s_t &) {
    caHalMemDevice* dev=static_cast<caHalMemDevice*>(instance);
    caSysLog *pLog=dev->GetDeviceLog(dev);
    PLOG(pLog, irq_4) << " IrqService4 " << caEnd::endl;
    return 1;
}

u32 caHalMemDevice::IrqService5(IDevice * instance,u8 *, s_t, s_t &) {
    caHalMemDevice* dev=static_cast<caHalMemDevice*>(instance);
    caSysLog *pLog=dev->GetDeviceLog(dev);
    PLOG(pLog, irq_5) << " IrqService5 " << caEnd::endl;
    return 1;
}

u32 caHalMemDevice::IrqService6(IDevice * instance,u8 *, s_t, s_t &) {
    caHalMemDevice* dev=static_cast<caHalMemDevice*>(instance);
    caSysLog *pLog=dev->GetDeviceLog(dev);
    PLOG(pLog, irq_6) << " IrqService6 " << caEnd::endl;
    return 1;
}

u32 caHalMemDevice::IrqService7(IDevice * instance,u8 *, s_t, s_t &) {
    caHalMemDevice* dev=static_cast<caHalMemDevice*>(instance);
    caSysLog *pLog=dev->GetDeviceLog(dev);
    PLOG(pLog, irq_7) << " IrqService7 " << caEnd::endl;
    return 1;
}

u32 caHalMemDevice::IrqService8(IDevice * instance, u8 *, s_t, s_t &) {
    caHalMemDevice* dev=static_cast<caHalMemDevice*>(instance);
    caSysLog *pLog=dev->GetDeviceLog(dev);
    PLOG(pLog, irq_8) << " IrqService8 " << caEnd::endl;
    return 1;
}



