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
#include "adapter.h"
#include "scheduler.h"

extern u32 __ram_start__;
extern u32 __ram_end__;
extern u32 __heap_base__;
extern u32 __heap_end__;

static u32 mem_phy_min_addr(void) {
    return __ram_start__;
}

static u32 mem_phy_max_addr(void) {
    return __ram_end__;
}

static u32 mem_phy_size(void) {
    return __heap_end__-__heap_base__;
}

static u32 *mem_heap_start_addr(void) {
    return (u32*)(__heap_base__);
}

static u32 *mem_heap_end_addr(void) {
    return (u32 *)(__heap_end__);
}


static hal_llc_mem_io hal_llc_mem = {
    sim_get_tick,
    mem_phy_min_addr,
    mem_phy_max_addr,
    mem_phy_size,
    mem_heap_start_addr,
    mem_heap_end_addr
};

static hal_llc_scheduler_io hal_llc_scheduler = {
    sim_get_tick,
    sim_to_tick,
    sim_add_task,
    sim_is_valid_context,
    sim_lock_switch_context,
    sim_unlock_switch_context,
    sim_svc_request,
    sim_start_scheduler,
    sim_stop_scheduler
};


// Hardware connectors to COM1 (usually debug)
static hal_llc_com_io hal_llc_com1 = {
    nullptr,
    sim_uart_configure, //hll_config
    sim_get_tick, //hll_time
    sim_uart_enable_int, //hll_en_int
    sim_uart_EnableIrqTx, //hll_start_tx
    sim_uart_stop, //hll_stop
    sim_uart_enable, //hll_enable
    sim_uart_dump, //hll_dump
    sim_uart_get_errors, //hll_get_errors
    caScheduler::WakeUp, //hll_wakeuprx
    caScheduler::WakeUp, //hll_wakeuptx
    caHalDeviceRules::IrqService1, //hll_irq_tx : set from device obj
    caHalDeviceRules::IrqService2, //hll_irq_rx : set from device obj   
    sim_uart_send,
    sim_uart_recv
};


// Hardware connectors sys timer
static hal_llc_sys_time hal_llc_time_1 = {
    nullptr,
    sim_timer_configure, // empthy funzion base timer fix conf to scheduler task
    sim_get_tick, //system tick count
    sim_timer_get_psec,
    sim_timer_get_nsec,
    sim_timer_get_usec,
    sim_timer_get_msec,
    sim_timer_get_sec,
    sim_timer_get_min,
    sim_timer_get_hour,
    sim_timer_get_day, // TO DO mounth, year , millenium
    sim_timer_set_time,
    sim_free_counter,
    sim_timer_dump,
    sim_to_tick,
    sim_timer_start, // systimer 1 alway run...
    sim_timer_stop,
    caScheduler::WakeUp,
    caScheduler::WakeUp,
    caHalDeviceRules::IrqService1,
    caHalDeviceRules::IrqService2,
};


static hal_llc_interrupt hal_llc_int_req = {
    sim_int_enable_all,
    sim_int_disable_all,
    sim_int_wait_for_interrupt,
    sim_wait_for_ever
};


static hal_llc_reset hal_llc_reset_req{
    sim_reset_reset, //TODO FIX
    sim_reset_shutdown,
    sim_reset_restart,
    sim_led_ledsoff,        
    sim_led_ledson,        
    sim_led_ledoff,        
    sim_led_ledon,        
};