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
    a.Init(buff, 100,0);
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
    a << "test";
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
    a << "test";
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
    s8 t=0;
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
    u8 buff[100]={121,242};
    caTokenizeSStream<s8> b;
    CA_ASSERT(b.Empty() == true);
    b.Init((s8 *)buff,100,2);
    CA_ASSERT(b.Size() == 2);
    CA_ASSERT(b.Capacity() == 99);
    CA_ASSERT(b.Str() == (s8*)buff);
    CA_ASSERT(b.Good() == true);
    CA_ASSERT(b.Empty() == false);
    u8 t=0;
    b>>t;    
    CA_ASSERT(t == 121);
    CA_ASSERT(b.Size() == 2);
    CA_ASSERT(b.Remain() == 1);
    CA_ASSERT(b.Capacity() == 99);
    CA_ASSERT(b.Str() == (s8*)&buff[1]);
    CA_ASSERT(b.Good() == true);
    CA_ASSERT(b.Empty() == false);
    b>>t;    
    CA_ASSERT(t == 242);
    CA_ASSERT(b.Size() == 2);
    CA_ASSERT(b.Remain() == 0);
    CA_ASSERT(b.Capacity() == 99);
    CA_ASSERT(b.Str() == (s8*)&buff[2]);
    CA_ASSERT(b.Good() == true);
    CA_ASSERT(b.Empty() == true);
    b.Clear();
    CA_ASSERT(b.Size() == 0);
    CA_ASSERT(b.Capacity() == 99);
    CA_ASSERT(b.Available() == 99);
    CA_ASSERT(b.Str() == (s8*)buff);
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
    a<<" 1234  -3245";
    caTokenizeSStream<s8> b;
    CA_ASSERT(b.Empty() == true);
    b.Init(a);
    CA_ASSERT(b.Size() == 12);
    CA_ASSERT(b.Capacity() == 99);
    CA_ASSERT(b.Str() == buff);
    CA_ASSERT(b.Good() == true);
    CA_ASSERT(b.Empty() == false);
    s16 t=0;
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
    a<<" 1234  47325";
    caTokenizeSStream<s8> b;
    CA_ASSERT(b.Empty() == true);
    b.Init(a);
    CA_ASSERT(b.Size() == 12);
    CA_ASSERT(b.Capacity() == 99);
    CA_ASSERT(b.Str() == buff);
    CA_ASSERT(b.Good() == true);
    CA_ASSERT(b.Empty() == false);
    u16 t=0;
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
    a<<" 1234  -3245";
    caTokenizeSStream<s8> b;
    CA_ASSERT(b.Empty() == true);
    b.Init(a);
    CA_ASSERT(b.Size() == 12);
    CA_ASSERT(b.Capacity() == 99);
    CA_ASSERT(b.Str() == buff);
    CA_ASSERT(b.Good() == true);
    CA_ASSERT(b.Empty() == false);
    s32 t=0;
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
    a<<" 1234  473253456";
    caTokenizeSStream<s8> b;
    CA_ASSERT(b.Empty() == true);
    b.Init(a);
    CA_ASSERT(b.Size() == 12);
    CA_ASSERT(b.Capacity() == 99);
    CA_ASSERT(b.Str() == buff);
    CA_ASSERT(b.Good() == true);
    CA_ASSERT(b.Empty() == false);
    u32 t=0;
    b>>t;    
    CA_ASSERT(t == 1234);
    CA_ASSERT(b.Size() == 12);
    CA_ASSERT(b.Remain() == 5);
    CA_ASSERT(b.Capacity() == 99);
    CA_ASSERT(b.Str() == &buff[7]);
    CA_ASSERT(b.Good() == true);
    CA_ASSERT(b.Empty() == false);
    b>>t;    
    CA_ASSERT(t == 473253456);
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
