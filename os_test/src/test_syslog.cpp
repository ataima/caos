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
#include "syslog.h"
#include "memory.h"
#include <iostream>

static struct timespec start_time = {0, 0};

static u32 hll_time(void) {
    u32 us;
    struct timespec spec, diff;
    clock_gettime(CLOCK_REALTIME, &spec);
    diff.tv_sec = (time_t) difftime(spec.tv_sec, start_time.tv_sec);
    if (spec.tv_nsec > start_time.tv_nsec)
        diff.tv_nsec = spec.tv_nsec - start_time.tv_nsec;
    else {
        spec.tv_sec--;
        diff.tv_nsec = spec.tv_nsec - start_time.tv_nsec + 1000000000;
    }
    us = (u32) (diff.tv_sec * 1000000 + diff.tv_nsec / 1000);
    if (us == 0)
        us = 1;
    return us;
}

static u32 hll_dummy(void) {
    return 7;
}

static u32 hll_conf(u32, u32, u32, u32) {
    return 1;
}

hal_llc_sys_time hal_llc_time{
    nullptr,
    hll_conf,
    hll_time, //system tick count
    hll_dummy,
    hll_dummy,
    hll_dummy,
    hll_dummy,
    hll_dummy,
    hll_dummy,
    hll_dummy,
    hll_dummy,
    nullptr, nullptr, nullptr, nullptr, nullptr,
    nullptr, nullptr, nullptr, nullptr};

class caSysLog_test_class
: public caTester {
    CA_TEST_SUITE(caSysLog_test_class);
    CA_TEST(caSysLog_test_class::test1, "Init test");
    CA_TEST(caSysLog_test_class::test2, "stream test");
    CA_TEST_SUITE_END();

    void setUp(void) {
        caMemory::Init(&hal_llc_mem);
    }
    void test1(void);
    void test2(void);

    void tearDown(void) {
    }

};

REGISTER_CLASS(caSysLog_test_class);

void caSysLog_test_class::test1(void) {
    _START();
    _INFO("to check Init/ destroy function of caSysLog");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();
    caSysLog a;
    u32 res = a.Init(1000, deviceloglevels::end_device_log_lev);
    CA_ASSERT(res == true);
    CA_ASSERT(a.GetBase(irq_1) != nullptr);
    CA_ASSERT(a.GetBase(irq_2) != nullptr);
    CA_ASSERT(a.GetBase(irq_3) != nullptr);
    CA_ASSERT(a.GetBase(irq_4) != nullptr);
    CA_ASSERT(a.GetBase(irq_5) != nullptr);
    CA_ASSERT(a.GetBase(irq_6) != nullptr);
    CA_ASSERT(a.GetBase(irq_7) != nullptr);
    CA_ASSERT(a.GetBase(irq_8) != nullptr);
    CA_ASSERT(a.GetBase(device) != nullptr);
    CA_ASSERT(a.GetBase(error) != nullptr);
    CA_ASSERT(a.GetBase(info) != nullptr);
    CA_ASSERT(caMemory::Find(a.GetBase(irq_1)) > 1000);
    CA_ASSERT(caMemory::Find(a.GetBase(irq_2)) > 1000);
    CA_ASSERT(caMemory::Find(a.GetBase(irq_3)) > 1000);
    CA_ASSERT(caMemory::Find(a.GetBase(irq_4)) > 1000);
    CA_ASSERT(caMemory::Find(a.GetBase(irq_5)) > 1000);
    CA_ASSERT(caMemory::Find(a.GetBase(irq_6)) > 1000);
    CA_ASSERT(caMemory::Find(a.GetBase(irq_7)) > 1000);
    CA_ASSERT(caMemory::Find(a.GetBase(irq_8)) > 1000);
    CA_ASSERT(caMemory::Find(a.GetBase(device)) > 1000);
    CA_ASSERT(caMemory::Find(a.GetBase(error)) > 1000);
    CA_ASSERT(caMemory::Find(a.GetBase(info)) > 1000);
    res = a.Destroy();
    CA_ASSERT(res == true);
    CA_ASSERT(caMemory::Find(a.GetBase(irq_1)) == 0);
    CA_ASSERT(caMemory::Find(a.GetBase(irq_2)) == 0);
    CA_ASSERT(caMemory::Find(a.GetBase(irq_3)) == 0);
    CA_ASSERT(caMemory::Find(a.GetBase(irq_4)) == 0);
    CA_ASSERT(caMemory::Find(a.GetBase(irq_5)) == 0);
    CA_ASSERT(caMemory::Find(a.GetBase(irq_6)) == 0);
    CA_ASSERT(caMemory::Find(a.GetBase(irq_7)) == 0);
    CA_ASSERT(caMemory::Find(a.GetBase(irq_8)) == 0);
    CA_ASSERT(caMemory::Find(a.GetBase(device)) == 0);
    CA_ASSERT(caMemory::Find(a.GetBase(error)) == 0);
    CA_ASSERT(caMemory::Find(a.GetBase(info)) == 0);
}

