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
#include "haljobdevice.h"
#include "memory.h"
#include <vector>
#include <algorithm>

/* SAME test_caThread but use hwardware abstract layer device */



typedef struct param {
    const char *name;
    caJobPriority p;
    thFunc func;
    u32 par1;
    u32 par2;
    u32 stack;
} jobParam;

extern void start_time_scheduler(void);

class caHalJobDevice_test_class
: public caTester {
    CA_TEST_SUITE(caHalJobDevice_test_class);
    CA_TEST(caHalJobDevice_test_class::test1, "AddJob 1");
    CA_TEST(caHalJobDevice_test_class::test2, "AddJob 2");
    CA_TEST_SUITE_END();

    void setUp(void) {
        caMemory::Init();
        start_time_scheduler();
    }

    void tearDown(void) {
    }

    void test1(void);
    void test2(void);

};

REGISTER_CLASS(caHalJobDevice_test_class);

static u32 dummy(u32 idx, u32 p1, u32 p2) {
    std::cout << "IDEX  =" << idx << std::endl;
    std::cout << "PARM 1=" << p1 << std::endl;
    std::cout << "PARM 2=" << p2 << std::endl;
    return 0;
}

void caHalJobDevice_test_class::test1(void) {
    _START();
    _INFO("to check haljobdevice function function of class caThread");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();
    caScheduler::Init(caSchedulerMode::RoundRobin);
    caHalJobDevice jobber(&hal_llc_scheduler, ioCtrlRequest::Task);
    caJobConfigure conf;
    caDeviceHandle port;
    u32 res1 = caHalDeviceRules::Open(&jobber, &conf,
            &port, ioCtrlRequest::Task);
    CA_ASSERT(res1 == deviceError::no_error);
    CA_ASSERT(caHalDeviceRules::IsValidHandle(
            port.handle, ioCtrlRequest::Task) == true);
    caJobDeviceCtrl ctrl;
    ctrl.command = caJobDeviceCtrl::IoJobCtrlDirect::jobAddUserJob;
    jobParam input{"PROVA", caJobPriority::caThLevel6,
        dummy, 1000, 2000, 0x4000};
    caMemAux<u32>::MemCpy(ctrl.params, (u32*) (&input), sizeof (jobParam));
    res1 = caHalDeviceRules::IoCtrl(&jobber, &port, &ctrl, ioCtrlRequest::Task);
    CA_ASSERT(res1 == deviceError::no_error);
    ctrl.command = caJobDeviceCtrl::IoJobCtrlDirect::jobGetSize;
    res1 = caHalDeviceRules::IoCtrl(&jobber, &port, &ctrl, ioCtrlRequest::Task);
    CA_ASSERT(ctrl.params[0] == (1));
    CA_ASSERT(res1 == deviceError::no_error);
    caScheduler::GetNextContext();
    caThreadContext ctx;
    port.rdBuff = (u8*) & ctx;
    port.rdSize = sizeof (caThreadContext);
    res1 = caHalDeviceRules::Read(&jobber, &port, ioCtrlRequest::Task);
    CA_ASSERT(res1 == deviceError::no_error);
    CA_ASSERT(port.rdSize == 0);
    CA_ASSERT(port.readed == sizeof (caThreadContext));
    u32 res = memcmp(ctx.name, "PROVA", 5);
    CA_ASSERT(res == 0);
    CA_ASSERT(ctx.cur_prio == caJobPriority::caThLevel6);
    CA_ASSERT(ctx.pcb[3] == 1000);
    CA_ASSERT(ctx.pcb[4] == 2000);
    CA_ASSERT((ctx.stack_start - ctx.stack_end) > 0x4000);
    CA_ASSERT(ctx.status == caJobStatus::thInit);
    ctrl.command = caJobDeviceCtrl::IoJobCtrlDirect::jobDestroy;
    ctrl.params[0] = ctx.index;
    res1 = caHalDeviceRules::IoCtrl(&jobber, &port, &ctrl, ioCtrlRequest::Task);
    CA_ASSERT(res1 == deviceError::no_error);
}

void caHalJobDevice_test_class::test2(void) {
    _START();
    _INFO("to check haljobdevice function function of class caThread");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();
    caScheduler::Init(caSchedulerMode::Priority);
    caHalJobDevice jobber(&hal_llc_scheduler, ioCtrlRequest::Task);
    caJobConfigure conf;
    caDeviceHandle port;
    u32 res1 = caHalDeviceRules::Open(&jobber, &conf,
            &port, ioCtrlRequest::Task);
    CA_ASSERT(res1 == deviceError::no_error);
    CA_ASSERT(caHalDeviceRules::IsValidHandle(
            port.handle, ioCtrlRequest::Task) == true);
    caJobDeviceCtrl ctrl;
    ctrl.command = caJobDeviceCtrl::IoJobCtrlDirect::jobAddUserJob;
    jobParam input{"PROVA1", caJobPriority::caThLevel6,
        dummy, 1000, 2000, 0x4000};
    caMemAux<u32>::MemCpy(ctrl.params, (u32*) (&input), sizeof (jobParam));
    res1 = caHalDeviceRules::IoCtrl(&jobber, &port, &ctrl, ioCtrlRequest::Task);
    CA_ASSERT(res1 == deviceError::no_error);
    ctrl.command = caJobDeviceCtrl::IoJobCtrlDirect::jobGetSize;
    res1 = caHalDeviceRules::IoCtrl(&jobber, &port, &ctrl, ioCtrlRequest::Task);
    CA_ASSERT(ctrl.params[0] == (1));
    CA_ASSERT(res1 == deviceError::no_error);
    jobParam input1{"PROVA2", caJobPriority::caThLevel2,
        dummy, 1000, 2000, 0x4000};
    ctrl.command = caJobDeviceCtrl::IoJobCtrlDirect::jobAddUserJob;
    caMemAux<u32>::MemCpy(ctrl.params, (u32*) (&input1), sizeof (jobParam));
    res1 = caHalDeviceRules::IoCtrl(&jobber, &port, &ctrl, ioCtrlRequest::Task);
    CA_ASSERT(res1 == deviceError::no_error);
    ctrl.command = caJobDeviceCtrl::IoJobCtrlDirect::jobGetSize;
    res1 = caHalDeviceRules::IoCtrl(&jobber, &port, &ctrl, ioCtrlRequest::Task);
    CA_ASSERT(ctrl.params[0] == (2));
    CA_ASSERT(res1 == deviceError::no_error);
    for (res1 = 0; res1 < 1000; res1++)
        caScheduler::GetNextContext();
    s8 buff[1024];
    caStringStream<s8> ss;
    ss.Init(buff, sizeof (buff));
    ctrl.command = caJobDeviceCtrl::IoJobCtrlDirect::jobList;
    ctrl.ss = &ss;
    res1 = caHalDeviceRules::IoCtrl(&jobber, &port, &ctrl, ioCtrlRequest::Task);
    CA_ASSERT(res1 == deviceError::no_error);
    std::cout << ss.Str() << std::endl;
    ctrl.command = caJobDeviceCtrl::IoJobCtrlDirect::jobDestroyAll;
    res1 = caHalDeviceRules::IoCtrl(&jobber, &port, &ctrl, ioCtrlRequest::Task);
    CA_ASSERT(res1 == deviceError::no_error);
}
