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
#include "arm_c_types.h"
#include "idevice.h"
#include "stream.h"
#include <iostream>


caCSTR (caEnd::endl,"\r\n");

class caStringStream_test_class
: public caTester {
    CA_TEST_SUITE(caStringStream_test_class);
    CA_TEST(caStringStream_test_class::test1, "ctor test");
    CA_TEST(caStringStream_test_class::test2, "Init test");
    CA_TEST(caStringStream_test_class::test3, "<< s8 test");
    CA_TEST(caStringStream_test_class::test4, "<< u8 test");
    CA_TEST(caStringStream_test_class::test5, "<< s16 test");
    CA_TEST(caStringStream_test_class::test6, "<< s16 (-) test");
    CA_TEST(caStringStream_test_class::test7, "<< u16 test");
    CA_TEST(caStringStream_test_class::test8, "<< s32 test");
    CA_TEST(caStringStream_test_class::test9, "<< s32 (-) test");
    CA_TEST(caStringStream_test_class::test10, "<< u32  test");
    CA_TEST(caStringStream_test_class::test11, "<< caStringArray  test");
    CA_TEST(caStringStream_test_class::test12, "<< const char *  test");
    CA_TEST(caStringStream_test_class::test12bis, "<< caCSTR  test");
    CA_TEST(caStringStream_test_class::test13, "<< *s8 test");
    CA_TEST(caStringStream_test_class::test14, "<< *u8 test");
    CA_TEST(caStringStream_test_class::test15, "<< *s16 test");
    CA_TEST(caStringStream_test_class::test16, "<< *u16 test");
    CA_TEST(caStringStream_test_class::test17, "<< *s32 test");
    CA_TEST(caStringStream_test_class::test18, "<< *u32 test");
    CA_TEST(caStringStream_test_class::test19, "<< *u32 test");
    CA_TEST(caStringStream_test_class::test20, "filler test");
    CA_TEST(caStringStream_test_class::test21, "terminator test");
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
    void test10(void);
    void test11(void);
    void test12(void);
    void test12bis(void);
    void test13(void);
    void test14(void);
    void test15(void);
    void test16(void);
    void test17(void);
    void test18(void);
    void test19(void);
    void test20(void);
    void test21(void);

    void tearDown(void) {
    }

};

REGISTER_CLASS(caStringStream_test_class);

void caStringStream_test_class::test1(void) {
    _START();
    _INFO("to check ctor of caStringStream");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();
    caStringStream<char> a;
    CA_ASSERT(a.Size() == 0);
    CA_ASSERT(a.Capacity() == 0);
    CA_ASSERT(a.Str() == NULL);
    CA_ASSERT(a.Good() == false);
}

void caStringStream_test_class::test2(void) {
    _START();
    _INFO("to check Init of caStringStream");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();
    char buff[100];
    caStringStream<char> a;
    CA_ASSERT(a.Size() == 0);
    CA_ASSERT(a.Capacity() == 0);
    CA_ASSERT(a.Str() == NULL);
    CA_ASSERT(a.Good() == false);
    CA_ASSERT(a.Empty() == true);

    a.Init(buff, 100);
    CA_ASSERT(a.Size() == 0);
    CA_ASSERT(a.Capacity() == 99);
    CA_ASSERT(a.Str() == buff);
    CA_ASSERT(a.Good() == true);
    CA_ASSERT(a.Empty() == true);
}

void caStringStream_test_class::test3(void) {
    _START();
    _INFO("to check operator << s8 of caStringStream");
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
    CA_ASSERT(a.Empty() == true);
    a << 't';
    CA_ASSERT(a.Size() == 1);
    CA_ASSERT(a.Capacity() == 99);
    CA_ASSERT(a.Str() == buff);
    CA_ASSERT(a.Good() == true);
    CA_ASSERT(buff[0] == 't');
    a.Clear();
    CA_ASSERT(a.Size() == 0);
    CA_ASSERT(a.Capacity() == 99);
    CA_ASSERT(a.Str() == buff);
    CA_ASSERT(a.Good() == true);
    CA_ASSERT(a.Empty() == true);
}

