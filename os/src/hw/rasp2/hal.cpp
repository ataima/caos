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
#include "hw_func.h"
#include "auxmain.h"
#include "gpio.h"
#include "miniuart.h"
#include "interrupt.h"
#include "sysirqctrl.h"
#include "cpu.h"
#include "systimer.h"
#include "scheduler.h"
#include "sysleds.h"

/* WARNING DO NO TOUCH DECLARATION OF LINKER SEGMENT ITS OK!!!*/

extern s8 __ram_start__;
extern s8 __ram_end__;
extern s8 __ram_size__;
extern s8 __heap_base__;
extern s8 __heap_end__;

extern s8 __user_ram_start__;
extern s8 __user_ram_end__;


extern "C" {

    static u64 mem_phy_min_addr(void) {
        return (u64) (&__ram_start__);
    }

    static u64 mem_phy_max_addr(void) {
        return (u64) (&__ram_end__);
    }

    static u64 mem_phy_size(void) {
        return (u64) (&__ram_size__);
    }

    static u64 mem_heap_start_addr(void) {
        return (u64) (&__heap_end__);
    }

    static u64 mem_heap_end_addr(void) {
        return (u64) (&__heap_base__);
    }

    static u64 mem_user_start_addr(void) {
        return (u64) (&__user_ram_start__);
    }

    static u64 mem_user_end_addr(void) {
        return (u64) (&__user_ram_end__);
    }


    // test as first call after start ...

    void memory_check_one(u32 *where) {
        *where = 0x5a5a5a5a;
        if (*where != 0x5a5a5a5a) {
            Dbg::Put("Dram Error 0x5a5a5a5a at ", (u32) where);
        }
        *where = 0xa5a5a5a5;
        if (*where != 0xa5a5a5a5) {
            Dbg::Put("Dram Error 0xa5a5a5a5 at ", (u32) where);
        }
        *where = 0;
    }

    // first call after start.s

    void memory_check_dram(void) {
        u32 *h_start;
        u32 *h_end;
        h_start = (u32*) (mem_user_start_addr());
        h_end = (u32*) (mem_user_end_addr());
        Dbg::Put("START=", (u32) h_start);
        Dbg::Put("STOP=", (u32) h_end);
        while (h_start < h_end) {
            memory_check_one(h_start);
            h_start++;
            u32 v = (u32) (h_start) % 0x100000;
            if (!v) {
                Dbg::Put("Checking at ", (u32) h_start);
            }
        }
    }

    

    static u32 start_system_timer(void) {
        u32 res = false;
        if (caSysTimer::Init(SYS_CLOCK_TIMER, SYS_TIMER_TICK)) {
            if (caSysTimer::EnableCounter(1)) {
                if (caSysTimer::EnableTimer(1)) {
                    res = caSysTimer::IrqEnable();
                }
            }
        }
        caArmCpu::EnableIrqFiq();
        caArmCpu::EnableInt();
        return res;
    }

    static u32 stop_system_timer(void) {
        u32 res = false;
        if (caSysTimer::EnableCounter(0)) {
            if (caSysTimer::EnableTimer(0)) {
                res = caSysTimer::IrqDisable();
            }
        }
        caArmCpu::DisableIrqFiq();
        caArmCpu::DisableInt();
        return res;
    }

    static u32 irq_request_1(void *, u8 *, s_t, s_t &) {
        caMiniUart::Send('1');
        return 0;
    }

    static u32 irq_request_2(void *, u8 *, s_t, s_t &) {
        caMiniUart::Send('2');
        return 0;
    }



    static void req_svc_7961(void) {
        Dbg::Put("reschedule\n");
        caArmCpu::SVC_7961();
    }

    static void stop_scheduler(void) {
        caSysTimer::EnableTimer(0);
    }

    static void start_scheduler(void) {
        caSysTimer::EnableTimer(1);
    }
    
    caThreadContext * get_current_context(void){
        return caScheduler::GetCurrentContext();
    }

    void get_next_context(void){
        caScheduler::GetNextContext();
    }

}



// Hardware connectors sys timer
hal_llc_sys_time hal_llc_time_1 = {
    nullptr,
    caSysTimer::Configure, // empthy funzion base timer fix conf to scheduler task
    caSysTimer::GetCount, //system tick count
    caSysTimer::GetPsec,
    caSysTimer::GetNsec,
    caSysTimer::GetUsec,
    caSysTimer::GetMsec,
    caSysTimer::GetSec,
    caSysTimer::GetMin,
    caSysTimer::GetHour,
    caSysTimer::GetDay, // TO DO mounth, year , millenium
    caSysTimer::SetTime,
    caSysTimer::ReadFreeCounter,
    caSysTimer::Dump,
    caSysTimer::ToTick,
    start_system_timer, // systimer 1 alway run...
    stop_system_timer,
    caScheduler::WakeUp,
    caScheduler::WakeUp,
    irq_request_1,
    irq_request_2
};


hal_llc_interrupt hal_llc_int_req = {
    caArmCpu::EnableAll,
    caArmCpu::DisableAll,
    caArmCpu::WaitForInterrupt,
    caArmCpu::WaitForEver,

};


hal_llc_reset hal_llc_reset_req{
    sysReset, //TODO FIX
    sysShutDown,
    sysRestart,
    caSysLed::LedsOff,
    caSysLed::LedsOn,
    caSysLed::LedOff,
    caSysLed::LedOn};


// Hardware connectors to COM1 (usually debug)
hal_llc_com_io hal_llc_com1 = {
    nullptr,
    caMiniUart::Configure, //hll_config
    caSysTimer::GetCount, //hll_time
    caMiniUart::EnableInt, //hll_en_int
    caMiniUart::EnableIrqTx, //hll_start_tx
    caMiniUart::Stop, //hll_stop
    caMiniUart::Enable, //hll_enable
    caMiniUart::Dump, //hll_dump
    caMiniUart::GetErrors, //hll_get_errors
    caScheduler::WakeUp, //hll_wakeuprx
    caScheduler::WakeUp, //hll_wakeuptx
    caHalDeviceRules::IrqService1, //hll_irq_tx : set from device obj
    caHalDeviceRules::IrqService2, //hll_irq_rx : set from device obj   
    caMiniUart::Send,
    caMiniUart::Recv
};


hal_llc_scheduler_io hal_llc_scheduler = {
    caSysTimer::GetCount,
    caSysTimer::ToTick,
    caScheduler::AddTask,
    caScheduler::IsValidContext,
    caIrqCtrl::LockSwitchContext,
    caIrqCtrl::UnLockSwitchContext,
    req_svc_7961,
    start_scheduler,
    stop_scheduler,
};

hal_llc_mem_io hal_llc_mem = {
    caSysTimer::GetCount,
    mem_phy_min_addr,
    mem_phy_max_addr,
    mem_phy_size,
    mem_heap_start_addr,
    mem_heap_end_addr,
    mem_user_start_addr,
    mem_user_end_addr,
    caArmCpu::MemoryFailure
};
