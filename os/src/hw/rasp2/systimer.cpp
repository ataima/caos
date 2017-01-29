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

#include "sysleds.h"
#include "systimer.h"
#include "miniuart.h"
#include "sysirqctrl.h"
#include "memaux.h"

#if HAVE_AP804_SYS_TIMER
// min freq = 1


sysTimerStatus caSysTimer::st;

u32 caSysTimer::Init(u32 tick_hz, u32 timer_hz) {
    caMemAux<u32>::MemSet((u32*) & st, 0, sizeof (st));
    system_ap804_timer(ap804);
    Ap804Control cntl;
    if (timer_hz > 0xffff)
        return FALSE;
    caSysTimer::EnableTimer(0);
    caSysTimer::EnableCounter(0);
    cntl.asReg = ap804->Control.asReg;
    u32 clkdiv = 0;
    u32 prescaler = BCM2836_CLOCK_FREQ / tick_hz;
    if (prescaler > 255) {
        clkdiv = 1;
        prescaler = BCM2836_CLOCK_FREQ / (tick_hz << 4);
        if (prescaler > 255) {
            clkdiv = 2;
            prescaler = BCM2836_CLOCK_FREQ / (tick_hz << 8);
            if (prescaler > 255) {
                return FALSE; // too low tick freq
            }
        }
    }
    if (prescaler < 255) {
        if (timer_hz == 0)
            timer_hz = 1;
        ap804->Reload = ap804->Load = tick_hz / timer_hz;
        // to max speed , timer on to load value with reload set
        ap804->Predivider = prescaler;
        cntl.asBit.clkscale = clkdiv & 0x3;
        //cntl.asBit.counter_mode = 1; //23 bit
        cntl.asBit.prescaler = (u8) ((prescaler - 1) & 0xff);
        ap804->Control.asReg = cntl.asReg;
        st.mn_ClkHz = tick_hz;
        st.mn_IrqHz = timer_hz;
        return TRUE;
    }
    return FALSE;
}

u32 caSysTimer::EnableTimer(u32 status) {
    system_ap804_timer(ap804);
    Ap804Control cntl;
    cntl.asReg = ap804->Control.asReg;
    cntl.asBit.enable = 0; //stop timer always
    ap804->Control.asReg = cntl.asReg;
    if (status != 0) {
        cntl.asBit.enable = 1;
        ap804->Control.asReg = cntl.asReg;
    }
    return cntl.asBit.enable;
}

u32 caSysTimer::EnableCounter(u32 status) {
    system_ap804_timer(ap804);
    Ap804Control cntl;
    u32 timer_run;
    cntl.asReg = ap804->Control.asReg;
    if (cntl.asBit.enablecnt != (status & 1)) {
        timer_run = cntl.asBit.enable;
        cntl.asBit.enable = 0; //stop timer always
        ap804->Control.asReg = cntl.asReg;
        cntl.asBit.enablecnt = (status & 1);
        cntl.asBit.enable = timer_run & 1;
        ap804->Control.asReg = cntl.asReg;
    }
    return ap804->Control.asBit.enablecnt;
}

u32 caSysTimer::IrqDisable(void) {
    system_ap804_timer(ap804);
    Ap804Control cntl;
    cntl.asReg = ap804->Control.asReg;
    cntl.asBit.enableirq = 0;
    ap804->Control.asReg = cntl.asReg;
    return caIrqCtrl::DisableIrqSysTimer();
}

u32 caSysTimer::FastIrqDisable(void) {
    system_ap804_timer(ap804);
    Ap804Control cntl;
    cntl.asReg = ap804->Control.asReg;
    cntl.asBit.enableirq = 0;
    ap804->Control.asReg = cntl.asReg;
    return caIrqCtrl::DisableFastIrqSysTimer();
}

u32 caSysTimer::IrqEnable(void) {
    system_ap804_timer(ap804);
    Ap804Control cntl;
    cntl.asReg = ap804->Control.asReg;
    cntl.asBit.enableirq = 1;
    ap804->Control.asReg = cntl.asReg;
    if (ap804->Control.asBit.enableirq) {
        return caIrqCtrl::EnableIrqSysTimer();
    }
    return 0;
}

u32 caSysTimer::FastIrqEnable(void) {
    system_ap804_timer(ap804);
    Ap804Control cntl;
    cntl.asReg = ap804->Control.asReg;
    cntl.asBit.enableirq = 1;
    ap804->Control.asReg = cntl.asReg;
    if (ap804->Control.asBit.enableirq) {
        return caIrqCtrl::EnableFastIrqSysTimer();
    }
    return 0;
}

u32 caSysTimer::SetTime(u32 day, u32 hour, u32 min, u32 sec) {
    u32 res = deviceError::no_error;
    if (sec > 59)sec = 0;
    if (min > 59)min = 0;
    if (hour > 23)hour = 0;
    st.mn_Msec = sec;
    st.mn_Min = min;
    st.mn_Hour = hour;
    st.mn_Day = day;
    return res;
}