void caStringStream_test_class::test4(void) {
    _START();
    _INFO("to check operator << u8 of caStringStream");
    _INFO("Test out on format dec,hex,bin");
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
    CA_ASSERT(a.Empty() == true);
    u8 t = 126;
    a << caStringFormat::dec;
    a << t;
    CA_ASSERT(a.Size() == 3);
    CA_ASSERT(a.Capacity() == 99);
    CA_ASSERT(a.Available() == 96);
    CA_ASSERT(a.Str() == buff);
    CA_ASSERT(a.Good() == true);
    CA_ASSERT(buff[0] == '1');
    CA_ASSERT(buff[1] == '2');
    CA_ASSERT(buff[2] == '6');
    a.Clear();
    CA_ASSERT(a.Size() == 0);
    CA_ASSERT(a.Capacity() == 99);
    CA_ASSERT(a.Available() == 99);
    CA_ASSERT(a.Str() == buff);
    CA_ASSERT(a.Good() == true);
    a << caStringFormat::bin;
    a << t;
    CA_ASSERT(a.Size() == 10);
    CA_ASSERT(a.Capacity() == 99);
    CA_ASSERT(a.Available() == 89);
    CA_ASSERT(a.Str() == buff);
    CA_ASSERT(a.Good() == true);
    CA_ASSERT(buff[0] == '0');
    CA_ASSERT(buff[1] == 'b');
    CA_ASSERT(buff[2] == '0');
    CA_ASSERT(buff[3] == '1');
    CA_ASSERT(buff[4] == '1');
    CA_ASSERT(buff[5] == '1');
    CA_ASSERT(buff[6] == '1');
    CA_ASSERT(buff[7] == '1');
    CA_ASSERT(buff[8] == '1');
    CA_ASSERT(buff[9] == '0');
    a.Clear();
    a << caStringFormat::hex;
    a << t;
    CA_ASSERT(a.Size() == 4);
    CA_ASSERT(a.Capacity() == 99);
    CA_ASSERT(a.Available() == 95);
    CA_ASSERT(a.Str() == buff);
    CA_ASSERT(a.Good() == true);
    CA_ASSERT(buff[0] == '0');
    CA_ASSERT(buff[1] == 'x');
    CA_ASSERT(buff[2] == '7');
    CA_ASSERT(buff[3] == 'E');
    a.Clear();
}

void caStringStream_test_class::test5(void) {
    _START();
    _INFO("to check operator << s16 of caStringStream");
    _INFO("Test out on format dec,hex,bin");
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
    CA_ASSERT(a.Empty() == true);
    s16 t = 12345; //0x3039
    a << caStringFormat::dec;
    a << t;
    CA_ASSERT(a.Size() == 5);
    CA_ASSERT(a.Capacity() == 99);
    CA_ASSERT(a.Available() == 94);
    CA_ASSERT(a.Str() == buff);
    CA_ASSERT(a.Good() == true);
    CA_ASSERT(buff[0] == '1');
    CA_ASSERT(buff[1] == '2');
    CA_ASSERT(buff[2] == '3');
    CA_ASSERT(buff[3] == '4');
    CA_ASSERT(buff[4] == '5');
    a.Clear();
    CA_ASSERT(a.Size() == 0);
    CA_ASSERT(a.Capacity() == 99);
    CA_ASSERT(a.Available() == 99);
    CA_ASSERT(a.Str() == buff);
    CA_ASSERT(a.Good() == true);
    a << caStringFormat::bin;
    a << t;
    CA_ASSERT(a.Size() == 18);
    CA_ASSERT(a.Capacity() == 99);
    CA_ASSERT(a.Available() == 81);
    CA_ASSERT(a.Str() == buff);
    CA_ASSERT(a.Good() == true);
    CA_ASSERT(buff[0] == '0');
    CA_ASSERT(buff[1] == 'b');
    CA_ASSERT(buff[2] == '0');
    CA_ASSERT(buff[3] == '0');
    CA_ASSERT(buff[4] == '1');
    CA_ASSERT(buff[5] == '1');
    CA_ASSERT(buff[6] == '0');
    CA_ASSERT(buff[7] == '0');
    CA_ASSERT(buff[8] == '0');
    CA_ASSERT(buff[9] == '0');
    CA_ASSERT(buff[10] == '0');
    CA_ASSERT(buff[11] == '0');
    CA_ASSERT(buff[12] == '1');
    CA_ASSERT(buff[13] == '1');
    CA_ASSERT(buff[14] == '1');
    CA_ASSERT(buff[15] == '0');
    CA_ASSERT(buff[16] == '0');
    CA_ASSERT(buff[17] == '1');
    a.Clear();
    a << caStringFormat::hex;
    a << t;
    CA_ASSERT(a.Size() == 6);
    CA_ASSERT(a.Capacity() == 99);
    CA_ASSERT(a.Available() == 93);
    CA_ASSERT(a.Str() == buff);
    CA_ASSERT(a.Good() == true);
    CA_ASSERT(buff[0] == '0');
    CA_ASSERT(buff[1] == 'x');
    CA_ASSERT(buff[2] == '3');
    CA_ASSERT(buff[3] == '0');
    CA_ASSERT(buff[4] == '3');
    CA_ASSERT(buff[5] == '9');
    a.Clear();
}

