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
#include "comdevice.h"

static struct timespec start_time = {0,0};
static u32 tcom_speed = 0;
static u32 tcom_stop = 0;
static u32 tcom_parity = 0;
static u32 tcom_data = 0;
static u32 tcom_enable_rx=0;

static void param_reset(void)
{
    tcom_speed = tcom_stop = tcom_parity = tcom_data = tcom_enable_rx= 0;
}

static u32 hll_config(caIDeviceConfigure *conf)
{
    u32 res = -1;
    if (conf != NULL)
    {
        caComDeviceConfigure * setup = (caComDeviceConfigure *) (conf);
        tcom_speed = setup->speed;
        tcom_stop = setup->stop;
        tcom_parity = setup->parity;
        tcom_data = setup->data;
        res = 0;
    }
    return res;
}

static u32 hll_time(void)
{
    u32 ms; 
    struct timespec spec, diff;
    clock_gettime(CLOCK_REALTIME, &spec);
    diff.tv_sec = (time_t)difftime(spec.tv_sec, start_time.tv_sec);
    if (spec.tv_nsec > start_time.tv_nsec)
        diff.tv_nsec = spec.tv_nsec - start_time.tv_nsec;
    else
    {
        spec.tv_sec--;
        diff.tv_nsec = spec.tv_nsec - start_time.tv_nsec+1000000000;
    }
    ms=(u32)(diff.tv_sec*1000+diff.tv_nsec/1000000);
    return ms;
}

static u32 hll_en_int_rx(void){
    tcom_enable_rx=1;
    return 0;
}

const hal_ll_com_io hal_ll_com1 = {
    hll_config,         // hll_config   -> Open
    NULL,               //hll_write
    NULL,               //hll_read
    hll_time,           //hll_time      -> Open
    hll_en_int_rx,      //hll_en_int_rx -> Open
    NULL,               //hll_start_tx
    NULL,               //hll_stop
    NULL,               //hll_enable
    NULL                //hll_dump
};

class caHalComDevice_test_class
: public caTester
{
    CA_TEST_SUITE(caHalComDevice_test_class);
    CA_TEST(caHalComDevice_test_class::test1, " Open test");
    CA_TEST(caHalComDevice_test_class::test2, " Close test");
    CA_TEST(caHalComDevice_test_class::test3, " Read test");
    CA_TEST(caHalComDevice_test_class::test4, " Write test");
    CA_TEST_SUITE_END();

    void setUp(void)
    {
        clock_gettime(CLOCK_REALTIME, &start_time);
    }

    void test1(void);
    void test2(void);
    void test3(void);
    void test4(void);

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
    caHalComDevice comDev(& hal_ll_com1);
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
    CA_ASSERT(tcom_enable_rx == 1);
    param_reset();
}

void caHalComDevice_test_class::test2(void)
{
    _START();
    _INFO("to check  function caHalComDevice");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();
}

void caHalComDevice_test_class::test3(void)
{
    _START();
    _INFO("to check caHalComDevice");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();
}

void caHalComDevice_test_class::test4(void)
{
    _START();
    _INFO("to check caHalComDevice");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();
}
