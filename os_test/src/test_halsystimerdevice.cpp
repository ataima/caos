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
#include "halsystimerdevice.h"
#include "memory.h"
#include <iostream>

static struct timespec start_time = {0, 0};
struct timespec curr_spec = {0, 0};
static u32 timer_conf1 = 0;
static u32 timer_conf2 = 0;
static u32 timer_conf3 = 0;
static u32 timer_conf4 = 0;
static u32 timer_tick = 0;
static u32 timer_day = 0;
static u32 timer_hour = 0;
static u32 timer_min = 0;
static u32 timer_sec = 0;
static u32 timer_start = 0;
static u32 timer_stop = 0;
static u32 timer_wakeup_1 = 0;
static u32 timer_wakeup_2 = 0;
static u32 timer_dump = 1;



static void param_reset(void)
{
    timer_conf1 = timer_conf2 = timer_conf3 = timer_conf4 =
            timer_day = timer_hour = timer_min = timer_sec =
            timer_start = timer_stop = timer_wakeup_1 = timer_wakeup_2 =
            timer_tick = timer_dump=0;
}

static u32 hll_config(u32 conf1, u32 conf2, u32 conf3, u32 conf4)
{
    u32 res = -1;
    timer_conf1 = conf1;
    timer_conf2 = conf2;
    timer_conf3 = conf3;
    timer_conf4 = conf4;
    res = 1;
    return res;
}

static u32 hll_time(void)
{
    u32 us;
    struct timespec diff;
    clock_gettime(CLOCK_REALTIME, &curr_spec);
    diff.tv_sec = (time_t) difftime(curr_spec.tv_sec, start_time.tv_sec);
    if (curr_spec.tv_nsec > start_time.tv_nsec)
        diff.tv_nsec = curr_spec.tv_nsec - start_time.tv_nsec;
    else
    {
        curr_spec.tv_sec--;
        diff.tv_nsec = curr_spec.tv_nsec - start_time.tv_nsec + 1000000000;
    }
    us = (u32) (diff.tv_sec * 1000000 + diff.tv_nsec / 1000);
    if (us == 0)
        us = 1;
    return us;
}

static u32 hll_getps(void)
{
    hll_time();
    return 0;
}

static u32 hll_getns(void)
{
    hll_time();
    return (u32) (curr_spec.tv_nsec & 0xffff);
}

static u32 hll_getus(void)
{
    hll_time();
    return (u32) (curr_spec.tv_nsec / 1000);
}

static u32 hll_getms(void)
{
    hll_time();
    return (u32) (curr_spec.tv_nsec / 1000000);
}

static u32 hll_getsec(void)
{
    hll_time();
    return (u32) (curr_spec.tv_sec % 60);
}

static u32 hll_getmin(void)
{
    hll_time();
    return 12;
}

static u32 hll_gethour(void)
{
    hll_time();
    return 21;
}

static u32 hll_getday(void)
{
    hll_time();
    return 7;
}

static u32 hll_dump(caStringStream<s8> *ss)
{
   (*ss) << "IOCTRL DUMP" << caEnd::endl;
   timer_dump=1;
    return 0;
}

static u32 hll_settime(u32 day, u32 hour, u32 min, u32 sec)
{
    u32 res = -1;
    timer_day = day;
    timer_hour = hour;
    timer_min = min;
    timer_sec = sec;
    res = 0;
    return res;
}

static u32 hll_totick(u32 tick)
{
    hll_time();
    timer_tick = 1000 + tick;
    return 0;
}

static u32 hll_start(void)
{
    hll_time();
    timer_start = 1;
    return 0;
}

static u32 hll_stop(void)
{
    hll_time();
    timer_conf1 = 0;
    timer_conf2 = 0;
    timer_conf3 = 0;
    timer_conf4 = 0;
    timer_stop = 1;
    timer_start = 0;
    return 0;
}

static void hll_wakeup_1(u32 thid)
{
    hll_time();
    timer_wakeup_1 = thid;
}

static void hll_wakeup_2(u32 thid)
{
    hll_time();
    timer_wakeup_2 = thid;
}



hal_llc_sys_time hal_llc_time_1 = {
    NULL,
    hll_config, // empthy funzion base timer fix conf to scheduler task
    hll_time, //system tick count
    hll_getps,
    hll_getns,
    hll_getus,
    hll_getms,
    hll_getsec,
    hll_getmin,
    hll_gethour,
    hll_getday, // TO DO mounth, year , millenium
    hll_settime,
    hll_dump,
    hll_totick,
    hll_start,
    hll_stop,
    hll_wakeup_1,
    hll_wakeup_2,
    caHalDeviceRules::IrqService1,
    caHalDeviceRules::IrqService2,
};