void caStringStream_test_class::test6(void) {
    _START();
    _INFO("to check operator << s16 of caStringStream");
    _INFO("with s16 a negative number");
    _INFO("Test out on format dec,hex,bin");
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
    s16 t = -12345; //0xcfc7
    a << caStringFormat::dec;
    a << t;
    CA_ASSERT(a.Size() == 6);
    CA_ASSERT(a.Capacity() == 99);
    CA_ASSERT(a.Available() == 93);
    CA_ASSERT(a.Str() == buff);
    CA_ASSERT(a.Good() == true);
    CA_ASSERT(buff[0] == '-');
    CA_ASSERT(buff[1] == '1');
    CA_ASSERT(buff[2] == '2');
    CA_ASSERT(buff[3] == '3');
    CA_ASSERT(buff[4] == '4');
    CA_ASSERT(buff[5] == '5');
    a.Clear();
    CA_ASSERT(a.Size() == 0);
    CA_ASSERT(a.Capacity() == 99);
    CA_ASSERT(a.Available() == 99);
    CA_ASSERT(a.Str() == buff);
    CA_ASSERT(a.Good() == true);
    a << caStringFormat::bin;
    a << t;
    CA_ASSERT(a.Size() == 18);
    CA_ASSERT(a.Capacity() == 99);
    CA_ASSERT(a.Available() == 81);
    CA_ASSERT(a.Str() == buff);
    CA_ASSERT(a.Good() == true);
    CA_ASSERT(buff[0] == '0');
    CA_ASSERT(buff[1] == 'b');
    CA_ASSERT(buff[2] == '1');
    CA_ASSERT(buff[3] == '1');
    CA_ASSERT(buff[4] == '0');
    CA_ASSERT(buff[5] == '0');
    CA_ASSERT(buff[6] == '1');
    CA_ASSERT(buff[7] == '1');
    CA_ASSERT(buff[8] == '1');
    CA_ASSERT(buff[9] == '1');
    CA_ASSERT(buff[10] == '1');
    CA_ASSERT(buff[11] == '1');
    CA_ASSERT(buff[12] == '0');
    CA_ASSERT(buff[13] == '0');
    CA_ASSERT(buff[14] == '0');
    CA_ASSERT(buff[15] == '1');
    CA_ASSERT(buff[16] == '1');
    CA_ASSERT(buff[17] == '1');
    a.Clear();
    a << caStringFormat::hex;
    a << t;
    CA_ASSERT(a.Size() == 6);
    CA_ASSERT(a.Capacity() == 99);
    CA_ASSERT(a.Available() == 93);
    CA_ASSERT(a.Str() == buff);
    CA_ASSERT(a.Good() == true);
    CA_ASSERT(buff[0] == '0');
    CA_ASSERT(buff[1] == 'x');
    CA_ASSERT(buff[2] == 'C');
    CA_ASSERT(buff[3] == 'F');
    CA_ASSERT(buff[4] == 'C');
    CA_ASSERT(buff[5] == '7');
    a.Clear();
}

void caStringStream_test_class::test7(void) {
    _START();
    _INFO("to check operator << u16 of caStringStream");
    _INFO("Test out on format dec,hex,bin");
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
    u16 t = 0x9123; //37155
    a << caStringFormat::dec;
    a << t;
    CA_ASSERT(a.Size() == 5);
    CA_ASSERT(a.Capacity() == 99);
    CA_ASSERT(a.Available() == 94);
    CA_ASSERT(a.Str() == buff);
    CA_ASSERT(a.Good() == true);
    CA_ASSERT(buff[0] == '3');
    CA_ASSERT(buff[1] == '7');
    CA_ASSERT(buff[2] == '1');
    CA_ASSERT(buff[3] == '5');
    CA_ASSERT(buff[4] == '5');
    a.Clear();
    CA_ASSERT(a.Size() == 0);
    CA_ASSERT(a.Capacity() == 99);
    CA_ASSERT(a.Available() == 99);
    CA_ASSERT(a.Str() == buff);
    CA_ASSERT(a.Good() == true);
    a << caStringFormat::bin;
    a << t;
    CA_ASSERT(a.Size() == 18);
    CA_ASSERT(a.Capacity() == 99);
    CA_ASSERT(a.Available() == 81);
    CA_ASSERT(a.Str() == buff);
    CA_ASSERT(a.Good() == true);
    CA_ASSERT(buff[0] == '0');
    CA_ASSERT(buff[1] == 'b');
    CA_ASSERT(buff[2] == '1');
    CA_ASSERT(buff[3] == '0');
    CA_ASSERT(buff[4] == '0');
    CA_ASSERT(buff[5] == '1');
    CA_ASSERT(buff[6] == '0');
    CA_ASSERT(buff[7] == '0');
    CA_ASSERT(buff[8] == '0');
    CA_ASSERT(buff[9] == '1');
    CA_ASSERT(buff[10] == '0');
    CA_ASSERT(buff[11] == '0');
    CA_ASSERT(buff[12] == '1');
    CA_ASSERT(buff[13] == '0');
    CA_ASSERT(buff[14] == '0');
    CA_ASSERT(buff[15] == '0');
    CA_ASSERT(buff[16] == '1');
    CA_ASSERT(buff[17] == '1');
    a.Clear();
    a << caStringFormat::hex;
    a << t;
    CA_ASSERT(a.Size() == 6);
    CA_ASSERT(a.Capacity() == 99);
    CA_ASSERT(a.Available() == 93);
    CA_ASSERT(a.Str() == buff);
    CA_ASSERT(a.Good() == true);
    CA_ASSERT(buff[0] == '0');
    CA_ASSERT(buff[1] == 'x');
    CA_ASSERT(buff[2] == '9');
    CA_ASSERT(buff[3] == '1');
    CA_ASSERT(buff[4] == '2');
    CA_ASSERT(buff[5] == '3');
    a.Clear();
}

