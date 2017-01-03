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

#include "auxmain.h"
#include "gpio.h"
#include "miniuart.h"
#include "interrupt.h"
#include "sysirqctrl.h"
#include "cpu.h"
#include "systimer.h"
#include "thread.h"
#include "scheduler.h"


extern u32 __ram_start__;
extern u32 __ram_end__;

static u32 mem_phy_min_addr(void) {
    return __ram_start__;
}

static u32 mem_phy_max_addr(void) {
    return __ram_end__;
}

const hal_ll_mem_io hal_ll_mem = {
    mem_phy_min_addr,
    mem_phy_max_addr
};



const hal_ll_scheduler_io hal_ll_scheduler = {
    caScheduler::IsValidContext
};


// Hardware connectors to COM1 (usually debug)
hal_ll_com_io hal_ll_com1 = {
    NULL,
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
    caHalDeviceRules::IrqServiceRx, //hll_irq_rx : set from device obj
    caHalDeviceRules::IrqServiceTx, //hll_irq_tx : set from device obj    
};

// Hardware connectors to COM1 (usually debug)
hal_ll_sys_time hal_ll_time{
    caSysTimer::GetCount, //system tick count
    caSysTimer::GetMsec,
    caSysTimer::GetSec,
    caSysTimer::GetMin,
    caSysTimer::GetHour,
    caSysTimer::GetDay // TO DO mounth, year , millenium
};

