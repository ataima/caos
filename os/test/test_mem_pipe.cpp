#include "config.h"
#include "caos_c_types.h"
#include "test.h"

#if MEM_PIPE_DEVICE   && TEST
#if TEST_MEM_PIPE  
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


static u32 loop1 = 0;
static u32 loop2 = 0;


extern u32 start_system_timer(void);
extern u32 stop_system_timer(void);

static u32 sourceTask(u32, u32) {
    TIN();
    caMemDeviceConfigure in1, in2;
    caDevicePort port1, port2;
    caMemAux::MemCpy((u32*) in1.name, (u32*) "pippoSource\0", 3);
    in1.size = 1024 * 1024 * 32; //1M
    in1.host_guest = caMemDeviceConfigure::requestMode::host;
    deviceError res;
    u8 buff[1000];
    caStringStream<u8> ss;
    ss.Init(buff, 1000);
    res = caOS::Open("MEMPIPE", in1, port1);
    Dbg::Put("OPEN HOST SOURCE TASK =", res);
    if (res == deviceError::no_error) {
        caMemAux::MemCpy((u32*) in2.name, (u32*) "pippoDest\0\0\0", 3);
        in2.host_guest = caMemDeviceConfigure::requestMode::guest;
        in2.size = 0;
        do {
            Sleep(1000);
            res = caOS::Open("MEMPIPE", in2, port2);
            Dbg::Put("OPEN GUEST SOURCE TASK =", res);
        } while (res != deviceError::no_error);
    }
    if (res == deviceError::no_error) {
        while (loop1 < 100) {
            ss << (u32) caScheduler::GetCurrentContext() <<
                    " - Source TASK : write on Mem Pipe " << ss.Endl(port1);
            port2.rdBuff = (u8*) buff;
            port2.readed = 0;
            port2.rdSize = 30;
            while (port2.readed < 30) {
                res = caOS::Read(port2);
                Sleep(10);
            }
            Dbg::Put((const char *) buff);
            loop1++;
        }
    }
    res = caOS::Close(port1);
    Dbg::Put("CLOSE  SOURCE TASK =", res);
    res = caOS::Close(port2);
    Dbg::Put("CLOSE  SOURCE TASK =", res);
    TOUT();
    return 0;
}

static u32 destTask(u32, u32) {
    Sleep(10);

    TIN();
    caMemDeviceConfigure in1, in2;
    caDevicePort port1, port2;
    caMemAux::MemCpy((u32*) in1.name, (u32*) "pippoSource\0", 3);
    in1.host_guest = caMemDeviceConfigure::requestMode::guest;
    in1.size = 0;
    deviceError res;
    u8 buff_in[1000];
    caStringStream<u8> ss;
    ss.Init(buff_in, 1000);
    do {
        Sleep(1000);
        res = caOS::Open("MEMPIPE", in1, port1);
        Dbg::Put("OPEN GUEST DEST TASK =", res);
    } while (res != deviceError::no_error);
    if (res == deviceError::no_error) {
        caMemAux::MemCpy((u32*) in2.name, (u32*) "pippoDest\0\0\0", 3);
        in2.size = 1024 * 1024 * 32; //1M
        in2.host_guest = caMemDeviceConfigure::requestMode::host;
        res = caOS::Open("MEMPIPE", in2, port2);
        Dbg::Put("OPEN HOST DEST TASK =", res);
    }
    if (res == deviceError::no_error) {
        while (loop2 < 100) {
            port1.rdBuff = (u8*) buff_in;
            port1.readed = 0;
            port1.rdSize = 30;
            while (port1.readed < 30) {
                res = caOS::Read(port1);
                Sleep(10);
            }
            Dbg::Put((const char *) buff_in);
            ss << (u32) caScheduler::GetCurrentContext() <<
                    " - Dest TASK : write on Mem Pipe " << ss.Endl(port2);
            loop2++;
        }
    }
    res = caOS::Close(port1);
    Dbg::Put("CLOSE DEST TASK =", res);
    res = caOS::Close(port2);
    Dbg::Put("CLOSE DEST TASK =", res);
    TOUT();
    return 0;
}

static void test_01(u32 & success, u32 &failed) {
    //TEST MEM PIPE      
    u32 th1 = caThread::CreateThread("source",
            caThreadPriority::caThLevel6,
            sourceTask);
    u32 th2 = caThread::CreateThread("dest",
            caThreadPriority::caThLevel6,
            destTask);
    start_system_timer();
    //
    while (1) {
        //leave this throw irq/fiq scheduler interrupt
    };
    stop_system_timer();
    caThread::DestroyThread(th1);
    caThread::DestroyThread(th2);
    if (loop1 == 100 && loop2 == 100)
        success++;
    else
        failed++;
}

void test_mem_pipe(u32 & success, u32 &failed) {
    test_01(success, failed);
}


#endif
#endif
