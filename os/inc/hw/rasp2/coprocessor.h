#ifndef CA_ARMCOPR_HEADER
#define CA_ARMCOPR_HEADER
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

//To survive with coprocessor
#include "bcm2836.h"
#include "stream.h"

class caArmCprs {
public:
    // ABOUT CODE !!
    // explain return value is on R0 : bx lr  -> pc=lr : return 0 silente warning 
    static u32 GetMainId(void) __attribute__ ((naked)) {
        asm volatile ("MRC p15,0,r0,c0,c0,0");
        asm volatile ("BX LR");
        return 0;
    }

    static u32 GetTCMType(void) __attribute__ ((naked)) {
        asm volatile ("MRC p15,0,r0,c0,c0,2");
        asm volatile ("BX LR");
        return 0;
    }

    static u32 GetMultiprocessorAffinity(void) __attribute__ ((naked)) {
        asm volatile ("MRC p15,0,r0,c0,c0,5");
        asm volatile ("BX LR");
        return 0;
    }

    static u32 GetRevisionId(void) __attribute__ ((naked)) {
        asm volatile ("MRC p15,0,r0,c0,c0,6");
        asm volatile ("BX LR");
        return 0;
    }

    static u32 GetProcessorFeature0(void) __attribute__ ((naked)) {
        asm volatile ("MRC p15,0,r0,c0,c1,0");
        asm volatile ("BX LR");
        return 0;
    }

    static u32 GetProcessorFeature1(void) __attribute__ ((naked)) {
        asm volatile ("MRC p15,0,r0,c0,c1,1");
        asm volatile ("BX LR");
        return 0;
    }

    static u32 GetDebugFeature0(void) __attribute__ ((naked)) {
        asm volatile ("MRC p15,0,r0,c0,c1,2");
        asm volatile ("BX LR");
        return 0;
    }

    static u32 GetAuxiliaryuFeature0(void) __attribute__ ((naked)) {
        asm volatile ("MRC p15,0,r0,c0,c1,3");
        asm volatile ("BX LR");
        return 0;
    }

    static u32 GetMemoryModeluFeature0(void) __attribute__ ((naked)) {
        asm volatile ("MRC p15,0,r0,c0,c1,4");
        asm volatile ("BX LR");
        return 0;
    }

    static u32 GetMemoryModeluFeature1(void) __attribute__ ((naked)) {
        asm volatile ("MRC p15,0,r0,c0,c1,5");
        asm volatile ("BX LR");
        return 0;
    }

    static u32 GetMemoryModeluFeature2(void) __attribute__ ((naked)) {
        asm volatile ("MRC p15,0,r0,c0,c1,6");
        asm volatile ("BX LR");
        return 0;
    }

    static u32 GetMemoryModeluFeature3(void) __attribute__ ((naked)) {
        asm volatile ("MRC p15,0,r0,c0,c1,7");
        asm volatile ("BX LR");
        return 0;
    }

    static u32 GetInstructionSetAttribute0(void) __attribute__ ((naked)) {
        asm volatile ("MRC p15,0,r0,c0,c2,0");
        asm volatile ("BX LR");
        return 0;
    }

    static u32 GetInstructionSetAttribute1(void) __attribute__ ((naked)) {
        asm volatile ("MRC p15,0,r0,c0,c2,1");
        asm volatile ("BX LR");
        return 0;
    }

    static u32 GetInstructionSetAttribute2(void) __attribute__ ((naked)) {
        asm volatile ("MRC p15,0,r0,c0,c2,2");
        asm volatile ("BX LR");
        return 0;
    }

    static u32 GetInstructionSetAttribute3(void) __attribute__ ((naked)) {
        asm volatile ("MRC p15,0,r0,c0,c2,3");
        asm volatile ("BX LR");
        return 0;
    }

    static u32 GetInstructionSetAttribute4(void) __attribute__ ((naked)) {
        asm volatile ("MRC p15,0,r0,c0,c2,4");
        asm volatile ("BX LR");
        return 0;
    }

