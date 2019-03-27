////////////////////////////////////////////////////////////////////////////////
//    Copyright (C) 2019  Angelo Coppi (angelogkcop at hotmail.com )
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

#include "imx6.h"

#include "cpu.h"


CpuRegs cpu_reg;

using namespace caArmCpu {

void SetControl(eReq v, u32 res) {
    switch (v) {
        case nsacr:
            asm volatile ("MCR p15,0,%0,c1,c1,2" : "=r" (res));
            break;
        default:break;
    }
}

u32 GetControl(eReq v) {
    u32 res = 0;
    switch (v) {
        case nsacr:
            asm volatile ("MRC p15,0,%0,c1,c1,2" : "=r" (res));
            break;
    }
    return res;
}

void Dump(u32 *p) {
    u32 i;
    printf("Dump : \r\n");
    for (i = 0; i < 32; i += 4) {
        printf("> %0x08x", (u32) p );
        printf(" - %0x08x", *p++);
        printf(" %0x08x", *p++);
        printf(" %0x08x", *p++);
        printf(" %0x08x", *p++);
    }
}

void GetRegs(CpuRegs *regs) {
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

void GetMainIdCpuInfo(void) {
    MainIdRegister mid;
    printf("> c.a.O.S. : [ ");
    mid.asReg = GetMainId();
    switch (mid.asBit.manft) {
        case 0x41:
            printf("CPU : ARM Limited");
            break;
        case 0x44:
            printf("CPU : DEC");
            break;
        case 0x4d:
            printf("CPU : FreeScale");
            break;
        case 0x51:
            printf("CPU : QualComm");
            break;
        case 0x56:
            printf("CPU : Marvell");
            break;
        case 0x69:
            printf("CPU : Intel Corporation");
            break;
        default:
            printf("CPU : MFT = ", mid.asBit.manft, Dbg::kformat::dec, false);
            break;
    }
    switch (mid.asBit.arch) {
        case 1:
            printf(" ARV4");
            break;
        case 2:
            printf(" ARV4T");
            break;
        case 3:
            printf(" ARV5");
            break;
        case 4:
            printf(" ARV5T");
            break;
        case 5:
            printf(" ARV5TE");
            break;
        case 6:
            printf(" ARV5TEJ");
            break;
        case 7:
            printf(" ARV6");
            break;
        case 15:
            printf(" ARV7");
            break;
    }
    printf(" V:%d", mid.asBit.variant);
    printf(" N:%d", mid.asBit.part_number);
    printf(" R:%d", mid.asBit.revision);
    printf(" ]\r\n")
}

void DumpRegs(CpuRegs * regs) {
    printf("CPU REGS :\r\n");
    printf("R0      = %0x08x : %d\r\n", regs->r[0],regs->r[0]);
    printf("R1      = %0x08x : %d\r\n", regs->r[1],regs->r[1]);
    printf("R2      = %0x08x : %d\r\n", regs->r[2],regs->r[2]);
    printf("R3      = %0x08x : %d\r\n", regs->r[3],regs->r[3]);
    printf("R4      = %0x08x : %d\r\n", regs->r[4],regs->r[4]);
    printf("R5      = %0x08x : %d\r\n", regs->r[5],regs->r[5]);
    printf("R6      = %0x08x : %d\r\n", regs->r[6],regs->r[6]);
    printf("R7      = %0x08x : %d\r\n", regs->r[7],regs->r[7]);
    printf("R8      = %0x08x : %d\r\n", regs->r[8],regs->r[8]);
    printf("R9      = %0x08x : %d\r\n", regs->r[9],regs->r[9]);
    printf("R10     = %0x08x : %d\r\n", regs->r[10],regs->r[10]);
    printf("R11     = %0x08x : %d\r\n", regs->r[11],regs->r[11]);
    printf("R12     = %0x08x : %d\r\n", regs->r[12],regs->r[12]);
    printf("R13(SP) = %0x08x : %d\r\n", regs->r[13],regs->r[13]);
    printf("R14(LR) = %0x08x : %d\r\n", regs->r[14],regs->r[14]);
    printf("R15(PC) = %0x08x : %d\r\n", regs->r[15],regs->r[15]);
    printf("  <-- Call from here !\r\n");
}

void DumpCPSR(void) {
    u32 st = GetCPSR()&0x1f;
    switch (st) {
        case 0x10: printf("User Mode");
            break;
        case 0x11: printf("Fiq Mode");
            break;
        case 0x12: printf("Irq Mode");
            break;
        case 0x13: printf("Supervisor Mode");
            break;
        case 0x16: printf("Monitor Mode");
            break;
        case 0x17: printf("Abort Mode");
            break;
        case 0x1a: printf("Hypervisor Mode");
            break;
        case 0x1b: printf("Undefined Mode");
            break;
        case 0x1f: printf("System Mode");
            break;
        default: printf("?=", st);
            break;
    }
}


void JumpTo(u32 addr) {
    jump_to(addr);
}

void CopyRegsFromStack(u32 *ptr, CpuRegs *regs) {
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

void CopyRegsToStack(u32 *ptr, CpuRegs *regs) {
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

}
