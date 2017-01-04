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


#ifndef HAL_H
#define HAL_H

#include "config.h"
#include "caos_c_types.h"
#include "stream.h"
#include "devicehandle.h"
#include "idevice.h"


/* Hardware abstract layer access   */
typedef void (* abstract_functor_void_func)(void);
typedef u32(* abstract_functor_int_int_func)(u32 ms);
typedef void(* abstract_functor_void_int)(u32 thId);
typedef u32(* abstract_functor_int_void_func)(void);

typedef struct tag_hal_ll_mem_io {
    abstract_functor_int_void_func hll_mem_min_phy;
    abstract_functor_int_void_func hll_mem_max_phy;
} hal_ll_mem_io;

/* MEM RAM BORDER */
extern const hal_ll_mem_io hal_ll_mem;



typedef bool ( * abstract_functor_scheduler_valid_handler)(u32 t);

typedef struct tag_hal_ll_scheduler_io {
    abstract_functor_scheduler_valid_handler hll_scheduler_valid_handle;
} hal_ll_scheduler_io;
/* LL SCHEDULER*/
extern const hal_ll_scheduler_io hal_ll_scheduler;


/* interface to low level io  */
typedef u32(* abstract_functor_com_setup)(u32 speed, u32 stop, u32 parity, u32 data);
typedef u32(* abstract_functor_com_void)(void);
typedef u32(* abstract_functor_com_enable)(bool rx, bool tx);
typedef u32(* abstract_functor_com_dump)(caStringStream<s8> *ss);
typedef u32(* abstract_functor_com_irq_rx)(void *obj, u8 * rxbuff, s_t size, s_t & readed);
typedef u32(* abstract_functor_com_irq_tx)(void *obj, u8 * txbuff, s_t size, s_t & writed);
typedef u32(* abstract_functor_com_get_errors)(u32 & rxError, u32 & txError);



//Hardware connector for coms
typedef struct tag_hal_ll_com_io {
    void *hll_lnk_obj;
    const abstract_functor_com_setup hll_config;
    const abstract_functor_com_void hll_time;
    const abstract_functor_com_void hll_en_int_rx;
    const abstract_functor_com_void hll_start_tx;
    const abstract_functor_com_void hll_stop;
    const abstract_functor_com_enable hll_enable;
    const abstract_functor_com_dump hll_dump;
    const abstract_functor_com_get_errors hll_get_errors;
    const abstract_functor_void_int hll_wakeup_rx;
    const abstract_functor_void_int hll_wakeup_tx;
    const abstract_functor_com_irq_rx hll_irq_rx;
    const abstract_functor_com_irq_tx hll_irq_tx;
    const abstract_functor_void_int hll_send;
    const abstract_functor_com_void hll_receive;
} hal_ll_com_io;

//Hardware connector  to system time ..
typedef u32(* abstract_functor_set_time)(u32 day,u32 hour,u32 min, u32 sec);

typedef struct tag_hal_ll_sys_time {
    const abstract_functor_com_void hll_tick;
    const abstract_functor_com_void hll_ms;
    const abstract_functor_com_void hll_sec;
    const abstract_functor_com_void hll_min;
    const abstract_functor_com_void hll_hour;
    const abstract_functor_com_void hll_day; 
    const abstract_functor_set_time hll_settime;
    const abstract_functor_com_dump hll_dump;
    const abstract_functor_int_int_func hll_to_tick;
    const abstract_functor_int_void_func hll_start;
    const abstract_functor_int_void_func hll_stop;    
} hal_ll_sys_time;

//Hardware connector to switch context
typedef bool (* abstract_functor_lock_sw_ctx)(void);
typedef struct tag_hal_ll_sw_ctx {
    const abstract_functor_lock_sw_ctx hll_lock;
    const abstract_functor_lock_sw_ctx hll_unlock;
} hal_ll_sw_ctx;


//Hardware connector to interrupt high level functions

typedef struct tag_hal_ll_interrupt {
    const abstract_functor_void_func hll_enable;
    const abstract_functor_void_func hll_disable;
    const abstract_functor_void_func hll_wait_for_interrupt;
} hal_ll_interrupt;

//Hardware connector to system reset
typedef struct tag_hal_ll_reset {
    const abstract_functor_void_func hll_reset;
    const abstract_functor_void_func hll_shutdown;
    const abstract_functor_void_func hll_restart;
    const abstract_functor_void_func hll_leds_off;
    const abstract_functor_void_func hll_leds_on;
    const abstract_functor_void_int hll_led_off;    
    const abstract_functor_void_int hll_led_on;
} hal_ll_reset;





/* LINK to COM1 */
#if COM1_DEVICE 
extern hal_ll_com_io hal_ll_com1;
#endif
/* LINK to COM2 */
#if COM2_DEVICE 
extern hal_ll_com_io hal_ll_com2;
#endif
/* LINK to COM3 */
#if COM3_DEVICE 
extern hal_ll_com_io hal_ll_com3;
#endif
/* LINK to COM4 */
#if COM4_DEVICE 
extern hal_ll_com_io hal_ll_com4;
#endif
/* LINK to COM5 */
#if COM5_DEVICE 
extern hal_ll_com_io hal_ll_com5;
#endif
/* LINK to COM6 */
#if COM6_DEVICE 
extern hal_ll_com_io hal_ll_com6;
#endif
/* LINK to COM7 */
#if COM7_DEVICE 
extern hal_ll_com_io hal_ll_com7;
#endif
/* LINK to COM8 */
#if COM8_DEVICE 
extern hal_ll_com_io hal_ll_com8;
#endif

#if SYS_TIMER_DEVICE
extern hal_ll_sys_time hal_ll_time;
#endif

extern hal_ll_sw_ctx hal_ll_switch_ctx;

extern hal_ll_interrupt  hal_ll_int_req;

extern hal_ll_reset hal_ll_reset_req;
#endif /* HAL_H */

