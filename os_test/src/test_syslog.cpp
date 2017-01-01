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
#include "syslog.h"
#include "memory.h"

class caSysLog_test_class
: public caTester
{
    CA_TEST_SUITE(caSysLog_test_class);
    CA_TEST(caSysLog_test_class::test1, "Init test");
    CA_TEST(caSysLog_test_class::test2, "stream test");
    CA_TEST_SUITE_END();

    void setUp(void)
    {
        caMemory::Init();
    }
    void test1(void);
    void test2(void);


    void tearDown(void)
    {
    }

};

REGISTER_CLASS(caSysLog_test_class);

void caSysLog_test_class::test1(void)
{
    _START();
    _INFO("to check Init/ destroy function of caSysLog");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();
    caSysLog a;
    u32 res=a.Init(100);    
    CA_ASSERT(res==TRUE);
    CA_ASSERT(a.GetStream().Str()!=NULL);
    CA_ASSERT(a.GetCBuffer().GetBase()!=NULL);
    CA_ASSERT(caMemory::Find(a.GetStream().Str())>2048);
    CA_ASSERT(caMemory::Find(a.GetCBuffer().GetBase())>100);
    res=a.Destroy();
    CA_ASSERT(res==TRUE);
    CA_ASSERT(a.GetStream().Str()==NULL);
    CA_ASSERT(a.GetCBuffer().GetBase()==NULL);
}


void caSysLog_test_class::test2(void)
{
    _START();
    _INFO("to check stream functions of caSysLog");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();
    caSysLog caLog;
    u32 res=caLog.Init(100);    
    CA_ASSERT(res==TRUE);
    CA_ASSERT(caLog.GetStream().Str()!=NULL);
    CA_ASSERT(caLog.GetCBuffer().GetBase()!=NULL);
    CA_ASSERT(caMemory::Find(caLog.GetStream().Str())>2048);
    CA_ASSERT(caMemory::Find(caLog.GetCBuffer().GetBase())>100);    
    res=caLog.Destroy();
    CA_ASSERT(res==TRUE);
    CA_ASSERT(caLog.GetStream().Str()==NULL);
    CA_ASSERT(caLog.GetCBuffer().GetBase()==NULL);

}