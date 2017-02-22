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
#include <vector>
#include <algorithm>


static struct timespec start_time = {0, 0};
static struct timespec c_spec = {0, 0};

static bool isvalidcontext(u32 num) {
    if (num == 12345678)
        return false;
    if (num == 0)
        return false;
    return true;
}

static u32 hll_time(void) {
    u32 us;
    struct timespec diff;
    clock_gettime(CLOCK_REALTIME, &c_spec);
    diff.tv_sec = (time_t) difftime(c_spec.tv_sec, start_time.tv_sec);
    if (c_spec.tv_nsec > start_time.tv_nsec)
        diff.tv_nsec = c_spec.tv_nsec - start_time.tv_nsec;
    else {
        c_spec.tv_sec--;
        diff.tv_nsec = c_spec.tv_nsec - start_time.tv_nsec + 1000000000;
    }
    us = (u32) (diff.tv_sec * 1000000 + diff.tv_nsec / 1000);
    if (us == 0)
        us = 1;
    return us;
}

static u32 hll_totick(u32 ms) {
    return 100 + ms;
}

static bool hll_enable_lock(void) {
    std::cout << "SPINLOCK " << std::endl;
    return true;
}

static bool hll_enable_unlock(void) {
    std::cout << "SPINUNLOCK " << std::endl;
    return true;
}

static void hll_req_schedule(void) {
    std::cout << "REQ SCHEDULER " << std::endl;
    caScheduler::GetNextContext();
}

static bool hll_add_task(caThreadContext *ctx) {
    std::cout << "ADD TASK " << std::endl;
    caScheduler::AddTask(ctx);
    return true;
}

hal_llc_scheduler_io hal_llc_scheduler = {
    hll_time,
    hll_totick,
    hll_add_task,
    isvalidcontext,
    hll_enable_lock,
    hll_enable_unlock,
    hll_req_schedule
};

void start_time_scheduler(void) {
    clock_gettime(CLOCK_REALTIME, &start_time);
}


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
    CA_TEST(test_caThread_class::test4, "createThread & removeThread");
    CA_TEST(test_caThread_class::test5, "createThread & removeThread & sleep");
    CA_TEST(test_caThread_class::test6, "createThread pri 1 ");
    CA_TEST(test_caThread_class::test7, "createThread pri 2 ");
    CA_TEST(test_caThread_class::test8, "createThread pri 3 ");
    CA_TEST(test_caThread_class::test9, "change priority ");
    CA_TEST_SUITE_END();

    void setUp(void) {
        caMemory::Init(&hal_llc_mem);
        start_time_scheduler();

    }

    void tearDown(void) {
    }

    void test1(void);
    void test2(void);
    void test3(void);
    void test4(void);
    void test5(void);
    void test6(void);
    void test7(void);
    void test8(void);
    void test9(void);

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
    u32 index = caScheduler::AddJob("PROVA", caJobPriority::caThLevel6,
            dummy, 1000, 2000, 0x4000);
    CA_ASSERT(index == (0));
    caScheduler::GetNextContext();
    caThreadContext *ctx = caScheduler::GetCurrentContext();
    u32 res = memcmp(ctx->name, "PROVA", 5);
    CA_ASSERT(res == 0);
    CA_ASSERT(ctx->cur_prio == caJobPriority::caThLevel6);
    CA_ASSERT(ctx->pcb[3] == 1000);
    CA_ASSERT(ctx->pcb[4] == 2000);
    CA_ASSERT((ctx->stack_start - ctx->stack_end) > 0x4000);
    CA_ASSERT(ctx->status == caJobStatus::thInit);
    CA_ASSERT(caScheduler::Destroy() == true);
}

