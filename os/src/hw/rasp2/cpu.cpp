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


#include "interrupt.h"
#include "miniuart.h"
#include "cpu.h"
#include "coprocessor.h"


CpuRegs cpu_reg;

void caArmCpu::SetControl(eReq v, u32 res) {
    switch (v) {
        case nsacr:
            asm volatile ("MCR p15,0,%0,c1,c1,2" : "=r" (res));
            break;
        default:break;
    }
}

u32 caArmCpu::GetControl(eReq v) {
    u32 res = 0;
    switch (v) {
        case nsacr:
            asm volatile ("MRC p15,0,%0,c1,c1,2" : "=r" (res));
            break;
    }
    return res;
}

void caArmCpu::Dump(u32 *p) {
    u32 i;
    Dbg::Put("Dump : \r\n");
    for (i = 0; i < 32; i += 4) {
        Dbg::Put("> ", (u32) p, Dbg::kformat::hex, false);
        Dbg::Put(" - ", *p++, Dbg::kformat::hex, false);
        Dbg::Put(" ", *p++, Dbg::kformat::hex, false);
        Dbg::Put(" ", *p++, Dbg::kformat::hex, false);
        Dbg::Put(" ", *p++, Dbg::kformat::hex);
    }
}

void caArmCpu::GetRegs(CpuRegs *regs) {
    if (regs != nullptr) {
        asm volatile ("STMFD    sp!, {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}");
        asm volatile ("STMFD    sp!, {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}");
        regs->r[15] = (u32) GetLinkRegisterR14() - 4; // pc to call GeRegs
        regs->r[13] = (u32) GetStackPointerR13() + 112; // sp before 2*56 to call GeRegs
        asm volatile ("LDMFD    sp!, {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}");
        CopyRegsFromStack();
        asm volatile ("LDMFD    sp!, {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}");
    }
}

void caArmCpu::GetMainIdCpuInfo(void) {
    MainIdRegister mid;
    mid.asReg = caArmCprs::GetMainId();
    switch (mid.asBit.manft) {
        case 0x41:
            Dbg::Put("@CPU : ARM Limited");
            break;
        case 0x44:
            Dbg::Put("@CPU : DEC");
            break;
        case 0x4d:
            Dbg::Put("@CPU : FreeScale");
            break;
        case 0x51:
            Dbg::Put("@CPU : QualComm");
            break;
        case 0x56:
            Dbg::Put("@CPU : Marvell");
            break;
        case 0x69:
            Dbg::Put("@CPU : Intel Corporation");
            break;
        default:
            Dbg::Put("@CPU : MFT = ", mid.asBit.manft, Dbg::kformat::dec, false);
            break;
    }
    switch (mid.asBit.arch) {
        case 1:
            Dbg::Put(" ARV4");
            break;
        case 2:
            Dbg::Put(" ARV4T");
            break;
        case 3:
            Dbg::Put(" ARV5");
            break;
        case 4:
            Dbg::Put(" ARV5T");
            break;
        case 5:
            Dbg::Put(" ARV5TE");
            break;
        case 6:
            Dbg::Put(" ARV5TEJ");
            break;
        case 7:
            Dbg::Put(" ARV6");
            break;
        case 15:
            Dbg::Put(" ARV7");
            break;
    }
    Dbg::Put(" V:", mid.asBit.variant, Dbg::kformat::dec, false);
    Dbg::Put(" N:", mid.asBit.part_number, Dbg::kformat::dec, false);
    Dbg::Put(" R:", mid.asBit.revision, Dbg::kformat::dec, false);
    Dbg::Put("\r\n");

}

void caArmCpu::DumpRegs(CpuRegs * regs) {
    Dbg::Put("CPU REGS :\r\n");
    Dbg::Put("  R0     = ", regs->r[0],Dbg::kformat::hex, false);
    Dbg::Put("  R1     = ", regs->r[1],Dbg::kformat::hex, false);
    Dbg::Put("  R2     = ", regs->r[2],Dbg::kformat::hex, false);
    Dbg::Put("  R3     = ", regs->r[3]);
    Dbg::Put("  R4     = ", regs->r[4],Dbg::kformat::hex, false);
    Dbg::Put("  R5     = ", regs->r[5],Dbg::kformat::hex, false);
    Dbg::Put("  R6     = ", regs->r[6],Dbg::kformat::hex, false);
    Dbg::Put("  R7     = ", regs->r[7]);
    Dbg::Put("  R8     = ", regs->r[8],Dbg::kformat::hex, false);
    Dbg::Put("  R9     = ", regs->r[9],Dbg::kformat::hex, false);
    Dbg::Put("  R10    = ", regs->r[10],Dbg::kformat::hex, false);
    Dbg::Put("  R11    = ", regs->r[11]);
    Dbg::Put("  R12    = ", regs->r[12],Dbg::kformat::hex, false);
    Dbg::Put("  R13(SP)= ", regs->r[13],Dbg::kformat::hex, false);
    Dbg::Put("  R14(LR)= ", regs->r[14],Dbg::kformat::hex, false);
    Dbg::Put("  R15(PC)= ", regs->r[15]);
    //Dbg::Put("  <-- Call from here !\r\n");
}

void caArmCpu::DumpRegs() {
    CpuRegs reg;
    GetRegs(&reg);
    DumpRegs(&reg);
}



void caArmCpu::DumpCPSR(void) {
    u32 st = caArmCpu::GetCPSR()&0x1f;
    switch (st) {
        case 0x10: Dbg::Put("User Mode");
            break;
        case 0x11: Dbg::Put("Fiq Mode");
            break;
        case 0x12: Dbg::Put("Irq Mode");
            break;
        case 0x13: Dbg::Put("Supervisor Mode");
            break;
        case 0x16: Dbg::Put("Monitor Mode");
            break;
        case 0x17: Dbg::Put("Abort Mode");
            break;
        case 0x1a: Dbg::Put("Hypervisor Mode");
            break;
        case 0x1b: Dbg::Put("Undefined Mode");
            break;
        case 0x1f: Dbg::Put("System Mode");
            break;
        default: Dbg::Put("?=", st);
            break;
    }
}

extern "C" void jump_to(u32);

void caArmCpu::JumpTo(u32 addr) {
    jump_to(addr);
}

void caArmCpu::CopyRegsFromStack(u32 *ptr, CpuRegs *regs) {
    regs->r[0] = *ptr++;
    regs->r[1] = *ptr++;
    regs->r[2] = *ptr++;
    regs->r[3] = *ptr++;
    regs->r[4] = *ptr++;
    regs->r[5] = *ptr++;
    regs->r[6] = *ptr++;
    regs->r[7] = *ptr++;
    regs->r[8] = *ptr++;
    regs->r[10] = *ptr++;
    regs->r[11] = *ptr++;
    regs->r[12] = *ptr++;
    regs->r[14] = *ptr++;
}

void caArmCpu::CopyRegsToStack(u32 *ptr, CpuRegs *regs) {
    *ptr++ = regs->r[0];
    *ptr++ = regs->r[1];
    *ptr++ = regs->r[2];
    *ptr++ = regs->r[3];
    *ptr++ = regs->r[4];
    *ptr++ = regs->r[5];
    *ptr++ = regs->r[6];
    *ptr++ = regs->r[7];
    *ptr++ = regs->r[8];
    *ptr++ = regs->r[10];
    *ptr++ = regs->r[11];
    *ptr++ = regs->r[12];
    *ptr++ = regs->r[14];
}