void caSysLog_test_class::test2(void) {
    _START();
    _INFO("to check stream functions of caSysLog");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();
    caSysLog a;
    u32 res = a.Init(1000, deviceloglevels::end_device_log_lev);
    CA_ASSERT(res == true);
    CA_ASSERT(a.GetBase(irq_1) != nullptr);
    CA_ASSERT(a.GetBase(irq_2) != nullptr);
    CA_ASSERT(a.GetBase(irq_3) != nullptr);
    CA_ASSERT(a.GetBase(irq_4) != nullptr);
    CA_ASSERT(a.GetBase(irq_5) != nullptr);
    CA_ASSERT(a.GetBase(irq_6) != nullptr);
    CA_ASSERT(a.GetBase(irq_7) != nullptr);
    CA_ASSERT(a.GetBase(irq_8) != nullptr);
    CA_ASSERT(a.GetBase(device) != nullptr);
    CA_ASSERT(a.GetBase(error) != nullptr);
    CA_ASSERT(a.GetBase(info) != nullptr);
    CA_ASSERT(caMemory::Find(a.GetBase(irq_1)) > 1000);
    CA_ASSERT(caMemory::Find(a.GetBase(irq_2)) > 1000);
    CA_ASSERT(caMemory::Find(a.GetBase(irq_3)) > 1000);
    CA_ASSERT(caMemory::Find(a.GetBase(irq_4)) > 1000);
    CA_ASSERT(caMemory::Find(a.GetBase(irq_5)) > 1000);
    CA_ASSERT(caMemory::Find(a.GetBase(irq_6)) > 1000);
    CA_ASSERT(caMemory::Find(a.GetBase(irq_7)) > 1000);
    CA_ASSERT(caMemory::Find(a.GetBase(irq_8)) > 1000);
    CA_ASSERT(caMemory::Find(a.GetBase(device)) > 1000);
    CA_ASSERT(caMemory::Find(a.GetBase(error)) > 1000);
    CA_ASSERT(caMemory::Find(a.GetBase(info)) > 1000);
    a.Enable();
    LOG(a, irq_1) << "a irq error" << caEnd::endl;
    LOG(a, irq_2) << "a irq error" << caEnd::endl;
    LOG(a, irq_3) << "a irq error" << caEnd::endl;
    LOG(a, irq_4) << "a irq error" << caEnd::endl;
    LOG(a, irq_5) << "a irq error" << caEnd::endl;
    LOG(a, irq_6) << "a irq error" << caEnd::endl;
    LOG(a, irq_7) << "a irq error" << caEnd::endl;
    LOG(a, irq_8) << "a irq error" << caEnd::endl;
    LOG(a, device) << "a drivers error" << caEnd::endl;
    LOG(a, error) << "a error error" << caEnd::endl;
    LOG(a, info) << "a info error" << caEnd::endl;
    std::cout << a.GetBase(irq_1);
    std::cout << a.GetBase(irq_2);
    std::cout << a.GetBase(irq_3);
    std::cout << a.GetBase(irq_4);
    std::cout << a.GetBase(irq_5);
    std::cout << a.GetBase(irq_6);
    std::cout << a.GetBase(irq_7);
    std::cout << a.GetBase(irq_8);
    std::cout << a.GetBase(device);
    std::cout << a.GetBase(error);
    std::cout << a.GetBase(info);
    CA_ASSERT(strlen(a.GetBase(irq_1)) > 0);
    CA_ASSERT(strlen(a.GetBase(irq_2)) > 0);
    CA_ASSERT(strlen(a.GetBase(irq_3)) > 0);
    CA_ASSERT(strlen(a.GetBase(irq_4)) > 0);
    CA_ASSERT(strlen(a.GetBase(irq_5)) > 0);
    CA_ASSERT(strlen(a.GetBase(irq_6)) > 0);
    CA_ASSERT(strlen(a.GetBase(irq_7)) > 0);
    CA_ASSERT(strlen(a.GetBase(irq_8)) > 0);
    CA_ASSERT(strlen(a.GetBase(device)) > 0);
    CA_ASSERT(strlen(a.GetBase(error)) > 0);
    CA_ASSERT(strlen(a.GetBase(info)) > 0);
    res = a.Destroy();
    CA_ASSERT(res == true);
    CA_ASSERT(caMemory::Find(a.GetBase(irq_1)) == 0);
    CA_ASSERT(caMemory::Find(a.GetBase(irq_2)) == 0);
    CA_ASSERT(caMemory::Find(a.GetBase(irq_3)) == 0);
    CA_ASSERT(caMemory::Find(a.GetBase(irq_4)) == 0);
    CA_ASSERT(caMemory::Find(a.GetBase(irq_5)) == 0);
    CA_ASSERT(caMemory::Find(a.GetBase(irq_6)) == 0);
    CA_ASSERT(caMemory::Find(a.GetBase(irq_7)) == 0);
    CA_ASSERT(caMemory::Find(a.GetBase(irq_8)) == 0);
    CA_ASSERT(caMemory::Find(a.GetBase(device)) == 0);
    CA_ASSERT(caMemory::Find(a.GetBase(error)) == 0);
    CA_ASSERT(caMemory::Find(a.GetBase(info)) == 0);

}