void test_caThread_class::test2(void) {
    _START();
    _INFO("to check CreateThread function of class caThread with 2 thread");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();
    caScheduler::Init(caSchedulerMode::RoundRobin);
    u32 index1 = caScheduler::AddJob("PROVA1", caJobPriority::caThLevel6,
            dummy, 1000, 2000, 0x4000);
    u32 index2 = caScheduler::AddJob("PROVA2", caJobPriority::caThLevel1,
            dummy, 1000, 2000, 0x4000);
    CA_ASSERT(index1 == (0));
    CA_ASSERT(index2 == (1));
    caScheduler::GetNextContext();
    caThreadContext *ctx = caScheduler::GetCurrentContext();
    CA_ASSERT(ctx->status == caJobStatus::thInit);
    std::cout << (const char *) ctx->name << std::endl;
    CA_ASSERT(memcmp(ctx->name, "PROVA1", 6) == 0);
    CA_ASSERT(ctx->cur_prio == caJobPriority::caThLevel6);
    caScheduler::GetNextContext();
    ctx = caScheduler::GetCurrentContext();
    CA_ASSERT(ctx->status == caJobStatus::thInit);
    std::cout << (const char *) ctx->name << std::endl;
    CA_ASSERT(memcmp(ctx->name, "PROVA2", 6) == 0);
    CA_ASSERT(ctx->cur_prio == caJobPriority::caThLevel1);
    caScheduler::GetNextContext();
    ctx = caScheduler::GetCurrentContext();
    CA_ASSERT(ctx->status == caJobStatus::thInit);
    std::cout << (const char *) ctx->name << std::endl;
    CA_ASSERT(memcmp(ctx->name, "PROVA1", 6) == 0);
    CA_ASSERT(ctx->cur_prio == caJobPriority::caThLevel6);
    caScheduler::GetNextContext();
    ctx = caScheduler::GetCurrentContext();
    CA_ASSERT(ctx->status == caJobStatus::thInit);
    std::cout << (const char *) ctx->name << std::endl;
    CA_ASSERT(memcmp(ctx->name, "PROVA2", 6) == 0);
    CA_ASSERT(ctx->cur_prio == caJobPriority::caThLevel1);
    CA_ASSERT(caScheduler::Destroy() == true)
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
        u32 index1 = caScheduler::AddJob(buff, caJobPriority::caThLevel6,
                dummy, 1000, 2000);
        CA_ASSERT(index1 == (i));
    }
    CA_ASSERT(caScheduler::Size() == 20);
    for (i = 0; i < 100; i++) {
        sprintf(buff, "PROVA%03d", i % 20);
        caScheduler::GetNextContext();
        caThreadContext *ctx = caScheduler::GetCurrentContext();
        CA_ASSERT(ctx->status == caJobStatus::thInit);
        CA_ASSERT(ctx->cur_prio == caJobPriority::caThLevel6);
        CA_ASSERT(memcmp(ctx->name, buff, 8) == 0);
        CA_ASSERT(ctx->pcb[3] == 1000);
        CA_ASSERT(ctx->pcb[4] == 2000);
    }
    CA_ASSERT(caScheduler::Destroy() == true);
}

void test_caThread_class::test4(void) {
    _START();
    _INFO("to check CreateThread function of class caThread with 100 and remove  thread");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();
    caScheduler::Init(caSchedulerMode::RoundRobin);
    u32 i, u;
    s8 buff[100];
    caThreadContext * ctxs[20];
    for (i = 0; i < 20; i++) {
        sprintf(buff, "PROVA%03d", i);
        u32 index1 = caScheduler::AddJob(buff, caJobPriority::caThLevel6,
                dummy, 1000, 2000);
        CA_ASSERT(index1 == (i));
    }
    CA_ASSERT(caScheduler::Size() == 20);
    for (i = 0; i < 100; i++) {
        sprintf(buff, "PROVA%03d", i % 20);
        caScheduler::GetNextContext();
        caThreadContext *ctx = caScheduler::GetCurrentContext();
        CA_ASSERT(ctx->status == caJobStatus::thInit);
        ctxs[i % 20] = ctx;
        CA_ASSERT(ctx->cur_prio == caJobPriority::caThLevel6);
        CA_ASSERT(memcmp(ctx->name, buff, 8) == 0);
        std::cout << ctx->name << " : " << ctx->time << "<-->" << buff << std::endl;
        CA_ASSERT(ctx->pcb[3] == 1000);
        CA_ASSERT(ctx->pcb[4] == 2000);
    }
    for (u = 1; u < 15; u++) {
        CA_ASSERT(caScheduler::RemoveJob(ctxs[0]->index) == true);
        CA_ASSERT(caScheduler::Size() == 20 - u + 1);
        for (i = 0; i < caScheduler::Size(); i++) {
            if (i == 0)
                sprintf(buff, "PROVA%03d", caScheduler::Size() - 1);
            else
                sprintf(buff, "PROVA%03d", i);
            caScheduler::GetNextContext();
            caThreadContext *ctx = caScheduler::GetCurrentContext();
            ctxs[i] = ctx;
            CA_ASSERT(ctx->status == caJobStatus::thInit);
            CA_ASSERT(ctx->cur_prio == caJobPriority::caThLevel6);
            std::cout << ctx->name << " : " << ctx->time << "<-->" << buff << std::endl;
            CA_ASSERT(memcmp(ctx->name, buff, 8) == 0);
            CA_ASSERT(ctx->pcb[3] == 1000);
            CA_ASSERT(ctx->pcb[4] == 2000);
        }
        CA_ASSERT(caScheduler::Size() == 20 - u);
    }
    CA_ASSERT(caScheduler::Destroy() == true);
}

