#ifndef CA_CACHE_HEADER
#define CA_CACHE_HEADER
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

class caCache {
public:
    static u32 Init(void);

    static u32 Start(void);

    static u32 Stop(void);

    static u32 IsEnabled(void);

    static u32 GetCacheType(void) __attribute__((naked)) {
        register u32 res = 0;
        asm volatile ("MRC p15,0,%0,c0,c0,1" : "=r" (res));
        asm volatile ("BX LR");
        return res;
    }

    static u32 GetCacheSizeId(void) __attribute__((naked)) {
        register u32 res = 0;
        asm volatile ("MRC p15,1,%0,c0,c0,0" : "=r" (res));
        asm volatile ("BX LR");
        return res;
    }

    static u32 GetCacheLevelId(void) __attribute__((naked)) {
        register u32 res = 0;
        asm volatile ("MRC p15,1,%0,c0,c0,1" : "=r" (res));
        asm volatile ("BX LR");
        return res;
    }

    static u32 GetCacheSizeSelection(void) __attribute__((naked)) {
        register u32 res = 0;
        asm volatile ("MRC p15,2,%0,c0,c0,0" : "=r" (res));
        asm volatile ("BX LR");
        return res;
    }

    static void SetCacheSizeSelection(register u32) __attribute__((naked)) {
        asm volatile ("MCR p15,2,r0,c0,c0,0");
        asm volatile ("BX LR");
    }

    static void SetBPIALL(register u32) __attribute__((naked)) {
        asm volatile ("MCR p15,0,r0,c7,c5,6");
        asm volatile ("BX LR");
    }

    static void SetBPIALLIS(register u32) __attribute__((naked)) {
        asm volatile ("MCR p15,0,r0,c7,c1,6");
        asm volatile ("BX LR");
    }

    static void SetBPIMVA(register u32) __attribute__((naked)) {
        asm volatile ("MCR p15,0,r0,c7,c5,7");
        asm volatile ("BX LR");
    }

    static void SetDCCIMVAC(register u32) __attribute__((naked)) {
        asm volatile ("MCR p15,0,r0,c7,c14,1");
        asm volatile ("BX LR");
    }

    static void SetDCCISW(register u32) __attribute__((naked)) {
        asm volatile ("MCR p15,0,r0,c7,c14,2");
        asm volatile ("BX LR");
    }

    static void SetDCCMVAC(register u32) __attribute__((naked)) {
        asm volatile ("MCR p15,0,r0,c7,c10,1");
        asm volatile ("BX LR");
    }

    static void SetDCCMVAU(register u32) __attribute__((naked)) {
        asm volatile ("MCR p15,0,r0,c7,c11,1");
        asm volatile ("BX LR");
    }

    static void SetDCCSW(register u32) __attribute__((naked)) {
        asm volatile ("MCR p15,0,r0,c7,c10,2");
        asm volatile ("BX LR");
    }

    static void SetDCIMVAC(register u32) __attribute__((naked)) {
        asm volatile ("MCR p15,0,r0,c7,c6,1");
        asm volatile ("BX LR");
    }

    static void SetDCISW(register u32) __attribute__((naked)) {
        asm volatile ("MCR p15,0,r0,c7,c6,2");
        asm volatile ("BX LR");
    }

    static void SetICIALLU(register u32) __attribute__((naked)) {
        asm volatile ("MCR p15,0,r0,c7,c5,0");
        asm volatile ("BX LR");
    }

    static void SetICIALLUIS(register u32) __attribute__((naked)) {
        asm volatile ("MCR p15,0,r0,c7,c1,0");
        asm volatile ("BX LR");
    }

    static void SetICIMVAU(register u32) __attribute__((naked)) {
        asm volatile ("MCR p15,0,r0,c7,c5,1");
        asm volatile ("BX LR");
    }

    static u32 GetL2Control(void) __attribute__((naked)) {
        register u32 res = 0;
        asm volatile ("MRC p15,1,r0,c9,c0,2" : "=r" (res));
        asm volatile ("BX LR");
        return res;
    }

    static u32 GetL2ExtendedControl(void) __attribute__((naked)) {
        register u32 res = 0;
        asm volatile ("MRC p15,1,r0,c9,c0,3" : "=r" (res));
        asm volatile ("BX LR");
        return res;
    }

    static void SetL2ExtendedControl(register u32) __attribute__((naked)) {
        asm volatile ("MCR p15,1,r0,c9,c0,3");
        asm volatile ("BX LR");
    }
};



#endif 