    static u32 GetInstructionSetAttribute5(void) __attribute__ ((naked)) {
        asm volatile ("MRC p15,0,r0,c0,c2,5");
        asm volatile ("BX LR");
        return 0;
    }

    static u32 GetAuxiliaryId(void) __attribute__ ((naked)) {
        asm volatile ("MRC p15,1,r0,c0,c0,2");
        asm volatile ("BX LR");
        return 0;
    }

    static u32 GetVirtualizationProcessorId(void) __attribute__ ((naked)) {
        asm volatile ("MRC p15,4,r0,c0,c0,0");
        asm volatile ("BX LR");
        return 0;
    }

    static void SetVirtualizationProcessorId(register u32) __attribute__ ((naked)) {
        asm volatile ("MCR p15,4,r0,c0,c0,0");
        asm volatile ("BX LR");
    }

    static u32 GetVirtualizationMultiProcessorId(void) __attribute__ ((naked)) {
        asm volatile ("MRC p15,4,r0,c0,c0,5");
        asm volatile ("BX LR");
        return 0;
    }

    static void SetVirtualizationMultiProcessorId(register u32) __attribute__ ((naked)) {
        asm volatile ("MCR p15,4,r0,c0,c0,5");
        asm volatile ("BX LR");
    }

    static u32 GetSystemControl(void)__attribute__ ((naked)) __attribute__ ((naked)) {
        asm volatile ("MRC p15,0,r0,c1,c0,0");
        asm volatile ("BX LR");
        return 0;
    }

    static void SetSystemControl(register u32) __attribute__ ((naked)) __attribute__ ((naked)) {
        asm volatile ("MCR p15,0,r0,c1,c0,0");
        asm volatile ("BX LR");
    }

    static u32 GetPrimaryRegionRemap(void) __attribute__ ((naked)) {
        asm volatile ("MRC p15,0,r0,c10,c2,0");
        asm volatile ("BX LR");
        return 0;
    }

    static void SetPrimaryRegionRemap(register u32) __attribute__ ((naked)) {
        asm volatile ("MCR p15,0,r0,c10,c2,0");
        asm volatile ("BX LR");
    }

    static u32 GetMAIR_0(void) __attribute__ ((naked)) {
        asm volatile ("MRC p15,0,r0,c10,c2,0");
        asm volatile ("BX LR");
        return 0;
    }

    static void SetMAIR_0(register u32) __attribute__ ((naked)) {
        asm volatile ("MCR p15,0,r0,c10,c2,0");
        asm volatile ("BX LR");
    }

    static u32 GetMAIR_1(void) __attribute__ ((naked)) {
        asm volatile ("MRC p15,0,r0,c10,c2,1");
        asm volatile ("BX LR");
        return 0;
    }

    static void SetMAIR_1(register u32) __attribute__ ((naked)) {
        asm volatile ("MCR p15,0,r0,c10,c2,1");
        asm volatile ("BX LR");
    }

    static u32 GetNormalMemoryRemap(void) __attribute__ ((naked)) {
        asm volatile ("MRC p15,0,r0,c10,c2,1");
        asm volatile ("BX LR");
        return 0;
    }

    static void SetNormalMemoryRemap(register u32) __attribute__ ((naked)) {
        asm volatile ("MCR p15,0,r0,c10,c2,1");
        asm volatile ("BX LR");
    }

    static u32 GetAuxiliaryControl(void) __attribute__ ((naked)) {
        asm volatile ("MRC p15,0,r0,c1,c0,1");
        asm volatile ("BX LR");
        return 0;
    }

    static void SetAuxiliaryControl(register u32) __attribute__ ((naked)) {
        asm volatile ("MCR p15,0,r0,c1,c0,1");
        asm volatile ("BX LR");
    }

    static u32 GetCoprocessorAccessControl(void) __attribute__ ((naked)) {
        asm volatile ("MRC p15,0,r0,c1,c0,2");
        asm volatile ("BX LR");
        return 0;
    }

    static void SetCoprocessorAccessControl(register u32) __attribute__ ((naked)) {
        asm volatile ("MCR p15,0,r0,c1,c0,2");
        asm volatile ("BX LR");
    }

