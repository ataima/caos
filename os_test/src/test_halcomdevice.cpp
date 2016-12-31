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
#include "halcomdevice.h"


static struct timespec start_time = {0, 0};
static u32 tcom_speed = 0;
static u32 tcom_stop = 0;
static u32 tcom_parity = 0;
static u32 tcom_data = 0;
static u32 tcom_enable_int_rx = 0;
static u32 tcom_stop_com = 0;
static u32 tcom_start_tx = 0;
static u32 tcom_enable_rx = 0;
static u32 tcom_enable_tx = 0;
static u32 tcom_dump = 0;

static void param_reset(void)
{
    tcom_speed = tcom_stop = tcom_parity = tcom_data = tcom_enable_int_rx =
            tcom_start_tx = tcom_stop_com = tcom_enable_rx = tcom_enable_tx = 
            tcom_dump=0;
}

static u32 hll_config(u32 speed, u32 stop, u32 parity, u32 data)
{
    u32 res = -1;
    tcom_speed = speed;
    tcom_stop = stop;
    tcom_parity = parity;
    tcom_data = data;
    res = 0;

    return res;
}

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

static u32 hll_en_int_rx(void)
{
    tcom_enable_int_rx = 1;
    return 0;
}

static u32 hll_stop(void)
{
    param_reset();
    tcom_stop_com = 1;
    return 0;
}

static u32 hll_get_errors(u32 &rxErr, u32 &txErr)
{
    rxErr = 127;
    txErr = 95;
    return 0;
}

static u32 hll_start_tx(void)
{
    tcom_start_tx = 1;
    return 0;
}

static u32 hll_enable(bool rx, bool tx)
{
    tcom_enable_rx = rx;
    tcom_enable_tx = tx;
    return 0;
}

static u32 hll_dump(caStringStream<s8> *ss)
{
    (*ss)<< "IOCTRL DUMP"<<caEnd::endl;
    tcom_dump=1;
    return 0;
}

// connector to virtual hardware
hal_ll_com_io hal_ll_com1 = {
    NULL,
    hll_config, //hll_config   -> Open
    hll_time, //hll_time      -> Open
    hll_en_int_rx, //hll_en_int_rx -> Open
    hll_start_tx, //hll_start_tx
    hll_stop, //hll_stop
    hll_enable, //hll_enable
    hll_dump, //hll_dump
    hll_get_errors, //hll_get_errors
    NULL, //hll_wakeup_rx
    NULL, //hll_wakeup_tx
    caHalDeviceRules::IrqServiceRx, //hll_irq_rx : set from device obj
    caHalDeviceRules::IrqServiceTx, //hll_irq_tx : set from device obj 
};

class caHalComDevice_test_class
: public caTester
{
    CA_TEST_SUITE(caHalComDevice_test_class);
    CA_TEST(caHalComDevice_test_class::test1, " Open test");
    CA_TEST(caHalComDevice_test_class::test2, " Close test");
    CA_TEST(caHalComDevice_test_class::test3, " Read test");
    CA_TEST(caHalComDevice_test_class::test4, " Write test");
    CA_TEST(caHalComDevice_test_class::test5, " IoCtrl enable test");
    CA_TEST(caHalComDevice_test_class::test6, " IoCtrl Dump test");
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

    void tearDown(void)
    {
    }


};

REGISTER_CLASS(caHalComDevice_test_class);

void caHalComDevice_test_class::test1(void)
{
    _START();
    _INFO("to check Open function of caHalComDevice");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();
    param_reset();
    caHalComDevice comDev(& hal_ll_com1, ioCtrlRequest::Com1);
    caComDeviceConfigure setup;
    setup.speed = 12345678;
    setup.stop = 11223344;
    setup.parity = 88776655;
    setup.data = 12312312;
    caDeviceHandle portIO;
    u32 res = comDev.Open(&setup, &portIO);
    CA_ASSERT(res == deviceError::no_error);
    CA_ASSERT(tcom_speed == 12345678);
    CA_ASSERT(tcom_stop == 11223344);
    CA_ASSERT(tcom_parity == 88776655);
    CA_ASSERT(tcom_data == 12312312);
    CA_ASSERT(portIO.handle != 0);
    CA_ASSERT((portIO.handle & ioCtrlRequest::Com1) == ioCtrlRequest::Com1);
    CA_ASSERT((caHalDeviceRules::IsValidHandle(portIO.handle, ioCtrlRequest::Com1) == true));
    CA_ASSERT(tcom_enable_int_rx == 1);
    CA_ASSERT(tcom_stop_com == 0);
    CA_ASSERT(portIO.status == caDeviceHandle::statusHandle::Open);
    CA_ASSERT(portIO.tStart != 0);
    CA_ASSERT(portIO.tStart == portIO.tLast);
    CA_ASSERT(portIO.tStop == 0);
    CA_ASSERT(portIO.wrError == 95);
    CA_ASSERT(portIO.rdError == 127);
    CA_ASSERT(portIO.tLastCmd == caDeviceAction::caActionOpen);
    param_reset();
}

