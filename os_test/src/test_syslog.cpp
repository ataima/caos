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
#include "systimer.h"
#include "syslog.h"
#include "memory.h"
#include <iostream>

static struct timespec start_time = {0, 0};


static u32 hll_time(void)
{
    u32 us;
    struct timespec spec, diff;
    clock_gettime(CLOCK_REALTIME, &spec);
    diff.tv_sec = (time_t) difftime(spec.tv_sec, start_time.tv_sec);
    if (spec.tv_nsec > start_time.tv_nsec)
        diff.tv_nsec = spec.tv_nsec - start_time.tv_nsec;
    else
    {
        spec.tv_sec--;
        diff.tv_nsec = spec.tv_nsec - start_time.tv_nsec + 1000000000;
    }
    us = (u32) (diff.tv_sec * 1000000 + diff.tv_nsec / 1000);
    if (us == 0)
        us = 1;
    return us;
}

static u32 hll_dummy(void)
{    
    return 7;
}

hal_ll_sys_time hal_ll_time{
    hll_time, //system tick count
    hll_dummy,
    hll_dummy,
    hll_dummy,
    hll_dummy,
    hll_dummy 
};

class caSysLog_test_class
: public caTester
{
    CA_TEST_SUITE(caSysLog_test_class);
    CA_TEST(caSysLog_test_class::test1, "Init test");
    CA_TEST(caSysLog_test_class::test2, "stream test");
    CA_TEST_SUITE_END();

    void setUp(void)
    {
        caMemory::Init();
    }
    void test1(void);
    void test2(void);

    void tearDown(void)
    {
    }

};

REGISTER_CLASS(caSysLog_test_class);

void caSysLog_test_class::test1(void)
{
    _START();
    _INFO("to check Init/ destroy function of caSysLog");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();
    caSysLog a;
    u32 res = a.Init(1000,end_log_lev);
    CA_ASSERT(res == TRUE);
    CA_ASSERT(a.GetBase(panic) != NULL);
    CA_ASSERT(a.GetBase(kernel) != NULL);
    CA_ASSERT(a.GetBase(irq) != NULL);
    CA_ASSERT(a.GetBase(device) != NULL);
    CA_ASSERT(a.GetBase(error) != NULL);
    CA_ASSERT(a.GetBase(info) != NULL);
    CA_ASSERT(caMemory::Find(a.GetBase(panic)) > 1000);
    CA_ASSERT(caMemory::Find(a.GetBase(kernel)) > 1000);
    CA_ASSERT(caMemory::Find(a.GetBase(irq)) > 1000);
    CA_ASSERT(caMemory::Find(a.GetBase(device)) > 1000);
    CA_ASSERT(caMemory::Find(a.GetBase(error)) > 1000);
    CA_ASSERT(caMemory::Find(a.GetBase(info)) > 1000);
    res = a.Destroy();
    CA_ASSERT(res == TRUE);
    CA_ASSERT(caMemory::Find(a.GetBase(panic)) == 0);
    CA_ASSERT(caMemory::Find(a.GetBase(kernel)) == 0);
    CA_ASSERT(caMemory::Find(a.GetBase(irq)) == 0);
    CA_ASSERT(caMemory::Find(a.GetBase(device)) == 0);
    CA_ASSERT(caMemory::Find(a.GetBase(error)) == 0);
    CA_ASSERT(caMemory::Find(a.GetBase(info)) == 0);
}

void caSysLog_test_class::test2(void)
{
    _START();
    _INFO("to check stream functions of caSysLog");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();
    caSysLog a;
    u32 res = a.Init(1000,end_log_lev);
    CA_ASSERT(res == TRUE);
    CA_ASSERT(a.GetBase(panic) != NULL);
    CA_ASSERT(a.GetBase(kernel) != NULL);
    CA_ASSERT(a.GetBase(irq) != NULL);
    CA_ASSERT(a.GetBase(device) != NULL);
    CA_ASSERT(a.GetBase(error) != NULL);
    CA_ASSERT(a.GetBase(info) != NULL);
    CA_ASSERT(caMemory::Find(a.GetBase(panic)) > 1000);
    CA_ASSERT(caMemory::Find(a.GetBase(kernel)) > 1000);
    CA_ASSERT(caMemory::Find(a.GetBase(irq)) > 1000);
    CA_ASSERT(caMemory::Find(a.GetBase(device)) > 1000);
    CA_ASSERT(caMemory::Find(a.GetBase(error)) > 1000);
    CA_ASSERT(caMemory::Find(a.GetBase(info)) > 1000);
    a.Enable();
    LOG(a, panic) << "a panic error" << caEnd::endl;
    LOG(a, kernel) << "a kernel error" << caEnd::endl;
    LOG(a, irq) << "a irq error" << caEnd::endl;
    LOG(a, device) << "a drivers error" << caEnd::endl;
    LOG(a, error) << "a error error" << caEnd::endl;
    LOG(a, info) << "a info error" << caEnd::endl;
    std::cout<<a.GetBase(panic);
    std::cout<<a.GetBase(kernel);
    std::cout<<a.GetBase(irq);
    std::cout<<a.GetBase(device);
    std::cout<<a.GetBase(error);
    std::cout<<a.GetBase(info);
    CA_ASSERT(strlen(a.GetBase(panic))>0);
    CA_ASSERT(strlen(a.GetBase(kernel))>0);
    CA_ASSERT(strlen(a.GetBase(irq))>0);
    CA_ASSERT(strlen(a.GetBase(device))>0);
    CA_ASSERT(strlen(a.GetBase(error))>0);
    CA_ASSERT(strlen(a.GetBase(info))>0);
    res = a.Destroy();
    CA_ASSERT(res == TRUE);
    CA_ASSERT(caMemory::Find(a.GetBase(panic)) == 0);
    CA_ASSERT(caMemory::Find(a.GetBase(kernel)) == 0);
    CA_ASSERT(caMemory::Find(a.GetBase(irq)) == 0);
    CA_ASSERT(caMemory::Find(a.GetBase(device)) == 0);
    CA_ASSERT(caMemory::Find(a.GetBase(error)) == 0);
    CA_ASSERT(caMemory::Find(a.GetBase(info)) == 0);

}