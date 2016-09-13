#include "config.h"
#include "bcm2836.h"
#include "idevice.h"
#include "stream.h"
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