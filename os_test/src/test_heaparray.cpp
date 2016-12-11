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
#include "heaparray.h"

class caHeapArray_test_class
: public caTester
{
    CA_TEST_SUITE(caHeapArray_test_class);
    CA_TEST(caHeapArray_test_class::test1, "ctor test");
    CA_TEST_SUITE_END();
    void setUp(void)    {
    }
    void test1(void);

    void tearDown(void){
    }

};

REGISTER_CLASS(caHeapArray_test_class);

void caHeapArray_test_class::test1(void)
{
    _START();
    _INFO("to check ctor of caHeapArray");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();  
}