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
    caArmCpu::DumpRegs();
    caArmCpu::Dump(caArmCpu::GetStackPointerR13());
    Dbg::Put("Undefined !!\r\n");
    Dbg::Put("USR = ",lr_usr);
    Dbg::Put("SVC = ",lr_svc);
    Dbg::Put("IRQ = ",lr_irq);
    Dbg::Put("UND = ",lr_und-4);    
    caArmCprs::DumpDFSR();
    Dbg::Put("IFSR = ",caArmCprs::GetIFSR());
    while (1);
}

void caInterruptRequest::Abort(u32 lr_usr, u32 lr_svc,
        u32 lr_irq, u32 lr_abt) {
    caArmCpu::DisableInt();
    caArmCpu::DumpRegs();
    caArmCpu::Dump(caArmCpu::GetStackPointerR13());
    Dbg::Put("Abort !!\r\n");
    Dbg::Put("USR = ",lr_usr);
    Dbg::Put("SVC = ",lr_svc);
    Dbg::Put("IRQ = ",lr_irq);
    Dbg::Put("ABT = ",lr_abt-4);    
    caArmCprs::DumpDFSR();
    Dbg::Put("IFSR = ",caArmCprs::GetIFSR());
    while (1);
}

void caInterruptRequest::Prefetch(u32 lr_usr, u32 lr_svc, u32 lr_irq, u32 lr_abt) {
    caArmCpu::DisableInt();
    caArmCpu::DumpRegs();
    caArmCpu::Dump(caArmCpu::GetStackPointerR13());
    Dbg::Put("Prefetch !!\r\n");
    Dbg::Put("USR = ",lr_usr);
    Dbg::Put("SVC = ",lr_svc);
    Dbg::Put("IRQ = ",lr_irq);
    Dbg::Put("ABT = ",lr_abt-4);    
    caArmCprs::DumpDFSR();
    Dbg::Put("IFSR = ",caArmCprs::GetIFSR());
    while (1);
}

void caInterruptRequest::Hypervisor(u32 lr_usr, u32 lr_svc, u32 lr_irq, u32 lr_abt) {
    caArmCpu::DisableInt();
    caArmCpu::DumpRegs();
    caArmCpu::Dump(caArmCpu::GetStackPointerR13());
    Dbg::Put("Hypervisor !!\r\n");
    Dbg::Put("USR = ",lr_usr);
    Dbg::Put("SVC = ",lr_svc);
    Dbg::Put("IRQ = ",lr_irq);
    Dbg::Put("ABT = ",lr_abt-4);    
    caArmCprs::DumpDFSR();
    Dbg::Put("IFSR = ",caArmCprs::GetIFSR());
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
}


u32 caInterruptRequest::IRQ(void) {
    return caIrqCtrl::SelectServiceIrq();
}

u32 caInterruptRequest::FIQ(void) {
    return caIrqCtrl::SelectServiceFiq();
}