void caStringStream_test_class::test8(void) {
    _START();
    _INFO("to check operator << s32 of caStringStream");
    _INFO("Test out on format dec,hex,bin");
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
    s32 t = 1234567890; //0x499602D2
    a << caStringFormat::dec;
    a << t;
    CA_ASSERT(a.Size() == 10);
    CA_ASSERT(a.Capacity() == 99);
    CA_ASSERT(a.Available() == 89);
    CA_ASSERT(a.Str() == buff);
    CA_ASSERT(a.Good() == true);
    CA_ASSERT(buff[0] == '1');
    CA_ASSERT(buff[1] == '2');
    CA_ASSERT(buff[2] == '3');
    CA_ASSERT(buff[3] == '4');
    CA_ASSERT(buff[4] == '5');
    CA_ASSERT(buff[5] == '6');
    CA_ASSERT(buff[6] == '7');
    CA_ASSERT(buff[7] == '8');
    CA_ASSERT(buff[8] == '9');
    CA_ASSERT(buff[9] == '0');
    a.Clear();
    CA_ASSERT(a.Size() == 0);
    CA_ASSERT(a.Capacity() == 99);
    CA_ASSERT(a.Available() == 99);
    CA_ASSERT(a.Str() == buff);
    CA_ASSERT(a.Good() == true);
    a << caStringFormat::bin;
    a << t;
    CA_ASSERT(a.Size() == 34);
    CA_ASSERT(a.Capacity() == 99);
    CA_ASSERT(a.Available() == 65);
    CA_ASSERT(a.Str() == buff);
    CA_ASSERT(a.Good() == true);
    CA_ASSERT(buff[0] == '0');
    CA_ASSERT(buff[1] == 'b');
    CA_ASSERT(buff[2] == '0');
    CA_ASSERT(buff[3] == '1');
    CA_ASSERT(buff[4] == '0');
    CA_ASSERT(buff[5] == '0');
    CA_ASSERT(buff[6] == '1');
    CA_ASSERT(buff[7] == '0');
    CA_ASSERT(buff[8] == '0');
    CA_ASSERT(buff[9] == '1');
    CA_ASSERT(buff[10] == '1');
    CA_ASSERT(buff[11] == '0');
    CA_ASSERT(buff[12] == '0');
    CA_ASSERT(buff[13] == '1');
    CA_ASSERT(buff[14] == '0');
    CA_ASSERT(buff[15] == '1');
    CA_ASSERT(buff[16] == '1');
    CA_ASSERT(buff[17] == '0');
    CA_ASSERT(buff[18] == '0');
    CA_ASSERT(buff[19] == '0');
    CA_ASSERT(buff[20] == '0');
    CA_ASSERT(buff[21] == '0');
    CA_ASSERT(buff[22] == '0');
    CA_ASSERT(buff[23] == '0');
    CA_ASSERT(buff[24] == '1');
    CA_ASSERT(buff[25] == '0');
    CA_ASSERT(buff[26] == '1');
    CA_ASSERT(buff[27] == '1');
    CA_ASSERT(buff[28] == '0');
    CA_ASSERT(buff[29] == '1');
    CA_ASSERT(buff[30] == '0');
    CA_ASSERT(buff[31] == '0');
    CA_ASSERT(buff[32] == '1');
    CA_ASSERT(buff[33] == '0');
    //499602D2
    a.Clear();
    a << caStringFormat::hex;
    a << t;
    CA_ASSERT(a.Size() == 10);
    CA_ASSERT(a.Capacity() == 99);
    CA_ASSERT(a.Available() == 89);
    CA_ASSERT(a.Str() == buff);
    CA_ASSERT(a.Good() == true);
    CA_ASSERT(buff[0] == '0');
    CA_ASSERT(buff[1] == 'x');
    CA_ASSERT(buff[2] == '4');
    CA_ASSERT(buff[3] == '9');
    CA_ASSERT(buff[4] == '9');
    CA_ASSERT(buff[5] == '6');
    CA_ASSERT(buff[6] == '0');
    CA_ASSERT(buff[7] == '2');
    CA_ASSERT(buff[8] == 'D');
    CA_ASSERT(buff[9] == '2');
    a.Clear();
}

