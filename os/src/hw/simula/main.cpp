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

#include <stdio.h>
#include <stdlib.h>
#include <hal.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include "adapter.h"
//in sim mode have to create the funcion for all HAL_CONNECTORS...


typedef void * (*functor)(void *);



u32 *sim_mem = NULL;
u32 *sim_heap = NULL;

u32 __ram_start__;
u32 __ram_end__;
u32 __heap_base__;
u32 __heap_end__;

void allocate_global_rams(void) {
    sim_mem = new u32[100000];
    sim_heap = new u32[100000];
    __ram_start__ = ptr_to_uint(sim_mem);
    __ram_end__ = ptr_to_uint(&sim_mem[100000]);
    //    std::cout << "> c.a.O.S. : [  Available memory = " << __ram_end__ - __ram_start__
    //            << " ] " < std::endl;
    __heap_base__ = ptr_to_uint(sim_heap);
    __heap_end__ = ptr_to_uint(&sim_heap[100000]);
}

void free_global_rams() {
    delete []sim_mem;
    delete []sim_heap;    
    __ram_start__ = __ram_end__ = __heap_base__ =__heap_end__ =0;
}

pthread_t * CreateThread(functor entry, void *param) {
    int ret;
    pthread_attr_t tattr;
    pthread_t *thread_id;
    thread_id = new pthread_t();
    if (thread_id != NULL) {
        ret = pthread_attr_init(&tattr);
        if (ret == 0 || ret == EBUSY) {
            ret = pthread_attr_setdetachstate(&tattr, PTHREAD_CREATE_JOINABLE);
            if (ret == 0) {
                ret = pthread_create(thread_id, &tattr, entry, param);
            }
        }
    }
    if (ret == 0)
        return thread_id;
    else
        return NULL;
}

void JoinThread(pthread_t *thread_id) {
    pthread_join(*thread_id, NULL);
    delete thread_id;
    ;
}

void DestroyThread(pthread_t *thread_id) {
    if (thread_id != NULL) {
        pthread_cancel(*thread_id);
        delete thread_id;
    }
}

int SleepThread(unsigned int delay) {
    if (delay < 1000)
        usleep(delay * 1000);
    else {
        usleep((delay % 1000)*1000);
        sleep(delay / 1000);
    }
    return 0;
}

typedef struct tag_sys_timer_status {
    volatile u32 mn_Usec;
    volatile u32 mn_Msec;
    volatile u32 mn_Sec;
    volatile u32 mn_Min;
    volatile u32 mn_Hour;
    volatile u32 mn_Day;
    volatile u32 mn_IrqCount;
} sysTimerStatus;


sysTimerStatus st;
// simulate the thread tickm  swaps...

void * tick_thread(void *) {
    s_t dummy;
    while (1) {
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
        SleepThread(1);
    }
}

/*************************************************/
/// ADAPTERS

bool sim_is_valid_context(u32 /*thid*/) {
    return true;
}

bool sim_lock_switch_context(void) {
    return true;
}

bool sim_unlock_switch_context(void) {
    return true;
}

void sim_svc_request(void) {

}

u32 sim_get_tick(void) {
    return st.mn_IrqCount;
}

u32 sim_to_tick(u32 ms) {
    return ms;
}

u32 sim_uart_configure(u32 /*speed*/, u32 /*stop*/, u32 /*parity*/, u32 /*data*/) {
    return 0;
}

u32 sim_uart_enable_int(void) {
    return 0;
}

u32 sim_uart_EnableIrqTx(void) {
    return 0;
}

u32 sim_uart_stop(void) {
    return 0;
}

u32 sim_uart_enable(bool /*rx*/, bool /*tx*/) {
    return 0;
}

u32 sim_uart_dump(caStringStream<s8> * /*ptr_ss*/) {
    return 0;
}

u32 sim_uart_get_errors(u32 & /*rxError*/, u32 & /*txError*/) {
    return 0;
}

void sim_uart_send(u32 /*c*/) {

}

u32 sim_uart_recv(void) {
    return 0;
}

u32 sim_timer_configure(u32, u32, u32, u32) {
    return 0;
}

u32 sim_timer_get_psec(void) {
    return 0;
}

u32 sim_timer_get_nsec(void) {
    return 0;
}

u32 sim_timer_get_usec(void) {
    return st.mn_Usec;
}

u32 sim_timer_get_msec(void) {
    return st.mn_Msec;
}

u32 sim_timer_get_sec(void) {
    return st.mn_Sec;
}

u32 sim_timer_get_min(void) {
    return st.mn_Min;
}

u32 sim_timer_get_hour(void) {
    return st.mn_Hour;
}

u32 sim_timer_get_day(void) {
    return st.mn_Day;
}

u32 sim_timer_set_time(u32 day, u32 hour, u32 min, u32 sec) {
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

u32 sim_timer_dump(caStringStream<s8> * ss) {
    u32 res = 0;
    if (ss != NULL) {
        (*ss) << "simulated timer ..." << caEnd::endl;
        res = ss->Size();
    }
    return res;
}

void sim_int_disable_all(void) {
}

void sim_int_enable_all(void) {
}

void sim_int_wait_for_interrupt(void) {

}

void sim_reset_reset(void) {
    exit(0);
}

void sim_reset_shutdown(void) {
    exit(0);
}

void sim_reset_restart(void) {
    exit(0);
}

void sim_led_ledsoff(void) {

}

void sim_led_ledson(void) {

}

void sim_led_ledoff(u32) {

}

void sim_led_ledon(u32) {

}



pthread_t *tick_th = NULL;

void create_thread_tick(void) {
    caMemAux<u8>::MemZero((u8*) & st, sizeof (st));
    void *param = NULL;
    tick_th = CreateThread(tick_thread, param);
}

void stop_thread_tick(void) {
    DestroyThread(tick_th);
    tick_th = NULL;
}

int main(void) {
    allocate_global_rams();
    create_thread_tick();
    hal_main();
    stop_thread_tick();
    free_global_rams();
    return 0;
}




