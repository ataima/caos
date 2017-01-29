
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

class caTokenizeSStream_test_class
: public caTester {
    CA_TEST_SUITE(caTokenizeSStream_test_class);
    CA_TEST(caTokenizeSStream_test_class::test1, "ctor test");
    CA_TEST(caTokenizeSStream_test_class::test2, "Init test");
    CA_TEST(caTokenizeSStream_test_class::test3, "init from caStringStream test");
    CA_TEST(caTokenizeSStream_test_class::test4, ">> s8 test");
    CA_TEST(caTokenizeSStream_test_class::test5, ">> u8 test");
    CA_TEST(caTokenizeSStream_test_class::test6, ">> s16 test");
    CA_TEST(caTokenizeSStream_test_class::test7, ">> u16 test");
    CA_TEST(caTokenizeSStream_test_class::test8, ">> s32 test");
    CA_TEST(caTokenizeSStream_test_class::test9, ">> u32 test");
    CA_TEST(caTokenizeSStream_test_class::test16, " (hex) >> s16 test");
    CA_TEST(caTokenizeSStream_test_class::test17, " (hex) >> u16 test");
    CA_TEST(caTokenizeSStream_test_class::test18, " (hex) >> s32 test");
    CA_TEST(caTokenizeSStream_test_class::test19, " (hex) >> u32 test");
    CA_TEST(caTokenizeSStream_test_class::test26, " (bin) >> s16 test");
    CA_TEST(caTokenizeSStream_test_class::test27, " (bin) >> u16 test");
    CA_TEST(caTokenizeSStream_test_class::test28, " (bin) >> s32 test");
    CA_TEST(caTokenizeSStream_test_class::test29, " (bin) >> u32 test");
    CA_TEST(caTokenizeSStream_test_class::test30, " Foward ");
    CA_TEST_SUITE_END();

    void setUp(void) {
    }
    void test1(void);
    void test2(void);
    void test3(void);
    void test4(void);
    void test5(void);
    void test6(void);
    void test7(void);
    void test8(void);
    void test9(void);
    void test16(void);
    void test17(void);
    void test18(void);
    void test19(void);
    void test26(void);
    void test27(void);
    void test28(void);
    void test29(void);
    void test30(void);

    void tearDown(void) {
    }

};

REGISTER_CLASS(caTokenizeSStream_test_class);

void caTokenizeSStream_test_class::test1(void) {
    _START();
    _INFO("to check ctor of caTokenizeSStream");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();
    caTokenizeSStream<char> a;
    CA_ASSERT(a.Size() == 0);
    CA_ASSERT(a.Capacity() == 0);
    CA_ASSERT(a.Str() == NULL);
    CA_ASSERT(a.Good() == false);
}

void caTokenizeSStream_test_class::test2(void) {
    _START();
    _INFO("to check Init of caTokenizeSStream");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();
    char buff[100];
    caTokenizeSStream<char> a;
    CA_ASSERT(a.Size() == 0);
    CA_ASSERT(a.Capacity() == 0);
    CA_ASSERT(a.Str() == NULL);
    CA_ASSERT(a.Good() == false);
    a.Init(buff, 100, 0);
    CA_ASSERT(a.Size() == 0);
    CA_ASSERT(a.Capacity() == 99);
    CA_ASSERT(a.Str() == buff);
    CA_ASSERT(a.Good() == true);
    a.Clear();
    CA_ASSERT(a.Size() == 0);
    CA_ASSERT(a.Capacity() == 99);
    CA_ASSERT(a.Available() == 99);
    CA_ASSERT(a.Str() == buff);
    CA_ASSERT(a.Good() == true);
}

