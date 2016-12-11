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
#include "memaux.h"



class caMemAux_test_class
: public caTester
{
    CA_TEST_SUITE(caMemAux_test_class);
    CA_TEST(caMemAux_test_class::test1, "MemCpy u32 test");
    CA_TEST(caMemAux_test_class::test2, "MemCpy u16  test");
    CA_TEST(caMemAux_test_class::test3, "MemCpy u8  test");
    CA_TEST(caMemAux_test_class::test4, "MemSet u32 test");
    CA_TEST(caMemAux_test_class::test5, "MemSet u16  test");
    CA_TEST(caMemAux_test_class::test6, "MemSet u8  test");
    CA_TEST_SUITE_END();
    void setUp(void)    {
    }
    void test1(void);
    void test2(void);
    void test3(void);
    void test4(void);
    void test5(void);
    void test6(void);

    void tearDown(void){
    }

};

REGISTER_CLASS(caMemAux_test_class);



void caMemAux_test_class::test1(void)
{
    _START();
    _INFO("to check MemCpy u32 of caMemAux");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();  
    u32 b[10]={1,2,3,4,5,6,7,8,9,10};
    u32 e[10];
    void  * ptr=caMemAux::MemCpy(e,b,sizeof(b)/sizeof(u32));
    CA_ASSERT(ptr==&e[10]);
    CA_ASSERT(memcmp(b,e,sizeof(b))==0);      
}


void caMemAux_test_class::test2(void)
{
    _START();
    _INFO("to check MemCpy u16 of caMemAux");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();  
    u16 b[10]={1,2,3,4,5,6,7,8,9,10};
    u16 e[10];
    void  * ptr=caMemAux::MemCpy(e,b,sizeof(b)/sizeof(u16));
    CA_ASSERT(ptr==&e[10]);
    CA_ASSERT(memcmp(b,e,sizeof(b))==0);      
    
}

void caMemAux_test_class::test3(void)
{
    _START();
    _INFO("to check MemCpy u8 of caMemAux");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();  
    u8 b[10]={1,2,3,4,5,6,7,8,9,10};
    u8 e[10];
    void  * ptr=caMemAux::MemCpy(e,b,sizeof(b));
    CA_ASSERT(ptr==&e[10]);
    CA_ASSERT(memcmp(b,e,sizeof(b))==0);      
    
}


void caMemAux_test_class::test4(void)
{
    _START();
    _INFO("to check MemSet u32 of caMemAux");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();  
    u32 b[10]={5,5,5,5,5,5,5,5,5,5};
    u32 e[10];
    void  * ptr=caMemAux::MemSet(e,5,sizeof(b)/sizeof(u32));
    CA_ASSERT(ptr==&e[10]);
    CA_ASSERT(memcmp(b,e,sizeof(b))==0);      
}

void caMemAux_test_class::test5(void)
{
    _START();
    _INFO("to check MemSet u16 of caMemAux");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();  
    u16 b[10]={5,5,5,5,5,5,5,5,5,5};
    u16 e[10];
    void  * ptr=caMemAux::MemSet(e,5,sizeof(b)/sizeof(u16));
    CA_ASSERT(ptr==&e[10]);
    CA_ASSERT(memcmp(b,e,sizeof(b))==0);      
}

void caMemAux_test_class::test6(void)
{
    _START();
    _INFO("to check MemSet u8 of caMemAux");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();  
    u8 b[10]={5,5,5,5,5,5,5,5,5,5};
    u8 e[10];
    void  * ptr=caMemAux::MemSet(e,5,sizeof(b));
    CA_ASSERT(ptr==&e[10]);
    CA_ASSERT(memcmp(b,e,sizeof(b))==0);      
}
