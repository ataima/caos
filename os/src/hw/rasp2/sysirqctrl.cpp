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
#include "miniuart.h"
#include "systimer.h"
#include "cpu.h"
#include "interrupt.h"
#include "memory.h"
#include "sysirqctrl.h"
#include "systimer.h"
#include "scheduler.h"



caAtomicLock caIrqCtrl::Lock;

void caIrqCtrl::Init(void) {
    system_irq_control(irq);
    irq->fiq.asReg = 0;
    irq->basedisable.asReg = 0xffffffff;
    irq->disable0.asReg = 0xffffffff;
    irq->disable1.asReg = 0xffffffff;
}

bool caIrqCtrl::LockSwitchContext() {
    bool res = false;
    if (Lock.Get() == LOCK_FREE) {
        if (Lock.Lock() == 1) {
            res = true;
        }
    }
    return res;
}

bool caIrqCtrl::UnLockSwitchContext() {
    bool res = false;
    if (Lock.Get() == LOCK_V) {
        if (Lock.UnLock() == 1) {
            res = true;
        }
    }
    return res;
}

u32 caIrqCtrl::SelectServiceIrq(void) {
    u32 res = 0;
    //from pending irq priorized call to service routine
    system_irq_control(irq);
    // MAX PRIORITY
    if (irq->basepending.asBit.timer) {
        caSysTimer::IrqService();
        if (Lock.Get() == LOCK_FREE) {
        res = -1;
        }
    }
    if (irq->basepending.asBit.pending1) {
        if (irq->gpu0.asBit.irqaux29 == 1) {
#if ( DEBUG_COM || HAVE_SPI_1 || HAVE_SPI_2)
            //Dbg::Put("AUX = ", irq->basepending.asReg);
            system_aux(aux);
            if (aux->irq.asBit.miniuart) {
                // caMiniUart::IrqService();
            } else
                if (aux->irq.asBit.spi_m_1) {
                Dbg::Put("SP1 = ", irq->basepending.asReg);
            } else
                if (aux->irq.asBit.spi_m_2) {
                Dbg::Put("SP2 = ", irq->basepending.asReg);
            }
#endif            
        } else {
            Dbg::Put("BASE = ", irq->basepending.asReg);
            Dbg::Put("GPU0 = ", irq->gpu0.asReg);
            Dbg::Put("MISSING ISR TO CALL\r\n");
            while(1){};
        }
    }
    if (irq->basepending.asBit.pending2) {
        Dbg::Put("BASE = ", irq->basepending.asReg);
        Dbg::Put("GPU1 = ", irq->gpu1.asReg);
        Dbg::Put("MISSING ISR TO CALL\r\n");
        while(1){};
    }
    return res;
}

u32 caIrqCtrl::SelectServiceFiq(void) {
    u32 res = 0;
    system_irq_control(irq);
    if (irq->fiq.asBit.enable) {
        u32 source = irq->fiq.asBit.source;
        switch (source) {
            case 64:caSysTimer::IrqService();
                Dbg::Put("FIQ TIMER = ", irq->basepending.asReg);
                if (Lock.Get() == LOCK_FREE && Lock.Lock() == 1) {
                    res = -1;
                    Lock.UnLock();
                }
                break;
            default:
                Dbg::Put("\r\n\r\n---------------------------");
                Dbg::Put("RX FAST IRQ REQUEST  \r\n");
                Dbg::Put("MISSING ISR TO CALL  \r\n");
                Dbg::Put("STOP!!! \r\n");
                Dbg::Put("---------------------------\r\n\r\n");
                while (1) {
                }
                break;
        }
    } else {
        Dbg::Put("\r\n\r\n---------------------------");
        Dbg::Put("RX FAST IRQ REQUEST BUT \r\n");
        Dbg::Put("FIQ CTRL IS DISABLED!!! \r\n");
        Dbg::Put("STOP!!! \r\n");
        Dbg::Put("---------------------------\r\n\r\n");
        while (1) {
        }
    }
    return res;
}

void caIrqCtrl::Dump(void) {
    system_irq_control(irq);
    Dbg::Put("bcm 2835/6 irq ctrl :\r\n");
    Dbg::Put("BASIC PENDING", (void *) &irq->basepending, irq->basepending.asReg);
    Dbg::Put("PENDING 0", (void *) &irq->gpu0, irq->gpu0.asReg);
    Dbg::Put("PENDING 1", (void *) &irq->gpu1, irq->gpu1.asReg);
    Dbg::Put("FIQ", (void *) &irq->fiq, irq->fiq.asReg);
    Dbg::Put("ENABLE 0", (void *) &irq->enable0, irq->enable0.asReg);
    Dbg::Put("ENABLE 1", (void *) &irq->enable1, irq->enable1.asReg);
    Dbg::Put("BASE ENABLE", (void *) &irq->baseenable, irq->baseenable.asReg);
    Dbg::Put("DISABLE 0", (void *) &irq->disable0, irq->disable0.asReg);
    Dbg::Put("DISABLE 1", (void *) &irq->disable1, irq->disable1.asReg);
    Dbg::Put("BASE DISABLE", (void *) &irq->basedisable, irq->basedisable.asReg);
    Dbg::Put("\r\n");
}