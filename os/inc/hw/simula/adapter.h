#ifndef _SIMULA_ADAPTER_
#define _SIMULA_ADAPTER_
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

extern u32 __ram_start__;
extern u32 __ram_end__;

u32 sim_get_tick(void);
u32 sim_to_tick(u32 ms);
int hal_main(void);


bool sim_is_valid_context(u32 thid);
bool sim_lock_switch_context(void);
bool sim_unlock_switch_context(void);
void sim_svc_request(void);
u32 sim_uart_configure(u32 speed, u32 stop, u32 parity, u32 data);
u32 sim_uart_enable_int(void);
u32 sim_uart_EnableIrqTx(void);
u32 sim_uart_stop(void);
u32 sim_uart_enable(bool rx, bool tx);
u32 sim_uart_dump(caStringStream<s8> * ptr_ss);
u32 sim_uart_get_errors(u32 &rxError, u32 & txError);
void sim_uart_send(u32 c);
u32 sim_uart_recv(void);
u32 sim_uart_configure(u32, u32, u32, u32);
u32 sim_timer_configure(u32, u32, u32, u32);
u32 sim_timer_get_psec(void);
u32 sim_timer_get_nsec(void);
u32 sim_timer_get_usec(void);
u32 sim_timer_get_msec(void);
u32 sim_timer_get_sec(void);
u32 sim_timer_get_min(void);
u32 sim_timer_get_hour(void);
u32 sim_timer_get_day(void);
u32 sim_timer_set_time(u32 day, u32 hour, u32 min, u32 sec);
u32 sim_timer_dump(caStringStream<s8> * ss);
void sim_int_disable_all(void);
void sim_int_enable_all(void);
void sim_int_wait_for_interrupt(void);
void sim_reset_reset(void);
void sim_reset_shutdown(void);
void sim_reset_restart(void);
void sim_led_ledsoff(void);
void sim_led_ledson(void);
void sim_led_ledoff(u32);
void sim_led_ledon(u32);


#endif