class caHalSysTimerDevice_test_class
: public caTester
{
    CA_TEST_SUITE(caHalSysTimerDevice_test_class);
    CA_TEST(caHalSysTimerDevice_test_class::test1, " Open test");
    CA_TEST(caHalSysTimerDevice_test_class::test2, " Close test");
    CA_TEST(caHalSysTimerDevice_test_class::test3, " Read test");
    CA_TEST(caHalSysTimerDevice_test_class::test4, " Write test");
    CA_TEST(caHalSysTimerDevice_test_class::test5, " IoCtrl enable test");
    CA_TEST(caHalSysTimerDevice_test_class::test6, " IoCtrl Dump test");
    CA_TEST(caHalSysTimerDevice_test_class::test8, " IoCtrl Signal_1 test");
    CA_TEST(caHalSysTimerDevice_test_class::test9, " IoCtrl Signal_2 test");
    CA_TEST(caHalSysTimerDevice_test_class::test10, " IoCtrl Logs test");
    CA_TEST_SUITE_END();

    void setUp(void)
    {
        clock_gettime(CLOCK_REALTIME, &start_time);
    }

    void test1(void);
    void test2(void);
    void test3(void);
    void test4(void);
    void test5(void);
    void test6(void);
    void test8(void);
    void test9(void);
    void test10(void);

    void tearDown(void)
    {
    }


};

REGISTER_CLASS(caHalSysTimerDevice_test_class);

void caHalSysTimerDevice_test_class::test1(void)
{
    _START();
    _INFO("to check Open function of caHalSysTimerDevice");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();
    param_reset();
    caHalSysTimerDevice timerDev(& hal_llc_time_1, ioCtrlRequest::SysTimer1);
    caSysTimerConfigure setup;
    setup.tick_ps = 12345678;
    setup.clock_ps = 11223344;
    setup.prescaler_ps = 88776655;
    setup.irq_ps = 12312312;
    caDeviceHandle portIO;
    u32 res = timerDev.Open(&setup, &portIO);
    CA_ASSERT(res == deviceError::no_error);
    CA_ASSERT(timer_conf1 == 12345678);
    CA_ASSERT(timer_conf2 == 11223344);
    CA_ASSERT(timer_conf3 == 88776655);
    CA_ASSERT(timer_conf4 == 12312312);
    CA_ASSERT(timer_start == 1);
    CA_ASSERT(timer_stop == 0);

    CA_ASSERT(portIO.handle != 0);
    CA_ASSERT((portIO.handle & ioCtrlRequest::SysTimer1) == ioCtrlRequest::SysTimer1);
    CA_ASSERT((caHalDeviceRules::IsValidHandle(portIO.handle, ioCtrlRequest::SysTimer1) == true));
    CA_ASSERT(portIO.status == caDeviceHandle::statusHandle::Open);
    CA_ASSERT(portIO.tStart != 0);
    CA_ASSERT(portIO.tStart == portIO.tLast);
    CA_ASSERT(portIO.tStop == 0);
    CA_ASSERT(portIO.wrError == 0);
    CA_ASSERT(portIO.rdError == 0);
    CA_ASSERT(portIO.tLastCmd == caDeviceAction::caActionOpen);
    param_reset();
}

void caHalSysTimerDevice_test_class::test2(void)
{
    _START();
    _INFO("to check Close function caHalSysTimerDevice");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();
    param_reset();
    caHalSysTimerDevice timerDev(& hal_llc_time_1, ioCtrlRequest::SysTimer5);
    caSysTimerConfigure setup;
    setup.tick_ps = 12345678;
    setup.clock_ps = 11223344;
    setup.prescaler_ps = 88776655;
    setup.irq_ps = 12312312;
    caDeviceHandle portIO;
    u32 res = timerDev.Open(&setup, &portIO);
    CA_ASSERT(res == deviceError::no_error);
    CA_ASSERT(timer_conf1 == 12345678);
    CA_ASSERT(timer_conf2 == 11223344);
    CA_ASSERT(timer_conf3 == 88776655);
    CA_ASSERT(timer_conf4 == 12312312);
    CA_ASSERT(timer_start == 1);
    CA_ASSERT(timer_stop == 0);
    CA_ASSERT(portIO.handle != 0);
    CA_ASSERT((portIO.handle & ioCtrlRequest::SysTimer5) == ioCtrlRequest::SysTimer5);
    CA_ASSERT((caHalDeviceRules::IsValidHandle(portIO.handle, ioCtrlRequest::SysTimer5) == true));
    CA_ASSERT(portIO.status == caDeviceHandle::statusHandle::Open);
    CA_ASSERT(portIO.tStart != 0);
    CA_ASSERT(portIO.tStart == portIO.tLast);
    CA_ASSERT(portIO.tStop == 0);
    CA_ASSERT(portIO.wrError == 0);
    CA_ASSERT(portIO.rdError == 0);
    CA_ASSERT(portIO.tLastCmd == caDeviceAction::caActionOpen);
    res = timerDev.Close(&portIO);
    CA_ASSERT(res == deviceError::no_error);
    CA_ASSERT(timer_conf1 == 0);
    CA_ASSERT(timer_conf2 == 0);
    CA_ASSERT(timer_conf3 == 0);
    CA_ASSERT(timer_conf4 == 0);
    CA_ASSERT(timer_start == 0);
    CA_ASSERT(timer_stop == 1);
    CA_ASSERT(portIO.handle == (ioCtrlRequest::SysTimer5 | BASE_HANDLE));
    CA_ASSERT(portIO.tStop != 0);
    CA_ASSERT(portIO.tStop >= portIO.tStart);
    CA_ASSERT(portIO.tLastCmd == caDeviceAction::caActionClose);
    CA_ASSERT(portIO.status == caDeviceHandle::statusHandle::Close);
    CA_ASSERT(portIO.tStop == portIO.tLast);
    param_reset();
}