void caStringStream_test_class::test9(void) {
    _START();
    _INFO("to check operator << s32 of caStringStream");
    _INFO("value of s32 negative");
    _INFO("Test out on format dec,hex,bin");
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
    s32 t = -1234567890; //0xB669FD2E
    a << caStringFormat::dec;
    a << t;
    CA_ASSERT(a.Size() == 11);
    CA_ASSERT(a.Capacity() == 99);
    CA_ASSERT(a.Available() == 88);
    CA_ASSERT(a.Str() == buff);
    CA_ASSERT(a.Good() == true);
    CA_ASSERT(buff[0] == '-');
    CA_ASSERT(buff[1] == '1');
    CA_ASSERT(buff[2] == '2');
    CA_ASSERT(buff[3] == '3');
    CA_ASSERT(buff[4] == '4');
    CA_ASSERT(buff[5] == '5');
    CA_ASSERT(buff[6] == '6');
    CA_ASSERT(buff[7] == '7');
    CA_ASSERT(buff[8] == '8');
    CA_ASSERT(buff[9] == '9');
    CA_ASSERT(buff[10] == '0');
    a.Clear();
    CA_ASSERT(a.Size() == 0);
    CA_ASSERT(a.Capacity() == 99);
    CA_ASSERT(a.Available() == 99);
    CA_ASSERT(a.Str() == buff);
    CA_ASSERT(a.Good() == true);
    a << caStringFormat::bin;
    a << t;
    CA_ASSERT(a.Size() == 34);
    CA_ASSERT(a.Capacity() == 99);
    CA_ASSERT(a.Available() == 65);
    CA_ASSERT(a.Str() == buff);
    CA_ASSERT(a.Good() == true);
    CA_ASSERT(buff[0] == '0');
    CA_ASSERT(buff[1] == 'b');
    CA_ASSERT(buff[2] == '1');
    CA_ASSERT(buff[3] == '0');
    CA_ASSERT(buff[4] == '1');
    CA_ASSERT(buff[5] == '1');
    CA_ASSERT(buff[6] == '0');
    CA_ASSERT(buff[7] == '1');
    CA_ASSERT(buff[8] == '1');
    CA_ASSERT(buff[9] == '0');
    CA_ASSERT(buff[10] == '0');
    CA_ASSERT(buff[11] == '1');
    CA_ASSERT(buff[12] == '1');
    CA_ASSERT(buff[13] == '0');
    CA_ASSERT(buff[14] == '1');
    CA_ASSERT(buff[15] == '0');
    CA_ASSERT(buff[16] == '0');
    CA_ASSERT(buff[17] == '1');
    CA_ASSERT(buff[18] == '1');
    CA_ASSERT(buff[19] == '1');
    CA_ASSERT(buff[20] == '1');
    CA_ASSERT(buff[21] == '1');
    CA_ASSERT(buff[22] == '1');
    CA_ASSERT(buff[23] == '1');
    CA_ASSERT(buff[24] == '0');
    CA_ASSERT(buff[25] == '1');
    CA_ASSERT(buff[26] == '0');
    CA_ASSERT(buff[27] == '0');
    CA_ASSERT(buff[28] == '1');
    CA_ASSERT(buff[29] == '0');
    CA_ASSERT(buff[30] == '1');
    CA_ASSERT(buff[31] == '1');
    CA_ASSERT(buff[32] == '1');
    CA_ASSERT(buff[33] == '0');
    //B669FD2E
    a.Clear();
    a << caStringFormat::hex;
    a << t;
    CA_ASSERT(a.Size() == 10);
    CA_ASSERT(a.Capacity() == 99);
    CA_ASSERT(a.Available() == 89);
    CA_ASSERT(a.Str() == buff);
    CA_ASSERT(a.Good() == true);
    CA_ASSERT(buff[0] == '0');
    CA_ASSERT(buff[1] == 'x');
    CA_ASSERT(buff[2] == 'B');
    CA_ASSERT(buff[3] == '6');
    CA_ASSERT(buff[4] == '6');
    CA_ASSERT(buff[5] == '9');
    CA_ASSERT(buff[6] == 'F');
    CA_ASSERT(buff[7] == 'D');
    CA_ASSERT(buff[8] == '2');
    CA_ASSERT(buff[9] == 'E');
    a.Clear();
}