void caTokenizeSStream_test_class::test3(void) {
    _START();
    _INFO(" caTokenizeSStream: to check init from caStringStream ");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();
    char buff[100];
    caStringStream<char> a;
    a.Init(buff, 100);
    CA_ASSERT(a.Size() == 0);
    CA_ASSERT(a.Capacity() == 99);
    CA_ASSERT(a.Str() == buff);
    CA_ASSERT(a.Good() == true);
    caCSTR(t, "test");
    a << t;
    CA_ASSERT(a.Size() == 4);
    CA_ASSERT(a.Capacity() == 99);
    CA_ASSERT(a.Str() == buff);
    CA_ASSERT(a.Good() == true);
    CA_ASSERT(buff[0] == 't');
    caTokenizeSStream<char> b;
    CA_ASSERT(b.Empty() == true);
    b.Init(a);
    CA_ASSERT(b.Size() == 4);
    CA_ASSERT(b.Capacity() == 99);
    CA_ASSERT(b.Str() == buff);
    CA_ASSERT(b.Good() == true);
    CA_ASSERT(b.Empty() == false);
    b.Clear();
    CA_ASSERT(b.Size() == 0);
    CA_ASSERT(b.Capacity() == 99);
    CA_ASSERT(b.Available() == 99);
    CA_ASSERT(b.Str() == buff);
    CA_ASSERT(b.Good() == true);
    CA_ASSERT(b.Empty() == true);
}

void caTokenizeSStream_test_class::test4(void) {
    _START();
    _INFO(" caTokenizeSStream: to check operator >> s8 ");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();
    char buff[100];
    caStringStream<char> a;
    a.Init(buff, 100);
    CA_ASSERT(a.Size() == 0);
    CA_ASSERT(a.Capacity() == 99);
    CA_ASSERT(a.Str() == buff);
    CA_ASSERT(a.Good() == true);
    caCSTR(f, "test");
    a << f;
    CA_ASSERT(a.Size() == 4);
    CA_ASSERT(a.Capacity() == 99);
    CA_ASSERT(a.Str() == buff);
    CA_ASSERT(a.Good() == true);
    CA_ASSERT(buff[0] == 't');
    caTokenizeSStream<char> b;
    CA_ASSERT(b.Empty() == true);
    b.Init(a);
    CA_ASSERT(b.Size() == 4);
    CA_ASSERT(b.Capacity() == 99);
    CA_ASSERT(b.Str() == buff);
    CA_ASSERT(b.Good() == true);
    CA_ASSERT(b.Empty() == false);
    s8 t = 0;
    b>>t;
    CA_ASSERT(t == 't');
    CA_ASSERT(b.Size() == 4);
    CA_ASSERT(b.Remain() == 3);
    CA_ASSERT(b.Capacity() == 99);
    CA_ASSERT(b.Str() == &buff[1]);
    CA_ASSERT(b.Good() == true);
    CA_ASSERT(b.Empty() == false);
    b>>t;
    CA_ASSERT(t == 'e');
    CA_ASSERT(b.Size() == 4);
    CA_ASSERT(b.Remain() == 2);
    CA_ASSERT(b.Capacity() == 99);
    CA_ASSERT(b.Str() == &buff[2]);
    CA_ASSERT(b.Good() == true);
    CA_ASSERT(b.Empty() == false);
    b>>t;
    CA_ASSERT(t == 's');
    CA_ASSERT(b.Size() == 4);
    CA_ASSERT(b.Remain() == 1);
    CA_ASSERT(b.Capacity() == 99);
    CA_ASSERT(b.Str() == &buff[3]);
    CA_ASSERT(b.Good() == true);
    CA_ASSERT(b.Empty() == false);
    b>>t;
    CA_ASSERT(t == 't');
    CA_ASSERT(b.Size() == 4);
    CA_ASSERT(b.Remain() == 0);
    CA_ASSERT(b.Capacity() == 99);
    CA_ASSERT(b.Str() == &buff[4]);
    CA_ASSERT(b.Good() == true);
    CA_ASSERT(b.Empty() == true);
    b.Clear();
    CA_ASSERT(b.Size() == 0);
    CA_ASSERT(b.Capacity() == 99);
    CA_ASSERT(b.Available() == 99);
    CA_ASSERT(b.Str() == buff);
    CA_ASSERT(b.Good() == true);
    CA_ASSERT(b.Empty() == true);
}

