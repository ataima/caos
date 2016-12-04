#include "config.h"
#include "config.h"
#include "test.h"
#if TEST_LOCKING && TEST

#include "bcm2836.h"
#include "idevice.h"
#include "stream.h"
#include "auxmain.h"
#include "gpio.h"
#include "interrupt.h"
#include "atomiclock.h"
#include "miniuart.h"

static void test_01(u32 & success, u32 & /*&failed*/) {
    s8 buff[1024];
    caStringStream<s8> ss;
    ss.Init(buff, sizeof (buff));
    caAtomicLock aa;
    ss << "1- " << aa.Get() << caEnd::endl;
    Dbg::Put((const char *) ss.Str());
    success++;
}

static void test_02(u32 & success, u32 &failed) {
    s8 buff[1024];
    caStringStream<s8> ss;
    ss.Init(buff, sizeof (buff));
    caAtomicLock aa;
    ss << "3- " << aa.Get() << caEnd::endl;
    Dbg::Put((const char *) ss.Str());
    if (aa.Lock() == 1) {
        success++;
    } else {
        failed++;
        ss.Clear();
        ss << "TEST LOCKING : FAILED TEST 02:01 " << caEnd::endl;
        Dbg::Put((const char *) ss.Str());
    }
    ss.Clear();
    ss << "4- " << aa.Get() << caEnd::endl;
    Dbg::Put((const char *) ss.Str());
}

static void test_03(u32 & success, u32 &failed) {
    s8 buff[1024];
    caStringStream<s8> ss;
    ss.Init(buff, sizeof (buff));
    caAtomicLock aa;
    ss << "6- " << aa.Get() << caEnd::endl;
    Dbg::Put((const char *) ss.Str());
    if (aa.Lock() == 1) {
        success++;
    } else {
        failed++;
        ss.Clear();
        ss << "TEST LOCKING : FAILED TEST 03:01 " << caEnd::endl;
        Dbg::Put((const char *) ss.Str());
    }
    ss.Clear();
    ss << "7- " << aa.Get() << caEnd::endl;
    Dbg::Put((const char *) ss.Str());
    if (aa.UnLock() == 1) {
        success++;
    } else {
        failed++;
        ss.Clear();
        ss << "TEST LOCKING : FAILED TEST 03:02 " << caEnd::endl;
        Dbg::Put((const char *) ss.Str());
    }
    ss.Clear();
    ss << "8- " << aa.Get() << caEnd::endl;
    Dbg::Put((const char *) ss.Str());
}

static void test_04(u32 & success, u32 &failed) {
    s8 buff[1024];
    caStringStream<s8> ss;
    ss.Init(buff, sizeof (buff));
    caAtomicLock aa;
    ss << "9- " << aa.Get() << caEnd::endl;
    Dbg::Put((const char *) ss.Str());
    if (aa.Lock() == 1) {
        success++;
    } else {
        failed++;
        ss.Clear();
        ss << "TEST LOCKING : FAILED TEST 04:01 " << caEnd::endl;
        Dbg::Put((const char *) ss.Str());
    }
    ss.Clear();
    ss << "10- " << aa.Get() << caEnd::endl;
    Dbg::Put((const char *) ss.Str());
    if (aa.Lock() == 0) {
        success++;
    } else {
        failed++;
        ss.Clear();
        ss << "TEST LOCKING : FAILED TEST 04:02 " << caEnd::endl;
        Dbg::Put((const char *) ss.Str());
    }
    ss.Clear();
    ss << "11- " << aa.Get() << caEnd::endl;
    Dbg::Put((const char *) ss.Str());
    ss.Clear();
    ss << "12- " << aa.Get() << caEnd::endl;
    Dbg::Put((const char *) ss.Str());
    if (aa.UnLock() == 1) {
        success++;
    } else {
        failed++;
        ss.Clear();
        ss << "TEST LOCKING : FAILED TEST 04:03 " << caEnd::endl;
        Dbg::Put((const char *) ss.Str());
    }
    ss.Clear();
    ss << "13- " << aa.Get() << caEnd::endl;
    Dbg::Put((const char *) ss.Str());
    if (aa.UnLock() == 0) {
        success++;
    } else {
        failed++;
        ss.Clear();
        ss << "TEST LOCKING : FAILED TEST 04:04 " << caEnd::endl;
        Dbg::Put((const char *) ss.Str());
    }
    ss.Clear();
    ss << "14- " << aa.Get() << caEnd::endl;
    Dbg::Put((const char *) ss.Str());
}

void test_locking(u32 & success, u32 &failed) {
    test_01(success, failed);
    test_02(success, failed);
    test_03(success, failed);
    test_04(success, failed);
}


#endif

