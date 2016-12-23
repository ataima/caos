#include "config.h"
#include "caos_c_types.h"
#include "test.h"

#if SYS_TIMER_DEVICE && TEST
#if TEST_SYSTIMER_DEVICE
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


extern void DumpDevicePort(caDevicePort & port, caStringStream<s8> & ss);


static u32 idlelow = 0;
static u32 idlehigh = 0;
static u32 loop1 = 0;
static u32 loop2 = 0;
static caDevicePort ttyport;

extern u32 start_system_timer(void);
extern u32 stop_system_timer(void);

static u32 Task1(u32, u32) {
    Sleep(10);
    TIN();
    caSysTimerDeviceConfigure in;
    caDevicePort port;
    deviceError res;
    sysTimerStatus st;
    s8 buff[1000];
    caStringStream<s8> ss;
    ss.Init(buff, sizeof (buff));
    ss << "TTY = ";
    DumpDevicePort(ttyport, ss);
    ss << "END " << ss.Endl(ttyport);
    res = caOS::Open("SYSTIMER", in, port);
    if (res == deviceError::no_error) {
        while (loop1 < 100) {
            port.rdBuff = (u8*) & st;
            port.readed = 0;
            port.rdSize = sizeof (sysTimerStatus);
            res = caOS::Read(port);
            ss.Clear();
            ss << "PORT = ";
            DumpDevicePort(port, ss);
            ss << "END " << ss.Endl(ttyport);
            if (port.readed == sizeof (sysTimerStatus)) {
                ss.Clear();
                st.Dump(ss);
                ss << "loop1 = " << loop1 << ss.Endl(ttyport);
            }
            loop1++;
            caMiniUart::Recv();
        }
    }
    ss << "TRY CLOSE  TASK1 =" << res << ss.Endl(ttyport);
    res = caOS::Close(port);
    ss << "CLOSE  TASK1 =" << res << ss.Endl(ttyport);
    TOUT();
    return 0;
}

static u32 Idle(u32, u32) {
    idlelow = idlehigh = 0;
    while (1) {
        idlelow++;
        if (idlelow == 0)
            idlehigh++;
    }
}

static void test_01(u32 & success, u32 &failed) {
    //TEST MEM PIPE  
    caComDeviceConfigure in;
    deviceError res;
    in.speed = 115200;
    in.data = 8;
    in.parity = 0;
    in.stop = 1;
    res = caOS::Open("COM1", in, ttyport);
    if (res == deviceError::no_error) {

        u32 th1 = caThread::CreateThread("reader1",
                caThreadPriority::caThLevel2,
                Task1);
        u32 th3 = caThread::CreateThread("iddle",
                caThreadPriority::caThLevel2,
                Idle);
        start_system_timer();
        //
        while (1) {
            //leave this throw irq/fiq scheduler interrupt
        };
        stop_system_timer();
        caThread::DestroyThread(th1);
        caThread::DestroyThread(th3);
        res = caOS::Close(ttyport);
    }
    if (loop1 == 100 && loop2 == 100)
        success++;
    else
        failed++;
    Dbg::Put("IDDLE COUNT LOW=", idlelow);
    Dbg::Put("IDDLE COUNT HIGH=", idlehigh);
}

void test_systimer_device(u32 & success, u32 &failed) {
    test_01(success, failed);
}


#endif
#endif