void caTokenizeSStream_test_class::test5(void) {
    _START();
    _INFO(" caTokenizeSStream: to check operator >> u8 ");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();
    u8 buff[100] = {121, 242};
    caTokenizeSStream<s8> b;
    CA_ASSERT(b.Empty() == true);
    b.Init((s8 *) buff, 100, 2);
    CA_ASSERT(b.Size() == 2);
    CA_ASSERT(b.Capacity() == 99);
    CA_ASSERT(b.Str() == (s8*) buff);
    CA_ASSERT(b.Good() == true);
    CA_ASSERT(b.Empty() == false);
    u8 t = 0;
    b>>t;
    CA_ASSERT(t == 121);
    CA_ASSERT(b.Size() == 2);
    CA_ASSERT(b.Remain() == 1);
    CA_ASSERT(b.Capacity() == 99);
    CA_ASSERT(b.Str() == (s8*) & buff[1]);
    CA_ASSERT(b.Good() == true);
    CA_ASSERT(b.Empty() == false);
    b>>t;
    CA_ASSERT(t == 242);
    CA_ASSERT(b.Size() == 2);
    CA_ASSERT(b.Remain() == 0);
    CA_ASSERT(b.Capacity() == 99);
    CA_ASSERT(b.Str() == (s8*) & buff[2]);
    CA_ASSERT(b.Good() == true);
    CA_ASSERT(b.Empty() == true);
    b.Clear();
    CA_ASSERT(b.Size() == 0);
    CA_ASSERT(b.Capacity() == 99);
    CA_ASSERT(b.Available() == 99);
    CA_ASSERT(b.Str() == (s8*) buff);
    CA_ASSERT(b.Good() == true);
    CA_ASSERT(b.Empty() == true);
}

void caTokenizeSStream_test_class::test6(void) {
    _START();
    _INFO(" caTokenizeSStream: to check operator >> s16 ");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();
    char buff[100];
    caStringStream<char> a;
    a.Init(buff, 100);
    caCSTR(f, " 1234  -3245");
    a << f;
    caTokenizeSStream<s8> b;
    CA_ASSERT(b.Empty() == true);
    b.Init(a);
    CA_ASSERT(b.Size() == 12);
    CA_ASSERT(b.Capacity() == 99);
    CA_ASSERT(b.Str() == buff);
    CA_ASSERT(b.Good() == true);
    CA_ASSERT(b.Empty() == false);
    s16 t = 0;
    b>>t;
    CA_ASSERT(t == 1234);
    CA_ASSERT(b.Size() == 12);
    CA_ASSERT(b.Remain() == 5);
    CA_ASSERT(b.Capacity() == 99);
    CA_ASSERT(b.Str() == &buff[7]);
    CA_ASSERT(b.Good() == true);
    CA_ASSERT(b.Empty() == false);
    b>>t;
    CA_ASSERT(t == -3245);
    CA_ASSERT(b.Size() == 12);
    CA_ASSERT(b.Remain() == 0);
    CA_ASSERT(b.Capacity() == 99);
    CA_ASSERT(b.Str() == &buff[12]);
    CA_ASSERT(b.Good() == true);
    CA_ASSERT(b.Empty() == true);
    b.Clear();
    CA_ASSERT(b.Size() == 0);
    CA_ASSERT(b.Capacity() == 99);
    CA_ASSERT(b.Available() == 99);
    CA_ASSERT(b.Str() == buff);
    CA_ASSERT(b.Good() == true);
    CA_ASSERT(b.Empty() == true);
}