void caStringStream_test_class::test10(void) {
    _START();
    _INFO("to check operator << u32 of caStringStream");
    _INFO("Test out on format dec,hex,bin");
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
    u32 t = 3060399406; //0xB669FD2E
    a << caStringFormat::dec;
    a << t;
    CA_ASSERT(a.Size() == 10);
    CA_ASSERT(a.Capacity() == 99);
    CA_ASSERT(a.Available() == 89);
    CA_ASSERT(a.Str() == buff);
    CA_ASSERT(a.Good() == true);
    CA_ASSERT(buff[0] == '3');
    CA_ASSERT(buff[1] == '0');
    CA_ASSERT(buff[2] == '6');
    CA_ASSERT(buff[3] == '0');
    CA_ASSERT(buff[4] == '3');
    CA_ASSERT(buff[5] == '9');
    CA_ASSERT(buff[6] == '9');
    CA_ASSERT(buff[7] == '4');
    CA_ASSERT(buff[8] == '0');
    CA_ASSERT(buff[9] == '6');
    a.Clear();
    CA_ASSERT(a.Size() == 0);
    CA_ASSERT(a.Capacity() == 99);
    CA_ASSERT(a.Available() == 99);
    CA_ASSERT(a.Str() == buff);
    CA_ASSERT(a.Good() == true);
    a << caStringFormat::bin;
    a << t;
    CA_ASSERT(a.Size() == 34);
    CA_ASSERT(a.Capacity() == 99);
    CA_ASSERT(a.Available() == 65);
    CA_ASSERT(a.Str() == buff);
    CA_ASSERT(a.Good() == true);
    CA_ASSERT(buff[0] == '0');
    CA_ASSERT(buff[1] == 'b');
    CA_ASSERT(buff[2] == '1');
    CA_ASSERT(buff[3] == '0');
    CA_ASSERT(buff[4] == '1');
    CA_ASSERT(buff[5] == '1');
    CA_ASSERT(buff[6] == '0');
    CA_ASSERT(buff[7] == '1');
    CA_ASSERT(buff[8] == '1');
    CA_ASSERT(buff[9] == '0');
    CA_ASSERT(buff[10] == '0');
    CA_ASSERT(buff[11] == '1');
    CA_ASSERT(buff[12] == '1');
    CA_ASSERT(buff[13] == '0');
    CA_ASSERT(buff[14] == '1');
    CA_ASSERT(buff[15] == '0');
    CA_ASSERT(buff[16] == '0');
    CA_ASSERT(buff[17] == '1');
    CA_ASSERT(buff[18] == '1');
    CA_ASSERT(buff[19] == '1');
    CA_ASSERT(buff[20] == '1');
    CA_ASSERT(buff[21] == '1');
    CA_ASSERT(buff[22] == '1');
    CA_ASSERT(buff[23] == '1');
    CA_ASSERT(buff[24] == '0');
    CA_ASSERT(buff[25] == '1');
    CA_ASSERT(buff[26] == '0');
    CA_ASSERT(buff[27] == '0');
    CA_ASSERT(buff[28] == '1');
    CA_ASSERT(buff[29] == '0');
    CA_ASSERT(buff[30] == '1');
    CA_ASSERT(buff[31] == '1');
    CA_ASSERT(buff[32] == '1');
    CA_ASSERT(buff[33] == '0');
    //B669FD2E
    a.Clear();
    a << caStringFormat::hex;
    a << t;
    CA_ASSERT(a.Size() == 10);
    CA_ASSERT(a.Capacity() == 99);
    CA_ASSERT(a.Available() == 89);
    CA_ASSERT(a.Str() == buff);
    CA_ASSERT(a.Good() == true);
    CA_ASSERT(buff[0] == '0');
    CA_ASSERT(buff[1] == 'x');
    CA_ASSERT(buff[2] == 'B');
    CA_ASSERT(buff[3] == '6');
    CA_ASSERT(buff[4] == '6');
    CA_ASSERT(buff[5] == '9');
    CA_ASSERT(buff[6] == 'F');
    CA_ASSERT(buff[7] == 'D');
    CA_ASSERT(buff[8] == '2');
    CA_ASSERT(buff[9] == 'E');
    a.Clear();
}

void caStringStream_test_class::test11(void) {
    _START();
    _INFO("to check operator << & b of caStringStream");
    _INFO("concatenate two castringstream (faster mode: memcpy)");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();
    char buff_a[100];
    caStringStream<char> a;
    a.Init(buff_a, 100);
    CA_ASSERT(a.Size() == 0);
    CA_ASSERT(a.Capacity() == 99);
    CA_ASSERT(a.Str() == buff_a);
    CA_ASSERT(a.Good() == true);
    char buff_b[100];
    caStringStream<char> b;
    b.Init(buff_b, 100);
    CA_ASSERT(b.Size() == 0);
    CA_ASSERT(b.Capacity() == 99);
    CA_ASSERT(b.Str() == buff_b);
    CA_ASSERT(b.Good() == true);
    caCSTR( f,"pippo  ");
    a << f;
    CA_ASSERT(a.Size() == 7);
    CA_ASSERT(a.Capacity() == 99);
    CA_ASSERT(a.Str() == buff_a);
    CA_ASSERT(a.Good() == true);
    CA_ASSERT(strcmp(a.Str(), "pippo  ") == 0);
    caCSTR(g,"pluto  ");
    b << g;
    CA_ASSERT(b.Size() == 7);
    CA_ASSERT(b.Capacity() == 99);
    CA_ASSERT(b.Str() == buff_b);
    CA_ASSERT(b.Good() == true);
    CA_ASSERT(strcmp(b.Str(), "pluto  ") == 0);
    a << b;
    CA_ASSERT(b.Size() == 7);
    CA_ASSERT(b.Capacity() == 99);
    CA_ASSERT(b.Str() == buff_b);
    CA_ASSERT(b.Good() == true);
    CA_ASSERT(strcmp(b.Str(), "pluto  ") == 0);
    CA_ASSERT(a.Size() == 14);
    CA_ASSERT(a.Capacity() == 99);
    CA_ASSERT(a.Available() == 85);
    CA_ASSERT(a.Str() == buff_a);
    CA_ASSERT(a.Good() == true);
    CA_ASSERT(strcmp(a.Str(), "pippo  pluto  ") == 0);
    a << a;
    CA_ASSERT(a.Size() == 28);
    CA_ASSERT(a.Capacity() == 99);
    CA_ASSERT(a.Available() == 71);
    CA_ASSERT(a.Str() == buff_a);
    CA_ASSERT(a.Good() == true);
    CA_ASSERT(strcmp(a.Str(), "pippo  pluto  pippo  pluto  ") == 0);
    a << a;
    CA_ASSERT(a.Size() == 56);
    CA_ASSERT(a.Capacity() == 99);
    CA_ASSERT(a.Available() == 43);
    CA_ASSERT(a.Str() == buff_a);
    CA_ASSERT(a.Good() == true);
    CA_ASSERT(strcmp(a.Str(), "pippo  pluto  pippo  pluto  pippo  pluto  pippo  pluto  ") == 0);
    a << a;
    CA_ASSERT(a.Size() == 99);
    CA_ASSERT(a.Capacity() == 99);
    CA_ASSERT(a.Available() == 0);
    CA_ASSERT(a.Str() == buff_a);
    CA_ASSERT(a.Good() == false);
    CA_ASSERT(strcmp(a.Str(), "pippo  pluto  pippo  pluto  pippo  pluto  pippo  pluto  pippo  pluto  pippo  pluto  pippo  pluto  p") == 0);
}