void test_caThread_class::test5(void) {
    _START();
    _INFO("to check CreateThread function of class caThread with 2 thread and set sleep ");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();
    caScheduler::Init(caSchedulerMode::RoundRobin);
    u32 index1 = caScheduler::AddJob("PROVA1", caJobPriority::caThLevel6,
            dummy, 1000, 2000, 0x4000);
    u32 index2 = caScheduler::AddJob("PROVA2", caJobPriority::caThLevel1,
            dummy, 1000, 2000, 0x4000);
    CA_ASSERT(index1 == (0));
    CA_ASSERT(index2 == (1));
    caScheduler::GetNextContext();
    caThreadContext *ctx = caScheduler::GetCurrentContext();
    CA_ASSERT(ctx->status == caJobStatus::thInit);
    std::cout << (const char *) ctx->name << std::endl;
    CA_ASSERT(memcmp(ctx->name, "PROVA1", 6) == 0);
    CA_ASSERT(ctx->cur_prio == caJobPriority::caThLevel6);
    caScheduler::GetNextContext();
    ctx = caScheduler::GetCurrentContext();
    CA_ASSERT(ctx->status == caJobStatus::thInit);
    std::cout << (const char *) ctx->name << std::endl;
    CA_ASSERT(memcmp(ctx->name, "PROVA2", 6) == 0);
    CA_ASSERT(ctx->cur_prio == caJobPriority::caThLevel1);
    caScheduler::Sleep(3);
    caScheduler::GetNextContext();
    ctx = caScheduler::GetCurrentContext();
    CA_ASSERT(ctx->status == caJobStatus::thInit);
    std::cout << (const char *) ctx->name << std::endl;
    CA_ASSERT(memcmp(ctx->name, "PROVA1", 6) == 0);
    CA_ASSERT(ctx->cur_prio == caJobPriority::caThLevel6);
    caScheduler::GetNextContext();
    ctx = caScheduler::GetCurrentContext();
    CA_ASSERT(ctx->status == caJobStatus::thInit);
    std::cout << (const char *) ctx->name << std::endl;
    CA_ASSERT(memcmp(ctx->name, "PROVA1", 6) == 0);
    CA_ASSERT(ctx->cur_prio == caJobPriority::caThLevel6);
    caScheduler::GetNextContext();
    ctx = caScheduler::GetCurrentContext();
    CA_ASSERT(ctx->status == caJobStatus::thInit);
    std::cout << (const char *) ctx->name << std::endl;
    CA_ASSERT(memcmp(ctx->name, "PROVA1", 6) == 0);
    CA_ASSERT(ctx->cur_prio == caJobPriority::caThLevel6);
    caScheduler::GetNextContext();
    ctx = caScheduler::GetCurrentContext();
    CA_ASSERT(ctx->status == caJobStatus::thInit);
    std::cout << (const char *) ctx->name << std::endl;
    CA_ASSERT(memcmp(ctx->name, "PROVA1", 6) == 0);
    CA_ASSERT(ctx->cur_prio == caJobPriority::caThLevel6);
    caScheduler::GetNextContext();
    ctx = caScheduler::GetCurrentContext();
    CA_ASSERT(ctx->status == caJobStatus::thInit);
    std::cout << (const char *) ctx->name << std::endl;
    CA_ASSERT(memcmp(ctx->name, "PROVA1", 6) == 0);
    CA_ASSERT(ctx->cur_prio == caJobPriority::caThLevel6);
    caScheduler::GetNextContext();
    ctx = caScheduler::GetCurrentContext();
    CA_ASSERT(ctx->status == caJobStatus::thInit);
    std::cout << (const char *) ctx->name << std::endl;
    CA_ASSERT(memcmp(ctx->name, "PROVA1", 6) == 0);
    CA_ASSERT(ctx->cur_prio == caJobPriority::caThLevel6);
    caScheduler::GetNextContext();
    ctx = caScheduler::GetCurrentContext();
    CA_ASSERT(ctx->status == caJobStatus::thInit);
    std::cout << (const char *) ctx->name << std::endl;
    CA_ASSERT(memcmp(ctx->name, "PROVA1", 6) == 0);
    CA_ASSERT(ctx->cur_prio == caJobPriority::caThLevel6);
    caScheduler::GetNextContext();
    ctx = caScheduler::GetCurrentContext();
    CA_ASSERT(ctx->status == caJobStatus::thInit);
    std::cout << (const char *) ctx->name << std::endl;
    CA_ASSERT(memcmp(ctx->name, "PROVA1", 6) == 0);
    CA_ASSERT(ctx->cur_prio == caJobPriority::caThLevel6);
    caScheduler::GetNextContext();
    ctx = caScheduler::GetCurrentContext();
    CA_ASSERT(ctx->status == caJobStatus::thInit);
    std::cout << (const char *) ctx->name << std::endl;
    CA_ASSERT(memcmp(ctx->name, "PROVA1", 6) == 0);
    CA_ASSERT(ctx->cur_prio == caJobPriority::caThLevel6);
    caScheduler::GetNextContext();
    ctx = caScheduler::GetCurrentContext();
    CA_ASSERT(ctx->status == caJobStatus::thInit);
    std::cout << (const char *) ctx->name << std::endl;
    CA_ASSERT(memcmp(ctx->name, "PROVA1", 6) == 0);
    CA_ASSERT(ctx->cur_prio == caJobPriority::caThLevel6);

    CA_ASSERT(caScheduler::Destroy() == true)
}