void caTokenizeSStream_test_class::test7(void) {
    _START();
    _INFO(" caTokenizeSStream: to check operator >> u16 ");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();
    char buff[100];
    caStringStream<char> a;
    a.Init(buff, 100);
    caCSTR(f, " 1234  47325");
    a << f;
    caTokenizeSStream<s8> b;
    CA_ASSERT(b.Empty() == true);
    b.Init(a);
    CA_ASSERT(b.Size() == 12);
    CA_ASSERT(b.Capacity() == 99);
    CA_ASSERT(b.Str() == buff);
    CA_ASSERT(b.Good() == true);
    CA_ASSERT(b.Empty() == false);
    u16 t = 0;
    b>>t;
    CA_ASSERT(t == 1234);
    CA_ASSERT(b.Size() == 12);
    CA_ASSERT(b.Remain() == 5);
    CA_ASSERT(b.Capacity() == 99);
    CA_ASSERT(b.Str() == &buff[7]);
    CA_ASSERT(b.Good() == true);
    CA_ASSERT(b.Empty() == false);
    b>>t;
    CA_ASSERT(t == 47325);
    CA_ASSERT(b.Size() == 12);
    CA_ASSERT(b.Remain() == 0);
    CA_ASSERT(b.Capacity() == 99);
    CA_ASSERT(b.Str() == &buff[12]);
    CA_ASSERT(b.Good() == true);
    CA_ASSERT(b.Empty() == true);
    b.Clear();
    CA_ASSERT(b.Size() == 0);
    CA_ASSERT(b.Capacity() == 99);
    CA_ASSERT(b.Available() == 99);
    CA_ASSERT(b.Str() == buff);
    CA_ASSERT(b.Good() == true);
    CA_ASSERT(b.Empty() == true);
}

void caTokenizeSStream_test_class::test8(void) {
    _START();
    _INFO(" caTokenizeSStream: to check operator >> s32 ");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();
    char buff[100];
    caStringStream<char> a;
    a.Init(buff, 100);
    caCSTR(f, " 1234  -3245");
    a << f;
    caTokenizeSStream<s8> b;
    CA_ASSERT(b.Empty() == true);
    b.Init(a);
    CA_ASSERT(b.Size() == 12);
    CA_ASSERT(b.Capacity() == 99);
    CA_ASSERT(b.Str() == buff);
    CA_ASSERT(b.Good() == true);
    CA_ASSERT(b.Empty() == false);
    s32 t = 0;
    b>>t;
    CA_ASSERT(t == 1234);
    CA_ASSERT(b.Size() == 12);
    CA_ASSERT(b.Remain() == 5);
    CA_ASSERT(b.Capacity() == 99);
    CA_ASSERT(b.Str() == &buff[7]);
    CA_ASSERT(b.Good() == true);
    CA_ASSERT(b.Empty() == false);
    b>>t;
    CA_ASSERT(t == -3245);
    CA_ASSERT(b.Size() == 12);
    CA_ASSERT(b.Remain() == 0);
    CA_ASSERT(b.Capacity() == 99);
    CA_ASSERT(b.Str() == &buff[12]);
    CA_ASSERT(b.Good() == true);
    CA_ASSERT(b.Empty() == true);
    b.Clear();
    CA_ASSERT(b.Size() == 0);
    CA_ASSERT(b.Capacity() == 99);
    CA_ASSERT(b.Available() == 99);
    CA_ASSERT(b.Str() == buff);
    CA_ASSERT(b.Good() == true);
    CA_ASSERT(b.Empty() == true);
}

void caTokenizeSStream_test_class::test9(void) {
    _START();
    _INFO(" caTokenizeSStream: to check operator >> u32 ");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();
    char buff[100];
    caStringStream<char> a;
    a.Init(buff, 100);
    caCSTR(f, " 1234  473253456");
    a << f;
    caTokenizeSStream<s8> b;
    CA_ASSERT(b.Empty() == true);
    b.Init(a);
    CA_ASSERT(b.Size() == 16);
    CA_ASSERT(b.Capacity() == 99);
    CA_ASSERT(b.Str() == buff);
    CA_ASSERT(b.Good() == true);
    CA_ASSERT(b.Empty() == false);
    u32 t = 0;
    b>>t;
    CA_ASSERT(t == 1234);
    CA_ASSERT(b.Size() == 16);
    CA_ASSERT(b.Remain() == 9);
    CA_ASSERT(b.Capacity() == 99);
    CA_ASSERT(b.Str() == &buff[7]);
    CA_ASSERT(b.Good() == true);
    CA_ASSERT(b.Empty() == false);
    b>>t;
    CA_ASSERT(t == 473253456);
    CA_ASSERT(b.Size() == 16);
    CA_ASSERT(b.Remain() == 0);
    CA_ASSERT(b.Capacity() == 99);
    CA_ASSERT(b.Str() == &buff[16]);
    CA_ASSERT(b.Good() == true);
    CA_ASSERT(b.Empty() == true);
    b.Clear();
    CA_ASSERT(b.Size() == 0);
    CA_ASSERT(b.Capacity() == 99);
    CA_ASSERT(b.Available() == 99);
    CA_ASSERT(b.Str() == buff);
    CA_ASSERT(b.Good() == true);
    CA_ASSERT(b.Empty() == true);
}

