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
#include "cpu.h"
#include "coprocessor.h"
#include "cache.h"
#include "mmu.h"

#include "idevice.h"
#include "stream.h"
#include "systimer.h"

#define PAGE_TABLE_SIZE 0x1000  //as u32

extern u32 Base_TTB[PAGE_TABLE_SIZE];

u32 caMMU::Disable(void) {
    TIN();
    u32 res = 0;
    SysControlRegister sysc;
    sysc.asReg = caArmCprs::GetSystemControl();
    //disable I,C,M,Z bit
    sysc.asBit.M_e = 0;
    sysc.asBit.Z_e = 0;
    sysc.asBit.I_e = 0;
    sysc.asBit.C_e = 0;
    caArmCprs::SetSystemControl(sysc.asReg);
    SetTLBIALL(0);
    SetTLBIMVA(0);
    SetTLBIASID(0);
    SetTLBIMVAA(0);
    TOUT();
    return res;
}



#define START_PERI    0x3f0
#define END_PERI      0x410

u32 caMMU::CreateTTB(void) {
    u32 i, res = 0;
    TIN();
    SetTTBCR(0); //armv5 compatile
    TTBR0_Info tbr0;
    tbr0.asReg = Get32_TTBR0();
    // Dbg::Put("Base_TTB=", (u32) Base_TTB);
    // Dbg::Put("TTBADDR=", (u32) tbr0.asBit.TTBADDR);
    // Dbg::Put("TTBR0_Info=", (u32) tbr0.asReg);
    if (tbr0.asBit.TTBADDR != (u32) (Base_TTB)) {
        TTBSection1MB def;
        def.asReg = 0x00006c0e;
        TTBSection1MB peri;
        peri.asReg = 0x00002c02;
        /*
        Dbg::Put("DEFAULT PAGE TABLE=",def.asReg);
        Dbg::Put("TTBSection1MB.asBit.PXN=", def.asBit.PXN);
        Dbg::Put("TTBSection1MB.asBit.SECTION=", def.asBit.SECTION);
        Dbg::Put("TTBSection1MB.asBit.B=", def.asBit.B);
        Dbg::Put("TTBSection1MB.asBit.C=", def.asBit.C);
        Dbg::Put("TTBSection1MB.asBit.XN=", def.asBit.XN);
        Dbg::Put("TTBSection1MB.asBit.DOMAIN=", def.asBit.DOMAIN);
        Dbg::Put("TTBSection1MB.asBit.RESERVED=", def.asBit.reserved);
        Dbg::Put("TTBSection1MB.asBit.AP01=", def.asBit.AP01);
        Dbg::Put("TTBSection1MB.asBit.TEX=", def.asBit.TEX);
        Dbg::Put("TTBSection1MB.asBit.AP2=", def.asBit.AP2);
        Dbg::Put("TTBSection1MB.asBit.S=", def.asBit.S);
        Dbg::Put("TTBSection1MB.asBit.nG=", def.asBit.nG);
        Dbg::Put("TTBSection1MB.asBit.alwais0=", def.asBit.always0);
        Dbg::Put("TTBSection1MB.asBit.NS=", def.asBit.nS);
        Dbg::Put("TTBSection1MB.asBit.BASE=", def.asBit.BASE);
        Dbg::Put("PERI PAGE TABLE=",peri.asReg);
        Dbg::Put("TTBSection1MB.asBit.PXN=", peri.asBit.PXN);
        Dbg::Put("TTBSection1MB.asBit.SECTION=", peri.asBit.SECTION);
        Dbg::Put("TTBSection1MB.asBit.B=", peri.asBit.B);
        Dbg::Put("TTBSection1MB.asBit.C=", peri.asBit.C);
        Dbg::Put("TTBSection1MB.asBit.XN=", peri.asBit.XN);
        Dbg::Put("TTBSection1MB.asBit.DOMAIN=", peri.asBit.DOMAIN);
        Dbg::Put("TTBSection1MB.asBit.RESERVED=", peri.asBit.reserved);
        Dbg::Put("TTBSection1MB.asBit.AP01=", peri.asBit.AP01);
        Dbg::Put("TTBSection1MB.asBit.TEX=", peri.asBit.TEX);
        Dbg::Put("TTBSection1MB.asBit.AP2=", peri.asBit.AP2);
        Dbg::Put("TTBSection1MB.asBit.S=", peri.asBit.S);
        Dbg::Put("TTBSection1MB.asBit.nG=", peri.asBit.nG);
        Dbg::Put("TTBSection1MB.asBit.alwais0=", peri.asBit.always0);
        Dbg::Put("TTBSection1MB.asBit.NS=", peri.asBit.nS);
        Dbg::Put("TTBSection1MB.asBit.BASE=", peri.asBit.BASE);
         * */
        for (i = 0; i < START_PERI; i++) //0..3f0 0-> 3effffff + 1 ?
        {
            def.asBit.BASE = i & 0xfff; // default section
            Base_TTB[i] = def.asReg;
        }
        for (; i < END_PERI; i++) //3f0...410 3f000000 41000000 ?
        {
            peri.asBit.BASE = i & 0xfff; // peri no cache
            Base_TTB[i] = peri.asReg;
        }
        for (; i < END_PERI; i++) /// remain to map GPU
        {
            Base_TTB[i] = 0; // invalid
        }

        tbr0.asReg = (u32) (Base_TTB);
        tbr0.asBit.IRGN0 = 1;
        tbr0.asBit.RGN = 1;
        //Dbg::Put("1 TTBR0_Info=", (u32) tbr0.asReg);
        Set32_TTBR0(tbr0.asReg);
        tbr0.asReg = Get32_TTBR0();
        //Dbg::Put("2 TTBR0_Info=", (u32) tbr0.asReg);
        res = (tbr0.asBit.TTBADDR == (u32) (Base_TTB));
    }
    TOUT();
    return res;
}

