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
#include "memory.h"

 u32 __heap_base__[100000];
 u32 *__heap_end__=&__heap_base__[100000];

static u32 getstartmem(void){return ptr_to_uint(__heap_base__);}
static u32 getstopmem(void){return ptr_to_uint(__heap_end__);}

const hal_ll_mem_io hal_ll_mem={getstartmem,getstopmem};



class caMemory_test_class
: public caTester
{
    CA_TEST_SUITE(caMemory_test_class);
    CA_TEST(caMemory_test_class::test1, "  test");
    CA_TEST(caMemory_test_class::test2, "  test");
    CA_TEST(caMemory_test_class::test3, "  test");
    CA_TEST(caMemory_test_class::test4, "  test");
    CA_TEST(caMemory_test_class::test5, "  test");
    CA_TEST(caMemory_test_class::test6, "  test");
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

REGISTER_CLASS(caMemory_test_class);

void caMemory_test_class::test1(void)
{
    _START();
    _INFO("to check MemCpy u32 of caMemAux");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();
    
}

void caMemory_test_class::test2(void)
{
    _START();
    _INFO("to check MemCpy u16 of caMemAux");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();
    
}

void caMemory_test_class::test3(void)
{
    _START();
    _INFO("to check MemCpy u8 of caMemAux");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();
    

}

void caMemory_test_class::test4(void)
{
    _START();
    _INFO("to check MemSet u32 of caMemAux");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();
    
}

void caMemory_test_class::test5(void)
{
    _START();
    _INFO("to check MemSet u16 of caMemAux");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();
    
}

void caMemory_test_class::test6(void)
{
    _START();
    _INFO("to check MemSet u8 of caMemAux");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();
    
}