void caHalSysTimerDevice_test_class::test3(void)
{
    _START();
    _INFO("to check Read Function of caHalSysTimerDevice");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();
    param_reset();
    u32 rd;
    caHalSysTimerDevice timerDev(& hal_llc_time_1, ioCtrlRequest::SysTimer5);
    caSysTimerConfigure setup;
    setup.tick_ps = 12345678;
    setup.clock_ps = 11223344;
    setup.prescaler_ps = 88776655;
    setup.irq_ps = 12312312;
    caDeviceHandle portIO;
    u32 res = timerDev.Open(&setup, &portIO);
    CA_ASSERT(res == deviceError::no_error);
    CA_ASSERT(timer_conf1 == 12345678);
    CA_ASSERT(timer_conf2 == 11223344);
    CA_ASSERT(timer_conf3 == 88776655);
    CA_ASSERT(timer_conf4 == 12312312);
    CA_ASSERT(timer_start == 1);
    CA_ASSERT(timer_stop == 0);
    CA_ASSERT(portIO.handle != 0);
    CA_ASSERT((portIO.handle & ioCtrlRequest::SysTimer5) == ioCtrlRequest::SysTimer5);
    CA_ASSERT((caHalDeviceRules::IsValidHandle(portIO.handle, ioCtrlRequest::SysTimer5) == true));

    CA_ASSERT(portIO.status == caDeviceHandle::statusHandle::Open);
    CA_ASSERT(portIO.tStart != 0);
    CA_ASSERT(portIO.tStart == portIO.tLast);
    CA_ASSERT(portIO.tStop == 0);
    CA_ASSERT(portIO.wrError == 0);
    CA_ASSERT(portIO.rdError == 0);
    CA_ASSERT(portIO.tLastCmd == caDeviceAction::caActionOpen);
    //IRQ CALLBACK
    u8 msg[] = "hello world";
    hal_llc_time_1.hll_irq_1(hal_llc_time_1.hll_lnk_obj, msg, 12, rd);
    CA_ASSERT(rd==0);
    u8 buff[100];
    portIO.rdBuff = buff;
    portIO.rdSize = 100;
    portIO.readed = 0;
    res = timerDev.Read(&portIO);
    CA_ASSERT(res == deviceError::no_error);
    CA_ASSERT(portIO.rdSize == 68);
    CA_ASSERT(portIO.readed == 32);

    res = timerDev.Close(&portIO);
    CA_ASSERT(res == deviceError::no_error);
    CA_ASSERT(timer_conf1 == 0);
    CA_ASSERT(timer_conf2 == 0);
    CA_ASSERT(timer_conf3 == 0);
    CA_ASSERT(timer_conf4 == 0);
    CA_ASSERT(timer_start == 0);
    CA_ASSERT(timer_stop == 1);

    CA_ASSERT(portIO.handle == (ioCtrlRequest::SysTimer5 | BASE_HANDLE));
    CA_ASSERT(portIO.tStop != 0);
    CA_ASSERT(portIO.tStop >= portIO.tStart);
    CA_ASSERT(portIO.tLastCmd == caDeviceAction::caActionClose);
    CA_ASSERT(portIO.status == caDeviceHandle::statusHandle::Close);
    CA_ASSERT(portIO.tStop == portIO.tLast);
    param_reset();
}