void test_caThread_class::test6(void) {
    _START();
    _INFO("to check CreateThread function of class caThread with 2 thread");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();
    caScheduler::Init(caSchedulerMode::Priority);
    u32 index1 = caScheduler::AddJob("PROVA1", caJobPriority::caThLevel1,
            dummy, 1000, 2000, 0x4000);
    u32 index2 = caScheduler::AddJob("PROVA2", caJobPriority::caThLevel0,
            dummy, 1000, 2000, 0x4000);
    CA_ASSERT(index1 == (0));
    CA_ASSERT(index2 == (1));

    u32 i;
    for (i = 0; i < 1000; i++) {
        caScheduler::GetNextContext();
        caScheduler::GetCurrentContext();
    }
    caScheduler::GetNextContext();
    caThreadContext *ctx1 = caScheduler::GetCurrentContext();
    u32 nSw1 = ctx1->nswitch;
    caThreadContext *ctx2 = ctx1;
    while (ctx1 == ctx2) {
        caScheduler::GetNextContext();
        ctx2 = caScheduler::GetCurrentContext();
    }
    u32 nSw2 = ctx2->nswitch;
    std::cout << (const char *) ctx1->name
            << " : N =" << ctx1->nswitch << std::endl;
    std::cout << (const char *) ctx2->name
            << " : N =" << ctx2->nswitch << std::endl;
    // 1002/3->334*2=668, 334
    if (nSw1 > nSw2) {
        CA_ASSERT(nSw1 >= ((2 * nSw2) - 10) && nSw1 < (2 * nSw2));
        CA_ASSERT(memcmp(ctx1->name, "PROVA1", 6) == 0);
        CA_ASSERT(ctx1->priority == caJobPriority::caThLevel1);
        CA_ASSERT(memcmp(ctx2->name, "PROVA2", 6) == 0);
        CA_ASSERT(ctx2->priority == caJobPriority::caThLevel0);
    } else {
        CA_ASSERT(nSw2 >= ((2 * nSw1) - 10) && nSw2 < (2 * nSw1));
        CA_ASSERT(memcmp(ctx2->name, "PROVA1", 6) == 0);
        CA_ASSERT(ctx2->priority == caJobPriority::caThLevel0);
        CA_ASSERT(memcmp(ctx1->name, "PROVA2", 6) == 0);
        CA_ASSERT(ctx1->priority == caJobPriority::caThLevel1);
    }
    CA_ASSERT(caScheduler::Destroy() == true);
}