//////////////  HEX    

void caTokenizeSStream_test_class::test16(void) {
    _START();
    _INFO(" caTokenizeSStream: to check operator( hex ) >> s16 ");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();
    char buff[100];
    caStringStream<char> a;
    a.Init(buff, 100);
    caCSTR(f, " 0x1234  -3245");
    a << f;
    caTokenizeSStream<s8> b;
    CA_ASSERT(b.Empty() == true);
    b.Init(a);
    CA_ASSERT(b.Size() == 14);
    CA_ASSERT(b.Capacity() == 99);
    CA_ASSERT(b.Str() == buff);
    CA_ASSERT(b.Good() == true);
    CA_ASSERT(b.Empty() == false);
    s16 t = 0;
    b>>t;
    CA_ASSERT(t == 0x1234);
    CA_ASSERT(b.Size() == 14);
    CA_ASSERT(b.Remain() == 5);
    CA_ASSERT(b.Capacity() == 99);
    CA_ASSERT(b.Str() == &buff[9]);
    CA_ASSERT(b.Good() == true);
    CA_ASSERT(b.Empty() == false);
    b>>t;
    CA_ASSERT(t == -3245);
    CA_ASSERT(b.Size() == 14);
    CA_ASSERT(b.Remain() == 0);
    CA_ASSERT(b.Capacity() == 99);
    CA_ASSERT(b.Str() == &buff[14]);
    CA_ASSERT(b.Good() == true);
    CA_ASSERT(b.Empty() == true);
    b.Clear();
    CA_ASSERT(b.Size() == 0);
    CA_ASSERT(b.Capacity() == 99);
    CA_ASSERT(b.Available() == 99);
    CA_ASSERT(b.Str() == buff);
    CA_ASSERT(b.Good() == true);
    CA_ASSERT(b.Empty() == true);
}

void caTokenizeSStream_test_class::test17(void) {
    _START();
    _INFO(" caTokenizeSStream: to check operator ( hex ) >> u16 ");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();
    char buff[100];
    caStringStream<char> a;
    a.Init(buff, 100);
    caCSTR(f, " 0x1234  0xfbca");
    a << f;
    caTokenizeSStream<s8> b;
    CA_ASSERT(b.Empty() == true);
    b.Init(a);
    CA_ASSERT(b.Size() == 15);
    CA_ASSERT(b.Capacity() == 99);
    CA_ASSERT(b.Str() == buff);
    CA_ASSERT(b.Good() == true);
    CA_ASSERT(b.Empty() == false);
    u16 t = 0;
    b>>t;
    CA_ASSERT(t == 0x1234);
    CA_ASSERT(b.Size() == 15);
    CA_ASSERT(b.Remain() == 6);
    CA_ASSERT(b.Capacity() == 99);
    CA_ASSERT(b.Str() == &buff[9]);
    CA_ASSERT(b.Good() == true);
    CA_ASSERT(b.Empty() == false);
    b>>t;
    CA_ASSERT(t == 0xfbca);
    CA_ASSERT(b.Size() == 15);
    CA_ASSERT(b.Remain() == 0);
    CA_ASSERT(b.Capacity() == 99);
    CA_ASSERT(b.Str() == &buff[15]);
    CA_ASSERT(b.Good() == true);
    CA_ASSERT(b.Empty() == true);
    b.Clear();
    CA_ASSERT(b.Size() == 0);
    CA_ASSERT(b.Capacity() == 99);
    CA_ASSERT(b.Available() == 99);
    CA_ASSERT(b.Str() == buff);
    CA_ASSERT(b.Good() == true);
    CA_ASSERT(b.Empty() == true);
}

