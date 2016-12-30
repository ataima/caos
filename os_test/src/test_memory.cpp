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
#include <iostream>


static u32 theHeap[100000];
u32*  __heap_base__=&theHeap[0];
u32* __heap_end__=&theHeap[100000];

static u32 getstartmem(void){return ptr_to_uint(__heap_base__);}
static u32 getstopmem(void){return ptr_to_uint(__heap_end__);}

const hal_ll_mem_io hal_ll_mem={getstartmem,getstopmem};



class caMemory_test_class
: public caTester
{
    CA_TEST_SUITE(caMemory_test_class);
    CA_TEST(caMemory_test_class::test1, " Init test");
    CA_TEST(caMemory_test_class::test2, " Clean test");
    CA_TEST(caMemory_test_class::test3, " Allocate/Free test");
    CA_TEST_SUITE_END();

    void setUp(void)
    {
    }
    void test1(void);
    void test2(void);
    void test3(void);


    void tearDown(void)
    {
    }

};

REGISTER_CLASS(caMemory_test_class);

void caMemory_test_class::test1(void)
{
    _START();
    _INFO("to check Init function of caMemory");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();
    std::cout<<"HEAP BASE = "<<__heap_base__<<std::endl;
    std::cout<<"HEAP END = "<<__heap_end__<<std::endl;
    caMemory::Init();
    CA_ASSERT(caMemory::Good()!=0);
    CA_ASSERT(caMemory::GetStartAddress()==__heap_base__);
    CA_ASSERT(caMemory::GetEndAddress()==__heap_end__);
    CA_ASSERT(caMemory::GetTotalSize()==100000*sizeof(u32));
    CA_ASSERT(caMemory::GetAvailMemory()==((100000*sizeof(u32))-(3*BLOCKSIZE)));
    CA_ASSERT(caMemory::GetHeaderBlock()==BLOCKSIZE);       
}

void caMemory_test_class::test2(void)
{
    _START();
    _INFO("to check Clean function caMemory");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();
    std::cout<<"HEAP BASE = "<<__heap_base__<<std::endl;
    std::cout<<"HEAP END = "<<__heap_end__<<std::endl;
    CA_ASSERT(caMemory::Good()!=0);
    CA_ASSERT(caMemory::GetStartAddress()==__heap_base__);
    CA_ASSERT(caMemory::GetEndAddress()==__heap_end__);
    CA_ASSERT(caMemory::GetTotalSize()==100000*sizeof(u32));
    CA_ASSERT(caMemory::GetAvailMemory()==((100000*sizeof(u32))-(3*BLOCKSIZE)));
    CA_ASSERT(caMemory::GetHeaderBlock()==BLOCKSIZE);
    caMemory::Clean();
    CA_ASSERT(caMemory::Good()==0);
    CA_ASSERT(caMemory::GetStartAddress()==0);
    CA_ASSERT(caMemory::GetEndAddress()==0);
    CA_ASSERT(caMemory::GetTotalSize()==0);
    CA_ASSERT(caMemory::GetAvailMemory()==0);
    CA_ASSERT(caMemory::GetHeaderBlock()==BLOCKSIZE);
}

void caMemory_test_class::test3(void)
{
    _START();
    _INFO("to check Allocate free function of caMemory");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();
    caMemory::Init();
    CA_ASSERT(caMemory::Good()!=0);
    CA_ASSERT(caMemory::GetStartAddress()==__heap_base__);
    CA_ASSERT(caMemory::GetEndAddress()==__heap_end__);
    CA_ASSERT(caMemory::GetTotalSize()==100000*sizeof(u32));
    CA_ASSERT(caMemory::GetAvailMemory()==((100000*sizeof(u32))-(3*BLOCKSIZE)));
    CA_ASSERT(caMemory::GetHeaderBlock()==BLOCKSIZE);      
    void * p= caMemory::Allocate(caMemory::GetHeaderBlock()*100);
    CA_ASSERT(p!=NULL);
    CA_ASSERT(caMemory::GetAvailMemory()==((100000*sizeof(u32))-(5*BLOCKSIZE)-caMemory::GetHeaderBlock()*100));
    u32 size=0;
    u32 res=caMemory::Free(p,&size);
    CA_ASSERT(res==TRUE);
    CA_ASSERT(caMemory::GetHeaderBlock()*101==size);
    CA_ASSERT(caMemory::GetAvailMemory()==((100000*sizeof(u32))-(3*BLOCKSIZE)));   
    caMemory::Clean();
}