void caHalSysTimerDevice_test_class::test4(void)
{
    _START();
    _INFO("to check Write function caHalSysTimerDevice");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();
    param_reset();
    caHalSysTimerDevice timerDev(& hal_llc_time_1, ioCtrlRequest::SysTimer5);
    caSysTimerConfigure setup;
    setup.tick_ps = 12345678;
    setup.clock_ps = 11223344;
    setup.prescaler_ps = 88776655;
    setup.irq_ps = 12312312;
    caDeviceHandle portIO;
    u32 res = timerDev.Open(&setup, &portIO);
    CA_ASSERT(res == deviceError::no_error);
    CA_ASSERT(timer_conf1 == 12345678);
    CA_ASSERT(timer_conf2 == 11223344);
    CA_ASSERT(timer_conf3 == 88776655);
    CA_ASSERT(timer_conf4 == 12312312);
    CA_ASSERT(timer_start == 1);
    CA_ASSERT(timer_stop == 0);
    CA_ASSERT(portIO.handle != 0);
    CA_ASSERT((portIO.handle & ioCtrlRequest::SysTimer5) == ioCtrlRequest::SysTimer5);
    CA_ASSERT((caHalDeviceRules::IsValidHandle(portIO.handle, ioCtrlRequest::SysTimer5) == true));

    CA_ASSERT(portIO.status == caDeviceHandle::statusHandle::Open);
    CA_ASSERT(portIO.tStart != 0);
    CA_ASSERT(portIO.tStart == portIO.tLast);
    CA_ASSERT(portIO.tStop == 0);
    CA_ASSERT(portIO.wrError == 0);
    CA_ASSERT(portIO.rdError == 0);
    CA_ASSERT(portIO.tLastCmd == caDeviceAction::caActionOpen);
    u8 msg[] = "hello world";
    portIO.wrBuff = msg;
    portIO.wrSize = 4 * sizeof (u32);
    portIO.writed = 0;
    res = timerDev.Write(&portIO);
    CA_ASSERT(res == deviceError::no_error);
    CA_ASSERT(portIO.wrSize == 0);
    CA_ASSERT(portIO.writed == 4 * sizeof (u32));

    param_reset();
    u8 buff[100];
    u32 rd;
    //IRQ TX CALLBACK
    hal_llc_time_1.hll_irq_2(hal_llc_time_1.hll_lnk_obj, buff, 100, rd);
    CA_ASSERT(rd==0);

    res = timerDev.Close(&portIO);
    CA_ASSERT(res == deviceError::no_error);
    CA_ASSERT(timer_conf1 == 0);
    CA_ASSERT(timer_conf2 == 0);
    CA_ASSERT(timer_conf3 == 0);
    CA_ASSERT(timer_conf4 == 0);
    CA_ASSERT(timer_start == 0);
    CA_ASSERT(timer_stop == 1);

    CA_ASSERT(portIO.handle == (ioCtrlRequest::SysTimer5 | BASE_HANDLE));
    CA_ASSERT(portIO.tStop != 0);
    CA_ASSERT(portIO.tStop >= portIO.tStart);
    CA_ASSERT(portIO.tLastCmd == caDeviceAction::caActionClose);
    CA_ASSERT(portIO.status == caDeviceHandle::statusHandle::Close);
    CA_ASSERT(portIO.tStop == portIO.tLast);
    param_reset();
}

void caHalSysTimerDevice_test_class::test5(void)
{
    _START();
    _INFO("to check IoCtrl Enable function caHalSysTimerDevice");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();
    param_reset();
    caHalSysTimerDevice timerDev(& hal_llc_time_1, ioCtrlRequest::SysTimer6);
    caSysTimerConfigure setup;
    setup.tick_ps = 12345678;
    setup.clock_ps = 11223344;
    setup.prescaler_ps = 88776655;
    setup.irq_ps = 12312312;
    caDeviceHandle portIO;
    u32 res = timerDev.Open(&setup, &portIO);
    CA_ASSERT(res == deviceError::no_error);
    CA_ASSERT(timer_conf1 == 12345678);
    CA_ASSERT(timer_conf2 == 11223344);
    CA_ASSERT(timer_conf3 == 88776655);
    CA_ASSERT(timer_conf4 == 12312312);
    CA_ASSERT(timer_start == 1);
    CA_ASSERT(timer_stop == 0);
    CA_ASSERT(portIO.handle != 0);
    CA_ASSERT((portIO.handle & ioCtrlRequest::SysTimer6) == ioCtrlRequest::SysTimer6);
    CA_ASSERT((caHalDeviceRules::IsValidHandle(portIO.handle, ioCtrlRequest::SysTimer6) == true));

    CA_ASSERT(portIO.status == caDeviceHandle::statusHandle::Open);
    CA_ASSERT(portIO.tStart != 0);
    CA_ASSERT(portIO.tStart == portIO.tLast);
    CA_ASSERT(portIO.tStop == 0);
    CA_ASSERT(portIO.wrError == 0);
    CA_ASSERT(portIO.rdError == 0);
    CA_ASSERT(portIO.tLastCmd == caDeviceAction::caActionOpen);
    // IOCTRL 
    caSysTimerDeviceCtrl in;
    in.command = caSysTimerDeviceCtrl::IoCtrlDirect::sysTimerStart;
    res = timerDev.IoCtrl(&portIO, &in);
    CA_ASSERT(res == deviceError::no_error);

    CA_ASSERT(portIO.tLast >= portIO.tStart);
    CA_ASSERT(portIO.tLastCmd == caDeviceAction::caActionIoCtrl);
    in.command = caSysTimerDeviceCtrl::IoCtrlDirect::sysTimerStop;
    res = timerDev.IoCtrl(&portIO, &in);
    CA_ASSERT(res == deviceError::no_error);

    CA_ASSERT(portIO.tLast >= portIO.tStart);
    CA_ASSERT(portIO.tLastCmd == caDeviceAction::caActionIoCtrl);
    param_reset();
    res = timerDev.Close(&portIO);
    CA_ASSERT(res == deviceError::no_error);
    CA_ASSERT(timer_conf1 == 0);
    CA_ASSERT(timer_conf2 == 0);
    CA_ASSERT(timer_conf3 == 0);
    CA_ASSERT(timer_conf4 == 0);
    CA_ASSERT(timer_start == 0);
    CA_ASSERT(timer_stop == 1);

    CA_ASSERT(portIO.handle == (ioCtrlRequest::SysTimer6 | BASE_HANDLE));
    CA_ASSERT(portIO.tStop != 0);
    CA_ASSERT(portIO.tStop >= portIO.tStart);
    CA_ASSERT(portIO.tLastCmd == caDeviceAction::caActionClose);
    CA_ASSERT(portIO.status == caDeviceHandle::statusHandle::Close);
    CA_ASSERT(portIO.tStop == portIO.tLast);
    param_reset();
}