void caTokenizeSStream_test_class::test18(void) {
    _START();
    _INFO(" caTokenizeSStream: to check operator ( hex ) >> s32 ");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();
    char buff[100];
    caStringStream<char> a;
    a.Init(buff, 100);
    caCSTR(f, " 0x1234  -3245");
    a << f;
    caTokenizeSStream<s8> b;
    CA_ASSERT(b.Empty() == true);
    b.Init(a);
    CA_ASSERT(b.Size() == 14);
    CA_ASSERT(b.Capacity() == 99);
    CA_ASSERT(b.Str() == buff);
    CA_ASSERT(b.Good() == true);
    CA_ASSERT(b.Empty() == false);
    s32 t = 0;
    b>>t;
    CA_ASSERT(t == 0x1234);
    CA_ASSERT(b.Size() == 14);
    CA_ASSERT(b.Remain() == 5);
    CA_ASSERT(b.Capacity() == 99);
    CA_ASSERT(b.Str() == &buff[9]);
    CA_ASSERT(b.Good() == true);
    CA_ASSERT(b.Empty() == false);
    b>>t;
    CA_ASSERT(t == -3245);
    CA_ASSERT(b.Size() == 14);
    CA_ASSERT(b.Remain() == 0);
    CA_ASSERT(b.Capacity() == 99);
    CA_ASSERT(b.Str() == &buff[14]);
    CA_ASSERT(b.Good() == true);
    CA_ASSERT(b.Empty() == true);
    b.Clear();
    CA_ASSERT(b.Size() == 0);
    CA_ASSERT(b.Capacity() == 99);
    CA_ASSERT(b.Available() == 99);
    CA_ASSERT(b.Str() == buff);
    CA_ASSERT(b.Good() == true);
    CA_ASSERT(b.Empty() == true);
}

void caTokenizeSStream_test_class::test19(void) {
    _START();
    _INFO(" caTokenizeSStream: to check operator ( hex ) >> u32 ");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();
    char buff[100];
    caStringStream<char> a;
    a.Init(buff, 100);
    caCSTR(f, " 0x12345678  0xfacacaaa");
    a << f;
    caTokenizeSStream<s8> b;
    CA_ASSERT(b.Empty() == true);
    b.Init(a);
    CA_ASSERT(b.Size() == 23);
    CA_ASSERT(b.Capacity() == 99);
    CA_ASSERT(b.Str() == buff);
    CA_ASSERT(b.Good() == true);
    CA_ASSERT(b.Empty() == false);
    u32 t = 0;
    b>>t;
    CA_ASSERT(t == 0x12345678);
    CA_ASSERT(b.Size() == 23);
    CA_ASSERT(b.Remain() == 10);
    CA_ASSERT(b.Capacity() == 99);
    CA_ASSERT(b.Str() == &buff[13]);
    CA_ASSERT(b.Good() == true);
    CA_ASSERT(b.Empty() == false);
    b>>t;
    CA_ASSERT(t == 0xfacacaaa);
    CA_ASSERT(b.Size() == 23);
    CA_ASSERT(b.Remain() == 0);
    CA_ASSERT(b.Capacity() == 99);
    CA_ASSERT(b.Str() == &buff[23]);
    CA_ASSERT(b.Good() == true);
    CA_ASSERT(b.Empty() == true);
    b.Clear();
    CA_ASSERT(b.Size() == 0);
    CA_ASSERT(b.Capacity() == 99);
    CA_ASSERT(b.Available() == 99);
    CA_ASSERT(b.Str() == buff);
    CA_ASSERT(b.Good() == true);
    CA_ASSERT(b.Empty() == true);
}

//////////////  BIN     

