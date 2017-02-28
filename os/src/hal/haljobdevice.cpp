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
    IOpen=caHalJobDevice::Open;
    IClose=caHalJobDevice::Close;
    IWrite=caHalJobDevice::Write;
    IRead=caHalJobDevice::Read;
    IFlush=caHalJobDevice::Flush;
    IIoCtrl=caHalJobDevice::IoCtrl;
    IGetOpenFlag=caHalJobDevice::GetOpenFlag;
    IGetDeviceLog=caHalJobDevice::GetDeviceLog;
    ItoString=caHalJobDevice::toString;
    IIrqService1=caHalJobDevice::IrqService1;
    IIrqService2=caHalJobDevice::IrqService2;
    IIrqService3=caHalJobDevice::IrqService3;
    IIrqService4=caHalJobDevice::IrqService4;
    IIrqService5=caHalJobDevice::IrqService5;
    IIrqService6=caHalJobDevice::IrqService6;
    IIrqService7=caHalJobDevice::IrqService7;
    IIrqService8=caHalJobDevice::IrqService8;
}

u32 caHalJobDevice::Open(IDevice * instance,caIDeviceConfigure * /*setup*/, caDeviceHandle *port) {
    u32 res = deviceError::no_error;
    caHalJobDevice* dev=static_cast<caHalJobDevice*>(instance);
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

u32 caHalJobDevice::Close(IDevice * instance,caDeviceHandle *port) {
    u32 res = deviceError::no_error;
    caHalJobDevice* dev=static_cast<caHalJobDevice*>(instance);
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

u32 caHalJobDevice::Write(IDevice * instance,caDeviceHandle *port) {
    u32 res = deviceError::no_error;
    caHalJobDevice* dev=static_cast<caHalJobDevice*>(instance);
    caSysLog *pLog=dev->GetDeviceLog(dev);
    PLOG(pLog, device) << " in : isOpen = " << dev->isOpen << caEnd::endl;
    if (port->wrSize != 0) {
        u32 size = port->wrSize;
        //u32 * ptr = (u32*) port->wrBuff;
        // TODO INSERT WRITE DATA 
        port->writed += size;
        port->wrBuff += size;
        port->wrSize -= size;
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

u32 caHalJobDevice::Read(IDevice * instance,caDeviceHandle *port) {
    u32 res = deviceError::no_error;
    caHalJobDevice* dev=static_cast<caHalJobDevice*>(instance);
    caSysLog *pLog=dev->GetDeviceLog(dev);
    PLOG(pLog, device) << " in : isOpen = " << dev->isOpen << caEnd::endl;
    if (port->rdSize != 0) {
        s_t pSize = port->rdSize;
        if (pSize>sizeof (caThreadContext))
            pSize = sizeof (caThreadContext);
        u32 *dst = (u32 *) port->rdBuff;
        u32 *ctx = (u32 *) caScheduler::GetCurrentContext();
        caMemAux<u32>::MemCpy(dst, ctx, pSize);
        port->readed += pSize;
        port->rdBuff += pSize;
        port->rdSize -= pSize;
        port->tLast = hal_llc_time_1.hll_tick();
        port->tLastCmd = caDeviceAction::caActionRead;
        port->wrError = port->rdError = 0;
    } else {
        res = deviceError::error_invalid_handle_port_rd_size;
        PLOG(pLog, error) << " deviceError::error_invalid_handle_port_rd_size:"
                << port->rdSize << caEnd::endl;
    }
    PLOG(pLog, device) << " out : res = " << res << caEnd::endl;
    return res;
}

u32 caHalJobDevice::Flush(IDevice * instance,caDeviceHandle *port) {
    u32 res = deviceError::no_error;
    caHalJobDevice* dev=static_cast<caHalJobDevice*>(instance);
    port->tLast = dev->link->hll_tick();
    port->wrError = port->rdError = 0;
    port->tLastCmd = caDeviceAction::caActionFlush;
    return res;
}

u32 caHalJobDevice::IoCtrl(IDevice * instance,caDeviceHandle *port,
        caIDeviceCtrl *inp) {

    struct param {
        const char *name;
        caJobPriority p;
        thFunc func;
        u32 par1;
        u32 par2;
        u32 stack;
    };
    u32 res = deviceError::no_error;
    caHalJobDevice* dev=static_cast<caHalJobDevice*>(instance);
    caSysLog *pLog=dev->GetDeviceLog(dev);
    port->tLast = dev->link->hll_tick();
    PLOG(pLog, device) << " in : isOpen = " << dev->isOpen << caEnd::endl;
    caJobDeviceCtrl *in = static_cast<caJobDeviceCtrl *> (inp);
    switch (in->command) {
        default:
            PLOG(pLog, error) << " deviceError::error_ioctrl_command_error"
                    << caEnd::endl;
            res = deviceError::error_ioctrl_command_error;
            break;
        case caJobDeviceCtrl::IoJobCtrlDirect::jobDestroyAll:
            if (caScheduler::RemoveAllJobs() == false) {
                PLOG(pLog, error) << " deviceError::error_hal_jobs_remove_all"
                        << caEnd::endl;
                res = deviceError::error_hal_jobs_remove_all;
            }
            break;
        case caJobDeviceCtrl::IoJobCtrlDirect::jobDestroy:
            if (caScheduler::RemoveJob(inp->params[0]) == false) {
                PLOG(pLog, error) << " deviceError::error_hal_job_remove"
                        << caEnd::endl;
                res = deviceError::error_hal_job_remove;
            }
            break;
        case caJobDeviceCtrl::IoJobCtrlDirect::jobList:
        {
            u32 size = caScheduler::Dump(*inp->ss);
            inp->params[0] = size;
            if (size == 0) {
                PLOG(pLog, error) << " deviceError::error_hal_job_dump"
                        << caEnd::endl;
                res = deviceError::error_hal_job_dump;
            }
        }
            break;
        case caJobDeviceCtrl::IoJobCtrlDirect::jobGetSize:
            inp->params[0] = caScheduler::Size();
            break;
        case caJobDeviceCtrl::IoJobCtrlDirect::jobAddSuperVisorJob:
        {
            struct param *input = caAttach<struct param>::link(inp);
            u32 result = caScheduler::AddSuperVisorJob(input->name, input->p,
                    input->func, input->par1, input->par2, input->stack);
            inp->params[0] = result;
            if (result == (u32) CreateThreadResult::FailCreate) {
                PLOG(pLog, error) << " deviceError::error_hal_job_dump"
                        << caEnd::endl;
                res = deviceError::error_hal_job_dump;
            }
        }
            break;
        case caJobDeviceCtrl::IoJobCtrlDirect::jobAddSystemJob:
        {
            struct param *input = caAttach<struct param>::link(inp);
            u32 result = caScheduler::AddSystemJob(input->name, input->p, input->func,
                    input->par1, input->par2, input->stack);
            inp->params[0] = result;
            if (result == (u32) CreateThreadResult::FailCreate) {
                PLOG(pLog, error) << " deviceError::error_hal_job_sys_create"
                        << caEnd::endl;
                res = deviceError::error_hal_job_sys_create;
            }
        }
            break;
        case caJobDeviceCtrl::IoJobCtrlDirect::jobAddUserJob:
        {
            struct param *input = caAttach<struct param>::link(inp);
            u32 result = caScheduler::AddJob(input->name, input->p, input->func,
                    input->par1, input->par2, input->stack);
            inp->params[0] = result;
            if (result == (u32) CreateThreadResult::FailCreate) {
                PLOG(pLog, error) << " deviceError::error_hal_job_user_create"
                        << caEnd::endl;
                res = deviceError::error_hal_job_user_create;
            }
        }
            break;
        case caJobDeviceCtrl::IoJobCtrlDirect::jobWaitForSignal:
            res = caScheduler::WaitForSignal();
            if (res != deviceError::error_generic) {
                PLOG(pLog, error) << " deviceError::error_hal_job_wait_for_signal"
                        << caEnd::endl;
                res = deviceError::error_hal_job_wait_for_signal;
            }
            break;
        case caJobDeviceCtrl::IoJobCtrlDirect::jobSleep:
            res = caScheduler::Sleep(in->params[0]);
            if (res != deviceError::error_generic) {
                PLOG(pLog, error) << " deviceError::error_hal_job_sleep"
                        << caEnd::endl;
                res = deviceError::error_hal_job_sleep;
            }
            break;
        case caJobDeviceCtrl::IoJobCtrlDirect::jobChangePriority:
            res = caScheduler::ChangePriority(in->params[0],
                    (caJobPriority) in->params[1]);
            if (res != deviceError::error_generic) {
                PLOG(pLog, error) << " deviceError::error_hal_job_change_priority"
                        << caEnd::endl;
                res = deviceError::error_hal_job_change_priority;
            }
            break;
        case caJobDeviceCtrl::IoJobCtrlDirect::jobGetThid:
            in->params[0] = caScheduler::GetCurrentTaskId();
            break;
    }
    port->tLastCmd = caDeviceAction::caActionIoCtrl;
    PLOG(pLog, device) << " out : res = " << res << caEnd::endl;
    return res;
}

u32 caHalJobDevice::IrqService1(IDevice * instance,u8 *, s_t size, s_t &) {
    caHalJobDevice* dev=static_cast<caHalJobDevice*>(instance);
    caSysLog *pLog=dev->GetDeviceLog(dev);
    PLOG(pLog, irq_1) << " IrqService1 S=" << size << caEnd::endl;
    return 1;
}

u32 caHalJobDevice::IrqService2(IDevice * instance,u8 *, s_t size, s_t &) {
    caHalJobDevice* dev=static_cast<caHalJobDevice*>(instance);
    caSysLog *pLog=dev->GetDeviceLog(dev);
    PLOG(pLog, irq_2) << " IrqService2 S=" << size << caEnd::endl;
    return 1;
}

u32 caHalJobDevice::IrqService3(IDevice * instance,u8 *, s_t, s_t &) {
    caHalJobDevice* dev=static_cast<caHalJobDevice*>(instance);
    caSysLog *pLog=dev->GetDeviceLog(dev);
    PLOG(pLog, irq_3) << " IrqService3 " << caEnd::endl;
    return 1;
}

u32 caHalJobDevice::IrqService4(IDevice * instance,u8 *, s_t, s_t &) {
    caHalJobDevice* dev=static_cast<caHalJobDevice*>(instance);
    caSysLog *pLog=dev->GetDeviceLog(dev);
    PLOG(pLog, irq_4) << " IrqService4 " << caEnd::endl;
    return 1;
}

u32 caHalJobDevice::IrqService5(IDevice * instance,u8 *, s_t, s_t &) {
    caHalJobDevice* dev=static_cast<caHalJobDevice*>(instance);
    caSysLog *pLog=dev->GetDeviceLog(dev);
    PLOG(pLog, irq_5) << " IrqService5 " << caEnd::endl;
    return 1;
}

u32 caHalJobDevice::IrqService6(IDevice * instance,u8 *, s_t, s_t &) {
    caHalJobDevice* dev=static_cast<caHalJobDevice*>(instance);
    caSysLog *pLog=dev->GetDeviceLog(dev);
    PLOG(pLog, irq_6) << " IrqService6 " << caEnd::endl;
    return 1;
}

u32 caHalJobDevice::IrqService7(IDevice * instance,u8 *, s_t, s_t &) {
    caHalJobDevice* dev=static_cast<caHalJobDevice*>(instance);
    caSysLog *pLog=dev->GetDeviceLog(dev);
    PLOG(pLog, irq_7) << " IrqService7 " << caEnd::endl;
    return 1;
}

u32 caHalJobDevice::IrqService8(IDevice * instance,u8 *, s_t, s_t &) {
    caHalJobDevice* dev=static_cast<caHalJobDevice*>(instance);
    caSysLog *pLog=dev->GetDeviceLog(dev);
    PLOG(pLog, irq_8) << " IrqService8 " << caEnd::endl;
    return 1;
}



