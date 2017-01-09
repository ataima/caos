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
#include "memaux.h"

class caStrAux_test_class
: public caTester
{
    CA_TEST_SUITE(caStrAux_test_class);
    CA_TEST(caStrAux_test_class::test1, "StrCmp  test");
    CA_TEST(caStrAux_test_class::test2, "MemCpy u16  test");
    CA_TEST(caStrAux_test_class::test3, "MemCpy u8  test");
    CA_TEST(caStrAux_test_class::test4, "MemSet u32 test");
    CA_TEST(caStrAux_test_class::test5, "MemSet u16  test");
    CA_TEST(caStrAux_test_class::test6, "MemSet u8  test");
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

REGISTER_CLASS(caStrAux_test_class);

void caStrAux_test_class::test1(void)
{
    _START();
    _INFO("to check StrCmp  of caStrAux");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();
    const s8* p1 = "pippo";
    const s8* p2 = "pippo";
    const s8* p3 = "pippu";
    const s8* p4 = "pi";

    CA_ASSERT(caStrAux::StrCmp(p1, p2) == 0);
    CA_ASSERT(caStrAux::StrCmp(p1, p3) == (u32) ('o' - 'u'));
    CA_ASSERT(caStrAux::StrCmp(p1, p4) != 0);
}

void caStrAux_test_class::test2(void)
{
    _START();
    _INFO("to check MemCpy u16 of caStrAux");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();
    const s8* p1 = "pippopippo";
    const s8* p2 = "pippoaippo";
    CA_ASSERT(caStrAux::StrNCmp(p1, p2, 5) == 0);
    CA_ASSERT(caStrAux::StrNCmp(p1, p2, 6) != 0);

}

void caStrAux_test_class::test3(void)
{
    _START();
    _INFO("to check MemCpy u8 of caStrAux");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();
    const s8* p1 = "pippo";
    const s8* p2 = "PIPPO";
    const s8* p3 = "PiPPu";
    const s8* p4 = "pI";

    CA_ASSERT(caStrAux::StrICmp(p1, p2) == 0);
    CA_ASSERT(caStrAux::StrICmp(p1, p3) == (u32) ('O' - 'U'));
    CA_ASSERT(caStrAux::StrICmp(p1, p4) != 0);
}

void caStrAux_test_class::test4(void)
{
    _START();
    _INFO("to check MemSet u32 of caStrAux");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();
    const s8* p1 = "pippo";
    s8 dest[10];
    CA_ASSERT(caStrAux::StrCpy(dest, p1) == &dest[5]);
    CA_ASSERT(memcmp(dest, p1, 6) == 0);
}

void caStrAux_test_class::test5(void)
{
    _START();
    _INFO("to check MemSet u16 of caStrAux");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();
    const s8* p1 = "pippopippopippo";
    s8 dest[10];
    CA_ASSERT(caStrAux::StrNCpy(dest, p1, sizeof (dest)) == &dest[10]);
    u32 res =memcmp(dest, p1, 10);
    CA_ASSERT( res== 0);
}

void caStrAux_test_class::test6(void)
{
    _START();
    _INFO("to check MemSet u8 of caStrAux");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();
    const s8* p1 = "pippopippopippo";
    CA_ASSERT(caStrAux::StrLen(p1) == strlen(p1));
}
