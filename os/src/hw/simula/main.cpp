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
#include "memory.h"
#include "kdebug.h"
#include "caos_version.h"
#include "scheduler.h"
//in sim mode have to create the funcion for all HAL_CONNECTORS...


typedef void * (*functor)(void *);



u32 *sim_mem = nullptr;
u32 *sim_heap = nullptr;

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
    __ram_start__ = __ram_end__ = __heap_base__ = __heap_end__ = 0;
}

pthread_t * CreateThread(functor entry, void *param) {
    int ret;
    pthread_attr_t tattr;
    pthread_t *thread_id;
    thread_id = new pthread_t();
    if (thread_id != nullptr) {
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
        return nullptr;
}

void JoinThread(pthread_t *thread_id) {
    pthread_join(*thread_id, nullptr);
    delete thread_id;
    ;
}

void DestroyThread(pthread_t *thread_id) {
    if (thread_id != nullptr) {
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
        hal_llc_time_1.hll_irq_1(hal_llc_time_1.hll_lnk_obj, nullptr, 0, dummy);
        if (st.mn_Msec == SYS_TIMER_TICK) {
            st.mn_Msec = 0;
            st.mn_Sec++;
            hal_llc_time_1.hll_irq_2(hal_llc_time_1.hll_lnk_obj, nullptr, 0, dummy);
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
    static int i=0;
    printf("[%06d]ENTER FUNC : %s\n",st.mn_IrqCount, __func__);
    
    return i++;
}

bool sim_lock_switch_context(void) {
    printf("[%06d]ENTER FUNC : %s\n",st.mn_IrqCount, __func__);
    return true;
}

bool sim_unlock_switch_context(void) {
    printf("[%06d]ENTER FUNC : %s\n",st.mn_IrqCount, __func__);
    return true;
}

void sim_svc_request(void) {
    printf("[%06d]ENTER FUNC : %s\n",st.mn_IrqCount, __func__);

}

u32 sim_get_tick(void) {
    printf("[%06d]ENTER FUNC : %s\n",st.mn_IrqCount, __func__);
    return st.mn_IrqCount;
}

u32 sim_to_tick(u32 ms) {
    printf("[%06d]ENTER FUNC : %s\n",st.mn_IrqCount, __func__);
    return ms;
}

u32 sim_uart_configure(u32 /*speed*/, u32 /*stop*/, u32 /*parity*/, u32 /*data*/) {
    printf("[%06d]ENTER FUNC : %s\n",st.mn_IrqCount, __func__);
    return 0;
}

u32 sim_uart_enable_int(void) {
    printf("[%06d]ENTER FUNC : %s\n",st.mn_IrqCount, __func__);
    return 0;
}

u32 sim_uart_EnableIrqTx(void) {
    printf("[%06d]ENTER FUNC : %s\n",st.mn_IrqCount, __func__);
    return 0;
}

u32 sim_uart_stop(void) {
    printf("[%06d]ENTER FUNC : %s\n",st.mn_IrqCount, __func__);
    return 0;
}

u32 sim_uart_enable(bool /*rx*/, bool /*tx*/) {
    printf("[%06d]ENTER FUNC : %s\n",st.mn_IrqCount, __func__);
    return 0;
}

u32 sim_uart_dump(caStringStream<s8> * /*ptr_ss*/) {
    printf("[%06d]ENTER FUNC : %s\n",st.mn_IrqCount, __func__);
    return 0;
}

u32 sim_uart_get_errors(u32 & /*rxError*/, u32 & /*txError*/) {
    printf("[%06d]ENTER FUNC : %s\n",st.mn_IrqCount, __func__);
    return 0;
}

void sim_uart_send(u32 c) {
    putchar(c);
}

u32 sim_uart_recv(void) {
    return getchar();
}

u32 sim_timer_configure(u32, u32, u32, u32) {
    printf("[%06d]ENTER FUNC : %s\n",st.mn_IrqCount, __func__);
    return 0;
}

u32 sim_timer_get_psec(void) {
    printf("[%06d]ENTER FUNC : %s\n",st.mn_IrqCount, __func__);
    return 0;
}

u32 sim_timer_get_nsec(void) {
    printf("[%06d]ENTER FUNC : %s\n",st.mn_IrqCount, __func__);
    return 0;
}

u32 sim_timer_get_usec(void) {
    printf("[%06d]ENTER FUNC : %s\n",st.mn_IrqCount, __func__);
    return st.mn_Usec;
}

u32 sim_timer_get_msec(void) {
    printf("[%06d]ENTER FUNC : %s\n",st.mn_IrqCount, __func__);
    return st.mn_Msec;
}

u32 sim_timer_get_sec(void) {
    printf("[%06d]ENTER FUNC : %s\n",st.mn_IrqCount, __func__);
    return st.mn_Sec;
}

u32 sim_timer_get_min(void) {
    printf("[%06d]ENTER FUNC : %s\n",st.mn_IrqCount, __func__);
    return st.mn_Min;
}

u32 sim_timer_get_hour(void) {
    printf("[%06d]ENTER FUNC : %s\n",st.mn_IrqCount, __func__);
    return st.mn_Hour;
}

u32 sim_timer_get_day(void) {
    printf("[%06d]ENTER FUNC : %s\n",st.mn_IrqCount, __func__);
    return st.mn_Day;
}

u32 sim_timer_set_time(u32 day, u32 hour, u32 min, u32 sec) {
    printf("[%06d]ENTER FUNC : %s\n",st.mn_IrqCount, __func__);
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
    printf("[%06d]ENTER FUNC : %s\n",st.mn_IrqCount, __func__);
    u32 res = 0;
    if (ss != nullptr) {
        (*ss) << "simulated timer ..." << caEnd::endl;
        res = ss->Size();
    }
    return res;
}

void sim_int_disable_all(void) {
    printf("[%06d]ENTER FUNC : %s\n",st.mn_IrqCount, __func__);
}

void sim_int_enable_all(void) {
    printf("[%06d]ENTER FUNC : %s\n",st.mn_IrqCount, __func__);
}

void sim_int_wait_for_interrupt(void) {
    printf("[%06d]ENTER FUNC : %s\n",st.mn_IrqCount, __func__);
    while(1){}

}

void sim_reset_reset(void) {
    printf("[%06d]ENTER FUNC : %s\n",st.mn_IrqCount, __func__);
    exit(0);
}

void sim_reset_shutdown(void) {
    printf("[%06d]ENTER FUNC : %s\n",st.mn_IrqCount, __func__);
    exit(0);
}

void sim_reset_restart(void) {
    printf("[%06d]ENTER FUNC : %s\n",st.mn_IrqCount, __func__);
    exit(0);
}

void sim_led_ledsoff(void) {
    printf("[%06d]ENTER FUNC : %s\n",st.mn_IrqCount, __func__);

}

void sim_led_ledson(void) {
    printf("[%06d]ENTER FUNC : %s\n",st.mn_IrqCount, __func__);

}

void sim_led_ledoff(u32) {
    printf("[%06d]ENTER FUNC : %s\n",st.mn_IrqCount, __func__);

}

void sim_led_ledon(u32) {
    printf("[%06d]ENTER FUNC : %s\n",st.mn_IrqCount, __func__);

}



pthread_t *tick_th = nullptr;

void create_thread_tick(void) {
    caMemAux<u8>::MemZero((u8*) & st, sizeof (st));
    void *param = nullptr;
    tick_th = CreateThread(tick_thread, param);
}

void stop_thread_tick(void) {
    DestroyThread(tick_th);
    tick_th = nullptr;
}


///typedef u32(*thFunc)(u32 idx, u32 p1, u32 p2);
typedef void (* froute)(thFunc f, u32 p1, u32 p2);
void * go_start_thread(void *p){
    caThreadContext *task=(caThreadContext *)(p);
    froute f=(froute)task->pcb[16];
    thFunc ef=(thFunc)task->pcb[2];
    u32 p1=task->pcb[3];
    u32 p2=task->pcb[4];
    f(ef,p1,p2);   
    return nullptr;
}

bool sim_add_task(caThreadContext *task){
    printf("[%06d]ENTER FUNC : %s\n",st.mn_IrqCount, __func__);
    task->thid=ptr_to_uint(CreateThread(go_start_thread,task));
    return true;;
}


int main(void) {
    s8 buff[256];
    allocate_global_rams();
    Dbg::Put("> c.a.O.S. : [ SIMULATOR ");
    Dbg::Put(" ]\r\n");
    caMemory::Init(&hal_llc_mem);
    caMemory::DumpAvail(buff, sizeof (buff));
    Dbg::Put(buff);
    Dbg::Put("> c.a.O.S. : [ ");
    Dbg::Put(caos_version);
    Dbg::Put(" ]\r\n");
    create_thread_tick();
    hal_main();
    stop_thread_tick();
    free_global_rams();
    return 0;
}




