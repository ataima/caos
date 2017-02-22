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

#include "heaparray.h"

class caHeapArray_test_class
: public caTester {
    CA_TEST_SUITE(caHeapArray_test_class);
    CA_TEST(caHeapArray_test_class::test1, "ctor test");
    CA_TEST(caHeapArray_test_class::test2, "init test");
    CA_TEST(caHeapArray_test_class::test3, "add test");
    CA_TEST_SUITE_END();

    void setUp(void) {
    }
    void test1(void);
    void test2(void);
    void test3(void);

    void tearDown(void) {
    }

};

REGISTER_CLASS(caHeapArray_test_class);

void caHeapArray_test_class::test1(void) {
    _START();
    _INFO("to check ctor of caHeapArray");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();
    caHeapArray<int> v;
    CA_ASSERT(v.Capacity() == 0);
    CA_ASSERT(v.Size() == 0);
    CA_ASSERT(v.Start() == nullptr);
    CA_ASSERT(v.Stop() == nullptr);
    CA_ASSERT(v.Good() == false);
    CA_ASSERT(v.Empty() == true);
}

void caHeapArray_test_class::test2(void) {
    _START();
    _INFO("to check ctor of caHeapArray");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();
    caHeapArray<int> v;
    CA_ASSERT(v.Capacity() == 0);
    CA_ASSERT(v.Size() == 0);
    CA_ASSERT(v.Start() == nullptr);
    CA_ASSERT(v.Stop() == nullptr);
    CA_ASSERT(v.Good() == false);
    CA_ASSERT(v.Empty() == true);
    int buff[100];
    v.Init(buff, sizeof (buff) / sizeof (int));
    CA_ASSERT(v.Capacity() == 100);
    CA_ASSERT(v.Size() == 0);
    CA_ASSERT(v.Start() == &buff[0]);
    CA_ASSERT(v.Stop() == &buff[99]);
    CA_ASSERT(v.Good() == true);
    CA_ASSERT(v.Empty() == true);
}

void caHeapArray_test_class::test3(void) {
    _START();
    _INFO("to check ctor of caHeapArray");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();
    caHeapArray<int> v;
    int buff[100];
    int u;
    v.Init(buff, sizeof (buff) / sizeof (int));
    CA_ASSERT(v.Capacity() == 100);
    CA_ASSERT(v.Size() == 0);
    CA_ASSERT(v.Start() == &buff[0]);
    CA_ASSERT(v.Stop() == &buff[99]);
    CA_ASSERT(v.Good() == true);
    CA_ASSERT(v.Empty() == true);
    CA_ASSERT(v.inqueue(10) == true);
    CA_ASSERT(v.inqueue(20) == true);
    CA_ASSERT(v.inqueue(30) == true);
    CA_ASSERT(v.inqueue(40) == true);
    CA_ASSERT(v.Size() == 4);
    CA_ASSERT(v.At(u, 0) == true);
    CA_ASSERT(u == 40);
    CA_ASSERT(v.outqueue(u) == 3);
    CA_ASSERT(u == 40);

    CA_ASSERT(v.Size() == 3);
    CA_ASSERT(v.At(u, 0) == true);
    CA_ASSERT(u == 30);
    CA_ASSERT(v.outqueue(u) == 2);
    CA_ASSERT(u == 30);

    CA_ASSERT(v.Size() == 2);
    CA_ASSERT(v.At(u, 0) == true);
    CA_ASSERT(u == 20);
    CA_ASSERT(v.outqueue(u) == 1);
    CA_ASSERT(u == 20);

    CA_ASSERT(v.Size() == 1);
    CA_ASSERT(v.At(u, 0) == true);
    CA_ASSERT(u == 10);
    CA_ASSERT(v.outqueue(u) == 0);
    CA_ASSERT(u == 10);

    CA_ASSERT(v.Size() == 0);
}
