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

class caArmCprs {
public:

    static u32 GetMainId(void) __attribute__((naked)) {
        register u32 res = 0;
        asm volatile ("MRC p15,0,%0,c0,c0,0" : "=r" (res));
        asm volatile ("BX LR");
        return res;
    }

    static u32 GetTCMType(void) __attribute__((naked)) {
        register u32 res = 0;
        asm volatile ("MRC p15,0,%0,c0,c0,2" : "=r" (res));
        asm volatile ("BX LR");
        return res;
    }

    static u32 GetMultiprocessorAffinity(void) __attribute__((naked)) {
        register u32 res = 0;
        asm volatile ("MRC p15,0,%0,c0,c0,5" : "=r" (res));
        asm volatile ("BX LR");
        return res;
    }

    static u32 GetRevisionId(void) __attribute__((naked)) {
        register u32 res = 0;
        asm volatile ("MRC p15,0,%0,c0,c0,6" : "=r" (res));
        asm volatile ("BX LR");
        return res;
    }

    static u32 GetProcessorFeature0(void) __attribute__((naked)) {
        register u32 res = 0;
        asm volatile ("MRC p15,0,%0,c0,c1,0" : "=r" (res));
        asm volatile ("BX LR");
        return res;
    }

    static u32 GetProcessorFeature1(void) __attribute__((naked)) {
        register u32 res = 0;
        asm volatile ("MRC p15,0,%0,c0,c1,1" : "=r" (res));
        asm volatile ("BX LR");
        return res;
    }

    static u32 GetDebugFeature0(void) __attribute__((naked)) {
        register u32 res = 0;
        asm volatile ("MRC p15,0,%0,c0,c1,2" : "=r" (res));
        asm volatile ("BX LR");
        return res;
    }

    static u32 GetAuxiliaryuFeature0(void) __attribute__((naked)) {
        register u32 res = 0;
        asm volatile ("MRC p15,0,%0,c0,c1,3" : "=r" (res));
        asm volatile ("BX LR");
        return res;
    }

    static u32 GetMemoryModeluFeature0(void) __attribute__((naked)) {
        register u32 res = 0;
        asm volatile ("MRC p15,0,%0,c0,c1,4" : "=r" (res));
        asm volatile ("BX LR");
        return res;
    }

    static u32 GetMemoryModeluFeature1(void) __attribute__((naked)) {
        register u32 res = 0;
        asm volatile ("MRC p15,0,%0,c0,c1,5" : "=r" (res));
        asm volatile ("BX LR");
        return res;
    }

    static u32 GetMemoryModeluFeature2(void) __attribute__((naked)) {
        register u32 res = 0;
        asm volatile ("MRC p15,0,%0,c0,c1,6" : "=r" (res));
        asm volatile ("BX LR");
        return res;
    }

    static u32 GetMemoryModeluFeature3(void) __attribute__((naked)) {
        register u32 res = 0;
        asm volatile ("MRC p15,0,%0,c0,c1,7" : "=r" (res));
        asm volatile ("BX LR");
        return res;
    }

    static u32 GetInstructionSetAttribute0(void) __attribute__((naked)) {
        register u32 res = 0;
        asm volatile ("MRC p15,0,%0,c0,c2,0" : "=r" (res));
        asm volatile ("BX LR");
        return res;
    }

    static u32 GetInstructionSetAttribute1(void) __attribute__((naked)) {
        register u32 res = 0;
        asm volatile ("MRC p15,0,%0,c0,c2,1" : "=r" (res));
        asm volatile ("BX LR");
        return res;
    }

    static u32 GetInstructionSetAttribute2(void) __attribute__((naked)) {
        register u32 res = 0;
        asm volatile ("MRC p15,0,%0,c0,c2,2" : "=r" (res));
        asm volatile ("BX LR");
        return res;
    }

    static u32 GetInstructionSetAttribute3(void) __attribute__((naked)) {
        register u32 res = 0;
        asm volatile ("MRC p15,0,%0,c0,c2,3" : "=r" (res));
        asm volatile ("BX LR");
        return res;
    }

    static u32 GetInstructionSetAttribute4(void) __attribute__((naked)) {
        register u32 res = 0;
        asm volatile ("MRC p15,0,%0,c0,c2,4" : "=r" (res));
        asm volatile ("BX LR");
        return res;
    }