    static u32 GetSecureConfiguration(void) __attribute__ ((naked)) {
        asm volatile ("MRC p15,0,r0,c1,c1,0");
        asm volatile ("BX LR");
        return 0;
    }

    static void SetSecureConfiguration(register u32) __attribute__ ((naked)) {
        asm volatile ("MCR p15,0,r0,c1,c1,0");
        asm volatile ("BX LR");
    }

    static u32 GetNonSecureAccessControl(void) __attribute__ ((naked)) {
        asm volatile ("MRC p15,0,r0,c1,c1,2");
        asm volatile ("BX LR");
        return 0;
    }

    static void SetNonSecureAccessControl(register u32) __attribute__ ((naked)) {
        asm volatile ("MCR p15,0,r0,c1,c1,2");
        asm volatile ("BX LR");
    }

    static u32 GetHypSystemControl(void) __attribute__ ((naked)) {
        asm volatile ("MRC p15,4,r0,c1,c0,0");
        asm volatile ("BX LR");
        return 0;
    }

    static void SetHypSystemControl(register u32) __attribute__ ((naked)) {
        asm volatile ("MCR p15,4,r0,c1,c0,0");
        asm volatile ("BX LR");
    }

    static u32 GetHypDebugControl(void) __attribute__ ((naked)) {
        asm volatile ("MRC p15,4,r0,c1,c1,1");
        asm volatile ("BX LR");
        return 0;
    }

    static void SetHypDebugControl(register u32) __attribute__ ((naked)) {
        asm volatile ("MCR p15,4,r0,c1,c1,1");
        asm volatile ("BX LR");
    }

    static u32 GetHypCoprocessorTrap(void) __attribute__ ((naked)) {
        asm volatile ("MRC p15,4,r0,c1,c1,2");
        asm volatile ("BX LR");
        return 0;
    }

    static void SetHypCoprocessorTrap(register u32) __attribute__ ((naked)) {
        asm volatile ("MCR p15,4,r0,c1,c1,2");
        asm volatile ("BX LR");
    }

    static u32 GetConfigurationBaseAddress(void) __attribute__ ((naked)) {
        asm volatile ("MRC p15,4,r0,c15,c0,0");
        asm volatile ("BX LR");
        return 0;
    }

    static u32 GetDACR(void) __attribute__ ((naked)) {
        asm volatile ("MRC p15,0,r0,c3,c0,0");
        asm volatile ("BX LR");
        return 0;
    }

    static void SetDACR(register u32) __attribute__ ((naked)) {
        asm volatile ("MCR p15,0,r0,c3,c0,0");
        asm volatile ("BX LR");
    }

    static u32 GetDFSR(void) __attribute__ ((naked)) {
        asm volatile ("MRC p15,0,r0,c5,c0,0");
        asm volatile ("BX LR");
        return 0;
    }

    static void SetDFSR(register u32) __attribute__ ((naked)) {
        asm volatile ("MCR p15,0,r0,c5,c0,0");
        asm volatile ("BX LR");
    }

    static u32 GetIFSR(void) __attribute__ ((naked)) {
        asm volatile ("MRC p15,0,r0,c5,c0,1");
        asm volatile ("BX LR");
        return 0;
    }

    static void SetIFSR(register u32) __attribute__ ((naked)) {
        asm volatile ("MCR p15,0,r0,c5,c0,1");
        asm volatile ("BX LR");
    }

    static u32 GetADFSR(void) __attribute__ ((naked)) {
        asm volatile ("MRC p15,0,r0,c5,c1,0");
        asm volatile ("BX LR");
        return 0;
    }

    static void SetADFSR(register u32) __attribute__ ((naked)) {
        asm volatile ("MCR p15,0,r0,c5,c1,0");
        asm volatile ("BX LR");
    }

    static u32 GetAIFSR(void) __attribute__ ((naked)) {
        asm volatile ("MRC p15,0,r0,c5,c1,1");
        asm volatile ("BX LR");
        return 0;
    }

    static void SetAIFSR(register u32) __attribute__ ((naked)) {
        asm volatile ("MCR p15,0,r0,c5,c1,1");
        asm volatile ("BX LR");
    }

