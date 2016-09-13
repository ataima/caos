////////////////////////////////////////////////////////////////////////////////
//    Copyright (C) 2011  Angelo Coppi (angelogkcop at hotmail.com )
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
#include "idevice.h"
#include "stream.h"
#include "interrupt.h"
#include "miniuart.h"
#include "cpu.h"
#include "softreq.h"
#include "thread.h"
#include "scheduler.h"


/// in board specific have to implement class caInterruptRequest
extern "C" {

    void ISR_Undefined(u32 lr_usr, u32 lr_svc, u32 lr_irq, u32 lr_fiq) {
        caInterruptRequest::Undefined(lr_usr, lr_svc, lr_irq, lr_fiq);
    }

    void ISR_Software(u32 ioctl, u32 *p1, u32 *p2, u32 *res) {
        caInterruptRequest::Software(ioctl, p1, p2, res);
    }

    void ISR_Abort(u32 lr_usr, u32 lr_svc, u32 lr_irq, u32 lr_fiq) {
        caInterruptRequest::Abort(lr_usr, lr_svc, lr_irq, lr_fiq);
    }

    u32 ISR_IRQ(void) {
        return caInterruptRequest::IRQ();
    }

    u32 ISR_FIQ(void) {
        return caInterruptRequest::FIQ();
    }

    void ISR_Prefetch(u32 lr_usr, u32 lr_svc, u32 lr_irq, u32 lr_abt) {
        caInterruptRequest::Prefetch(lr_usr, lr_svc, lr_irq, lr_abt);
    }

    void ISR_Hypervisor(void) {
        caInterruptRequest::Hypervisor();
    }

    void switchContext(void) {

        //asm volatile ("CPSID IAF"); //DISABLE INTERRUPT

        asm volatile ("DSB"); //TOTEST
        asm volatile ("ISB"); //TOTEST

        asm volatile ("STMFD SP!, {R0-R3}"); //PUSH r1-r3        
        asm volatile ("LDR R2, =_ZN11caScheduler12current_taskE");
        asm volatile ("LDR R0, [R2]"); //Load PCB_[0] into R0	
        // TO TEST 
        asm volatile ("PLD [R0]");
        asm volatile ("MRS R1, SPSR"); //Copy SPSR into R1
#if DEBUG_CHECK_SCHEDULER        
        asm volatile ("STMIA R0!, {R1}"); //Store SPSR into PCB[0] - no R14 so not jump to thread
#else        
        asm volatile ("STMIA R0!, {R1, R14}"); //Store LR (return address) into PCB[1]
#endif  
        asm volatile ("ADD R0, R0, #16"); //location of R[4]
        asm volatile ("STMIA R0!, {R4-R12}"); //store R4-R12 into PCB
        asm volatile ("STMIA R0, {R13-R14}^"); //store SP LR into PCB
        asm volatile ("SUB R4, R0, #52"); //R3 = PCB_[2]

        asm volatile ("LDMFD SP!, {R0-R3}"); //R0, R1 restored from Stack//
        asm volatile ("STMIA R4!, {R0-R3}"); //R0, R1 stored in PCB

        caScheduler::GetNextContext();

        asm volatile ("LDR R2, =_ZN11caScheduler12current_taskE");
        asm volatile ("MOV R0,R2");
        asm volatile ("LDR R0, [R2]"); //Load PCB_[0] into R0	
        // TO TEST 
        asm volatile ("PLDW [R0]");
#if DEBUG_CHECK_SCHEDULER        
        asm volatile ("LDMIA R0!, {R1}"); //Copy only. SPSR only check
#else        
        asm volatile ("LDMIA R0!, {R1, R14}"); //Copy top 2 contents of PCB i.e. SPSR, ret Address into R1, R14
#endif        
        asm volatile ("MSR SPSR_fsxc, R1"); // Copy Saved StateRegister into usr SPSR

        asm volatile ("LDMIA R0, {R0-R14}^"); //Load from PCB R0-R14

        //asm volatile ("CPSIE IAF"); //ENABLE INTERRUPT
        asm volatile ("ERET");
    }

}
