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

class testDevice
: public IDevice
{
    u32 isOpen;
public:

    testDevice()
    {
        isOpen = 0;
    }

    inline void setIsOpen(u32 v)
    {
        isOpen = v;
    }

    u32 Open(caIDeviceConfigure *conf, caDeviceHandle *port)
    {
        std::cout << "Call to : testDevice::Open( " << conf << " , " << port << " );" << std::endl;
        if (port)port->handle = 0xfff01002;
        return 0;
    }

    u32 Close(caDeviceHandle *port)
    {
        std::cout << "Call to : testDevice::Close( " << port << " );" << std::endl;
        return 0;
    }

    u32 Write(caDeviceHandle *port)
    {
        std::cout << "Call to : testDevice::Write( " << port << " );" << std::endl;
        return 0;
    }

    u32 Read(caDeviceHandle *port)
    {
        std::cout << "Call to : testDevice::Read( " << port << " );" << std::endl;
        return 0;
    }

    u32 Flush(caDeviceHandle *port)
    {
        std::cout << "Call to : testDevice::Flush( " << port << " );" << std::endl;
        return 0;
    }

    u32 IoCtrl(caDeviceHandle *port, caIDeviceCtrl *ctrl)
    {
        std::cout << "Call to : testDevice::IoCtrl( " << port << " , " << ctrl << " );" << std::endl;
        return 0;
    }

    u32 GetOpenFlag(void)
    {
        return isOpen;
    }

    u32 IrqServiceTx(u8 * txbuff, s_t size,s_t & writed)
    {
        CA_ASSERT(txbuff != NULL);
        CA_ASSERT(size == 1);
        writed=size;
        return 0;
    }

    u32 IrqServiceRx(u8 * rxbuff, s_t size,s_t & readed)
    {
        CA_ASSERT(rxbuff != NULL);
        CA_ASSERT(size == 1);
        readed=size;
        return 0;
    };
};

class testDevice_test_class
: public caTester
{
    CA_TEST_SUITE(testDevice_test_class);
    CA_TEST(testDevice_test_class::test1, "Open test");
    CA_TEST(testDevice_test_class::test2, "Close test");
    CA_TEST(testDevice_test_class::test3, "Write test");
    CA_TEST(testDevice_test_class::test4, "Read test");
    CA_TEST(testDevice_test_class::test5, "Flush test");
    CA_TEST(testDevice_test_class::test6, "Ioctrl test");
    CA_TEST(testDevice_test_class::test7, "GetOpenFlag test");
    CA_TEST(testDevice_test_class::test8, "IrqRx test");
    CA_TEST(testDevice_test_class::test9, "IrqTx test");
    CA_TEST_SUITE_END();

    void setUp(void)
    {
    }

    void test1(void)
    {
        testDevice t;
        CA_ASSERT(t.Open(NULL, NULL) == 0);
    }

    void test2(void)
    {
        testDevice t;
        CA_ASSERT(t.Close(NULL) == 0);
    }

    void test3(void)
    {
        testDevice t;
        CA_ASSERT(t.Write(NULL) == 0);
    }

    void test4(void)
    {
        testDevice t;
        CA_ASSERT(t.Read(NULL) == 0);
    }

    void test5(void)
    {
        testDevice t;
        CA_ASSERT(t.Flush(NULL) == 0);
    }

    void test6(void)
    {
        testDevice t;
        CA_ASSERT(t.IoCtrl(NULL, NULL) == 0);
    }

    void test7(void)
    {
        testDevice t;
        CA_ASSERT(t.GetOpenFlag() == 0);
        t.setIsOpen(12734);
        CA_ASSERT(t.GetOpenFlag() == 12734);
    }

    void test8(void)
    {
        u8 buff;
        testDevice t;
        s_t readed;
        CA_ASSERT(t.IrqServiceRx(&buff, 1,readed) == 0);
        CA_ASSERT(readed == 1);
        
    }

    void test9(void)
    {
        u8 buff;
        testDevice t;
        s_t writed;
        CA_ASSERT(t.IrqServiceTx(&buff, 1,writed) == 0);
        CA_ASSERT(writed == 1);
    }

    void tearDown(void)
    {
    }

};