    static u32 GetHADFSR(void) __attribute__ ((naked)) {
        asm volatile ("MRC p15,4,r0,c5,c1,0");
        asm volatile ("BX LR");
        return 0;
    }

    static void SetHADFSR(register u32) __attribute__ ((naked)) {
        asm volatile ("MCR p15,4,r0,c5,c1,0");
        asm volatile ("BX LR");
    }

    static u32 GetHAIFSR(void) __attribute__ ((naked)) {
        asm volatile ("MRC p15,4,r0,c5,c1,1");
        asm volatile ("BX LR");
        return 0;
    }

    static void SetHAIFSR(register u32) __attribute__ ((naked)) {
        asm volatile ("MCR p15,4,r0,c5,c1,1");
        asm volatile ("BX LR");
    }

    static u32 GetHSR(void) __attribute__ ((naked)) {
        asm volatile ("MRC p15,4,r0,c5,c2,0");
        asm volatile ("BX LR");
        return 0;
    }

    static void SetHSR(register u32) __attribute__ ((naked)) {
        asm volatile ("MCR p15,4,r0,c5,c2,0");
        asm volatile ("BX LR");
    }

    static u32 GetDFAR(void) __attribute__ ((naked)) {
        asm volatile ("MRC p15,0,r0,c6,c0,0");
        asm volatile ("BX LR");
        return 0;
    }

    static void SetDFAR(register u32) __attribute__ ((naked)) {
        asm volatile ("MCR p15,0,r0,c6,c0,0");
        asm volatile ("BX LR");
    }

    static u32 GetIFAR(void) __attribute__ ((naked)) {
        asm volatile ("MRC p15,0,r0,c6,c0,2");
        asm volatile ("BX LR");
        return 0;
    }

    static void SetIFAR(register u32) __attribute__ ((naked)) {
        asm volatile ("MCR p15,0,r0,c6,c0,2");
        asm volatile ("BX LR");
    }

    static u32 GetHDFAR(void) __attribute__ ((naked)) {
        asm volatile ("MRC p15,4,r0,c6,c0,0");
        asm volatile ("BX LR");
        return 0;
    }

    static void SetHDFAR(register u32) __attribute__ ((naked)) __attribute__ ((naked)) {
        asm volatile ("MCR p15,4,r0,c6,c0,0");
        asm volatile ("BX LR");
    }

    static u32 GetHIFAR(void) __attribute__ ((naked)) {
        asm volatile ("MRC p15,4,r0,c6,c0,2");
        asm volatile ("BX LR");
        return 0;
    }

    static void SetHIFAR(register u32) __attribute__ ((naked)) {
        asm volatile ("MCR p15,4,r0,c6,c0,2");
        asm volatile ("BX LR");
    }

    static u32 GetHPFAR(void) __attribute__ ((naked)) {
        asm volatile ("MRC p15,4,r0,c6,c0,4");
        asm volatile ("BX LR");
        return 0;
    }

    static void SetHPFAR(register u32) __attribute__ ((naked)) {
        asm volatile ("MCR p15,4,r0,c6,c0,4");
        asm volatile ("BX LR");
    }

    static u32   DumpDFSR(caStringStream<s8> & ss);
    static u32   DumpISR(caStringStream<s8> & ss);
    static void  DumpDFSR();
    
};

typedef union tag_dfsr {

    struct tag_dfsr_bit {
        _RW_ u32 fault_status : 4; // bit0..3
        _RW_ u32 domain : 4; //bit 4..7
        _RW_ u32 reserved : 2; //bit 8
        _RW_ u32 raz : 1; // bit9
        _RW_ u32 most_fault_bit : 1; // bit10
        _RW_ u32 wnr : 1; // bit11
        _RW_ u32 ext : 1; // bit12
        _RW_ u32 cache_m : 1; // bit13
        _RW_ u32 unused : 18; // bit14-31
    } asBit;
    _RW_ u32 asReg;
} cprs_dfsr;


#endif /* CAARMCOPR_H */