void test_caThread_class::test7(void) {
    _START();
    _INFO("to check CreateThread function of class caThread with 3 thread");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();
    caScheduler::Init(caSchedulerMode::Priority);
    u32 index1 = caScheduler::AddJob("PROVA1", caJobPriority::caThLevel2,
            dummy, 1000, 2000, 0x4000);
    u32 index2 = caScheduler::AddJob("PROVA2", caJobPriority::caThLevel1,
            dummy, 1000, 2000, 0x4000);
    u32 index3 = caScheduler::AddJob("PROVA3", caJobPriority::caThLevel0,
            dummy, 1000, 2000, 0x4000);
    CA_ASSERT(index1 == (0));
    CA_ASSERT(index2 == (1));
    CA_ASSERT(index3 == (2));
    u32 i;
    for (i = 0; i < 1000; i++) {
        caScheduler::GetNextContext();
        caScheduler::GetCurrentContext();
    }
    caThreadContext *ctx1 = caScheduler::GetCurrentContext();
    std::vector<u32> SW;
    u32 nw = ctx1->nswitch;
    SW.push_back(nw);
    caThreadContext *ctx2 = ctx1;
    while (ctx1 == ctx2) {
        caScheduler::GetNextContext();
        ctx2 = caScheduler::GetCurrentContext();
    }
    nw = ctx2->nswitch;
    SW.push_back(nw);
    caThreadContext *ctx3 = ctx2;
    while (ctx3 == ctx2 || ctx3 == ctx1) {
        caScheduler::GetNextContext();
        ctx3 = caScheduler::GetCurrentContext();
    }
    nw = ctx3->nswitch;
    SW.push_back(nw);
    std::cout << (const char *) ctx1->name
            << " : N =" << ctx1->nswitch << std::endl;
    std::cout << (const char *) ctx2->name
            << " : N =" << ctx2->nswitch << std::endl;
    std::cout << (const char *) ctx3->name
            << " : N =" << ctx3->nswitch << std::endl;
    std::sort<std::vector<u32>::iterator>(SW.begin(), SW.end());
    //1000=x+2x+4x->1000/7->about 143 286 572
    CA_ASSERT(SW.at(2) > 560 && SW.at(2) < 580);
    CA_ASSERT(SW.at(1) > 270 && SW.at(1) < 290);
    CA_ASSERT(SW.at(0) > 130 && SW.at(0) < 150);
    CA_ASSERT(caScheduler::Destroy() == true);

}