void caTokenizeSStream_test_class::test26(void) {
    _START();
    _INFO(" caTokenizeSStream: to check operator( bin ) >> s16 ");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();
    char buff[100];
    caStringStream<char> a;
    a.Init(buff, 100);
    caCSTR(f, " 0b1010  0b1110");
    a << f;
    caTokenizeSStream<s8> b;
    CA_ASSERT(b.Empty() == true);
    b.Init(a);
    CA_ASSERT(b.Size() == 15);
    CA_ASSERT(b.Capacity() == 99);
    CA_ASSERT(b.Str() == buff);
    CA_ASSERT(b.Good() == true);
    CA_ASSERT(b.Empty() == false);
    s16 t = 0;
    b>>t;
    CA_ASSERT(t == 10);
    CA_ASSERT(b.Size() == 15);
    CA_ASSERT(b.Remain() == 6);
    CA_ASSERT(b.Capacity() == 99);
    CA_ASSERT(b.Str() == &buff[9]);
    CA_ASSERT(b.Good() == true);
    CA_ASSERT(b.Empty() == false);
    b>>t;
    CA_ASSERT(t == 14);
    CA_ASSERT(b.Size() == 15);
    CA_ASSERT(b.Remain() == 0);
    CA_ASSERT(b.Capacity() == 99);
    CA_ASSERT(b.Str() == &buff[15]);
    CA_ASSERT(b.Good() == true);
    CA_ASSERT(b.Empty() == true);
    b.Clear();
    CA_ASSERT(b.Size() == 0);
    CA_ASSERT(b.Capacity() == 99);
    CA_ASSERT(b.Available() == 99);
    CA_ASSERT(b.Str() == buff);
    CA_ASSERT(b.Good() == true);
    CA_ASSERT(b.Empty() == true);
}

void caTokenizeSStream_test_class::test27(void) {
    _START();
    _INFO(" caTokenizeSStream: to check operator ( bin ) >> u16 ");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();
    char buff[100];
    caStringStream<char> a;
    a.Init(buff, 100);
    caCSTR(f, " 0b1010  0b1110");
    a << f;
    caTokenizeSStream<s8> b;
    CA_ASSERT(b.Empty() == true);
    b.Init(a);
    CA_ASSERT(b.Size() == 15);
    CA_ASSERT(b.Capacity() == 99);
    CA_ASSERT(b.Str() == buff);
    CA_ASSERT(b.Good() == true);
    CA_ASSERT(b.Empty() == false);
    u16 t = 0;
    b>>t;
    CA_ASSERT(t == 10);
    CA_ASSERT(b.Size() == 15);
    CA_ASSERT(b.Remain() == 6);
    CA_ASSERT(b.Capacity() == 99);
    CA_ASSERT(b.Str() == &buff[9]);
    CA_ASSERT(b.Good() == true);
    CA_ASSERT(b.Empty() == false);
    b>>t;
    CA_ASSERT(t == 14);
    CA_ASSERT(b.Size() == 15);
    CA_ASSERT(b.Remain() == 0);
    CA_ASSERT(b.Capacity() == 99);
    CA_ASSERT(b.Str() == &buff[15]);
    CA_ASSERT(b.Good() == true);
    CA_ASSERT(b.Empty() == true);
    b.Clear();
    CA_ASSERT(b.Size() == 0);
    CA_ASSERT(b.Capacity() == 99);
    CA_ASSERT(b.Available() == 99);
    CA_ASSERT(b.Str() == buff);
    CA_ASSERT(b.Good() == true);
    CA_ASSERT(b.Empty() == true);
}