void caHalComDevice_test_class::test2(void)
{
    _START();
    _INFO("to check Close function caHalComDevice");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();
    param_reset();
    caHalComDevice comDev(& hal_ll_com1, ioCtrlRequest::Com5);
    caComDeviceConfigure setup;
    setup.speed = 12345678;
    setup.stop = 11223344;
    setup.parity = 88776655;
    setup.data = 12312312;
    caDeviceHandle portIO;
    u32 res = comDev.Open(&setup, &portIO);
    CA_ASSERT(res == deviceError::no_error);
    CA_ASSERT(tcom_speed == 12345678);
    CA_ASSERT(tcom_stop == 11223344);
    CA_ASSERT(tcom_parity == 88776655);
    CA_ASSERT(tcom_data == 12312312);
    CA_ASSERT(portIO.handle != 0);
    CA_ASSERT((portIO.handle & ioCtrlRequest::Com5) == ioCtrlRequest::Com5);
    CA_ASSERT((caHalDeviceRules::IsValidHandle(portIO.handle, ioCtrlRequest::Com5) == true));
    CA_ASSERT(tcom_enable_int_rx == 1);
    CA_ASSERT(tcom_stop_com == 0);
    CA_ASSERT(portIO.status == caDeviceHandle::statusHandle::Open);
    CA_ASSERT(portIO.tStart != 0);
    CA_ASSERT(portIO.tStart == portIO.tLast);
    CA_ASSERT(portIO.tStop == 0);
    CA_ASSERT(portIO.wrError == 95);
    CA_ASSERT(portIO.rdError == 127);
    CA_ASSERT(portIO.tLastCmd == caDeviceAction::caActionOpen);
    res = comDev.Close(&portIO);
    CA_ASSERT(res == deviceError::no_error);
    CA_ASSERT(tcom_speed == 0);
    CA_ASSERT(tcom_stop == 0);
    CA_ASSERT(tcom_parity == 0);
    CA_ASSERT(tcom_data == 0);
    CA_ASSERT(tcom_enable_int_rx == 0);
    CA_ASSERT(tcom_stop_com == 1);
    CA_ASSERT(portIO.handle == (ioCtrlRequest::Com5 | BASE_HANDLE));
    CA_ASSERT(portIO.tStop != 0);
    CA_ASSERT(portIO.tStop >= portIO.tStart);
    CA_ASSERT(portIO.tLastCmd == caDeviceAction::caActionClose);
    CA_ASSERT(portIO.status == caDeviceHandle::statusHandle::Close);
    CA_ASSERT(portIO.tStop == portIO.tLast);
    param_reset();
}

