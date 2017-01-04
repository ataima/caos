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
#include "systimer.h"
#include "cache.h"
#include "mmu.h"
#include "smp.h"

u32 caCache::Init(void) {
    u32 res = 0;
    return res;
}

u32 caCache::Start(void) {
    u32 res = 0;
    TIN();
    caSMP::Disable();
    caMMU::Disable();
    SetBPIALL(0);
    SetICIALLU(0);
    SetBPIALL(0);
    caMMU::CreateTTB();
    caMMU::SetDomain();
    caSMP::Enable();
    caMMU::Enable();
    if (caMMU::IsEnabled()) {
        SysControlRegister sysc;
        sysc.asReg = caArmCprs::GetSystemControl();
        //enable C bit
        sysc.asBit.C_e = 1;
        caArmCprs::SetSystemControl(sysc.asReg);
        //enable I bit
        sysc.asBit.I_e = 1;
        caArmCprs::SetSystemControl(sysc.asReg);
        //enable Z bit
        sysc.asBit.Z_e = 1;
        caArmCprs::SetSystemControl(sysc.asReg);
        res = IsEnabled();
    }
    TOUT();
    return res;
}

u32 caCache::Stop(void) {
    u32 res = 0;
    SysControlRegister sysc;
    sysc.asReg = caArmCprs::GetSystemControl();
    sysc.asBit.C_e = 0;
    sysc.asBit.I_e = 0;
    sysc.asBit.Z_e = 0;
    /*
    Dbg::Put("\r\n");
    Dbg::Put("SCR=", sysc.asReg);
    Dbg::Put("SysControlRegister.M_e=", sysc.asBit.M_e);
    Dbg::Put("SysControlRegister.A_e=", sysc.asBit.A_e);
    Dbg::Put("SysControlRegister.C_e=", sysc.asBit.C_e);
    Dbg::Put("SysControlRegister.reserved_0=", sysc.asBit.reserved_0);
    Dbg::Put("SysControlRegister.reserved_1=", sysc.asBit.reserved_1);
    Dbg::Put("SysControlRegister.SW_e=", sysc.asBit.SW_e);
    Dbg::Put("SysControlRegister.Z_e=", sysc.asBit.Z_e);
    Dbg::Put("SysControlRegister.I_e=", sysc.asBit.I_e);
    Dbg::Put("SysControlRegister.V_e=", sysc.asBit.V_e);
    Dbg::Put("SysControlRegister.reserved_1=", sysc.asBit.reserved_2);
    Dbg::Put("SysControlRegister.WXN=", sysc.asBit.WXN);
    Dbg::Put("SysControlRegister.UWXN=", sysc.asBit.UWXN);
    Dbg::Put("SysControlRegister.reserved_1=", sysc.asBit.reserved_7);
    Dbg::Put("SysControlRegister.EE=", sysc.asBit.EE);
    Dbg::Put("SysControlRegister.reserved_1=", sysc.asBit.reserved_10);
    Dbg::Put("SysControlRegister.TRE=", sysc.asBit.TRE);
    Dbg::Put("SysControlRegister.AFE=", sysc.asBit.AFE);
    Dbg::Put("SysControlRegister.TE=", sysc.asBit.TE);
    Dbg::Put("SysControlRegister.reserved_1=", sysc.asBit.reserved_11);
     * */
    caArmCprs::SetSystemControl(sysc.asReg);
    /*
        sysc.asReg = caArmCprs::GetSystemControl();
        Dbg::Put("\r\n");
        Dbg::Put("SCR=", sysc.asReg);
        Dbg::Put("SysControlRegister.M_e=", sysc.asBit.M_e);
        Dbg::Put("SysControlRegister.A_e=", sysc.asBit.A_e);
        Dbg::Put("SysControlRegister.C_e=", sysc.asBit.C_e);
        Dbg::Put("SysControlRegister.reserved_0=", sysc.asBit.reserved_0);
        Dbg::Put("SysControlRegister.reserved_1=", sysc.asBit.reserved_1);
        Dbg::Put("SysControlRegister.SW_e=", sysc.asBit.SW_e);
        Dbg::Put("SysControlRegister.Z_e=", sysc.asBit.Z_e);
        Dbg::Put("SysControlRegister.I_e=", sysc.asBit.I_e);
        Dbg::Put("SysControlRegister.V_e=", sysc.asBit.V_e);
        Dbg::Put("SysControlRegister.reserved_1=", sysc.asBit.reserved_2);
        Dbg::Put("SysControlRegister.WXN=", sysc.asBit.WXN);
        Dbg::Put("SysControlRegister.UWXN=", sysc.asBit.UWXN);
        Dbg::Put("SysControlRegister.reserved_1=", sysc.asBit.reserved_7);
        Dbg::Put("SysControlRegister.EE=", sysc.asBit.EE);
        Dbg::Put("SysControlRegister.reserved_1=", sysc.asBit.reserved_10);
        Dbg::Put("SysControlRegister.TRE=", sysc.asBit.TRE);
        Dbg::Put("SysControlRegister.AFE=", sysc.asBit.AFE);
        Dbg::Put("SysControlRegister.TE=", sysc.asBit.TE);
        Dbg::Put("SysControlRegister.reserved_1=", sysc.asBit.reserved_11);
     */
    return res;
}

u32 caCache::IsEnabled(void) {
    SysControlRegister sysc;
    sysc.asReg = caArmCprs::GetSystemControl();
    return (sysc.asBit.C_e && sysc.asBit.I_e);
}