void caTokenizeSStream_test_class::test28(void) {
    _START();
    _INFO(" caTokenizeSStream: to check operator ( bin ) >> s32 ");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();
    char buff[100];
    caStringStream<char> a;
    a.Init(buff, 100);
    caCSTR(f, " 0b1010  0b1110");
    a << f;
    caTokenizeSStream<s8> b;
    CA_ASSERT(b.Empty() == true);
    b.Init(a);
    CA_ASSERT(b.Size() == 15);
    CA_ASSERT(b.Capacity() == 99);
    CA_ASSERT(b.Str() == buff);
    CA_ASSERT(b.Good() == true);
    CA_ASSERT(b.Empty() == false);
    s32 t = 0;
    b>>t;
    CA_ASSERT(t == 10);
    CA_ASSERT(b.Size() == 15);
    CA_ASSERT(b.Remain() == 6);
    CA_ASSERT(b.Capacity() == 99);
    CA_ASSERT(b.Str() == &buff[9]);
    CA_ASSERT(b.Good() == true);
    CA_ASSERT(b.Empty() == false);
    b>>t;
    CA_ASSERT(t == 14);
    CA_ASSERT(b.Size() == 15);
    CA_ASSERT(b.Remain() == 0);
    CA_ASSERT(b.Capacity() == 99);
    CA_ASSERT(b.Str() == &buff[15]);
    CA_ASSERT(b.Good() == true);
    CA_ASSERT(b.Empty() == true);
    b.Clear();
    CA_ASSERT(b.Size() == 0);
    CA_ASSERT(b.Capacity() == 99);
    CA_ASSERT(b.Available() == 99);
    CA_ASSERT(b.Str() == buff);
    CA_ASSERT(b.Good() == true);
    CA_ASSERT(b.Empty() == true);
}

void caTokenizeSStream_test_class::test29(void) {
    _START();
    _INFO(" caTokenizeSStream: to check operator ( bin ) >> u32 ");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();
    char buff[100];
    caStringStream<char> a;
    a.Init(buff, 100);
    caCSTR(f, " 0b1010  0b1110");
    a << f;
    caTokenizeSStream<s8> b;
    CA_ASSERT(b.Empty() == true);
    b.Init(a);
    CA_ASSERT(b.Size() == 15);
    CA_ASSERT(b.Capacity() == 99);
    CA_ASSERT(b.Str() == buff);
    CA_ASSERT(b.Good() == true);
    CA_ASSERT(b.Empty() == false);
    u32 t = 0;
    b>>t;
    CA_ASSERT(t == 10);
    CA_ASSERT(b.Size() == 15);
    CA_ASSERT(b.Remain() == 6);
    CA_ASSERT(b.Capacity() == 99);
    CA_ASSERT(b.Str() == &buff[9]);
    CA_ASSERT(b.Good() == true);
    CA_ASSERT(b.Empty() == false);
    b>>t;
    CA_ASSERT(t == 14);
    CA_ASSERT(b.Size() == 15);
    CA_ASSERT(b.Remain() == 0);
    CA_ASSERT(b.Capacity() == 99);
    CA_ASSERT(b.Str() == &buff[15]);
    CA_ASSERT(b.Good() == true);
    CA_ASSERT(b.Empty() == true);
    b.Clear();
    CA_ASSERT(b.Size() == 0);
    CA_ASSERT(b.Capacity() == 99);
    CA_ASSERT(b.Available() == 99);
    CA_ASSERT(b.Str() == buff);
    CA_ASSERT(b.Good() == true);
    CA_ASSERT(b.Empty() == true);
}

void caTokenizeSStream_test_class::test30(void) {
    _START();
    _INFO(" caTokenizeSStream: to check Foward method ");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();
    caTokenizeSStream<s8> iss;
    caStringStream<s8> tt;
    s8 buft[1024];
    tt.Init(buft, 1024);
    iss.Init(tt);
    CA_ASSERT(iss.Size() == 0);
    CA_ASSERT(iss.Capacity() == 1023);
    CA_ASSERT(iss.Empty() == true);
    CA_ASSERT(iss.Str() == buft);
    tt << "pippo pluto paperino";
    iss.Forward(tt.Size());
    CA_ASSERT(iss.Size() == tt.Size());
    TokenString<s8> res;
    iss >> res;
    CA_ASSERT(strncmp(res.ptr, "pippo", 5) == 0);
    CA_ASSERT(res.size == 5);
    iss >> res;
    CA_ASSERT(strncmp(res.ptr, "pluto", 5) == 0);
    CA_ASSERT(res.size == 5);
    iss >> res;
    CA_ASSERT(strncmp(res.ptr, "paperino", 8) == 0);
    CA_ASSERT(res.size == 8);
    iss >> res;
    CA_ASSERT(res.ptr == NULL);
    CA_ASSERT(res.size == 0);
}