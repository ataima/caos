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


typedef u32(* abstract_functor_mem)(void);

typedef struct tag_hal_ll_mem_io {
    abstract_functor_mem hll_mem_min_phy;
    abstract_functor_mem hll_mem_max_phy;
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
typedef u32(* abstract_functor_com_setup)(caIDeviceConfigure * in);
typedef u32(* abstract_functor_com_write)(u32 *buff, s_t size);
typedef u32(* abstract_functor_com_read)(u32 *buff, s_t size);
typedef u32(* abstract_functor_com_void)(void);
typedef u32(* abstract_functor_com_enable)(bool rx, bool tx);
typedef u32(* abstract_functor_com_dump)(caStringStream<s8> *ss);

typedef struct tag_hal_ll_com_io {
    abstract_functor_com_setup hll_config;
    abstract_functor_com_write hll_write;
    abstract_functor_com_read hll_read;
    abstract_functor_com_void hll_time;
    abstract_functor_com_void hll_en_int;
    abstract_functor_com_void hll_start_tx;
    abstract_functor_com_void hll_stop;
    abstract_functor_com_enable hll_enable;
    abstract_functor_com_dump hll_dump;
} hal_ll_com_io;

/* LINK to COM1 */
#if COM1_DEVICE 
extern const hal_ll_com_io hal_ll_com1;
#endif
/* LINK to COM2 */
#if COM2_DEVICE 
extern const hal_ll_com_io hal_ll_com2;
#endif
/* LINK to COM3 */
#if COM3_DEVICE 
extern const hal_ll_com_io hal_ll_com3;
#endif
/* LINK to COM4 */
#if COM4_DEVICE 
extern const hal_ll_com_io hal_ll_com4;
#endif
/* LINK to COM5 */
#if COM5_DEVICE 
extern const hal_ll_com_io hal_ll_com5;
#endif
/* LINK to COM6 */
#if COM6_DEVICE 
extern const hal_ll_com_io hal_ll_com6;
#endif
/* LINK to COM7 */
#if COM7_DEVICE 
extern const hal_ll_com_io hal_ll_com7;
#endif
/* LINK to COM8 */
#if COM8_DEVICE 
extern const hal_ll_com_io hal_ll_com8;
#endif



#endif /* HAL_H */