void caStringStream_test_class::test12(void) {
    _START();
    _INFO("to check operator << const char *msg of caStringStream");
    _INFO("concatenate string (slower mode: no memcpy)");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();
    char buff_a[20];
    caStringStream<char> a;
    a.Init(buff_a, sizeof (buff_a));
    CA_ASSERT(a.Size() == 0);
    CA_ASSERT(a.Capacity() == 19);
    CA_ASSERT(a.Str() == buff_a);
    CA_ASSERT(a.Good() == true);
    a << "12345678901234567";
    CA_ASSERT(a.Size() == 17);
    CA_ASSERT(a.Capacity() == 19);
    CA_ASSERT(a.Available() == 2);
    CA_ASSERT(a.Str() == buff_a);
    CA_ASSERT(a.Good() == true);
    CA_ASSERT(strcmp(a.Str(), "12345678901234567") == 0);
    a << "12345678901234567";
    CA_ASSERT(a.Size() == 19);
    CA_ASSERT(a.Capacity() == 19);
    CA_ASSERT(a.Available() == 0);
    CA_ASSERT(a.Str() == buff_a);
    CA_ASSERT(a.Good() == false);
    CA_ASSERT(strcmp(a.Str(), "1234567890123456712") == 0);

}



void caStringStream_test_class::test12bis(void) {
    _START();
    _INFO("to check operator << const char *msg of caStringStream");
    _INFO("concatenate string (slower mode: no memcpy)");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();
    char buff_a[20];
    caStringStream<char> a;
    a.Init(buff_a, sizeof (buff_a));
    CA_ASSERT(a.Size() == 0);
    CA_ASSERT(a.Capacity() == 19);
    CA_ASSERT(a.Str() == buff_a);
    CA_ASSERT(a.Good() == true);
    caCSTR(f,"12345678901234567");
    a << f;
    CA_ASSERT(a.Size() == 17);
    CA_ASSERT(a.Capacity() == 19);
    CA_ASSERT(a.Available() == 2);
    CA_ASSERT(a.Str() == buff_a);
    CA_ASSERT(a.Good() == true);
    CA_ASSERT(strcmp(a.Str(), "12345678901234567") == 0);
    a << f;
    CA_ASSERT(a.Size() == 19);
    CA_ASSERT(a.Capacity() == 19);
    CA_ASSERT(a.Available() == 0);
    CA_ASSERT(a.Str() == buff_a);
    CA_ASSERT(a.Good() == false);
    CA_ASSERT(strcmp(a.Str(), "1234567890123456712") == 0);

}

void caStringStream_test_class::test13(void) {
    _START();
    _INFO("to check operator << *s8 of caStringStream");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();
    char buff_a[20];
    char buff_b[20];
    caStringStream<char> a;
    a.Init(buff_a, sizeof (buff_a));
    s8 t = 'A';
    sprintf(buff_b, "[0x%08X]%c", &t, t);
    a << &t;
    CA_ASSERT(strcmp(a.Str(), buff_b) == 0);
}

void caStringStream_test_class::test14(void) {
    _START();
    _INFO("to check operator << *u8 of caStringStream");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();
    char buff_a[20];
    char buff_b[20];
    caStringStream<char> a;
    a.Init(buff_a, sizeof (buff_a));
    u8 t = 169;
    a << caStringFormat::dec;
    sprintf(buff_b, "[0x%08X]%d", &t, t);
    a << &t;
    CA_ASSERT(strcmp(a.Str(), buff_b) == 0);
    a.Clear();
    a << caStringFormat::hex;
    sprintf(buff_b, "[0x%08X]0x%02X", &t, t);
    a << &t;
    CA_ASSERT(strcmp(a.Str(), buff_b) == 0);
}