REGISTER_CLASS(testDevice_test_class);

class caHalDeviceRules_test_class
: public caTester
{
    CA_TEST_SUITE(caHalDeviceRules_test_class);
    CA_TEST(caHalDeviceRules_test_class::test1, "Open test");
    CA_TEST(caHalDeviceRules_test_class::test2, "Close test");
    CA_TEST(caHalDeviceRules_test_class::test3, "Write test");
    CA_TEST(caHalDeviceRules_test_class::test4, "Read test");
    CA_TEST(caHalDeviceRules_test_class::test5, "IoCtrl test");
    CA_TEST(caHalDeviceRules_test_class::test6, "Flush test");
    CA_TEST_SUITE_END();

    void setUp(void)
    {
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

REGISTER_CLASS(caHalDeviceRules_test_class);

void caHalDeviceRules_test_class::test1(void)
{
    _START();
    _INFO("to check Open function of caHalDeviceRules");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();
    testDevice t;
    caIDeviceConfigure setup;
    caDeviceHandle port;
    u32 guid = 0xfff00000;
    memset(&port, 128, sizeof (port));
    CA_ASSERT(caHalDeviceRules::Open(NULL, NULL, NULL, 0) == deviceError::error_invalid_null_device);
    CA_ASSERT(caHalDeviceRules::Open(&t, NULL, NULL, 0) == deviceError::error_device_config_param);
    CA_ASSERT(caHalDeviceRules::Open(&t, &setup, NULL, 0) == deviceError::error_invalid_null_port);
    CA_ASSERT(caHalDeviceRules::Open(&t, &setup, &port, guid) == deviceError::no_error);
    CA_ASSERT(port.handle = 0xfff01002);
}

void caHalDeviceRules_test_class::test2(void)
{
    _START();
    _INFO("to check Close function of caHalDeviceRules");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();
    testDevice t;
    caDeviceHandle port;
    u32 guid = 0xfff00000;
    memset(&port, 0, sizeof (port));
    port.handle = 0xfff00000 + BASE_HANDLE + 1;
    t.setIsOpen(1);
    CA_ASSERT(caHalDeviceRules::Close(NULL, NULL, 0) == deviceError::error_invalid_null_device);
    CA_ASSERT(caHalDeviceRules::Close(&t, NULL, 0) == deviceError::error_invalid_null_port);
    CA_ASSERT(caHalDeviceRules::Close(&t, &port, guid) == deviceError::no_error);
    t.setIsOpen(0);
    CA_ASSERT(caHalDeviceRules::Close(&t, &port, guid) == deviceError::error_device_not_opened);
    t.setIsOpen(1);
    port.handle = 0xfff00000;
    CA_ASSERT(caHalDeviceRules::Close(&t, &port, guid) == deviceError::error_invalid_handle_port);
    port.handle = BASE_HANDLE + 1;
    CA_ASSERT(caHalDeviceRules::Close(&t, &port, guid) == deviceError::error_invalid_handle_port);

}

void caHalDeviceRules_test_class::test3(void)
{
    _START();
    _INFO("to check Write function of caHalDeviceRules");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();
    testDevice t;
    caDeviceHandle port;
    u32 guid = 0xfff00000;
    memset(&port, 0, sizeof (port));
    port.handle = 0xfff00000 + BASE_HANDLE + 1;
    t.setIsOpen(1);
    CA_ASSERT(caHalDeviceRules::Write(NULL, NULL, 0) == deviceError::error_invalid_null_device);
    CA_ASSERT(caHalDeviceRules::Write(&t, NULL, 0) == deviceError::error_invalid_null_port);
    CA_ASSERT(caHalDeviceRules::Write(&t, &port, guid) == deviceError::error_invalid_null_buffer_port);
    t.setIsOpen(0);
    CA_ASSERT(caHalDeviceRules::Write(&t, &port, guid) == deviceError::error_device_not_opened);
    t.setIsOpen(1);
    port.handle = 0xfff00000;
    CA_ASSERT(caHalDeviceRules::Write(&t, &port, guid) == deviceError::error_invalid_handle_port);
    port.handle = BASE_HANDLE + 1;
    CA_ASSERT(caHalDeviceRules::Write(&t, &port, guid) == deviceError::error_invalid_handle_port);
}

void caHalDeviceRules_test_class::test4(void)
{
    _START();
    _INFO("to check Read function of caHalDeviceRules");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();
    testDevice t;
    caDeviceHandle port;
    u32 guid = 0xfff00000;
    memset(&port, 0, sizeof (port));
    port.handle = 0xfff00000 + BASE_HANDLE + 1;
    t.setIsOpen(1);
    CA_ASSERT(caHalDeviceRules::Read(NULL, NULL, 0) == deviceError::error_invalid_null_device);
    CA_ASSERT(caHalDeviceRules::Read(&t, NULL, 0) == deviceError::error_invalid_null_port);

    CA_ASSERT(caHalDeviceRules::Read(&t, &port, guid) == deviceError::error_invalid_null_buffer_port);
    t.setIsOpen(0);
    CA_ASSERT(caHalDeviceRules::Read(&t, &port, guid) == deviceError::error_device_not_opened);
    t.setIsOpen(1);
    port.handle = 0xfff00000;
    CA_ASSERT(caHalDeviceRules::Read(&t, &port, guid) == deviceError::error_invalid_handle_port);
    port.handle = BASE_HANDLE + 1;
    CA_ASSERT(caHalDeviceRules::Read(&t, &port, guid) == deviceError::error_invalid_handle_port);
}

void caHalDeviceRules_test_class::test5(void)
{
    _START();
    _INFO("to check IoCtrl function of caHalDeviceRules");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();
    testDevice t;
    caDeviceHandle port;
    u32 guid = 0xfff00000;
    memset(&port, 0, sizeof (port));
    port.handle = 0xfff00000 + BASE_HANDLE + 1;
    caIDeviceCtrl inp;
    t.setIsOpen(1);
    CA_ASSERT(caHalDeviceRules::IoCtrl(NULL, NULL, NULL, 0) == deviceError::error_invalid_null_device);
    CA_ASSERT(caHalDeviceRules::IoCtrl(&t, NULL, NULL, 0) == deviceError::error_invalid_null_port);
    CA_ASSERT(caHalDeviceRules::IoCtrl(&t, &port, NULL, 0) == deviceError::error_device_config_param);

    CA_ASSERT(caHalDeviceRules::IoCtrl(&t, &port, &inp, guid) == deviceError::no_error);
    t.setIsOpen(0);
    CA_ASSERT(caHalDeviceRules::IoCtrl(&t, &port, &inp, guid) == deviceError::error_device_not_opened);
    t.setIsOpen(1);
    port.handle = 0xfff00000;
    CA_ASSERT(caHalDeviceRules::IoCtrl(&t, &port, &inp, guid) == deviceError::error_invalid_handle_port);
    port.handle = BASE_HANDLE + 1;
    CA_ASSERT(caHalDeviceRules::IoCtrl(&t, &port, &inp, guid) == deviceError::error_invalid_handle_port);

}

void caHalDeviceRules_test_class::test6(void)
{
    _START();
    _INFO("to check Flush function of caHalDeviceRules");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();
    testDevice t;
    caDeviceHandle port;
    u32 guid = 0xfff00000;
    memset(&port, 0, sizeof (port));
    port.handle = 0xfff00000 + BASE_HANDLE + 1;
    t.setIsOpen(1);
    CA_ASSERT(caHalDeviceRules::Flush(NULL, NULL, 0) == deviceError::error_invalid_null_device);
    CA_ASSERT(caHalDeviceRules::Flush(&t, NULL, 0) == deviceError::error_invalid_null_port);

    CA_ASSERT(caHalDeviceRules::Flush(&t, &port, guid) == deviceError::no_error);
    t.setIsOpen(0);
    CA_ASSERT(caHalDeviceRules::Flush(&t, &port, guid) == deviceError::error_device_not_opened);
    t.setIsOpen(1);
    port.handle = 0xfff00000;
    CA_ASSERT(caHalDeviceRules::Flush(&t, &port, guid) == deviceError::error_invalid_handle_port);
    port.handle = BASE_HANDLE + 1;
    CA_ASSERT(caHalDeviceRules::Flush(&t, &port, guid) == deviceError::error_invalid_handle_port);
}