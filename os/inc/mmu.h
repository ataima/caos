#ifndef CA_MMU_HEADER
#define CA_MMU_HEADER

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


class caMMU {
public:
    static u32 Disable(void);
    static u32 Enable(void);
    static u32 CreateTTB(void);
    static u32 SetDomain(void);
    static u32 IsEnabled(void);

    static void SetTLBIALLIS(register u32) __attribute__((naked)) {
        asm volatile ("MCR p15,0,r0,c8,c3,0");
        asm volatile ("BX LR");
    }

    static void SetTLBIMVAIS(register u32) __attribute__((naked)) {
        asm volatile ("MCR p15,0,r0,c8,c3,1");
        asm volatile ("BX LR");
    }

    static void SetTLBIASIDIS(register u32) __attribute__((naked)) {
        asm volatile ("MCR p15,0,r0,c8,c3,2");
        asm volatile ("BX LR");
    }

    static void SetTLBIMVAAIS(register u32) __attribute__((naked)) {
        asm volatile ("MCR p15,0,r0,c8,c3,3");
        asm volatile ("BX LR");
    }

    static void SetITLBIALL(register u32) __attribute__((naked)) {
        asm volatile ("MCR p15,0,r0,c8,c5,0");
        asm volatile ("BX LR");
    }

    static void SetITLBIMVA(register u32) __attribute__((naked)) {
        asm volatile ("MCR p15,0,r0,c8,c5,1");
        asm volatile ("BX LR");
    }

    static void SetITLBIASID(register u32) __attribute__((naked)) {
        asm volatile ("MCR p15,0,r0,c8,c5,2");
        asm volatile ("BX LR");
    }

    static void SetDTLBIALL(register u32) __attribute__((naked)) {
        asm volatile ("MCR p15,0,r0,c8,c6,0");
        asm volatile ("BX LR");
    }

    static void SetDTLBIMVA(register u32) __attribute__((naked)) {
        asm volatile ("MCR p15,0,r0,c8,c6,1");
        asm volatile ("BX LR");
    }

    static void SetDTLBIASID(register u32) __attribute__((naked)) {
        asm volatile ("MCR p15,0,r0,c8,c6,2");
        asm volatile ("BX LR");
    }

    static void SetTLBIALL(register u32) __attribute__((naked)) {
        asm volatile ("MCR p15,0,r0,c8,c7,0");
        asm volatile ("BX LR");
    }

    static void SetTLBIMVA(register u32) __attribute__((naked)) {
        asm volatile ("MCR p15,0,r0,c8,c7,1");
        asm volatile ("BX LR");
    }

    static void SetTLBIASID(register u32) __attribute__((naked)) {
        asm volatile ("MCR p15,0,r0,c8,c7,2");
        asm volatile ("BX LR");
    }

    static void SetTLBIMVAA(register u32) __attribute__((naked)) {
        asm volatile ("MCR p15,0,r0,c8,c7,3");
        asm volatile ("BX LR");
    }

    static void SetTLBIALLHIS(register u32) __attribute__((naked)) {
        asm volatile ("MCR p15,4,r0,c8,c3,0");
        asm volatile ("BX LR");
    }

    static void SetTLBIMVAHIS(register u32) __attribute__((naked)) {
        asm volatile ("MCR p15,4,r0,c8,c3,1");
        asm volatile ("BX LR");
    }

    static void SetTLBIALLNSNHIS(register u32) __attribute__((naked)) {
        asm volatile ("MCR p15,4,r0,c8,c3,4");
        asm volatile ("BX LR");
    }

    static void SetTLBIALLH(register u32) __attribute__((naked)) {
        asm volatile ("MCR p15,4,r0,c8,c7,0");
        asm volatile ("BX LR");
    }

    static void SetTLBIMVAH(register u32) __attribute__((naked)) {
        asm volatile ("MCR p15,4,r0,c8,c7,1");
        asm volatile ("BX LR");
    }

    static void SetTLBIALLNSNH(register u32) __attribute__((naked)) {
        asm volatile ("MCR p15,4,r0,c8,c7,4");
        asm volatile ("BX LR");
    }