void caHalComDevice_test_class::test3(void)
{
    _START();
    _INFO("to check Read Function of caHalComDevice");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();
    param_reset();
    u32 rd;
    caHalComDevice comDev(& hal_ll_com1, ioCtrlRequest::Com5);
    caComDeviceConfigure setup;
    setup.speed = 12345678;
    setup.stop = 11223344;
    setup.parity = 88776655;
    setup.data = 12312312;
    caDeviceHandle portIO;
    u32 res = comDev.Open(&setup, &portIO);
    CA_ASSERT(res == deviceError::no_error);
    CA_ASSERT(tcom_speed == 12345678);
    CA_ASSERT(tcom_stop == 11223344);
    CA_ASSERT(tcom_parity == 88776655);
    CA_ASSERT(tcom_data == 12312312);
    CA_ASSERT(portIO.handle != 0);
    CA_ASSERT((portIO.handle & ioCtrlRequest::Com5) == ioCtrlRequest::Com5);
    CA_ASSERT((caHalDeviceRules::IsValidHandle(portIO.handle, ioCtrlRequest::Com5) == true));
    CA_ASSERT(tcom_enable_int_rx == 1);
    CA_ASSERT(tcom_stop_com == 0);
    CA_ASSERT(portIO.status == caDeviceHandle::statusHandle::Open);
    CA_ASSERT(portIO.tStart != 0);
    CA_ASSERT(portIO.tStart == portIO.tLast);
    CA_ASSERT(portIO.tStop == 0);
    CA_ASSERT(portIO.wrError == 95);
    CA_ASSERT(portIO.rdError == 127);
    CA_ASSERT(portIO.tLastCmd == caDeviceAction::caActionOpen);
    //IRQ CALLBACK
    u8 msg[] = "hello world";
    hal_ll_com1.hll_irq_rx(hal_ll_com1.hll_lnk_obj, msg, 12, rd);
    CA_ASSERT(rd == 12);
    u8 buff[100];
    portIO.rdBuff = buff;
    portIO.rdSize = 100;
    portIO.readed = 0;
    res = comDev.Read(&portIO);
    CA_ASSERT(res == deviceError::no_error);
    CA_ASSERT(portIO.rdSize == 88);
    CA_ASSERT(portIO.readed == 12);
    CA_ASSERT(memcmp(buff, msg, 12) == 0);
    res = comDev.Close(&portIO);
    CA_ASSERT(res == deviceError::no_error);
    CA_ASSERT(tcom_speed == 0);
    CA_ASSERT(tcom_stop == 0);
    CA_ASSERT(tcom_parity == 0);
    CA_ASSERT(tcom_data == 0);
    CA_ASSERT(tcom_enable_int_rx == 0);
    CA_ASSERT(tcom_stop_com == 1);
    CA_ASSERT(portIO.handle == (ioCtrlRequest::Com5 | BASE_HANDLE));
    CA_ASSERT(portIO.tStop != 0);
    CA_ASSERT(portIO.tStop >= portIO.tStart);
    CA_ASSERT(portIO.tLastCmd == caDeviceAction::caActionClose);
    CA_ASSERT(portIO.status == caDeviceHandle::statusHandle::Close);
    CA_ASSERT(portIO.tStop == portIO.tLast);
    param_reset();
}

void caHalComDevice_test_class::test4(void)
{
    _START();
    _INFO("to check Write function caHalComDevice");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();
    param_reset();
    caHalComDevice comDev(& hal_ll_com1, ioCtrlRequest::Com5);
    caComDeviceConfigure setup;
    setup.speed = 12345678;
    setup.stop = 11223344;
    setup.parity = 88776655;
    setup.data = 12312312;
    caDeviceHandle portIO;
    u32 res = comDev.Open(&setup, &portIO);
    CA_ASSERT(res == deviceError::no_error);
    CA_ASSERT(tcom_speed == 12345678);
    CA_ASSERT(tcom_stop == 11223344);
    CA_ASSERT(tcom_parity == 88776655);
    CA_ASSERT(tcom_data == 12312312);
    CA_ASSERT(portIO.handle != 0);
    CA_ASSERT((portIO.handle & ioCtrlRequest::Com5) == ioCtrlRequest::Com5);
    CA_ASSERT((caHalDeviceRules::IsValidHandle(portIO.handle, ioCtrlRequest::Com5) == true));
    CA_ASSERT(tcom_enable_int_rx == 1);
    CA_ASSERT(tcom_stop_com == 0);
    CA_ASSERT(portIO.status == caDeviceHandle::statusHandle::Open);
    CA_ASSERT(portIO.tStart != 0);
    CA_ASSERT(portIO.tStart == portIO.tLast);
    CA_ASSERT(portIO.tStop == 0);
    CA_ASSERT(portIO.wrError == 95);
    CA_ASSERT(portIO.rdError == 127);
    CA_ASSERT(portIO.tLastCmd == caDeviceAction::caActionOpen);
    u8 msg[] = "hello world";
    portIO.wrBuff = msg;
    portIO.wrSize = 12;
    portIO.writed = 0;
    res = comDev.Write(&portIO);
    CA_ASSERT(res == deviceError::no_error);
    CA_ASSERT(portIO.wrSize == 0);
    CA_ASSERT(portIO.writed == 12);
    CA_ASSERT(tcom_start_tx == 1);
    param_reset();
    u8 buff[100];
    u32 rd;
    //IRQ TX CALLBACK
    hal_ll_com1.hll_irq_tx(hal_ll_com1.hll_lnk_obj, buff, 100, rd);
    CA_ASSERT(rd == 12);
    CA_ASSERT(memcmp(buff, msg, 12) == 0);
    res = comDev.Close(&portIO);
    CA_ASSERT(res == deviceError::no_error);
    CA_ASSERT(tcom_speed == 0);
    CA_ASSERT(tcom_stop == 0);
    CA_ASSERT(tcom_parity == 0);
    CA_ASSERT(tcom_data == 0);
    CA_ASSERT(tcom_enable_int_rx == 0);
    CA_ASSERT(tcom_stop_com == 1);
    CA_ASSERT(portIO.handle == (ioCtrlRequest::Com5 | BASE_HANDLE));
    CA_ASSERT(portIO.tStop != 0);
    CA_ASSERT(portIO.tStop >= portIO.tStart);
    CA_ASSERT(portIO.tLastCmd == caDeviceAction::caActionClose);
    CA_ASSERT(portIO.status == caDeviceHandle::statusHandle::Close);
    CA_ASSERT(portIO.tStop == portIO.tLast);
    param_reset();
}

