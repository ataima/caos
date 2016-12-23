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
#include "circularbuffer.h"

class caCircularBuffer_test_class
: public caTester
{
    CA_TEST_SUITE(caCircularBuffer_test_class);
    CA_TEST(caCircularBuffer_test_class::test1, "ctor test");
    CA_TEST(caCircularBuffer_test_class::test2, "Init test");
    CA_TEST(caCircularBuffer_test_class::test3, "push/pop test");
    CA_TEST(caCircularBuffer_test_class::test4, "multi push/pop test");
    CA_TEST(caCircularBuffer_test_class::test5, "maxsize push/pop test");
    CA_TEST(caCircularBuffer_test_class::test6, "overwrite test");
    CA_TEST_SUITE_END();

    void setUp(void)
    {
    }
    //
    void test1(void);
    void test2(void);
    void test3(void);
    void test4(void);
    void test5(void);
    void test6(void);
    //

    void tearDown(void)
    {
    }

};

REGISTER_CLASS(caCircularBuffer_test_class);

void caCircularBuffer_test_class::test1(void)
{
    _START();
    _INFO("to check ctor of caCircularBuffer");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();
    caCircularBuffer<int> a;
    CA_ASSERT(a.Size() == 0);
    CA_ASSERT(a.Capacity() == 0);
    CA_ASSERT(a.Empty() == true);
    CA_ASSERT(a.Available() == 0);
    CA_ASSERT(a.Good() == true);
}

void caCircularBuffer_test_class::test2(void)
{
    _START();
    _INFO("to check Init procudere of caCircularBuffer");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();
    int buff[16];
    caCircularBuffer<int> a;
    a.Init(buff, sizeof (buff) / sizeof (int));
    CA_ASSERT(a.Size() == 0);
    CA_ASSERT(a.Capacity() == 16);
    CA_ASSERT(a.Empty() == true);
    CA_ASSERT(a.Good() == true);
}

void caCircularBuffer_test_class::test3(void)
{
    _START();
    _INFO("to check PUSH/POP procudere of caCircularBuffer");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();
    int buff[16];
    caCircularBuffer<int> a;
    a.Init(buff, sizeof (buff) / sizeof (int));
    CA_ASSERT(a.Size() == 0);
    CA_ASSERT(a.Capacity() == 16);
    CA_ASSERT(a.Empty() == true);
    CA_ASSERT(a.Good() == true);
    CA_ASSERT(a.Push(100) == true);
    CA_ASSERT(a.Size() == 1);
    CA_ASSERT(a.Capacity() == 16);
    CA_ASSERT(a.Good() == true);
    CA_ASSERT(a.Empty() == false);
    int v;
    CA_ASSERT(a.Pop(v) == true);
    CA_ASSERT(v == 100);
    CA_ASSERT(a.Size() == 0);
    CA_ASSERT(a.Capacity() == 16);
    CA_ASSERT(a.Empty() == true);
    CA_ASSERT(a.Good() == true);
}

void caCircularBuffer_test_class::test4(void)
{
    _START();
    _INFO("to check multi PUSH/POP procudere of caCircularBuffer");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();
    int buff[16];
    caCircularBuffer<int> a;
    a.Init(buff, sizeof (buff) / sizeof (int));
    CA_ASSERT(a.Size() == 0);
    CA_ASSERT(a.Capacity() == 16);
    CA_ASSERT(a.Empty() == true);
    CA_ASSERT(a.Good() == true);
    int buff_a[8] = {100, 200, 300, 400, 500, 600, 700, 800};
    s_t wr;
    CA_ASSERT(a.Push(buff_a, 8, wr) == true);
    CA_ASSERT(a.Size() == 8);
    CA_ASSERT(wr == 8);
    CA_ASSERT(a.Capacity() == 16);
    CA_ASSERT(a.Good() == true);
    CA_ASSERT(a.Empty() == false);
    int v[8];
    s_t rd;
    CA_ASSERT(a.Pop(&v[0], 8, rd) == true);
    CA_ASSERT(v[0] == 100);
    CA_ASSERT(v[1] == 200);
    CA_ASSERT(v[2] == 300);
    CA_ASSERT(v[3] == 400);
    CA_ASSERT(v[4] == 500);
    CA_ASSERT(v[5] == 600);
    CA_ASSERT(v[6] == 700);
    CA_ASSERT(v[7] == 800);
    CA_ASSERT(rd == 8);
    CA_ASSERT(a.Size() == 0);
    CA_ASSERT(a.Capacity() == 16);
    CA_ASSERT(a.Empty() == true);
    CA_ASSERT(a.Good() == true);
}