void caHalSysTimerDevice_test_class::test6(void)
{
    _START();
    _INFO("to check IoCtrl Dump function caHalSysTimerDevice");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();
    param_reset();
    caHalSysTimerDevice timerDev(& hal_llc_time_1, ioCtrlRequest::SysTimer6);
    caSysTimerConfigure setup;
    setup.tick_ps = 12345678;
    setup.clock_ps = 11223344;
    setup.prescaler_ps = 88776655;
    setup.irq_ps = 12312312;
    caDeviceHandle portIO;
    u32 res = timerDev.Open(&setup, &portIO);
    CA_ASSERT(res == deviceError::no_error);
    CA_ASSERT(timer_conf1 == 12345678);
    CA_ASSERT(timer_conf2 == 11223344);
    CA_ASSERT(timer_conf3 == 88776655);
    CA_ASSERT(timer_conf4 == 12312312);
    CA_ASSERT(timer_start == 1);
    CA_ASSERT(timer_stop == 0);
    CA_ASSERT(portIO.handle != 0);
    CA_ASSERT((portIO.handle & ioCtrlRequest::SysTimer6) == ioCtrlRequest::SysTimer6);
    CA_ASSERT((caHalDeviceRules::IsValidHandle(portIO.handle, ioCtrlRequest::SysTimer6) == true));

    CA_ASSERT(portIO.status == caDeviceHandle::statusHandle::Open);
    CA_ASSERT(portIO.tStart != 0);
    CA_ASSERT(portIO.tStart == portIO.tLast);
    CA_ASSERT(portIO.tStop == 0);
    CA_ASSERT(portIO.wrError == 0);
    CA_ASSERT(portIO.rdError == 0);
    CA_ASSERT(portIO.tLastCmd == caDeviceAction::caActionOpen);
    // IOCTRL 
    caSysTimerDeviceCtrl in;
    in.command = caSysTimerDeviceCtrl::IoCtrlDirect::sysTimerListHardware;
    res = timerDev.IoCtrl(&portIO, &in);
    CA_ASSERT(res == deviceError::error_invalid_null_destination);
    caStringStream<s8> ss;
    s8 buff[100];
    ss.Init(buff, 100);
    in.ss = &ss;
    res = timerDev.IoCtrl(&portIO, &in);
    CA_ASSERT(res == deviceError::no_error);

    CA_ASSERT(ss.Size() == 13);
    CA_ASSERT(ss == "IOCTRL DUMP\r\n");
    CA_ASSERT(portIO.tLast >= portIO.tStart);
    CA_ASSERT(portIO.tLastCmd == caDeviceAction::caActionIoCtrl);
    param_reset();
    res = timerDev.Close(&portIO);
    CA_ASSERT(res == deviceError::no_error);
    CA_ASSERT(timer_conf1 == 0);
    CA_ASSERT(timer_conf2 == 0);
    CA_ASSERT(timer_conf3 == 0);
    CA_ASSERT(timer_conf4 == 0);
    CA_ASSERT(timer_start == 0);
    CA_ASSERT(timer_stop == 1);

    CA_ASSERT(portIO.handle == (ioCtrlRequest::SysTimer6 | BASE_HANDLE));
    CA_ASSERT(portIO.tStop != 0);
    CA_ASSERT(portIO.tStop >= portIO.tStart);
    CA_ASSERT(portIO.tLastCmd == caDeviceAction::caActionClose);
    CA_ASSERT(portIO.status == caDeviceHandle::statusHandle::Close);
    CA_ASSERT(portIO.tStop == portIO.tLast);
    param_reset();
}

