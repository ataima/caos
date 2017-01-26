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

// HARWARE ABSTRACT LAYER LOW LEVEL CONNECTOR hal_llc_
#ifndef HAL_H
#define HAL_H

#include "config.h"
#include "caos_c_types.h"
#include "stream.h"
#include "devicehandle.h"
#include "idevice.h"


/* Hardware abstract layer functor   */

typedef void (* abstract_functor_void_func)(void);
typedef void(* abstract_functor_void_int)(u32 thId);
typedef u32(* abstract_functor_int_void_func)(void);
typedef u32(* abstract_functor_int_bool_func)(bool);
typedef u32(* abstract_functor_int_2_bool_func)(bool, bool);
typedef u32(* abstract_functor_int_3_bool_func)(bool, bool, bool);
typedef u32(* abstract_functor_int_int_func)(u32 ms);
typedef u32(* abstract_functor_int_2_int_func)(u32, u32);
typedef u32(* abstract_functor_int_3_int_func)(u32, u32, u32);
typedef u32(* abstract_functor_int_4_int_func)(u32, u32, u32, u32);
typedef u32(* abstract_functor_int_2_int_ref_func)(u32 &, u32 &);
typedef bool(* abstract_functor_bool_void_func)(void);
typedef bool(* abstract_functor_bool_int_func)(u32);
typedef u32(* abstract_functor_spec_dump)(caStringStream<s8> *ss);
typedef u32(* abstract_functor_spec_irq)(void *obj, u8 * buff, s_t size, s_t & iosize);

typedef struct tag_hal_llc_mem_io {
    const abstract_functor_int_void_func hll_mem_min_phy;
    const abstract_functor_int_void_func hll_mem_max_phy;
} hal_llc_mem_io;




typedef bool ( * abstract_functor_bool_int_func)(u32 t);

typedef struct tag_hal_llc_scheduler_io {
    const abstract_functor_int_void_func hll_tick;
    const abstract_functor_int_int_func hll_to_tick;
    const abstract_functor_bool_int_func hll_scheduler_valid_handle;
    const abstract_functor_bool_void_func hll_lock;
    const abstract_functor_bool_void_func hll_unlock;
    const abstract_functor_void_func hll_req_scheduler;  // call software req cpu dependent
} hal_llc_scheduler_io;



//Hardware connector for all coms: every one connect to specific hardware

typedef struct tag_hal_llc_com_io {
    void *hll_lnk_obj;
    const abstract_functor_int_4_int_func hll_config;
    const abstract_functor_int_void_func hll_tick;
    const abstract_functor_int_void_func hll_en_int_rx;
    const abstract_functor_int_void_func hll_start_tx;
    const abstract_functor_int_void_func hll_stop;
    const abstract_functor_int_2_bool_func hll_enable;
    const abstract_functor_spec_dump hll_dump;
    const abstract_functor_int_2_int_ref_func hll_get_errors;
    const abstract_functor_void_int hll_wakeup_1;
    const abstract_functor_void_int hll_wakeup_2;
    const abstract_functor_spec_irq hll_irq_tx;
    const abstract_functor_spec_irq hll_irq_rx;
    const abstract_functor_void_int hll_send;
    const abstract_functor_int_void_func hll_receive;
} hal_llc_com_io;

//Hardware connector  to system time ..

typedef struct tag_hal_llc_sys_time {
    void *hll_lnk_obj;
    const abstract_functor_int_4_int_func hll_config;
    const abstract_functor_int_void_func hll_tick;
    const abstract_functor_int_void_func hll_ps;
    const abstract_functor_int_void_func hll_ns;
    const abstract_functor_int_void_func hll_us;
    const abstract_functor_int_void_func hll_ms;
    const abstract_functor_int_void_func hll_sec;
    const abstract_functor_int_void_func hll_min;
    const abstract_functor_int_void_func hll_hour;
    const abstract_functor_int_void_func hll_day;
    const abstract_functor_int_4_int_func hll_settime;
    const abstract_functor_spec_dump hll_dump;
    const abstract_functor_int_int_func hll_to_tick;
    const abstract_functor_int_void_func hll_start;
    const abstract_functor_int_void_func hll_stop;
    const abstract_functor_void_int hll_wakeup_1;
    const abstract_functor_void_int hll_wakeup_2;
    const abstract_functor_spec_irq hll_irq_1;
    const abstract_functor_spec_irq hll_irq_2;
} hal_llc_sys_time;


//Hardware connector to interrupt high level functions

typedef struct tag_hal_llc_interrupt {
    const abstract_functor_void_func hll_enable;
    const abstract_functor_void_func hll_disable;
    const abstract_functor_void_func hll_wait_for_interrupt;
} hal_llc_interrupt;

//Hardware connector to system reset

typedef struct tag_hal_llc_reset {
    const abstract_functor_void_func hll_reset;
    const abstract_functor_void_func hll_shutdown;
    const abstract_functor_void_func hll_restart;
    const abstract_functor_void_func hll_leds_off;
    const abstract_functor_void_func hll_leds_on;
    const abstract_functor_void_int hll_led_off;
    const abstract_functor_void_int hll_led_on;
} hal_llc_reset;





/* LINK to COM1 */

extern hal_llc_com_io hal_llc_com1;

/* LINK to COM2 */
#if COM2_DEVICE 
extern hal_llc_com_io hal_llc_com2;
#endif
/* LINK to COM3 */
#if COM3_DEVICE 
extern hal_llc_com_io hal_llc_com3;
#endif
/* LINK to COM4 */
#if COM4_DEVICE 
extern hal_llc_com_io hal_llc_com4;
#endif
/* LINK to COM5 */
#if COM5_DEVICE 
extern hal_llc_com_io hal_llc_com5;
#endif
/* LINK to COM6 */
#if COM6_DEVICE 
extern hal_llc_com_io hal_llc_com6;
#endif
/* LINK to COM7 */
#if COM7_DEVICE 
extern hal_llc_com_io hal_llc_com7;
#endif
/* LINK to COM8 */
#if COM8_DEVICE 
extern hal_llc_com_io hal_llc_com8;
#endif

extern hal_llc_sys_time hal_llc_time_1;


/* MEM RAM BORDER */
extern hal_llc_mem_io hal_llc_mem;

/* LL SCHEDULER*/
extern hal_llc_scheduler_io hal_llc_scheduler;

extern hal_llc_interrupt hal_llc_int_req;

extern hal_llc_reset hal_llc_reset_req;


// NOT IMPLEMENTED

extern "C" inline u32 not_implemented_base(void) {
    return 0;
}
#endif /* HAL_H */