void test_caThread_class::test8(void) {
    _START();
    _INFO("to check CreateThread function of class caThread with 7 thread");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();
    caScheduler::Init(caSchedulerMode::Priority);
    u32 index1 = caScheduler::AddJob("PROVA1", caJobPriority::caThLevel0,
            dummy, 1000, 2000, 0x4000);
    u32 index2 = caScheduler::AddJob("PROVA2", caJobPriority::caThLevel1,
            dummy, 1000, 2000, 0x4000);
    u32 index3 = caScheduler::AddJob("PROVA3", caJobPriority::caThLevel2,
            dummy, 1000, 2000, 0x4000);
    u32 index4 = caScheduler::AddJob("PROVA4", caJobPriority::caThLevel3,
            dummy, 1000, 2000, 0x4000);
    u32 index5 = caScheduler::AddJob("PROVA5", caJobPriority::caThLevel4,
            dummy, 1000, 2000, 0x4000);
    u32 index6 = caScheduler::AddJob("PROVA6", caJobPriority::caThLevel5,
            dummy, 1000, 2000, 0x4000);
    u32 index7 = caScheduler::AddJob("PROVA7", caJobPriority::caThLevel6,
            dummy, 1000, 2000, 0x4000);
    CA_ASSERT(index1 == (0));
    CA_ASSERT(index2 == (1));
    CA_ASSERT(index3 == (2));
    CA_ASSERT(index4 == (3));
    CA_ASSERT(index5 == (4));
    CA_ASSERT(index6 == (5));
    CA_ASSERT(index7 == (6));
    u32 i;
    for (i = 0; i < 1000; i++) {
        caScheduler::GetNextContext();
        caScheduler::GetCurrentContext();
    }
    caThreadContext * ctxs[7];
    std::vector<u32> SW;
    ctxs[0] = caScheduler::GetCurrentContext();
    u32 nw = ctxs[0]->nswitch;
    SW.push_back(nw);
    u32 u;
    i = 1;
    while (i < 7) {
        caScheduler::GetNextContext();
        caThreadContext *tmp = caScheduler::GetCurrentContext();
        for (u = 0; u < i; u++) {
            if (ctxs[u] == tmp)break;
        }
        if (u == i) {
            ctxs[i] = tmp;
            nw = tmp->nswitch;
            SW.push_back(nw);
            i++;
        }
    }

    std::sort<std::vector<u32>::iterator>(SW.begin(), SW.end());
    //1000=x+2x+4x+8x+16x+32x+64c->1000/127->about 8-6-32-64-128-256-512 
    CA_ASSERT(SW.at(0) > 6 && SW.at(0) < 10);
    CA_ASSERT(SW.at(1) > 14 && SW.at(1) < 18);
    CA_ASSERT(SW.at(2) > 28 && SW.at(2) < 36);
    CA_ASSERT(SW.at(3) > 60 && SW.at(3) < 70);
    CA_ASSERT(SW.at(4) > 120 && SW.at(4) < 140);
    CA_ASSERT(SW.at(5) > 240 && SW.at(5) < 270);
    CA_ASSERT(SW.at(6) > 480 && SW.at(6) < 520);
    CA_ASSERT(caScheduler::Destroy() == true);
}

static bool caThreadContext_less(caThreadContext *a, caThreadContext*b) {
    return a->nswitch < b->nswitch;
}