void caHalSysTimerDevice_test_class::test8(void)
{
    _START();
    _INFO("to check IoCtrl SignalRX  function caHalSysTimerDevice");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();
    param_reset();
    caHalSysTimerDevice timerDev(& hal_llc_time_1, ioCtrlRequest::SysTimer6);
    caSysTimerConfigure setup;
    setup.tick_ps = 12345678;
    setup.clock_ps = 11223344;
    setup.prescaler_ps = 88776655;
    setup.irq_ps = 12312312;
    caDeviceHandle portIO;
    u32 res = timerDev.Open(&setup, &portIO);
    CA_ASSERT(res == deviceError::no_error);
    CA_ASSERT(timer_conf1 == 12345678);
    CA_ASSERT(timer_conf2 == 11223344);
    CA_ASSERT(timer_conf3 == 88776655);
    CA_ASSERT(timer_conf4 == 12312312);
    CA_ASSERT(timer_start == 1);
    CA_ASSERT(timer_stop == 0);
    CA_ASSERT(portIO.handle != 0);
    CA_ASSERT((portIO.handle & ioCtrlRequest::SysTimer6) == ioCtrlRequest::SysTimer6);
    CA_ASSERT((caHalDeviceRules::IsValidHandle(portIO.handle, ioCtrlRequest::SysTimer6) == true));

    CA_ASSERT(portIO.status == caDeviceHandle::statusHandle::Open);
    CA_ASSERT(portIO.tStart != 0);
    CA_ASSERT(portIO.tStart == portIO.tLast);
    CA_ASSERT(portIO.tStop == 0);
    CA_ASSERT(portIO.wrError == 0);
    CA_ASSERT(portIO.rdError == 0);
    CA_ASSERT(portIO.tLastCmd == caDeviceAction::caActionOpen);
    caSysTimerDeviceCtrl in;
    in.command = caSysTimerDeviceCtrl::IoCtrlDirect::sysTimerAddSignal_1;
    in.param_1 = 12345678; // invalid handle from scheduler connector
    res = timerDev.IoCtrl(&portIO, &in);
    CA_ASSERT(res == deviceError::error_invalid_handle_port);
    CA_ASSERT(portIO.tLast >= portIO.tStart)
    in.param_1 = 100; // ok
    res = timerDev.IoCtrl(&portIO, &in);
    CA_ASSERT(res == deviceError::no_error);
    CA_ASSERT(portIO.tLast >= portIO.tStart);
    in.param_1 = 100; // already set
    res = timerDev.IoCtrl(&portIO, &in);
    CA_ASSERT(res == deviceError::error_signal_already_set);
    CA_ASSERT(portIO.tLast >= portIO.tStart);
    in.command = caSysTimerDeviceCtrl::IoCtrlDirect::sysTimerGetSignal_1;
    res = timerDev.IoCtrl(&portIO, &in);
    CA_ASSERT(res == deviceError::no_error);
    CA_ASSERT(in.param_1 == 100);
    //IRQ CALLBACK
    u8 msg[] = "hello world";
    u32 rd;
    hal_llc_time_1.hll_irq_1(hal_llc_time_1.hll_lnk_obj, msg, 12, rd);   
    CA_ASSERT(timer_wakeup_1 == 100);
    in.command = caSysTimerDeviceCtrl::IoCtrlDirect::sysTimerRemoveSignal_1;
    res = timerDev.IoCtrl(&portIO, &in);
    CA_ASSERT(res == deviceError::no_error);
    in.command = caSysTimerDeviceCtrl::IoCtrlDirect::sysTimerRemoveSignal_1;
    res = timerDev.IoCtrl(&portIO, &in);
    CA_ASSERT(res == deviceError::error_invalid_handle_port);
    res = timerDev.Close(&portIO);
    CA_ASSERT(res == deviceError::no_error);
    CA_ASSERT(timer_conf1 == 0);
    CA_ASSERT(timer_conf2 == 0);
    CA_ASSERT(timer_conf3 == 0);
    CA_ASSERT(timer_conf4 == 0);
    CA_ASSERT(timer_start == 0);
    CA_ASSERT(timer_stop == 1);

    CA_ASSERT(portIO.handle == (ioCtrlRequest::SysTimer6 | BASE_HANDLE));
    CA_ASSERT(portIO.tStop != 0);
    CA_ASSERT(portIO.tStop >= portIO.tStart);
    CA_ASSERT(portIO.tLastCmd == caDeviceAction::caActionClose);
    CA_ASSERT(portIO.status == caDeviceHandle::statusHandle::Close);
    CA_ASSERT(portIO.tStop == portIO.tLast);
    param_reset();
}

