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
#include "halmemdevice.h"
#include "memory.h"
#include <vector>
#include <algorithm>

/* SAME test_caThread but use hwardware abstract layer device */


extern void start_time_scheduler();

class caHalMemDevice_test_class
: public caTester {
    CA_TEST_SUITE(caHalMemDevice_test_class);
    CA_TEST(caHalMemDevice_test_class::test1, "allocate - free");
    CA_TEST(caHalMemDevice_test_class::test2, "list");
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

REGISTER_CLASS(caHalMemDevice_test_class);

extern hal_llc_mem_io hal_llc_mem;

void caHalMemDevice_test_class::test1(void) {
    _START();
    _INFO("to check halmemdevice function ");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();
    caHalMemDevice mem(&hal_llc_mem, ioCtrlRequest::Memory);
    caDeviceHandle port;
    caMemConfigure conf;
    u32 res1 = caHalDeviceRules::Open(&mem, &conf,
            &port, ioCtrlRequest::Memory);
    CA_ASSERT(res1 == deviceError::no_error);
    CA_ASSERT(caHalDeviceRules::IsValidHandle(
            port.handle, ioCtrlRequest::Memory) == true);
    port.wrSize = 50000;
    port.wrBuff = (u8*) & port;
    port.writed = 0;
    res1 = caHalDeviceRules::Write(&mem,&port, ioCtrlRequest::Memory);
    CA_ASSERT(res1 == deviceError::no_error);
    CA_ASSERT(port.wrBuff != (u8*)&port);
    CA_ASSERT(port.wrBuff != NULL);
    port.rdBuff = port.wrBuff;
    port.rdSize = 0;
    res1 = caHalDeviceRules::Read(&mem, &port, ioCtrlRequest::Memory);
    CA_ASSERT(res1 == deviceError::no_error);
    CA_ASSERT(port.rdBuff == NULL);
    CA_ASSERT(port.rdSize > port.wrSize);
}

void caHalMemDevice_test_class::test2(void) {
    _START();
    _INFO("to check halmemdevice function function of class caThread");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();
    caHalMemDevice mem(&hal_llc_mem, ioCtrlRequest::Memory);
    caDeviceHandle port;
    caMemConfigure conf;
    u32 res1 = caHalDeviceRules::Open(&mem, &conf,
            &port, ioCtrlRequest::Memory);
    CA_ASSERT(res1 == deviceError::no_error);
    CA_ASSERT(caHalDeviceRules::IsValidHandle(
            port.handle, ioCtrlRequest::Memory) == true);
    port.wrSize = 50000;
    port.wrBuff = (u8*)&port;
    port.writed = 0;
    res1 = caHalDeviceRules::Write(&mem, &port, ioCtrlRequest::Memory);
    CA_ASSERT(res1 == deviceError::no_error);
    CA_ASSERT(port.wrBuff != (u8*)&port);
    CA_ASSERT(port.wrBuff != NULL);
    caMemoryDeviceCtrl ctrl;
    ctrl.command = caMemoryDeviceCtrl::IoMemCtrlDirect::memList;
    s8 buff[4096];
    caStringStream<s8> ss;
    ss.Init(buff, sizeof (buff));
    ctrl.ss = &ss;
    res1 = caHalDeviceRules::IoCtrl(&mem, &port, &ctrl, ioCtrlRequest::Memory);
    CA_ASSERT(res1 == deviceError::no_error);
    CA_ASSERT(ss.Size() > 0);
    std::cout<<ss.Str();
}