    static u32 GetInstructionSetAttribute5(void) __attribute__((naked)) {
        register u32 res = 0;
        asm volatile ("MRC p15,0,%0,c0,c2,5" : "=r" (res));
        asm volatile ("BX LR");
        return res;
    }

    static u32 GetAuxiliaryId(void) __attribute__((naked)) {
        register u32 res = 0;
        asm volatile ("MRC p15,1,%0,c0,c0,2" : "=r" (res));
        asm volatile ("BX LR");
        return res;
    }

    static u32 GetVirtualizationProcessorId(void) __attribute__((naked)) {
        register u32 res = 0;
        asm volatile ("MRC p15,4,%0,c0,c0,0" : "=r" (res));
        asm volatile ("BX LR");
        return res;
    }

    static void SetVirtualizationProcessorId(register u32) __attribute__((naked)) {
        asm volatile ("MCR p15,4,r0,c0,c0,0");
        asm volatile ("BX LR");
    }

    static u32 GetVirtualizationMultiProcessorId(void) __attribute__((naked)) {
        register u32 res = 0;
        asm volatile ("MRC p15,4,%0,c0,c0,5" : "=r" (res));
        asm volatile ("BX LR");
        return res;
    }

    static void SetVirtualizationMultiProcessorId(register u32) __attribute__((naked)) {
        asm volatile ("MCR p15,4,r0,c0,c0,5");
        asm volatile ("BX LR");
    }

    static u32 GetSystemControl(void)__attribute__((naked)) __attribute__((naked)) {
        register u32 res = 0;
        asm volatile ("MRC p15,0,%0,c1,c0,0" : "=r" (res));
        asm volatile ("BX LR");
        return res;
    }

    static void SetSystemControl(register u32) __attribute__((naked)) __attribute__((naked)) {
        asm volatile ("MCR p15,0,r0,c1,c0,0");
        asm volatile ("BX LR");
    }

    static u32 GetPrimaryRegionRemap(void) __attribute__((naked)) {
        register u32 res = 0;
        asm volatile ("MRC p15,0,%0,c10,c2,0" : "=r" (res));
        asm volatile ("BX LR");
        return res;
    }

    static void SetPrimaryRegionRemap(register u32) __attribute__((naked)) {
        asm volatile ("MCR p15,0,r0,c10,c2,0");
        asm volatile ("BX LR");
    }

    static u32 GetMAIR_0(void) __attribute__((naked)) {
        register u32 res = 0;
        asm volatile ("MRC p15,0,%0,c10,c2,0" : "=r" (res));
        asm volatile ("BX LR");
        return res;
    }

    static void SetMAIR_0(register u32) __attribute__((naked)) {
        asm volatile ("MCR p15,0,r0,c10,c2,0");
        asm volatile ("BX LR");
    }

    static u32 GetMAIR_1(void) __attribute__((naked)) {
        register u32 res = 0;
        asm volatile ("MRC p15,0,%0,c10,c2,1" : "=r" (res));
        asm volatile ("BX LR");
        return res;
    }

    static void SetMAIR_1(register u32) __attribute__((naked)) {
        asm volatile ("MCR p15,0,r0,c10,c2,1");
        asm volatile ("BX LR");
    }

    static u32 GetNormalMemoryRemap(void) __attribute__((naked)) {
        register u32 res = 0;
        asm volatile ("MRC p15,0,%0,c10,c2,1" : "=r" (res));
        asm volatile ("BX LR");
        return res;
    }

    static void SetNormalMemoryRemap(register u32) __attribute__((naked)) {
        asm volatile ("MCR p15,0,r0,c10,c2,1");
        asm volatile ("BX LR");
    }

    static u32 GetAuxiliaryControl(void) __attribute__((naked)) {
        register u32 res = 0;
        asm volatile ("MRC p15,0,%0,c1,c0,1" : "=r" (res));
        asm volatile ("BX LR");
        return res;
    }

    static void SetAuxiliaryControl(register u32) __attribute__((naked)) {
        asm volatile ("MCR p15,0,r0,c1,c0,1");
        asm volatile ("BX LR");
    }

    static u32 GetCoprocessorAccessControl(void) __attribute__((naked)) {
        register u32 res = 0;
        asm volatile ("MRC p15,0,%0,c1,c0,2" : "=r" (res));
        asm volatile ("BX LR");
        return res;
    }

    static void SetCoprocessorAccessControl(register u32) __attribute__((naked)) {
        asm volatile ("MCR p15,0,r0,c1,c0,2");
        asm volatile ("BX LR");
    }

