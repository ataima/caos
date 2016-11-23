
#include "CPPtester.h"
#include "docMacro.h"
#include "arm_c_types.h"
#include "idevice.h"
#include "stream.h"



class caStringStream_test_class
    :public caTester
{
    CA_TEST_SUITE(caStringStream_test_class);
    CA_TEST(caStringStream_test_class::test1,"ctor test");
    CA_TEST(caStringStream_test_class::test2,"Init test");
    CA_TEST(caStringStream_test_class::test3,"<< s8 test");
    CA_TEST_SUITE_END();
    void setUp(void){}
    void test1(void);
    void test2(void);
    void test3(void);
    void tearDown(void){}
};

REGISTER_CLASS(caStringStream_test_class);


void caStringStream_test_class::test1(void){
    _START();
    _INFO("to to check ctor of caStringStream");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();
    caStringStream<char> a;
    CA_ASSERT(a.Size()==0);
    CA_ASSERT(a.Capacity()==0);
    CA_ASSERT(a.Str()==NULL);
    CA_ASSERT(a.Good()==false);
}


void caStringStream_test_class::test2(void){
    _START();
    _INFO("to to check Init of caStringStream");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();
    char buff[100];
    caStringStream<char> a;
    CA_ASSERT(a.Size()==0);
    CA_ASSERT(a.Capacity()==0);
    CA_ASSERT(a.Str()==NULL);
    CA_ASSERT(a.Good()==false);
    a.Init(buff,100);
    CA_ASSERT(a.Size()==0);
    CA_ASSERT(a.Capacity()==99);
    CA_ASSERT(a.Str()==buff);
    CA_ASSERT(a.Good()==true);
}

void caStringStream_test_class::test3(void){
    _START();
    _INFO("to to check Initoperator << s8 of caStringStream");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();
    char buff[100];
    caStringStream<char> a;
    a.Init(buff,100);
    CA_ASSERT(a.Size()==0);
    CA_ASSERT(a.Capacity()==99);
    CA_ASSERT(a.Str()==buff);
    CA_ASSERT(a.Good()==true);
    a<<'t';
    CA_ASSERT(a.Size()==1);
    CA_ASSERT(a.Capacity()==99);
    CA_ASSERT(a.Str()==buff);
    CA_ASSERT(a.Good()==true);
    CA_ASSERT(buff[0]=='t');
    a.Clear();
    CA_ASSERT(a.Size()==1);
    CA_ASSERT(a.Capacity()==99);
    CA_ASSERT(a.Str()==buff);
    CA_ASSERT(a.Good()==true);
}