void caHalSysTimerDevice_test_class::test9(void)
{
    _START();
    _INFO("to check IoCtrl SignalTX  function caHalSysTimerDevice");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();
    param_reset();
    caHalSysTimerDevice timerDev(& hal_llc_time_1, ioCtrlRequest::SysTimer6);
    caSysTimerConfigure setup;
    setup.tick_ps = 12345678;
    setup.clock_ps = 11223344;
    setup.prescaler_ps = 88776655;
    setup.irq_ps = 12312312;
    caDeviceHandle portIO;
    u32 res = timerDev.Open(&setup, &portIO);
    CA_ASSERT(res == deviceError::no_error);
    CA_ASSERT(timer_conf1 == 12345678);
    CA_ASSERT(timer_conf2 == 11223344);
    CA_ASSERT(timer_conf3 == 88776655);
    CA_ASSERT(timer_conf4 == 12312312);
    CA_ASSERT(timer_start == 1);
    CA_ASSERT(timer_stop == 0);
    CA_ASSERT(portIO.handle != 0);
    CA_ASSERT((portIO.handle & ioCtrlRequest::SysTimer6) == ioCtrlRequest::SysTimer6);
    CA_ASSERT((caHalDeviceRules::IsValidHandle(portIO.handle, ioCtrlRequest::SysTimer6) == true));


    CA_ASSERT(portIO.status == caDeviceHandle::statusHandle::Open);
    CA_ASSERT(portIO.tStart != 0);
    CA_ASSERT(portIO.tStart == portIO.tLast);
    CA_ASSERT(portIO.tStop == 0);
    CA_ASSERT(portIO.wrError == 0);
    CA_ASSERT(portIO.rdError == 0);
    CA_ASSERT(portIO.tLastCmd == caDeviceAction::caActionOpen);
    caSysTimerDeviceCtrl in;
    in.command = caSysTimerDeviceCtrl::IoCtrlDirect::sysTimerAddSignal_2;
    in.param_2 = 12345678; // invalid handle from scheduler connector
    res = timerDev.IoCtrl(&portIO, &in);
    CA_ASSERT(res == deviceError::error_invalid_handle_port);
    CA_ASSERT(portIO.tLast >= portIO.tStart)
    in.param_2 = 200; // ok
    res = timerDev.IoCtrl(&portIO, &in);
    CA_ASSERT(res == deviceError::no_error);
    CA_ASSERT(portIO.tLast >= portIO.tStart);
    in.param_2 = 200; // already set
    res = timerDev.IoCtrl(&portIO, &in);
    CA_ASSERT(res == deviceError::error_signal_already_set);
    CA_ASSERT(portIO.tLast >= portIO.tStart);
    in.command = caSysTimerDeviceCtrl::IoCtrlDirect::sysTimerGetSignal_2;
    res = timerDev.IoCtrl(&portIO, &in);
    CA_ASSERT(res == deviceError::no_error);
    CA_ASSERT(in.param_1 == 200);
    u8 msg[] = "hello world";
    portIO.wrBuff = msg;
    portIO.wrSize = 4*sizeof(u32);
    portIO.writed = 0;
    res = timerDev.Write(&portIO);
    CA_ASSERT(res == deviceError::no_error);
    CA_ASSERT(portIO.wrSize == 0);
    CA_ASSERT(portIO.writed == 4*sizeof(u32));


    u8 buff[100];
    u32 rd;
    //IRQ TX CALLBACK
    hal_llc_time_1.hll_irq_2(hal_llc_time_1.hll_lnk_obj, buff, 100, rd);
    CA_ASSERT(rd == 0);
   
    CA_ASSERT(timer_wakeup_2 == 200);
    in.command = caSysTimerDeviceCtrl::IoCtrlDirect::sysTimerRemoveSignal_2;
    res = timerDev.IoCtrl(&portIO, &in);
    CA_ASSERT(res == deviceError::no_error);
    in.command = caSysTimerDeviceCtrl::IoCtrlDirect::sysTimerRemoveSignal_2;
    res = timerDev.IoCtrl(&portIO, &in);
    CA_ASSERT(res == deviceError::error_invalid_handle_port);
    res = timerDev.Close(&portIO);
    CA_ASSERT(res == deviceError::no_error);
    CA_ASSERT(timer_conf1 == 0);
    CA_ASSERT(timer_conf2 == 0);
    CA_ASSERT(timer_conf3 == 0);
    CA_ASSERT(timer_conf4 == 0);
    CA_ASSERT(timer_start == 0);
    CA_ASSERT(timer_stop == 1);

    CA_ASSERT(portIO.handle == (ioCtrlRequest::SysTimer6 | BASE_HANDLE));
    CA_ASSERT(portIO.tStop != 0);
    CA_ASSERT(portIO.tStop >= portIO.tStart);
    CA_ASSERT(portIO.tLastCmd == caDeviceAction::caActionClose);
    CA_ASSERT(portIO.status == caDeviceHandle::statusHandle::Close);
    CA_ASSERT(portIO.tStop == portIO.tLast);
    param_reset();
}