void caHalComDevice_test_class::test5(void)
{
    _START();
    _INFO("to check IoCtrl Enable function caHalComDevice");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();
    param_reset();
    caHalComDevice comDev(& hal_ll_com1, ioCtrlRequest::Com6);
    caComDeviceConfigure setup;
    setup.speed = 12345678;
    setup.stop = 11223344;
    setup.parity = 88776655;
    setup.data = 12312312;
    caDeviceHandle portIO;
    u32 res = comDev.Open(&setup, &portIO);
    CA_ASSERT(res == deviceError::no_error);
    CA_ASSERT(tcom_speed == 12345678);
    CA_ASSERT(tcom_stop == 11223344);
    CA_ASSERT(tcom_parity == 88776655);
    CA_ASSERT(tcom_data == 12312312);
    CA_ASSERT(portIO.handle != 0);
    CA_ASSERT((portIO.handle & ioCtrlRequest::Com6) == ioCtrlRequest::Com6);
    CA_ASSERT((caHalDeviceRules::IsValidHandle(portIO.handle, ioCtrlRequest::Com6) == true));
    CA_ASSERT(tcom_enable_int_rx == 1);
    CA_ASSERT(tcom_stop_com == 0);
    CA_ASSERT(portIO.status == caDeviceHandle::statusHandle::Open);
    CA_ASSERT(portIO.tStart != 0);
    CA_ASSERT(portIO.tStart == portIO.tLast);
    CA_ASSERT(portIO.tStop == 0);
    CA_ASSERT(portIO.wrError == 95);
    CA_ASSERT(portIO.rdError == 127);
    CA_ASSERT(portIO.tLastCmd == caDeviceAction::caActionOpen);
    // IOCTRL 
    caComDeviceCtrl in;
    in.command = caComDeviceCtrl::IoCtrlDirect::comStart;
    res = comDev.IoCtrl(&portIO, &in);
    CA_ASSERT(res == deviceError::no_error);
    CA_ASSERT(tcom_enable_rx == 1);
    CA_ASSERT(tcom_enable_tx == 1);
    CA_ASSERT(portIO.tLast >= portIO.tStart);
    CA_ASSERT(portIO.tLastCmd == caDeviceAction::caActionIoCtrl);
    in.command = caComDeviceCtrl::IoCtrlDirect::comStop;
    res = comDev.IoCtrl(&portIO, &in);
    CA_ASSERT(res == deviceError::no_error);
    CA_ASSERT(tcom_enable_rx == 0);
    CA_ASSERT(tcom_enable_tx == 0);
    CA_ASSERT(portIO.tLast >= portIO.tStart);
    CA_ASSERT(portIO.tLastCmd == caDeviceAction::caActionIoCtrl);
    param_reset();
    res = comDev.Close(&portIO);
    CA_ASSERT(res == deviceError::no_error);
    CA_ASSERT(tcom_speed == 0);
    CA_ASSERT(tcom_stop == 0);
    CA_ASSERT(tcom_parity == 0);
    CA_ASSERT(tcom_data == 0);
    CA_ASSERT(tcom_enable_rx == 0);
    CA_ASSERT(tcom_stop_com == 1);
    CA_ASSERT(portIO.handle == (ioCtrlRequest::Com6 | BASE_HANDLE));
    CA_ASSERT(portIO.tStop != 0);
    CA_ASSERT(portIO.tStop >= portIO.tStart);
    CA_ASSERT(portIO.tLastCmd == caDeviceAction::caActionClose);
    CA_ASSERT(portIO.status == caDeviceHandle::statusHandle::Close);
    CA_ASSERT(portIO.tStop == portIO.tLast);
    param_reset();
}

