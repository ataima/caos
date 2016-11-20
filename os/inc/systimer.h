#ifndef  _SYS_TIMER_HEADER_
#define  _SYS_TIMER_HEADER_

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
#include "stream.h"

#ifdef HAVE_AP804_SYS_TIMER

typedef struct tag_sys_timer_status {
    u32 mn_ClkHz;
    u32 mn_IrqHz;
    volatile u32 mn_Msec;
    volatile u32 mn_Sec;
    volatile u32 mn_Min;
    volatile u32 mn_Hour;
    volatile u32 mn_Day;
    volatile u32 mn_IrqCount;
    volatile u32 mn_FreeRunning;

    void Dump(caStringStream<s8> & ss) {
        ss << caStringFormat::dec;
        ss << "CLOCK HZ  =" << mn_ClkHz << caEnd::endl;
        ss << "IRQ   HZ  =" << mn_IrqHz << caEnd::endl;
        ss << "MILLISEC  =" << mn_Msec << caEnd::endl;
        ss << "SECOND    =" << mn_Sec << caEnd::endl;
        ss << "MINUTE    =" << mn_Min << caEnd::endl;
        ss << "HOUR      =" << mn_Hour << caEnd::endl;
        ss << "DAY       =" << mn_Day << caEnd::endl;
        ss << "IRQ TOTAL =" << mn_IrqCount << caEnd::endl;
        ss << "FREE COUNT=" << mn_FreeRunning << caEnd::endl;
        ss.Str();
    }
} sysTimerStatus;

typedef struct tag_sys_timer_set {
    u32 mn_Sec;
    u32 mn_Min;
    u32 mn_Hour;
    u32 mn_Day;
} sysTimerSet;

class caSysTimer {
private:
    static sysTimerStatus st;
public:
    static u32 Init(u32 tick_hz, u32 timer_hz);
    static u32 EnableTimer(u32 status);
    static u32 EnableCounter(u32 status);
    static u32 TimerRunnig(void);
    static u32 CounterRunnig(void);
    static u32 IrqDisable(void);
    static u32 IrqEnable(void);
    static u32 FastIrqDisable(void);
    static u32 FastIrqEnable(void);
    static u32 IrqStatus(void);
    static u32 ReadFreeCounter(void);
    static u32 ToTick(u32 ms);
    static u32 ToTime(u32 tick);
    static void IrqService(void);
    static u32 Dump(caStringStream<s8> & ss);

    static u32 GetStatus(sysTimerStatus * reqStatus, u32 reqSize);
    static u32 SetTime(sysTimerSet * req);

    inline static u32 GetTickFreq(void) {
        return st.mn_ClkHz;
    }

    inline static u32 GetTimerFreq(void) {
        return st.mn_IrqHz;
    }

    inline static u32 GetCount(void) {
        return st.mn_IrqCount;
    }

    inline static u32 GetMsec(void) {
        return st.mn_Msec;
    }

    inline static u32 GetSec(void) {
        return st.mn_Sec;
    }

    inline static u32 GetMin(void) {
        return st.mn_Min;
    }

    inline static u32 GetHour(void) {
        return st.mn_Hour;
    }

    inline static u32 GetDay(void) {
        return st.mn_Day;
    }

private:
    static void AckIrq(void);
    static void Load(u32 value);
};




#endif

#endif