void caHalSysTimerDevice_test_class::test10(void)
{
    _START();
    _INFO("to check IoCtrl Logs  function caHalSysTimerDevice");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();
    param_reset();
    caHalSysTimerDevice timerDev(& hal_llc_time_1, ioCtrlRequest::SysTimer6);
    caSysTimerConfigure setup;
    setup.tick_ps = 12345678;
    setup.clock_ps = 11223344;
    setup.prescaler_ps = 88776655;
    setup.irq_ps = 12312312;
    caMemory::Init();
    caDeviceHandle portIO;
    caSysTimerDeviceCtrl in;
    in.param_1 = 16512;
    in.param_2 = (u32) deviceloglevels::end_device_log_lev;
    in.command = caSysTimerDeviceCtrl::IoCtrlDirect::sysTimerLogCreate;
    u32 res = timerDev.IoCtrl(&portIO, &in);
    CA_ASSERT(res == deviceError::no_error);
    in.command = caSysTimerDeviceCtrl::IoCtrlDirect::sysTimerLogStart;
    res = timerDev.IoCtrl(&portIO, &in);
    CA_ASSERT(res == deviceError::no_error);
    res = timerDev.Open(&setup, &portIO);
    CA_ASSERT(res == deviceError::no_error);
    CA_ASSERT(timer_conf1 == 12345678);
    CA_ASSERT(timer_conf2 == 11223344);
    CA_ASSERT(timer_conf3 == 88776655);
    CA_ASSERT(timer_conf4 == 12312312);
    CA_ASSERT(timer_start == 1);
    CA_ASSERT(timer_stop == 0);
    CA_ASSERT(portIO.handle != 0);
    CA_ASSERT((portIO.handle & ioCtrlRequest::SysTimer6) == ioCtrlRequest::SysTimer6);
    CA_ASSERT((caHalDeviceRules::IsValidHandle(portIO.handle, ioCtrlRequest::SysTimer6) == true));

    CA_ASSERT(portIO.status == caDeviceHandle::statusHandle::Open);
    CA_ASSERT(portIO.tStart != 0);
    CA_ASSERT(portIO.tStart == portIO.tLast);
    CA_ASSERT(portIO.tStop == 0);
    CA_ASSERT(portIO.wrError == 0);
    CA_ASSERT(portIO.rdError == 0);
    CA_ASSERT(portIO.tLastCmd == caDeviceAction::caActionOpen);

    in.command = caSysTimerDeviceCtrl::IoCtrlDirect::sysTimerAddSignal_2;
    in.param_2 = 12345678; // invalid handle from scheduler connector
    res = timerDev.IoCtrl(&portIO, &in);
    CA_ASSERT(res == deviceError::error_invalid_handle_port);
    CA_ASSERT(portIO.tLast >= portIO.tStart)
    in.param_2 = 200; // ok
    res = timerDev.IoCtrl(&portIO, &in);
    CA_ASSERT(res == deviceError::no_error);
    CA_ASSERT(portIO.tLast >= portIO.tStart);
    in.param_2 = 200; // already set
    res = timerDev.IoCtrl(&portIO, &in);
    CA_ASSERT(res == deviceError::error_signal_already_set);
    CA_ASSERT(portIO.tLast >= portIO.tStart);
    in.command = caSysTimerDeviceCtrl::IoCtrlDirect::sysTimerGetSignal_2;
    res = timerDev.IoCtrl(&portIO, &in);
    CA_ASSERT(res == deviceError::no_error);
    CA_ASSERT(in.param_1 == 200);
    u8 msg[] = "hello world";
    portIO.wrBuff = msg;
    portIO.wrSize = 4*sizeof(u32);
    portIO.writed = 0;
    res = timerDev.Write(&portIO);
    CA_ASSERT(res == deviceError::no_error);
    CA_ASSERT(portIO.wrSize == 0);
    CA_ASSERT(portIO.writed ==  4*sizeof(u32));


    u8 buff[100];
    u32 rd;
    //IRQ TX CALLBACK
    hal_llc_time_1.hll_irq_2(hal_llc_time_1.hll_lnk_obj, buff, 100, rd);
    CA_ASSERT(rd == 0);

    CA_ASSERT(timer_wakeup_2 == 200);
    in.command = caSysTimerDeviceCtrl::IoCtrlDirect::sysTimerRemoveSignal_2;
    res = timerDev.IoCtrl(&portIO, &in);
    CA_ASSERT(res == deviceError::no_error);
    in.command = caSysTimerDeviceCtrl::IoCtrlDirect::sysTimerRemoveSignal_2;
    res = timerDev.IoCtrl(&portIO, &in);
    CA_ASSERT(res == deviceError::error_invalid_handle_port);
    res = timerDev.Close(&portIO);
    CA_ASSERT(res == deviceError::no_error);
    CA_ASSERT(timer_conf1 == 0);
    CA_ASSERT(timer_conf2 == 0);
    CA_ASSERT(timer_conf3 == 0);
    CA_ASSERT(timer_conf4 == 0);
    CA_ASSERT(timer_start == 0);
    CA_ASSERT(timer_stop == 1);


    CA_ASSERT(portIO.handle == (ioCtrlRequest::SysTimer6 | BASE_HANDLE));
    CA_ASSERT(portIO.tStop != 0);
    CA_ASSERT(portIO.tStop >= portIO.tStart);
    CA_ASSERT(portIO.tLastCmd == caDeviceAction::caActionClose);
    CA_ASSERT(portIO.status == caDeviceHandle::statusHandle::Close);
    CA_ASSERT(portIO.tStop == portIO.tLast);
    in.command = caSysTimerDeviceCtrl::IoCtrlDirect::sysTimerLogStop;
    res = timerDev.IoCtrl(&portIO, &in);
    CA_ASSERT(res == deviceError::no_error);
    char out[8000];
    caStringStream<s8> ss;
    u32 u = 0;
    for (u = 0; u < deviceloglevels::end_device_log_lev; u++)
    {
        ss.Init(out, 8000);
        in.ss = &ss;
        in.param_1 = u;
        in.command = caSysTimerDeviceCtrl::IoCtrlDirect::sysTimerLogGet;
        res = timerDev.IoCtrl(&portIO, &in);
        std::cout << out << std::endl;
    }
    CA_ASSERT(res == deviceError::no_error);
    in.command = caSysTimerDeviceCtrl::IoCtrlDirect::sysTimerLogDestroy;
    res = timerDev.IoCtrl(&portIO, &in);
    CA_ASSERT(res == deviceError::no_error);
    param_reset();

}