void caCircularBuffer_test_class::test5(void)
{
    _START();
    _INFO("to check maxsize PUSH/POP procudere of caCircularBuffer");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();
    int buff[8];
    caCircularBuffer<int> a;
    a.Init(buff, sizeof (buff) / sizeof (int));
    CA_ASSERT(a.Size() == 0);
    CA_ASSERT(a.Capacity() == 8);
    CA_ASSERT(a.Empty() == true);
    CA_ASSERT(a.Good() == true);
    int buff_a[8] = {100, 200, 300, 400, 500, 600, 700, 800};
    s_t wr;
    CA_ASSERT(a.Push(buff_a, 8, wr) == true);
    CA_ASSERT(a.Size() == 8);
    CA_ASSERT(wr == 8);
    CA_ASSERT(a.Capacity() == 8);
    CA_ASSERT(a.Good() == false);
    CA_ASSERT(a.Empty() == false);
    int v[8];
    s_t rd;
    CA_ASSERT(a.Pop(&v[0], 8, rd) == true);
    CA_ASSERT(v[0] == 100);
    CA_ASSERT(v[1] == 200);
    CA_ASSERT(v[2] == 300);
    CA_ASSERT(v[3] == 400);
    CA_ASSERT(v[4] == 500);
    CA_ASSERT(v[5] == 600);
    CA_ASSERT(v[6] == 700);
    CA_ASSERT(v[7] == 800);
    CA_ASSERT(rd == 8);
    CA_ASSERT(a.Size() == 0);
    CA_ASSERT(a.Capacity() == 8);
    CA_ASSERT(a.Empty() == true);
    CA_ASSERT(a.Good() == true);
    // retry from postion = start=stop=8
    CA_ASSERT(a.Push(buff_a, 8, wr) == true);
    CA_ASSERT(a.Size() == 8);
    CA_ASSERT(wr == 8);
    CA_ASSERT(a.Capacity() == 8);
    CA_ASSERT(a.Good() == false);
    CA_ASSERT(a.Empty() == false);
    CA_ASSERT(a.Pop(&v[0], 8, rd) == true);
    CA_ASSERT(v[0] == 100);
    CA_ASSERT(v[1] == 200);
    CA_ASSERT(v[2] == 300);
    CA_ASSERT(v[3] == 400);
    CA_ASSERT(v[4] == 500);
    CA_ASSERT(v[5] == 600);
    CA_ASSERT(v[6] == 700);
    CA_ASSERT(v[7] == 800);
    CA_ASSERT(rd == 8);
    CA_ASSERT(a.Size() == 0);
    CA_ASSERT(a.Capacity() == 8);
    CA_ASSERT(a.Empty() == true);
    CA_ASSERT(a.Good() == true);
}

void caCircularBuffer_test_class::test6(void)
{
    _START();
    _INFO("to check owerwrite procudere of caCircularBuffer");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();
    int buff[6];
    caCircularBuffer<int> a;
    a.Init(buff, sizeof (buff) / sizeof (int));
    CA_ASSERT(a.Size() == 0);
    CA_ASSERT(a.Capacity() == 6);
    CA_ASSERT(a.Empty() == true);
    CA_ASSERT(a.Good() == true);
    int buff_a[8] = {100, 200, 300, 400, 500, 600, 700, 800};
    s_t wr;
    CA_ASSERT(a.Push(buff_a, 8, wr) == false);
    CA_ASSERT(a.Size() == 6);
    CA_ASSERT(wr == 6);
    CA_ASSERT(a.Capacity() == 6);
    CA_ASSERT(a.Good() == false);
    CA_ASSERT(a.Empty() == false);
    int v[8];
    memset(v, 0, 8 * sizeof (int));
    s_t rd;
    CA_ASSERT(a.Pop(&v[0], 8, rd) == false);
    CA_ASSERT(v[0] == 100);
    CA_ASSERT(v[1] == 200);
    CA_ASSERT(v[2] == 300);
    CA_ASSERT(v[3] == 400);
    CA_ASSERT(v[4] == 500);
    CA_ASSERT(v[5] == 600);
    CA_ASSERT(v[6] == 0);
    CA_ASSERT(v[7] == 0);
    CA_ASSERT(rd == 6);
    CA_ASSERT(a.Size() == 0);
    CA_ASSERT(a.Capacity() == 6);
    CA_ASSERT(a.Empty() == true);
    CA_ASSERT(a.Good() == true);
}