void caStringStream_test_class::test15(void) {
    _START();
    _INFO("to check operator << *s16 of caStringStream");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();
    char buff_a[20];
    char buff_b[20];
    caStringStream<char> a;
    a.Init(buff_a, sizeof (buff_a));
    s16 t = -4589;
    a << caStringFormat::dec;
    sprintf(buff_b, "[0x%08X]%d", &t, t);
    a << &t;
    CA_ASSERT(strcmp(a.Str(), buff_b) == 0);
}

void caStringStream_test_class::test16(void) {
    _START();
    _INFO("to check operator << *u16 of caStringStream");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();
    char buff_a[20];
    char buff_b[20];
    caStringStream<char> a;
    a.Init(buff_a, sizeof (buff_a));
    u16 t = 43256;
    a << caStringFormat::dec;
    sprintf(buff_b, "[0x%08X]%d", &t, t);
    a << &t;
    CA_ASSERT(strcmp(a.Str(), buff_b) == 0);
}

void caStringStream_test_class::test17(void) {
    _START();
    _INFO("to check operator << *s32 of caStringStream");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();
    char buff_a[40];
    char buff_b[40];
    caStringStream<char> a;
    a.Init(buff_a, sizeof (buff_a));
    s32 t = -13243256;
    a << caStringFormat::dec;
    sprintf(buff_b, "[0x%08X]%d", &t, t);
    a << &t;
    CA_ASSERT(strcmp(a.Str(), buff_b) == 0);
}

void caStringStream_test_class::test18(void) {
    _START();
    _INFO("to check operator << *u32 of caStringStream");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();
    char buff_a[40];
    char buff_b[40];
    caStringStream<char> a;
    a.Init(buff_a, sizeof (buff_a));
    u32 t = 0x0efacaca;
    a << caStringFormat::dec;
    sprintf(buff_b, "[0x%08X]%d", &t, t);
    a << &t;
    CA_ASSERT(strcmp(a.Str(), buff_b) == 0);
}

void caStringStream_test_class::test19(void) {
    _START();
    _INFO("to check operator << * b a ptr of caStringStream");
    _INFO("concatenate two castringstream (faster mode: memcpy)");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();
    char buff_a[100];
    caStringStream<char> a;
    a.Init(buff_a, 100);
    CA_ASSERT(a.Size() == 0);
    CA_ASSERT(a.Capacity() == 99);
    CA_ASSERT(a.Str() == buff_a);
    CA_ASSERT(a.Good() == true);
    char buff_b[100];
    caStringStream<char> b;
    b.Init(buff_b, 100);
    CA_ASSERT(b.Size() == 0);
    CA_ASSERT(b.Capacity() == 99);
    CA_ASSERT(b.Str() == buff_b);
    CA_ASSERT(b.Good() == true);
    caCSTR(f,"pippo  ");
    a << f;
    CA_ASSERT(a.Size() == 7);
    CA_ASSERT(a.Capacity() == 99);
    CA_ASSERT(a.Str() == buff_a);
    CA_ASSERT(a.Good() == true);
    CA_ASSERT(strcmp(a.Str(), "pippo  ") == 0);
    caCSTR(g,"pluto  ");
    b << g;
    CA_ASSERT(b.Size() == 7);
    CA_ASSERT(b.Capacity() == 99);
    CA_ASSERT(b.Str() == buff_b);
    CA_ASSERT(b.Good() == true);
    CA_ASSERT(strcmp(b.Str(), "pluto  ") == 0);
    a << &b;
    CA_ASSERT(b.Size() == 7);
    CA_ASSERT(b.Capacity() == 99);
    CA_ASSERT(b.Str() == buff_b);
    CA_ASSERT(b.Good() == true);
    CA_ASSERT(strcmp(b.Str(), "pluto  ") == 0);
    CA_ASSERT(a.Size() == 14);
    CA_ASSERT(a.Capacity() == 99);
    CA_ASSERT(a.Available() == 85);
    CA_ASSERT(a.Str() == buff_a);
    CA_ASSERT(a.Good() == true);
    CA_ASSERT(strcmp(a.Str(), "pippo  pluto  ") == 0);
}

void caStringStream_test_class::test20(void) {
    _START();
    _INFO("to check filler object");
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
    caStringFiller v('a', 10);
    caCSTR(f,"pippo");
    a << f;
    a.Fix(v);
    a << v;
    CA_ASSERT(strcmp(a.Str(), "pippoaaaaaaaaaa") == 0);
    CA_ASSERT(v.ch == 'a');
    CA_ASSERT(v.width == 10);
    CA_ASSERT(v.end == 15);
}

void caStringStream_test_class::test21(void) {
    _START();
    _INFO("to check terminator object");
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
    caCSTR (f,"pippo");
    a << f << caEnd::endl;
    CA_ASSERT(strcmp(a.Str(), "pippo\r\n") == 0);
}