#include "config.h"
#include "caos_c_types.h"
#include "test.h"
#if TEST_COM1_DEVICE && TEST

#include "bcm2836.h"
#include "idevice.h"
#include "stream.h"
#include "auxmain.h"
#include "gpio.h"
#include "interrupt.h"
#include "miniuart.h"
#include "memory.h"
#include "memaux.h"
#include "thread.h"
#include "scheduler.h"
#include "systimer.h"
#include "systimerdevice.h"
#include "comdevice.h"
#include "atomiclock.h"

extern void DumpDevicePort(caDevicePort & port, caStringStream<s8> & ss);


volatile u32 idlelow = 0;
volatile u32 idlehigh = 0;
volatile u32 loop1 = 0;
static caAtomicLock lock;

extern u32 start_system_timer(void);
extern u32 stop_system_timer(void);

u32 Task1(u32 /*p1*/, u32 /*p2*/) {
    Dbg::Put("TASK 1 Enter\r\n");
    caComDeviceConfigure in;
    caDevicePort port;
    deviceError res;
    Sleep(1);
    in.speed = 115200;
    in.data = 8;
    in.parity = 0;
    in.stop = 1;
    res = caOS::Open("COM1", in, port);
    Sleep(10);
    Dbg::Put("caDevice::Open=", res);
    if (res == deviceError::no_error) {
        while (loop1 < 10) {
            const char *msg = "TH1 HELLO\r\n\0";
            port.wrBuff = (u8*) msg;
            port.wrSize = 11;
            res = caOS::Write(port);
            Sleep(10);
            loop1++;
        }
        const char *msg = "B CLOSE\r\n\0";
        port.wrBuff = (u8*) msg;
        port.wrSize = 9;
        res = caOS::Write(port);
        Sleep(20);
        res = caOS::Close(port);
        Sleep(100);
        Dbg::Put("Close=", res);
    } else {
        Dbg::Put("ERROR ON OPEN COM1\r\n");
    }
    Dbg::Put("Call UnloCk\r\n");
    lock.UnLock();
    Dbg::Put("Exit TASK 1\r\n");
    return 0;
}

u32 Task3(u32 p1, u32 p2) {
    Dbg::Put("TASK ENTER=", p1);
    caComDeviceConfigure in;
    caDevicePort port;
    deviceError res;
    Sleep(1);
    s8 buff[1024];
    caStringStream<s8> ss;
    ss.Init(buff, sizeof (buff));
    u32 loop = 0;
    in.speed = 115200;
    in.data = 8;
    in.parity = 0;
    in.stop = 1;
    res = caOS::Open("COM1", in, port);
    Sleep(10);
    if (res == deviceError::no_error) {
        while (loop < 100) {
            ss.Clear();
            ss << "TASK " << p1 << " HELLO !!!!" << ss.Endl(port);
            Sleep(10);
            loop++;
        }
        res = caOS::Close(port);
        Sleep(100);
    } else {
        Dbg::Put("ERROR ON OPEN COM1\r\n");
    }
    Dbg::Put("Close=", p1);
    return 0;
}

u32 Task2(u32, u32) {
    TIN();
    caComDeviceConfigure in;
    caDevicePort port;
    deviceError res;
    s8 buff[1024];
    caStringStream<s8> ss;
    ss.Init(buff, sizeof (buff));
    in.speed = 115200;
    in.data = 8;
    in.parity = 0;
    in.stop = 1;
    res = caOS::Open("COM1", in, port);
    ss.Clear();
    //DumpDevicePort(port, ss);
    //ss << "Port:" << res << ss.Endl(port);
    if (res == deviceError::no_error) {
        while (loop1 < 10) {
            ss.Clear();
            ss << "TH2 HELLO !!!!" << ss.Endl(port);
            Sleep(10);
            loop1++;
        }
    }
    res = caOS::Close(port);
    TOUT();
    lock.UnLock();
    return 0;
}

u32 Idle(u32, u32) {
    idlelow = idlehigh = 0;
    Dbg::Put("IDLE ENTER\r\n");
    while (lock.Lock() == 0) {
        idlelow++;
        if (idlelow == 0)
            idlehigh++;
    }
    lock.UnLock();
    Dbg::Put("IDLE EXIT\r\n");
    return 0;
}

static void test_02(u32 & success, u32 &failed) {
    lock.Lock();
    u32 th0 = caThread::CreateThread("iddle",
            caJobPriority::caThLevel1,
            Idle);

    u32 th1 = caThread::CreateThread("tty1",
            caJobPriority::caThLevel4,
            Task1);
    Dbg::Put("th1 =", th1);
    Dbg::Put("th0 =", th0);
    Dbg::Put("lock =", lock.Get());

    start_system_timer();
    //

    while (lock.Get() != LOCK_FREE) {
        //leave this throw irq/fiq scheduler interrupt
    };
    stop_system_timer();
    Dbg::Put("IDDLE COUNT LOW=", idlelow);
    Dbg::Put("IDDLE COUNT HIGH=", idlehigh);
    caThread::DestroyThread(th1);
    caThread::DestroyThread(th0);
    if (loop1 == 10)
        success++;
    else
        failed++;
}

static void test_03(u32 & success, u32 &failed) {
    lock.Lock();
    u32 th2 = caThread::CreateThread("iddle",
            caJobPriority::caThLevel1,
            Idle, 0, 0, 0x4000);
    u32 th1 = caThread::CreateThread("tty1",
            caJobPriority::caThLevel6,
            Task1, 0, 0, 0x4000);
    u32 th3 = caThread::CreateThread("tty2",
            caJobPriority::caThLevel6,
            Task2, 0, 0, 0x4000);
    start_system_timer();
    while (lock.Get() != LOCK_FREE) {
        //leave this throw irq/fiq scheduler interrupt
    };
    stop_system_timer();
    Dbg::Put("IDDLE COUNT LOW=", idlelow);
    Dbg::Put("IDDLE COUNT HIGH=", idlehigh);
    caThread::DestroyThread(th1);
    caThread::DestroyThread(th2);
    caThread::DestroyThread(th3);

    if (loop1 == 100)
        success++;
    else
        failed++;
}

static void test_04(u32 & success, u32 &failed) {
    u32 i = 0;
    u32 th1, th2 = caThread::CreateThread("iddle",
            caJobPriority::caThLevel1,
            Idle, 0, 0, 0x4000);
    for (i = 0; i < 100; i++) {
        th1 = caThread::CreateThread("tty100",
                caJobPriority::caThLevel6,
                Task3, i);
    }
    start_system_timer();
    caInterruptRequest::WaitForInterrupt();
    stop_system_timer();
    Dbg::Put("IDDLE COUNT LOW=", idlelow);
    Dbg::Put("IDDLE COUNT HIGH=", idlehigh);
    caThread::DestroyThread(th2);
    for (i = 0; i < 100; i++)
        caThread::DestroyThread(th1 - i);

    if (loop1 == 100)
        success++;
    else
        failed++;
}

void test_com1_device(u32 & success, u32 &failed) {
    test_04(success, failed);
    test_02(success, failed);
    test_03(success, failed);
}


#endif