void test_caThread_class::test9(void) {
    _START();
    _INFO("to check CreateThread function of class caThread with 7 thread");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();
    caScheduler::Init(caSchedulerMode::Priority);
    u32 index1 = caScheduler::AddJob("PROVA1", caJobPriority::caThLevel0,
            dummy, 1000, 2000, 0x4000);
    u32 index2 = caScheduler::AddJob("PROVA2", caJobPriority::caThLevel1,
            dummy, 1000, 2000, 0x4000);
    u32 index3 = caScheduler::AddJob("PROVA3", caJobPriority::caThLevel2,
            dummy, 1000, 2000, 0x4000);
    u32 index4 = caScheduler::AddJob("PROVA4", caJobPriority::caThLevel3,
            dummy, 1000, 2000, 0x4000);
    u32 index5 = caScheduler::AddJob("PROVA5", caJobPriority::caThLevel4,
            dummy, 1000, 2000, 0x4000);
    u32 index6 = caScheduler::AddJob("PROVA6", caJobPriority::caThLevel5,
            dummy, 1000, 2000, 0x4000);
    u32 index7 = caScheduler::AddJob("PROVA7", caJobPriority::caThLevel6,
            dummy, 1000, 2000, 0x4000);
    CA_ASSERT(index1 == (0));
    CA_ASSERT(index2 == (1));
    CA_ASSERT(index3 == (2));
    CA_ASSERT(index4 == (3));
    CA_ASSERT(index5 == (4));
    CA_ASSERT(index6 == (5));
    CA_ASSERT(index7 == (6));
    std::vector<caThreadContext *> ctxs;
    ctxs.insert(ctxs.begin(), 7, nullptr);
    u32 i;
    for (i = 0; i < 1000; i++) {
        caScheduler::GetNextContext();
        caThreadContext *tmp = caScheduler::GetCurrentContext();
        ctxs[tmp->index] = tmp;
    }

    std::sort<std::vector<caThreadContext *>::iterator >(ctxs.begin(), ctxs.end(), caThreadContext_less);

    //1000=x+2x+4x+8x+16x+32x+64c->1000/127->about 8-6-32-64-128-256-512 
    CA_ASSERT(ctxs.at(0)->nswitch > 6 && ctxs.at(0)->nswitch < 10);
    CA_ASSERT(ctxs.at(0)->index == 0);
    CA_ASSERT(ctxs.at(1)->nswitch > 14 && ctxs.at(1)->nswitch < 18);
    CA_ASSERT(ctxs.at(1)->index == 1);
    CA_ASSERT(ctxs.at(2)->nswitch > 28 && ctxs.at(2)->nswitch < 36);
    CA_ASSERT(ctxs.at(2)->index == 2);
    CA_ASSERT(ctxs.at(3)->nswitch > 60 && ctxs.at(3)->nswitch < 70);
    CA_ASSERT(ctxs.at(3)->index == 3);
    CA_ASSERT(ctxs.at(4)->nswitch > 120 && ctxs.at(4)->nswitch < 140);
    CA_ASSERT(ctxs.at(4)->index == 4);
    CA_ASSERT(ctxs.at(5)->nswitch > 240 && ctxs.at(5)->nswitch < 270);
    CA_ASSERT(ctxs.at(5)->index == 5);
    CA_ASSERT(ctxs.at(6)->nswitch > 480 && ctxs.at(6)->nswitch < 520);
    CA_ASSERT(ctxs.at(6)->index == 6);
    CA_ASSERT(caScheduler::ChangePriority(0, caJobPriority::caThLevel6) == deviceError::no_error);
    for (i = 0; i < 1000; i++) {
        caScheduler::GetNextContext();
        caScheduler::GetCurrentContext();
    }
    //+1000=2x+4x+8x+16x+32x+64x+64x->1000/190 ->10-21-42-84-168-336+336
    //672-480-6 -> recover thread 0 500 tick
    std::sort<std::vector<caThreadContext *>::iterator >(ctxs.begin(), ctxs.end(), caThreadContext_less);
    CA_ASSERT(ctxs.at(0)->nswitch > (14 + 10) && ctxs.at(0)->nswitch < (18 + 10));
    CA_ASSERT(ctxs.at(0)->index == 1);
    CA_ASSERT(ctxs.at(1)->nswitch > (28 + 20) && ctxs.at(1)->nswitch < (36 + 20));
    CA_ASSERT(ctxs.at(1)->index == 2);
    CA_ASSERT(ctxs.at(2)->nswitch > (60 + 35) && ctxs.at(2)->nswitch < (70 + 35));
    CA_ASSERT(ctxs.at(2)->index == 3);
    CA_ASSERT(ctxs.at(3)->nswitch > (120 + 70) && ctxs.at(3)->nswitch < (140 + 70));
    CA_ASSERT(ctxs.at(3)->index == 4);
    CA_ASSERT(ctxs.at(4)->nswitch > (240 + 110) && ctxs.at(4)->nswitch < (270 + 110));
    CA_ASSERT(ctxs.at(4)->index == 5);
    CA_ASSERT(ctxs.at(5)->nswitch > (6 + 550) && ctxs.at(5)->nswitch < (10 + 580));
    CA_ASSERT(ctxs.at(5)->index == 0);
    CA_ASSERT(ctxs.at(6)->nswitch > (480 + 180) && ctxs.at(6)->nswitch < (520 + 220));
    CA_ASSERT(ctxs.at(6)->index == 6);
    s8 buff[4096];
    caStringStream<s8> ss;
    ss.Init(buff, sizeof (buff));
    caScheduler::Dump(ss);
    std::cout << buff;
    CA_ASSERT(caScheduler::Destroy() == true);
}