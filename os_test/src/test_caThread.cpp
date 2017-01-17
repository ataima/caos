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

#include "CPPtester.h"
#include "docMacro.h"
#include "hal.h"
#include <iostream>
#include "scheduler.h"
#include "memory.h"


static u32 th_enable = 0;
static u32 th_disable = 0;
static u32 th_wait = 0;

static void hll_enable(void) {
    th_enable = 1;
}

static void hll_disable(void) {
    th_disable = 1;
}

static void hll_wait(void) {
    th_wait = 1;
}



hal_llc_interrupt hal_llc_int_req{
    hll_enable,
    hll_disable,
    hll_wait};

extern void start_time_scheduler(void);

class test_caThread_class
: public caTester {
    CA_TEST_SUITE(test_caThread_class);
    CA_TEST(test_caThread_class::test1, "createThread 1");
    CA_TEST(test_caThread_class::test2, "createThread 2");
    CA_TEST(test_caThread_class::test3, "createThread 3");
    CA_TEST(test_caThread_class::test3, "createThread & removeThread");
    CA_TEST_SUITE_END();

    void setUp(void) {
        caMemory::Init();
        start_time_scheduler();

    }

    void tearDown(void) {
    }

    void test1(void);
    void test2(void);
    void test3(void);
    void test4(void);

};

REGISTER_CLASS(test_caThread_class);

static u32 dummy(u32 idx, u32 p1, u32 p2) {
    std::cout << "IDEX  =" << idx << std::endl;
    std::cout << "PARM 1=" << p1 << std::endl;
    std::cout << "PARM 2=" << p2 << std::endl;
    return 0;
}

void test_caThread_class::test1(void) {
    _START();
    _INFO("to check CreateThread function of class caThread");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();
    caScheduler::Init(caSchedulerMode::RoundRobin);
    u32 index = caScheduler::AddJob("PROVA", caThreadPriority::caThLevel6,
            dummy, 1000, 2000, 0x4000);
    CA_ASSERT(index == (BASE_HANDLE + 1));
    caScheduler::GetNextContext();
    caThreadContext *ctx = caScheduler::GetCurrentContext();
    u32 res = memcmp(ctx->name, "PROVA", 5);
    CA_ASSERT(res == 0);
    CA_ASSERT(ctx->cur_prio == caThreadPriority::caThLevel6);
    CA_ASSERT(ctx->pcb[3] == 1000);
    CA_ASSERT(ctx->pcb[4] == 2000);
    CA_ASSERT((ctx->stack_start - ctx->stack_end) > 0x4000);
    caScheduler::Destroy();
}

void test_caThread_class::test2(void) {
    _START();
    _INFO("to check CreateThread function of class caThread with 2 thread");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();
    caScheduler::Init(caSchedulerMode::RoundRobin);
    u32 index1 = caScheduler::AddJob("PROVA1", caThreadPriority::caThLevel6,
            dummy, 1000, 2000, 0x4000);
    u32 index2 = caScheduler::AddJob("PROVA2", caThreadPriority::caThLevel1,
            dummy, 1000, 2000, 0x4000);
    CA_ASSERT(index1 == (BASE_HANDLE + 1));
    CA_ASSERT(index2 == (BASE_HANDLE + 2));
    caScheduler::GetNextContext();
    caThreadContext *ctx = caScheduler::GetCurrentContext();
    CA_ASSERT(memcmp(ctx->name, "PROVA1", 6) == 0);
    CA_ASSERT(ctx->cur_prio == caThreadPriority::caThLevel6);
    caScheduler::GetNextContext();
    ctx = caScheduler::GetCurrentContext();
    CA_ASSERT(memcmp(ctx->name, "PROVA2", 6) == 0);
    CA_ASSERT(ctx->cur_prio == caThreadPriority::caThLevel1);
    caScheduler::GetNextContext();
    ctx = caScheduler::GetCurrentContext();
    CA_ASSERT(memcmp(ctx->name, "PROVA1", 6) == 0);
    CA_ASSERT(ctx->cur_prio == caThreadPriority::caThLevel6);
    caScheduler::GetNextContext();
    ctx = caScheduler::GetCurrentContext();
    CA_ASSERT(memcmp(ctx->name, "PROVA2", 6) == 0);
    CA_ASSERT(ctx->cur_prio == caThreadPriority::caThLevel1);
    caScheduler::Destroy();
}

void test_caThread_class::test3(void) {
    _START();
    _INFO("to check CreateThread function of class caThread with 100 thread");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();
    caScheduler::Init(caSchedulerMode::RoundRobin);
    u32 i;
    s8 buff[100];

    for (i = 0; i < 20; i++) {
        sprintf(buff, "PROVA%03d", i);
        u32 index1 = caScheduler::AddJob(buff, caThreadPriority::caThLevel6,
                dummy, 1000, 2000);
        CA_ASSERT(index1 == (BASE_HANDLE + 1 + i));
    }
    CA_ASSERT(caScheduler::Size()==20);
    for (i = 0; i < 100; i++) {
        sprintf(buff, "PROVA%03d", i%20);
        caScheduler::GetNextContext();
        caThreadContext *ctx = caScheduler::GetCurrentContext();
        CA_ASSERT(ctx->cur_prio == caThreadPriority::caThLevel6);
        CA_ASSERT(memcmp(ctx->name, buff, 8) == 0);
        CA_ASSERT(ctx->pcb[3] == 1000);
        CA_ASSERT(ctx->pcb[4] == 2000);
    }
    caScheduler::Destroy();
}


void test_caThread_class::test4(void) {
    _START();
    _INFO("to check CreateThread function of class caThread with 100 and remove  thread");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();
    caScheduler::Init(caSchedulerMode::RoundRobin);
    u32 i;
    s8 buff[100];
    caThreadContext *ctxs[20];
    for (i = 0; i < 20; i++) {
        sprintf(buff, "PROVA%03d", i);
        u32 index1 = caScheduler::AddJob(buff, caThreadPriority::caThLevel6,
                dummy, 1000, 2000);
        CA_ASSERT(index1 == (BASE_HANDLE + 1 + i));
    }
    CA_ASSERT(caScheduler::Size()==20);
    for (i = 0; i < 100; i++) {
        sprintf(buff, "PROVA%03d", i%20);
        caScheduler::GetNextContext();
        caThreadContext *ctx = caScheduler::GetCurrentContext();
        ctxs[i%20]=ctx;
        CA_ASSERT(ctx->cur_prio == caThreadPriority::caThLevel6);
        CA_ASSERT(memcmp(ctx->name, buff, 8) == 0);
        CA_ASSERT(ctx->pcb[3] == 1000);
        CA_ASSERT(ctx->pcb[4] == 2000);
    }
    caScheduler::RemoveJob(ctxs[0]->index);
    CA_ASSERT(caScheduler::Size()==19);
    for (i = 0; i < 19; i++) {
        sprintf(buff, "PROVA%03d", i);
        caScheduler::GetNextContext();
        caThreadContext *ctx = caScheduler::GetCurrentContext();
        CA_ASSERT(ctx->cur_prio == caThreadPriority::caThLevel6);
        CA_ASSERT(memcmp(ctx->name, buff, 8) == 0);
        CA_ASSERT(ctx->pcb[3] == 1000);
        CA_ASSERT(ctx->pcb[4] == 2000);
    }
    caScheduler::Destroy();
}