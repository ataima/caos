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
#include "halpipedevice.h"
#include "memory.h"
#include <vector>
#include <algorithm>
#include "caos.h"

/* SAME test_caThread but use hwardware abstract layer device */


extern void start_time_scheduler();

class caHalPipeDevice_test_class
: public caTester {
    CA_TEST_SUITE(caHalPipeDevice_test_class);
    CA_TEST(caHalPipeDevice_test_class::test1, "allocate - free");
    CA_TEST(caHalPipeDevice_test_class::test2, "list");
    CA_TEST_SUITE_END();

    void setUp(void) {
        caMemory::Init(&hal_llc_mem);
        start_time_scheduler();
    }

    void tearDown(void) {
    }

    void test1(void);
    void test2(void);

};

REGISTER_CLASS(caHalPipeDevice_test_class);

extern hal_llc_mem_io hal_llc_mem;

static u32 sourceTask(void) {
    caPipeDeviceConfigure in1, in2;
    u32 loop1;
    caDeviceHandle port1, port2;
    caMemAux<u32>::MemCpy((u32*) in1.name, (u32*) "pippoSource\0", 12);
    in1.size = 1024 * 1024 * 32; //1M
    in1.host_guest = caPipeDeviceConfigure::requestMode::host;
    in1.size = 100;
    deviceError res;
    s8 buff[1000];
    caStringStream<s8> ss;
    ss.Init(buff, 1000);
    res = caOS::Open("PIPE", in1, port1);
    if (res == deviceError::no_error) {
        caMemAux<u32>::MemCpy((u32*) in2.name, (u32*) "pippoDest\0\0\0", 12);
        in2.host_guest = caPipeDeviceConfigure::requestMode::guest;
        in2.size = 0;
        do {
            res = caOS::Open("PIPE", in2, port2);
        } while (res != deviceError::no_error);
    }
    if (res == deviceError::no_error) {
        while (loop1 < 100) {
            ss << (u32) caScheduler::GetCurrentContext() <<
                    " - Source TASK : write on Mem Pipe " << caEnd::endl;
            res = caOS::Write(port1, ss);
            port2.rdBuff = (u8*) buff;
            port2.readed = 0;
            port2.rdSize = 30;
            while (port2.readed < 30) {
                res = caOS::Read(port2);
                usleep(1000);
            }
            loop1++;
        }
    }
    res = caOS::Close(port1);
    res = caOS::Close(port2);
    return 0;
}

static u32 destTask(void) {
    caPipeDeviceConfigure in1, in2;
    caDeviceHandle port1, port2;
    u32 loop2;
    caMemAux<u32>::MemCpy((u32*) in1.name, (u32*) "pippoSource\0", 3);
    in1.host_guest = caPipeDeviceConfigure::requestMode::guest;
    in1.size = 0;
    deviceError res;
    s8 buff_in[1000];
    caStringStream<s8> ss;
    ss.Init(buff_in, 1000);
    do {
        res = caOS::Open("PIPE", in1, port1);
    } while (res != deviceError::error_device_already_opened);
    if (res == deviceError::no_error) {
        caMemAux<u32>::MemCpy((u32*) in2.name, (u32*) "pippoDest\0\0\0", 3);
        in2.size = 1024 * 1024 * 32; //1M
        in2.host_guest = caPipeDeviceConfigure::requestMode::host;
        res = caOS::Open("PIPE", in2, port2);
    }
    if (res == deviceError::no_error) {
        while (loop2 < 100) {
            port1.rdBuff = (u8*) buff_in;
            port1.readed = 0;
            port1.rdSize = 30;
            while (port1.readed < 30) {
                res = caOS::Read(port1);
                usleep(1000);
            }
            ss << (u32) caScheduler::GetCurrentContext() <<
                    " - Dest TASK : write on Mem Pipe " << caEnd::endl;
            caOS::Write(port2, ss);
            loop2++;
        }
    }
    res = caOS::Close(port1);
    res = caOS::Close(port2);
    return 0;
}

void caHalPipeDevice_test_class::test1(void) {
    _START();
    _INFO("to check halpipedevice function ");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();
    caOS::Init();
    sourceTask();
    destTask();

}

void caHalPipeDevice_test_class::test2(void) {
    _START();
    _INFO("to check halmemdevice function function of class caThread");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();
    caOS::Init();
    
}
