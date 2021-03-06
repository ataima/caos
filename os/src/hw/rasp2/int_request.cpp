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

#include "bcm2836.h"

#include "sysleds.h"
#include "interrupt.h"  
#include "miniuart.h"
#include "systimer.h"
#include "sysirqctrl.h" 
#include "cpu.h" 
#include "memory.h"
#include "heaparray.h"
#include "scheduler.h"
#include "systimer.h"
#include "cache.h"
#include "coprocessor.h"

extern CpuRegs cpu_reg;
// ISR board specific implementation

void caInterruptRequest::Undefined(u32 lr_usr, u32 lr_svc,
        u32 lr_irq, u32 lr_und) {
    caArmCpu::DisableInt();
    caArmCpu::Dump(caArmCpu::GetStackPointerR13());
    s8 buffio[512];
    caStringStream<s8> ss;
    ss.Init(buffio, 512);
    ss << caStringFormat::hex;
    ss << "Undefined" << caEnd::endl;
    ss << "USR = " << lr_usr << caEnd::endl;
    ss << "SVC = " << lr_svc << caEnd::endl;
    ss << "IRQ = " << lr_irq << caEnd::endl;
    ss << "UND = " << lr_und-4 << caEnd::endl;
    caArmCprs::DumpDFSR(ss);
    u32 v=caArmCprs::GetIFSR();
    ss << "IFSR= " << v << caEnd::endl;
    Dbg::Put(ss.Str());
    while (1);
}

void caInterruptRequest::Software(u32 ioctl,
        u32 *p1, u32 *p2, u32 *res) {

    u32 type = (ioctl & ioCtrlRequest::maskIoCtrl);
    ioCtrlFunction request = (ioCtrlFunction) (ioctl & ioCtrlRequest::maskHandle);
#if DEBUG_IOCTL_SVC_REQ    
    caArmCpu::DisableInt();
    Dbg::Put("------>\r\n");
    Dbg::Put("TYPE    = ", type);
    Dbg::Put("REQUEST = ", request);
    Dbg::Put("P1      = ", (u32) p1);
    Dbg::Put("*P1     = ", *p1);
    Dbg::Put("P2      = ", (u32) p2);
    Dbg::Put("*P2     = ", *p2);
    Dbg::Put("RES     = ", (u32) res);
    Dbg::Put("*RES    = ", *res);
    caArmCpu::EnableInt();
#endif    
    /*
    switch (type) {
        case ioCtrlRequest::Memory:
     *res = caMemory::IoctlReq(request, p1, p2);
            break;
        case ioCtrlRequest::Scheduler:
     *res = caSchedulerDevice::IoctlReq(request, p1, p2);
            break;
        case ioCtrlRequest::Com1:
     *res = caComDevice::IoctlReq(request, p1, p2);
            break;
#if MEM_PIPE_DEVICE            
        case ioCtrlRequest::MemPipe:
     *res = caMemDevice::IoctlReq(request, p1, p2);
            break;
#endif            
#if SYS_TIMER_DEVICE            
        case ioCtrlRequest::SysTimer:
     *res = caSysTimerDevice::IoctlReq(request, p1, p2);
            break;
#endif            
#if CACHE_DEVICE            
        case ioCtrlRequest::Cache:
     *res = caCacheDevice::IoctlReq(request, p1, p2);
            break;
#endif            
    }
     * */
}

void caInterruptRequest::Abort(u32 lr_usr, u32 lr_svc,
        u32 lr_irq, u32 lr_abt) {
    caArmCpu::DisableInt();
    caArmCpu::Dump(caArmCpu::GetStackPointerR13());
    s8 buffio[512];
    caStringStream<s8> ss;
    ss.Init(buffio, 512);
    ss << caStringFormat::hex;
    ss << "Abort" << caEnd::endl;
    ss << "USR = " << lr_usr << caEnd::endl;
    ss << "SVC = " << lr_svc << caEnd::endl;
    ss << "IRQ = " << lr_irq << caEnd::endl;
    ss << "ABT = " << lr_abt-4 << caEnd::endl;
    caArmCprs::DumpDFSR(ss);
    u32 v=caArmCprs::GetIFSR();
    ss << "IFSR= " << v << caEnd::endl;
    Dbg::Put(ss.Str());
    while (1);
}

u32 caInterruptRequest::IRQ(void) {
    return caIrqCtrl::SelectServiceIrq();
}

u32 caInterruptRequest::FIQ(void) {
    return caIrqCtrl::SelectServiceFiq();
}

void caInterruptRequest::Prefetch(u32 lr_usr, u32 lr_svc, u32 lr_irq, u32 lr_abt) {
    caArmCpu::DisableInt();
    caArmCpu::Dump(caArmCpu::GetStackPointerR13());    
    s8 buffio[512];
    caStringStream<s8> ss;
    ss.Init(buffio, 512);
    ss << caStringFormat::hex;
    ss << "Prefetch" << caEnd::endl;
    ss << "USR = " << lr_usr << caEnd::endl;
    ss << "SVC = " << lr_svc << caEnd::endl;
    ss << "IRQ = " << lr_irq << caEnd::endl;
    ss << "ABT = " << lr_abt << caEnd::endl;
    caArmCprs::DumpDFSR(ss);
    u32 v=caArmCprs::GetIFSR();
    ss << "IFSR= " << v << caEnd::endl;
    Dbg::Put(ss.Str());
    while (1);
}

void caInterruptRequest::Hypervisor(u32 lr_usr, u32 lr_svc, u32 lr_irq, u32 lr_abt) {
    caArmCpu::DisableInt();
    caArmCpu::Dump(caArmCpu::GetStackPointerR13());  
    s8 buffio[512];
    caStringStream<s8> ss;
    ss.Init(buffio, 512);
    ss << caStringFormat::hex;
    ss << "Hypervisor" << caEnd::endl;
    ss << "USR = " << lr_usr << caEnd::endl;
    ss << "SVC = " << lr_svc << caEnd::endl;
    ss << "IRQ = " << lr_irq << caEnd::endl;
    ss << "ABT = " << lr_abt << caEnd::endl;
    caArmCprs::DumpDFSR(ss);
    u32 v=caArmCprs::GetIFSR();
    ss << "IFSR= " << v << caEnd::endl;    
    Dbg::Put(ss.Str());
    while (1);
}

