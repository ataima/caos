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
#include "atomiclock.h"






class caAtomicLock_test_class
: public caTester
{
    CA_TEST_SUITE(caAtomicLock_test_class);
    CA_TEST(caAtomicLock_test_class::test1, " ctor test");
    CA_TEST(caAtomicLock_test_class::test2, " lock test");
    CA_TEST(caAtomicLock_test_class::test3, " unlock test");
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
    caAtomicLock lock;
};

REGISTER_CLASS(caAtomicLock_test_class);

void caAtomicLock_test_class::test1(void)
{
    _START();
    _INFO("to check ctor function of caHalComDevice");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();
    CA_ASSERT(lock.Get()==0);
}

void caAtomicLock_test_class::test2(void)
{
    _START();
    _INFO("to check lock  function caHalComDevice");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();
    CA_ASSERT(lock.Lock()!=0);
    CA_ASSERT(lock.Get()!=0);
}

void caAtomicLock_test_class::test3(void)
{
    _START();
    _INFO("to check unlock caHalComDevice");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();    
    CA_ASSERT(lock.UnLock()!=0);
    CA_ASSERT(lock.Get()==0);
}


