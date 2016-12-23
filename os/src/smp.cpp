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

u32 caSMP::Disable() {
    AuxSysControlRegister aux;
    aux.asReg = caArmCprs::GetAuxiliaryControl();
    aux.asBit.SMP = 0;
    caArmCprs::SetAuxiliaryControl(aux.asReg);
    aux.asReg = caArmCprs::GetAuxiliaryControl();
    return (aux.asBit.SMP == 0);
}

u32 caSMP::Enable() {
    AuxSysControlRegister aux;
    aux.asReg = caArmCprs::GetAuxiliaryControl();
    aux.asBit.SMP = 1;
    caArmCprs::SetAuxiliaryControl(aux.asReg);
    aux.asReg = caArmCprs::GetAuxiliaryControl();
    return (aux.asBit.SMP == 1);
}