u32 caMMU::SetDomain(void) {
    u32 res = 0;
    TIN();
    res = caArmCprs::GetDACR();
    if (res != 0xffffffff) {
        caArmCprs::SetDACR(0xffffffff);
    }
    //res = caArmCprs::GetDACR();
    //Dbg::Put("DOMAIN=", res);
    res = 1;
    TOUT();
    return res;
}

u32 caMMU::Enable(void) {
    u32 res = 0;
    TIN();
    SysControlRegister sysc;
    sysc.asReg = caArmCprs::GetSystemControl();
    sysc.asBit.M_e = 1;
    /*
    Dbg::Put("1 Set SCR=",sysc.asReg);  */
    caArmCprs::SetSystemControl(sysc.asReg);
    /*
    sysc.asReg = caArmCprs::GetSystemControl();
    Dbg::Put("\r\n");    
    Dbg::Put("SCR=",sysc.asReg);    
    Dbg::Put("SysControlRegister.M_e=",sysc.asBit.M_e);
    Dbg::Put("SysControlRegister.A_e=",sysc.asBit.A_e);
    Dbg::Put("SysControlRegister.C_e=",sysc.asBit.C_e);
    Dbg::Put("SysControlRegister.reserved_0=",sysc.asBit.reserved_0);
    Dbg::Put("SysControlRegister.reserved_1=",sysc.asBit.reserved_1);
    Dbg::Put("SysControlRegister.SW_e=",sysc.asBit.SW_e);
    Dbg::Put("SysControlRegister.Z_e=",sysc.asBit.Z_e);
    Dbg::Put("SysControlRegister.I_e=",sysc.asBit.I_e);
    Dbg::Put("SysControlRegister.V_e=",sysc.asBit.V_e);
    Dbg::Put("SysControlRegister.reserved_1=",sysc.asBit.reserved_2);
    Dbg::Put("SysControlRegister.WXN=",sysc.asBit.WXN);
    Dbg::Put("SysControlRegister.UWXN=",sysc.asBit.UWXN);
    Dbg::Put("SysControlRegister.reserved_1=",sysc.asBit.reserved_7);
    Dbg::Put("SysControlRegister.EE=",sysc.asBit.EE);
    Dbg::Put("SysControlRegister.reserved_1=",sysc.asBit.reserved_10);
    Dbg::Put("SysControlRegister.TRE=",sysc.asBit.TRE);
    Dbg::Put("SysControlRegister.AFE=",sysc.asBit.AFE);
    Dbg::Put("SysControlRegister.TE=",sysc.asBit.TE);
    Dbg::Put("SysControlRegister.reserved_1=",sysc.asBit.reserved_11);
     * */
    sysc.asReg = caArmCprs::GetSystemControl();
    res = sysc.asBit.M_e;
    TOUT();
    return res;
}

u32 caMMU::IsEnabled(void) {
    TIN();
    SysControlRegister sysc;
    sysc.asReg = caArmCprs::GetSystemControl();
    TOUT();
    return sysc.asBit.M_e;
}