/// irq service is called from IRQ interrupt

void caSysTimer::IrqService(void) {
    s_t dummy;
    st.mn_IrqCount++;
    st.mn_Msec++;
    hal_llc_time_1.hll_irq_1(hal_llc_time_1.hll_lnk_obj, NULL, 0, dummy);
    if (st.mn_Msec == SYS_TIMER_TICK) {
        st.mn_Msec = 0;
        st.mn_Sec++;
        hal_llc_time_1.hll_irq_2(hal_llc_time_1.hll_lnk_obj, NULL, 0, dummy);
        if (st.mn_Sec == 60) {
            st.mn_Sec = 0;
            st.mn_Min++;
            if (st.mn_Min == 60) {
                st.mn_Min = 0;
                st.mn_Hour++;
                if (st.mn_Hour == 24) {
                    st.mn_Hour = 0;
                    st.mn_Day++;
                }
            }
        }
    }
    AckIrq();
}

void caSysTimer::AckIrq(void) {
    system_ap804_timer(ap804);
    if ((ap804->RawIrq & 1) != 0) {
        ap804->AckIrq = 0xffffffff;
    }
}

void caSysTimer::Load(u32 value) {
    system_ap804_timer(ap804);
    ap804->Reload = value;
}

u32 caSysTimer::ReadFreeCounter(void) {
    system_ap804_timer(ap804);
    return ap804->Counter;
}

u32 caSysTimer::Dump(caStringStream<s8> * ss) {
    u32 res = 0;
    if (ss != NULL) {
        system_ap804_timer(ap804);
        (*ss) << " --- SYS TIMER LIST ---" << caEnd::endl;
        (*ss) << caStringFormat::hex;
        (*ss) << "bcm 2835/6 AP804 Timer :" << caEnd::endl;
        (*ss) << "LOAD     [" << (u32) & ap804->Load << "]=" << ap804->Load << caEnd::endl;
        (*ss) << "CONTROL  [" << (u32) & ap804->Control.asReg << "]=" << ap804->Control.asReg << caEnd::endl;
        (*ss) << "COUNTER  [" << (u32) & ap804->Counter << "]=" << ap804->Counter << caEnd::endl;
        (*ss) << "ACKIRQ   [" << (u32) & ap804->AckIrq << "]=" << ap804->AckIrq << caEnd::endl;
        (*ss) << "MASKIRQ  [" << (u32) & ap804->MaskIrq << "]=" << ap804->MaskIrq << caEnd::endl;
        (*ss) << "PREDIV   [" << (u32) & ap804->Predivider << "]=" << ap804->Predivider << caEnd::endl;
        (*ss) << "RAWIRQ   [" << (u32) & ap804->RawIrq << "]=" << ap804->RawIrq << caEnd::endl;
        (*ss) << "RELOAD   [" << (u32) & ap804->Reload << "]=" << ap804->Reload << caEnd::endl;
        (*ss) << "VALUE    [" << (u32) & ap804->Value << "]=" << ap804->Value << caEnd::endl;
        (*ss).Str();
        res = ss->Size();
    }
    return res;
}

u32 caSysTimer::TimerRunnig(void) {
    system_ap804_timer(ap804);
    return ap804->Control.asBit.enable;
}

u32 caSysTimer::CounterRunnig(void) {
    system_ap804_timer(ap804);
    return ap804->Control.asBit.enablecnt;
}

u32 caSysTimer::IrqStatus(void) {
    system_ap804_timer(ap804);
    return ap804->Control.asBit.enableirq;
}

/**
 * 
 * @param ms =ms to convert in timer tick
 * @return 
 */
u32 caSysTimer::ToTick(u32 ms) {
    return ((st.mn_IrqHz * ms) / 1000);
}

/**
 * 
 * @param tick to convert in ms
 * @return 
 */
u32 caSysTimer::ToTime(u32 tick) {
    return ((1000 * tick) / (st.mn_IrqHz));

}

u32 caSysTimer::Start(void) {
    u32 res = FALSE;
    if (caSysTimer::Init(SYS_CLOCK_TIMER, SYS_TIMER_TICK)) {
        if (caSysTimer::EnableCounter(1)) {
            if (caSysTimer::EnableTimer(1)) {
                Dbg::Put("> c.a.O.S. : [ Start Schedule interrupt ... ]\r\n");
                res = caSysTimer::IrqEnable();
            }
        }
    }
    return res;
}

u32 caSysTimer::Stop(void) {
    u32 res = FALSE;
    if (caSysTimer::EnableCounter(0)) {
        if (caSysTimer::EnableTimer(0)) {
            res = caSysTimer::IrqDisable();
        }
    }
    return res;
}


#endif