    static u32 Get32_TTBR0(void) __attribute__((naked)) {
        register u32 res = 0;
        asm volatile ("MRC p15,0,%0,c2,c0,0" : "=r" (res));
        asm volatile ("BX LR");
        return res;
    }

    static void Set32_TTBR0(register u32) __attribute__((naked)) {
        asm volatile ("MCR p15,0,r0,c2,c0,0");
        asm volatile ("BX LR");
    }

    static u32 Get64_TTBR0(register u32 & upper) __attribute__((naked)) {
        register u32 res = 0;
        asm volatile ("MRRC p15,0,%0,%1,c2" : "=r" (res) : "r" (upper));
        asm volatile ("BX LR");
        return res;
    }

    static void Set64_TTBR0(register u32, register u32) __attribute__((naked)) {
        asm volatile ("MCRR p15,0,r0,r1,c2");
        asm volatile ("BX LR");
    }

    static u32 Get32_TTBR1(void) __attribute__((naked)) {
        register u32 res = 0;
        asm volatile ("MRC p15,0,%0,c2,c0,1" : "=r" (res));
        asm volatile ("BX LR");
        return res;
    }

    static void Set32_TTBR1(register u32) __attribute__((naked)) {
        asm volatile ("MCR p15,0,r0,c2,c0,1");
        asm volatile ("BX LR");
    }

    static u32 Get64_TTBR1(register u32 & upper) __attribute__((naked)) {
        register u32 res = 0;
        asm volatile ("MRRC p15,1,%0,%1,c2" : "=r" (res) : "r" (upper));
        asm volatile ("BX LR");
        return res;
    }

    static void Set64_TTBR1(register u32, register u32) __attribute__((naked)) {
        asm volatile ("MCRR p15,1,r0,r1,c2");
        asm volatile ("BX LR");
    }

    static u32 GetTTBCR(void) __attribute__((naked)) {
        register u32 res = 0;
        asm volatile ("MRC p15,0,%0,c2,c0,2" : "=r" (res));
        asm volatile ("BX LR");
        return res;
    }

    static void SetTTBCR(register u32) __attribute__((naked)) {
        asm volatile ("MCR p15,0,r0,c2,c0,2");
        asm volatile ("BX LR");
    }

    static u32 GetHTCR(void) __attribute__((naked)) {
        register u32 res = 0;
        asm volatile ("MRC p15,4,%0,c2,c0,2" : "=r" (res));
        asm volatile ("BX LR");
        return res;
    }

    static void SetHTCR(register u32) __attribute__((naked)) {
        asm volatile ("MCR p15,4,r0,c2,c0,2");
        asm volatile ("BX LR");
    }

    static u32 GetVTCR(void) __attribute__((naked)) {
        register u32 res = 0;
        asm volatile ("MRC p15,4,%0,c2,c1,2" : "=r" (res));
        asm volatile ("BX LR");
        return res;
    }

    static void SetVTCR(register u32) __attribute__((naked)) {
        asm volatile ("MCR p15,4,r0,c2,c1,2");
        asm volatile ("BX LR");
    }

    static u32 Get64_HTTBR(u32 & upper) __attribute__((naked)) {
        register u32 res = 0;
        asm volatile ("MRRC p15,4,%0,%1,c2" : "=r" (res) : "r" (upper));
        asm volatile ("BX LR");
        return res;
    }

    static void Set64_HTTBR(register u32, register u32) __attribute__((naked)) {
        asm volatile ("MCRR p15,4,r0,r1,c2");
        asm volatile ("BX LR");
    }

    static u32 Get64_VTTBR(u32 & upper) __attribute__((naked)) {
        register u32 res = 0;
        asm volatile ("MRRC p15,6,%0,%1,c2" : "=r" (res) : "r" (upper));
        asm volatile ("BX LR");
        return res;
    }

    static void Set64_VTTBR(register u32, register u32) __attribute__((naked)) {
        asm volatile ("MCRR p15,6,r0,r1,c2");
        asm volatile ("BX LR");
    }

    static u32 GetTLBType(void) __attribute__((naked)) {
        register u32 res = 0;
        asm volatile ("MRC p15,0,%0,c0,c0,3" : "=r" (res));
        asm volatile ("BX LR");
        return res;
    }


};


#endif 
