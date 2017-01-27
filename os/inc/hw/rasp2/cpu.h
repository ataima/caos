
#ifndef CPU_INFO_H
#define CPU_INFO_H
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

typedef struct regs_cpu {
    u32 r[32];
} CpuRegs;

class caArmCpu {
private:


public:

    typedef enum tag_req_info {
        nsacr,
    } eReq;

public:
    static u32 GetControl(eReq v);
    static void SetControl(eReq v, u32 value);
    static void GetMainIdCpuInfo(void);
    static void DumpRegs(CpuRegs *regs);
    static void GetRegs(CpuRegs *regs);

    static void Dump(u32 *addr);
    static void DumpCPSR(void);
    static void JumpTo(u32 addr);
    static void CopyRegsFromStack(u32 *ptr, CpuRegs *regs);
    static void CopyRegsToStack(u32 *ptr, CpuRegs *regs);

    static inline void PushR12LR(void) {
        asm volatile ("STMFD    sp!, {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}");
    }

    static inline void PopR12LR(void) {
        asm volatile ("LDMFD    sp!, {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}");
    }

    static inline void PopR12AndReturn(void) {
        asm volatile ("LDMFD    sp!, {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,pc}");
    }

    static inline u32 *GetLinkRegisterR14(void) {
        u32 *res;
        asm("mov %0,lr" : "=r" (res));
        return res;
    }

    static inline void DumpR0(void) {
        u32 val;
        PushR12LR();
        asm volatile("mov %0,r0" : "=r" (val));
        Dbg::Put("R[0] : ", val);
        PopR12LR();
    }

    static inline void DumpR1(void) {
        u32 val;
        PushR12LR();
        asm volatile("mov %0,r1" : "=r" (val));
        Dbg::Put("R[1] : ", val);
        PopR12LR();
    }

    static inline void DumpR2(void) {
        u32 val;
        PushR12LR();
        asm volatile("mov %0,r2" : "=r" (val));
        Dbg::Put("R[2] : ", val);
        PopR12LR();
    }

    static inline void DumpR3(void) {
        u32 val;
        PushR12LR();
        asm volatile("mov %0,r3" : "=r" (val));
        Dbg::Put("R[3] : ", val);
        PopR12LR();
    }

    static inline void DumpR4(void) {
        u32 val;
        PushR12LR();
        asm volatile("mov %0,r4" : "=r" (val));
        Dbg::Put("R[4] : ", val);
        PopR12LR();
    }

    static inline void DumpR5(void) {
        u32 val;
        PushR12LR();
        asm volatile("mov %0,r5" : "=r" (val));
        Dbg::Put("R[5] : ", val);
        PopR12LR();
    }

    static inline void DumpR6(void) {
        u32 val;
        PushR12LR();
        asm volatile("mov %0,r6" : "=r" (val));
        Dbg::Put("R[6] : ", val);
        PopR12LR();
    }

    static inline void DumpR7(void) {
        u32 val;
        PushR12LR();
        asm volatile("mov %0,r7" : "=r" (val));
        Dbg::Put("R[7] : ", val);
        PopR12LR();
    }

    static inline void DumpR8(void) {
        u32 val;
        PushR12LR();
        asm volatile("mov %0,r8" : "=r" (val));
        Dbg::Put("R[8] : ", val);
        PopR12LR();
    }

    static inline void DumpR9(void) {
        u32 val;
        PushR12LR();
        asm volatile("mov %0,r9" : "=r" (val));
        Dbg::Put("R[9] : ", val);
        PopR12LR();
    }

    static inline void DumpR10(void) {
        u32 val;
        PushR12LR();
        asm volatile("mov %0,r10" : "=r" (val));
        Dbg::Put("R[10] : ", val);
        PopR12LR();
    }

    static inline void DumpR11(void) {
        u32 val;
        PushR12LR();
        asm volatile("mov %0,r11" : "=r" (val));
        Dbg::Put("R[11] : ", val);
        PopR12LR();
    }

    static inline void DumpR12(void) {
        u32 val;
        PushR12LR();
        asm volatile("mov %0,r12" : "=r" (val));
        Dbg::Put("R[12] : ", val);
        PopR12LR();
    }

    static inline void DumpR13(void) {
        u32 val;
        PushR12LR();
        asm volatile("mov %0,r13" : "=r" (val));
        Dbg::Put("R[13] : ", val);
        PopR12LR();
    }

    static inline void DumpR14(void) {
        u32 val;
        PushR12LR();
        asm volatile("mov %0,r14" : "=r" (val));
        Dbg::Put("R[14] : ", val);
        PopR12LR();
    }

    static inline void DumpR15(void) {
        u32 val;
        PushR12LR();
        asm volatile("mov %0,r15" : "=r" (val));
        Dbg::Put("R[15] : ", val);
        PopR12LR();
    }

    static inline u32 *GetStackPointerR13(void) {
        u32 *res;
        asm("mov %0,sp" : "=r" (res));
        return res;
    }

