
#include "CPPtester.h"
#include "docMacro.h"
#include "stream.h"


class caStringStream_test_class
    :public caTester, public caStringStream<char>
{
    CA_TEST_SUITE(caStringStream_test_class);
    CA_TEST(caStringStream_test_class::test1,"ctor test");
    CA_TEST_SUITE_END();
    void setUp(void);
    void test1(void);
    void tearDown(void);
};

REGISTER_CLASS(caStringStream_test_class);


void caStringStream_test_class::test1(void){
    _START();
    _INFO("to to check ctor of caStringStream");
    _AUTHOR("Coppi Angelo");
    _PROJECT("C.A.O.S");
    _STOP();
    CA_ASSERT(cBuff==NULL);
    CA_ASSERT(start==NULL);
    CA_ASSERT(stop==NULL);    
    CA_ASSERT(size==0);
    CA_ASSERT(mode_dec==false);
    CA_ASSERT(mode_hex==false);
    CA_ASSERT(mode_bin==false);

}
