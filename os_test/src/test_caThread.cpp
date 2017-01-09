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
#include "thread.h"
#include "scheduler.h"
#include "memory.h"


static u32 th_enable = 0;
static u32 th_disable = 0;
static u32 th_wait = 0;

static void hll_enable(void)
{
    th_enable = 1;
}

static void hll_disable(void)
{
    th_disable = 1;
}

static void hll_wait(void)
{
    th_wait = 1;
}



hal_llc_interrupt hal_llc_int_req{
    hll_enable,
    hll_disable,
    hll_wait
};

class test_caThread_class
: public caTester
{
    CA_TEST_SUITE(test_caThread_class);
    CA_TEST(test_caThread_class::test1, "");
    CA_TEST_SUITE_END();

    void setUp(void)
    {
        caMemory::Init();
    }

    void tearDown(void)
    {
    }

    void test1(void);

};

REGISTER_CLASS(test_caThread_class);

static u32 dummy(u32 idx, u32 p1, u32 p2)
{
    std::cout << "IDEX  =" << idx << std::endl;
    std::cout << "PARM 1=" << p1 << std::endl;
    std::cout << "PARM 2=" << p2 << std::endl;
    return 0;
}

void test_caThread_class::test1(void)
{
    _START();
    _INFO("to check CreateThread function of class caThread");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();
    caScheduler::Init(caSchedulerMode::RoundRobin);
    u32 index = caThread::CreateThread("PROVA", caThreadPriority::caThLevel6,
                                       dummy, 1000, 2000, 0x4000);
    CA_ASSERT(index == (BASE_HANDLE + 1));
    caScheduler::GetNextContext();
    caThreadContext *ctx = caScheduler::GetCurrentContext();
    CA_ASSERT(strcmp(ctx->name, "PROVA") == 0);
    caScheduler::Destroy();
}