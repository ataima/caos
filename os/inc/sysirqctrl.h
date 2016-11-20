#ifndef IRQCTRL_H
#define IRQCTRL_H

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



#include "bcm2836.h"
#include "atomiclock.h"

class caIrqCtrl {
    static caAtomicLock Lock;
public:

    typedef enum source_fiq_def {
        gpu_0 = 0,
        gpu_1,
        gpu_2,
        gpu_3,
        gpu_4,
        gpu_5,
        gpu_6,
        gpu_7,
        gpu_8,
        gpu_9,

        gpu_10,
        gpu_11,
        gpu_12,
        gpu_13,
        gpu_14,
        gpu_15,
        gpu_16,
        gpu_17,
        gpu_18,
        gpu_19,

        gpu_20,
        gpu_21,
        gpu_22,
        gpu_23,
        gpu_24,
        gpu_25,
        gpu_26,
        gpu_27,
        gpu_28,
        gpu_29,

        gpu_30,
        gpu_31,
        gpu_32,
        gpu_33,
        gpu_34,
        gpu_35,
        gpu_36,
        gpu_37,
        gpu_38,
        gpu_39,

        gpu_40,
        gpu_41,
        gpu_42,
        gpu_43,
        gpu_44,
        gpu_45,
        gpu_46,
        gpu_47,
        gpu_48,
        gpu_49,

        gpu_50,
        gpu_51,
        gpu_52,
        gpu_53,
        gpu_54,
        gpu_55,
        gpu_56,
        gpu_57,
        gpu_58,
        gpu_59,

        gpu_60,
        gpu_61,
        gpu_62,
        gpu_63,
        arm_timer,
        arm_mailbox,
        arm_doorbell_0,
        arm_doorbell_1,
        gpu_halt_0,
        gpu_halt_1,
        illegal_access_type_1,
        illegal_access_type_0,
        reserverd
    } fiqSources;


public:

    static inline u32 EnableIrqAux(void) {
        system_irq_control(irq);
        irq->enable0.asBit.aux_int = 1;
        return irq->enable0.asBit.aux_int;
    }

    static inline u32 DisableIrqAux(void) {
        system_irq_control(irq);
        irq->disable0.asBit.aux_int = 1;
        return irq->disable0.asBit.aux_int;
    }

    static inline u32 EnableIrqSysTimer(void) {
        system_irq_control(irq);
        irq->baseenable.asBit.timerirq = 1;
        return irq->baseenable.asBit.timerirq;
    }

    static inline u32 DisableIrqSysTimer(void) {
        system_irq_control(irq);
        irq->basedisable.asBit.timerirq = 1;
        return irq->basedisable.asBit.timerirq;
    }

    static inline u32 PendingIrqSysTimer(void) {
        system_irq_control(irq);
        return irq->basepending.asBit.timer;
    }

    static inline u32 EnableFastIrq(void) {
        system_irq_control(irq);
        if (!irq->fiq.asBit.enable) {
            irq->fiq.asBit.enable = 1;
            return TRUE;
        }
        return TRUE;
    }

    static inline u32 DisableFastIrq(void) {
        system_irq_control(irq);
        if (irq->fiq.asBit.enable) {
            irq->fiq.asBit.enable = 0;
            return TRUE;
        }
        return FALSE;
    }

    static inline u32 EnableFastIrqSysTimer(void) {
        system_irq_control(irq);
        DisableFastIrq();
        irq->baseenable.asBit.timerirq = 1;
        if (irq->baseenable.asBit.timerirq) {
            irq->fiq.asBit.source = fiqSources::arm_timer;
            return EnableFastIrq();
        }
        return FALSE;
    }

    static inline u32 DisableFastIrqSysTimer(void) {
        system_irq_control(irq);
        DisableFastIrq();
        irq->basedisable.asBit.timerirq = 1;
        return irq->basedisable.asBit.timerirq == 0;
    }


    static u32 SelectServiceIrq(void);
    static u32 SelectServiceFiq(void);
    static void Init(void);
    static void Dump(void);
    static bool LockSwitchContext();
    static bool UnLockSwitchContext();
};

#endif /* IRQCTRL_H */