void caHalComDevice_test_class::test6(void)
{
    _START();
    _INFO("to check IoCtrl Dump function caHalComDevice");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();
    param_reset();
    caHalComDevice comDev(& hal_ll_com1, ioCtrlRequest::Com6);
    caComDeviceConfigure setup;
    setup.speed = 12345678;
    setup.stop = 11223344;
    setup.parity = 88776655;
    setup.data = 12312312;
    caDeviceHandle portIO;
    u32 res = comDev.Open(&setup, &portIO);
    CA_ASSERT(res == deviceError::no_error);
    CA_ASSERT(tcom_speed == 12345678);
    CA_ASSERT(tcom_stop == 11223344);
    CA_ASSERT(tcom_parity == 88776655);
    CA_ASSERT(tcom_data == 12312312);
    CA_ASSERT(portIO.handle != 0);
    CA_ASSERT((portIO.handle & ioCtrlRequest::Com6) == ioCtrlRequest::Com6);
    CA_ASSERT((caHalDeviceRules::IsValidHandle(portIO.handle, ioCtrlRequest::Com6) == true));
    CA_ASSERT(tcom_enable_int_rx == 1);
    CA_ASSERT(tcom_stop_com == 0);
    CA_ASSERT(portIO.status == caDeviceHandle::statusHandle::Open);
    CA_ASSERT(portIO.tStart != 0);
    CA_ASSERT(portIO.tStart == portIO.tLast);
    CA_ASSERT(portIO.tStop == 0);
    CA_ASSERT(portIO.wrError == 95);
    CA_ASSERT(portIO.rdError == 127);
    CA_ASSERT(portIO.tLastCmd == caDeviceAction::caActionOpen);
    // IOCTRL 
    caComDeviceCtrl in;
    in.command = caComDeviceCtrl::IoCtrlDirect::comListHardware;
    res = comDev.IoCtrl(&portIO, &in);
    CA_ASSERT(res == deviceError::error_invalid_null_destination);
    caStringStream<s8> ss;
    s8 buff[100];
    ss.Init(buff,100);
    in.ss=&ss;
    res = comDev.IoCtrl(&portIO, &in);
    CA_ASSERT(res == deviceError::no_error);
    CA_ASSERT(tcom_dump == 1);
    CA_ASSERT(ss.Size()==13);
    CA_ASSERT(memcmp(ss.Str(),"IOCTRL DUMP\r\n",11)==0);
    CA_ASSERT(portIO.tLast >= portIO.tStart);
    CA_ASSERT(portIO.tLastCmd == caDeviceAction::caActionIoCtrl);
    param_reset();
    res = comDev.Close(&portIO);
    CA_ASSERT(res == deviceError::no_error);
    CA_ASSERT(tcom_speed == 0);
    CA_ASSERT(tcom_stop == 0);
    CA_ASSERT(tcom_parity == 0);
    CA_ASSERT(tcom_data == 0);
    CA_ASSERT(tcom_enable_rx == 0);
    CA_ASSERT(tcom_stop_com == 1);
    CA_ASSERT(portIO.handle == (ioCtrlRequest::Com6 | BASE_HANDLE));
    CA_ASSERT(portIO.tStop != 0);
    CA_ASSERT(portIO.tStop >= portIO.tStart);
    CA_ASSERT(portIO.tLastCmd == caDeviceAction::caActionClose);
    CA_ASSERT(portIO.status == caDeviceHandle::statusHandle::Close);
    CA_ASSERT(portIO.tStop == portIO.tLast);
    param_reset();
}