    static inline void SetStackPointerR13(u32 *newsp) {
        asm("mov sp,0" : "=r" (newsp));
    }

    static inline u32 GetProgramCounterR15(void) {
        u32 res;
        asm("mov %0,pc" : "=r" (res));
        return res - 4;
    }

    static inline u32 GetIPR12(void) {
        u32 res;
        asm("mov %0,ip" : "=r" (res));
        return res;
    }

    static inline u32 GetFPR11(void) {
        u32 res;
        asm("mov %0,fp" : "=r" (res));
        return res;
    }

    static inline u32 GetSLR10(void) {
        u32 res;
        asm("mov %0,sl" : "=r" (res));
        return res;
    }

    static inline u32 GetCPSR(void) {
        u32 res;
        asm("mrs %0,cpsr" : "=r"(res));
        return res;
    }

    static inline u32 GetSCR(void) {
        u32 res;
        asm("mrc  p15,0,%0,c1,c1,0" : "=r"(res));
        return res;
    }

    static inline void SVC_001() {
        asm ("DSB"); //TOTEST
        asm ("ISB"); //TOTEST
        asm("svc #1");
    }

    static inline void SVC_7961() {
        asm ("DSB"); //TOTEST
        asm ("ISB"); //TOTEST
        asm ("svc #7961");
    }

    static inline void CopyRegsFromStack(void) {
        // r0 dest
        asm ("ldr r1, [sp,#0]"); // r0
        asm ("str r1, [r0,#0]");

        asm ("ldr r1, [sp,#4]"); // r1
        asm ("str r1, [r0,#4]");

        asm ("ldr r1, [sp,#8]"); // r2
        asm ("str r1, [r0,#8]");

        asm ("ldr r1, [sp,#12]"); // r3
        asm ("str r1, [r0,#12]");

        asm ("ldr r1, [sp,#16]"); // r4
        asm ("str r1, [r0,#16]");

        asm ("ldr r1, [sp,#20]"); // r5
        asm ("str r1, [r0,#20]");

        asm ("ldr r1, [sp,#24]"); // r6
        asm ("str r1, [r0,#24]");

        asm ("ldr r1, [sp,#28]"); // r7
        asm ("str r1, [r0,#28]");

        asm ("ldr r1, [sp,#32]"); // r8
        asm ("str r1, [r0,#32]");

        asm ("ldr r1, [sp,#36]"); // r9
        asm ("str r1, [r0,#36]");

        asm ("ldr r1, [sp,#40]"); // r10
        asm ("str r1, [r0,#40]");

        asm ("ldr r1, [sp,#44]"); // r11
        asm ("str r1, [r0,#44]");

        asm ("ldr r1, [sp,#48]"); // r12
        asm ("str r1, [r0,#48]");

        asm ("ldr r1, [sp,#52]"); // lr
        asm ("str r1, [r0,#56]");
    }

    static inline void EnableIrq(void) {
        asm volatile ("mrs r0,cpsr");
        asm volatile ("bic r0,r0,#0x80");
        asm volatile ("msr cpsr_c,r0");
    }

    static inline void DisableIrq(void) {
        asm volatile ("mrs r0,cpsr");
        asm volatile ("orr r0,r0,#0x80");
        asm volatile ("msr cpsr_c,r0");
    }

    static inline void EnableFiq(void) {
        asm volatile ("mrs r0,cpsr");
        asm volatile ("bic r0,r0,#0x40");
        asm volatile ("msr cpsr_c,r0");
    }

    static inline void DisableFiq(void) {
        asm volatile ("mrs r0,cpsr");
        asm volatile ("orr r0,r0,#0x40");
        asm volatile ("msr cpsr_c,r0");
    }

    static inline void EnableIrqFiq(void) {
        asm volatile ("mrs r0,cpsr");
        asm volatile ("bic r0,r0,#0xC0");
        asm volatile ("msr cpsr_c,r0");
    }

    static inline void DisableIrqFiq(void) {
        asm volatile ("mrs r0,cpsr");
        asm volatile ("orr r0,r0,#0xC0");
        asm volatile ("msr cpsr_c,r0");
    }

    static inline void EnableInt(void) {
        asm volatile("cpsie iaf");
    }

    static inline void DisableInt(void) {
        asm volatile("cpsid iaf");
    }

    static inline void WaitForInterrupt(void) {
        asm volatile("wfi");
    }

    static inline void DisableAll() {
        caArmCpu::DisableInt();
        caArmCpu::DisableIrqFiq();
    }

    static inline void EnableAll() {
        caArmCpu::EnableInt();
        caArmCpu::EnableIrqFiq();
    }


};

//// tempate to help 

template< typename T> inline T RegTo(caArmCpu::eReq control) {
    register T s;
    s.AsReg = caArmCpu::GetControl(control);
    return s;
}


#endif /* CPU_H */