    static u32 GetSecureConfiguration(void) __attribute__((naked)) {
        register u32 res = 0;
        asm volatile ("MRC p15,0,%0,c1,c1,0" : "=r" (res));
        asm volatile ("BX LR");
        return res;
    }

    static void SetSecureConfiguration(register u32) __attribute__((naked)) {
        asm volatile ("MCR p15,0,r0,c1,c1,0");
        asm volatile ("BX LR");
    }

    static u32 GetNonSecureAccessControl(void) __attribute__((naked)) {
        register u32 res = 0;
        asm volatile ("MRC p15,0,%0,c1,c1,2" : "=r" (res));
        asm volatile ("BX LR");
        return res;
    }

    static void SetNonSecureAccessControl(register u32) __attribute__((naked)) {
        asm volatile ("MCR p15,0,r0,c1,c1,2");
        asm volatile ("BX LR");
    }

    static u32 GetHypSystemControl(void) __attribute__((naked)) {
        register u32 res = 0;
        asm volatile ("MRC p15,4,%0,c1,c0,0" : "=r" (res));
        asm volatile ("BX LR");
        return res;
    }

    static void SetHypSystemControl(register u32) __attribute__((naked)) {
        asm volatile ("MCR p15,4,r0,c1,c0,0");
        asm volatile ("BX LR");
    }

    static u32 GetHypDebugControl(void) __attribute__((naked)) {
        register u32 res = 0;
        asm volatile ("MRC p15,4,%0,c1,c1,1" : "=r" (res));
        asm volatile ("BX LR");
        return res;
    }

    static void SetHypDebugControl(register u32) __attribute__((naked)) {
        asm volatile ("MCR p15,4,r0,c1,c1,1");
        asm volatile ("BX LR");
    }

    static u32 GetHypCoprocessorTrap(void) __attribute__((naked)) {
        register u32 res = 0;
        asm volatile ("MRC p15,4,%0,c1,c1,2" : "=r" (res));
        asm volatile ("BX LR");
        return res;
    }

    static void SetHypCoprocessorTrap(register u32) __attribute__((naked)) {
        asm volatile ("MCR p15,4,r0,c1,c1,2");
        asm volatile ("BX LR");
    }

    static u32 GetDataFaultStatus(void) __attribute__((naked)) {
        register u32 res = 0;
        asm volatile ("MRC p15,0,%0,c5,c0,0" : "=r" (res));
        asm volatile ("BX LR");
        return res;
    }

    static void GetDataFaultStatus(u32 val) __attribute__((naked)) {
        asm volatile ("MCR p15,0,%0,c5,c0,0" : "=r" (val));
        asm volatile ("BX LR");
    }

    static u32 GetInstructionFaultStatus(void) __attribute__((naked)) {
        register u32 res = 0;
        asm volatile ("MRC p15,0,%0,c5,c0,1" : "=r" (res));
        asm volatile ("BX LR");
        return res;
    }

    static void SetInstructionFaultStatus(register u32) __attribute__((naked)) {
        asm volatile ("MCR p15,0,r0,c5,c0,1");
        asm volatile ("BX LR");
    }

    static u32 GetConfigurationBaseAddress(void) __attribute__((naked)) {
        register u32 res = 0;
        asm volatile ("MRC p15,4,%0,c15,c0,0" : "=r" (res));
        asm volatile ("BX LR");
        return res;
    }

    static u32 GetDACR(void) __attribute__((naked)) {
        register u32 res = 0;
        asm volatile ("MRC p15,0,%0,c3,c0,0" : "=r" (res));
        asm volatile ("BX LR");
        return res;
    }

    static void SetDACR(register u32) __attribute__((naked)) {
        asm volatile ("MCR p15,0,r0,c3,c0,0");
        asm volatile ("BX LR");
    }

    static u32 GetDFSR(void) __attribute__((naked)) {
        register u32 res = 0;
        asm volatile ("MRC p15,0,%0,c5,c0,0" : "=r" (res));
        asm volatile ("BX LR");
        return res;
    }

    static void SetDFSR(register u32) __attribute__((naked)) {
        asm volatile ("MCR p15,0,r0,c5,c0,0");
        asm volatile ("BX LR");
    }

    static u32 GetIFSR(void) __attribute__((naked)) {
        register u32 res = 0;
        asm volatile ("MRC p15,0,%0,c5,c0,1" : "=r" (res));
        asm volatile ("BX LR");
        return res;
    }

    static void SetIFSR(register u32) __attribute__((naked)) {
        asm volatile ("MCR p15,0,r0,c5,c0,1");
        asm volatile ("BX LR");
    }

    static u32 GetADFSR(void) __attribute__((naked)) {
        register u32 res = 0;
        asm volatile ("MRC p15,0,%0,c5,c1,0" : "=r" (res));
        asm volatile ("BX LR");
        return res;
    }

    static void SetADFSR(register u32) __attribute__((naked)) {
        asm volatile ("MCR p15,0,r0,c5,c1,0");
        asm volatile ("BX LR");
    }

    static u32 GetAIFSR(void) __attribute__((naked)) {
        register u32 res = 0;
        asm volatile ("MRC p15,0,%0,c5,c1,1" : "=r" (res));
        asm volatile ("BX LR");
        return res;
    }

    static void SetAIFSR(register u32) __attribute__((naked)) {
        asm volatile ("MCR p15,0,r0,c5,c1,1");
        asm volatile ("BX LR");
    }

    static u32 GetHADFSR(void) __attribute__((naked)) {
        register u32 res = 0;
        asm volatile ("MRC p15,4,%0,c5,c1,0" : "=r" (res));
        asm volatile ("BX LR");
        return res;
    }

    static void SetHADFSR(register u32) __attribute__((naked)) {
        asm volatile ("MCR p15,4,r0,c5,c1,0");
        asm volatile ("BX LR");
    }

    static u32 GetHAIFSR(void) __attribute__((naked)) {
        register u32 res = 0;
        asm volatile ("MRC p15,4,%0,c5,c1,1" : "=r" (res));
        asm volatile ("BX LR");
        return res;
    }

    static void SetHAIFSR(register u32) __attribute__((naked)) {
        asm volatile ("MCR p15,4,r0,c5,c1,1");
        asm volatile ("BX LR");
    }

    static u32 GetHSR(void) __attribute__((naked)) {
        register u32 res = 0;
        asm volatile ("MRC p15,4,%0,c5,c2,0" : "=r" (res));
        asm volatile ("BX LR");
        return res;
    }

    static void SetHSR(register u32) __attribute__((naked)) {
        asm volatile ("MCR p15,4,r0,c5,c2,0");
        asm volatile ("BX LR");
    }

    static u32 GetDFAR(void) __attribute__((naked)) {
        register u32 res = 0;
        asm volatile ("MRC p15,0,%0,c6,c0,0" : "=r" (res));
        asm volatile ("BX LR");
        return res;
    }

    static void SetDFAR(register u32) __attribute__((naked)) {
        asm volatile ("MCR p15,0,r0,c6,c0,0");
        asm volatile ("BX LR");
    }

    static u32 GetIFAR(void) __attribute__((naked)) {
        register u32 res = 0;
        asm volatile ("MRC p15,0,%0,c6,c0,2" : "=r" (res));
        asm volatile ("BX LR");
        return res;
    }

    static void SetIFAR(register u32) __attribute__((naked)) {
        asm volatile ("MCR p15,0,r0,c6,c0,2");
        asm volatile ("BX LR");
    }

    static u32 GetHDFAR(void) __attribute__((naked)) {
        register u32 res = 0;
        asm volatile ("MRC p15,4,%0,c6,c0,0" : "=r" (res));
        asm volatile ("BX LR");
        return res;
    }

    static void SetHDFAR(register u32) __attribute__((naked)) __attribute__((naked)) {
        asm volatile ("MCR p15,4,r0,c6,c0,0");
        asm volatile ("BX LR");
    }

    static u32 GetHIFAR(void) __attribute__((naked)) {
        register u32 res = 0;
        asm volatile ("MRC p15,4,%0,c6,c0,2" : "=r" (res));
        asm volatile ("BX LR");
        return res;
    }

    static void SetHIFAR(register u32) __attribute__((naked)) {
        asm volatile ("MCR p15,4,r0,c6,c0,2");
        asm volatile ("BX LR");
    }

    static u32 GetHPFAR(void) __attribute__((naked)) {
        register u32 res = 0;
        asm volatile ("MRC p15,4,%0,c6,c0,4" : "=r" (res));
        asm volatile ("BX LR");
        return res;
    }

    static void SetHPFAR(register u32) __attribute__((naked)) {
        asm volatile ("MCR p15,4,r0,c6,c0,4");
        asm volatile ("BX LR");
    }


};




#endif /* CAARMCOPR_H */

