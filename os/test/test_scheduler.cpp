#include "config.h"
#include "caos_c_types.h"
#include "test.h"

#if TEST_SCHEDULER    && TEST 
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


static s8 buff[1024];

static u32 nullTask(u32, u32) {
    return 0;
}

static u32 mainTask(u32, u32) {
    return 0;
}

static u32 consoleTask(u32, u32) {
    return 0;
}

static void test_01(u32 & success, u32 &failed) {
    if (caScheduler::RemoveAllTask()) {
        success++;
    } else {
        caStringStream<s8> ss;
        ss.Init(buff, 1024);
        failed++;
        ss << "TEST SCHEDULER : FAILED TEST 01 " << caEnd::endl;
        Dbg::Put((const char *) ss.Str());
    }
}

static void test_02(u32 & success, u32 &failed) {
    caStringStream<s8> ss;
    ss.Init(buff, 1024);
    if (caScheduler::Init(caSchedulerMode::RoundRobin)) {
        success++;
        u32 t1 = caThread::CreateSystemThread("main",
                caThreadPriority::caThLevel3,
                mainTask);
        if (t1 == (u32) (-1)) {
            failed++;
            ss.Clear();
            ss << "TEST SCHEDULER : FAILED TEST 02:01 " << caEnd::endl;
            Dbg::Put((const char *) ss.Str());
        } else {
            success++;
        }

        u32 t2 = caThread::CreateThread("TTY",
                caThreadPriority::caThLevel6,
                consoleTask, 0, 0, 0x4000);
        if (t2 == (u32) (-1)) {
            failed++;
            ss.Clear();
            ss << "TEST SCHEDULER : FAILED TEST 02:02 " << caEnd::endl;
            Dbg::Put((const char *) ss.Str());
        } else {
            success++;
        }
        u32 t3 = caThread::CreateThread("idle",
                caThreadPriority::caThLevel0,
                nullTask);
        if (t3 == (u32) (-1)) {
            failed++;
            ss.Clear();
            ss << "TEST SCHEDULER : FAILED TEST 02:03 " << caEnd::endl;
            Dbg::Put((const char *) ss.Str());
        } else {
            success++;
        }
        u32 s = caScheduler::Size();
        if (s != 3) {
            failed++;
            ss.Clear();
            ss << "TEST SCHEDULER : FAILED TEST 02:04 " << caEnd::endl;
            Dbg::Put((const char *) ss.Str());
        } else {
            success++;
            caScheduler::GetNextContext();
            caThreadContext *ctx = caScheduler::GetCurrentContext();
            ss.Clear();
            ss << "CTX: " << (const char *) ctx->name << caEnd::endl;
            Dbg::Put((const char *) ss.Str());
            if (ctx->index != 0) {
                failed++;
                ss.Clear();
                ss << "TEST SCHEDULER : FAILED TEST 02:05 " << caEnd::endl;
                Dbg::Put((const char *) ss.Str());
            } else {
                success++;
                caScheduler::GetNextContext();
                ctx = caScheduler::GetCurrentContext();
                ss.Clear();
                ss << "CTX: " << (const char *) ctx->name << caEnd::endl;
                Dbg::Put((const char *) ss.Str());
                if (ctx->index != 1) {
                    failed++;
                    ss.Clear();
                    ss << "TEST SCHEDULER : FAILED TEST 02:06 " << caEnd::endl;
                    Dbg::Put((const char *) ss.Str());
                } else {
                    success++;
                    caScheduler::GetNextContext();
                    ctx = caScheduler::GetCurrentContext();
                    ss.Clear();
                    ss << "CTX: " << (const char *) ctx->name << caEnd::endl;
                    Dbg::Put((const char *) ss.Str());
                    if (ctx->index != 2) {
                        failed++;
                        ss.Clear();
                        ss << "TEST SCHEDULER : FAILED TEST 02:07 " << caEnd::endl;
                        Dbg::Put((const char *) ss.Str());
                    } else {
                        success++;
                        caScheduler::GetNextContext();
                        ctx = caScheduler::GetCurrentContext();
                        ss.Clear();
                        ss << "CTX: " << (const char *) ctx->name << caEnd::endl;
                        Dbg::Put((const char *) ss.Str());
                        if (ctx->index != 0) {
                            failed++;
                            ss.Clear();
                            ss << "TEST SCHEDULER : FAILED TEST 02:08 " << caEnd::endl;
                            Dbg::Put((const char *) ss.Str());
                        } else {
                            success++;
                        }
                    }
                }
            }
        }
    } else {
        failed++;
        ss.Clear();
        ss << "TEST SCHEDULER : FAILED TEST 02:09 " << caEnd::endl;
        Dbg::Put((const char *) ss.Str());
    }
}

static void test_03(u32 & success, u32 &failed) {
    caStringStream<s8> ss;
    ss.Init(buff, 1024);
    if (caScheduler::Init(caSchedulerMode::Priority)) {
        success++;
        u32 t1 = caThread::CreateSystemThread("main",
                caThreadPriority::caThLevel3,
                mainTask);
        if (t1 == (u32) (-1)) {
            failed++;
            ss.Clear();
            ss << "TEST SCHEDULER : FAILED TEST 03:01 " << caEnd::endl;
            Dbg::Put((const char *) ss.Str());
        } else {
            success++;
        }
        u32 t2 = caThread::CreateThread("TTY",
                caThreadPriority::caThLevel6,
                consoleTask, 0, 0, 0x4000);
        if (t2 == (u32) (-1)) {
            failed++;
            ss.Clear();
            ss << "TEST SCHEDULER : FAILED TEST 03:02 " << caEnd::endl;
            Dbg::Put((const char *) ss.Str());
        } else {
            success++;
        }
        u32 t3 = caThread::CreateThread("idle",
                caThreadPriority::caThLevel0,
                nullTask);
        if (t3 == (u32) (-1)) {
            failed++;
            ss.Clear();
            ss << "TEST SCHEDULER : FAILED TEST 03:03 " << caEnd::endl;
            Dbg::Put((const char *) ss.Str());
        } else {
            success++;
        }
        u32 s = caScheduler::Size();
        if (s != 3) {
            failed++;
            ss.Clear();
            ss << "TEST SCHEDULER : FAILED TEST 03:04 " << caEnd::endl;
            Dbg::Put((const char *) ss.Str());
        } else {
            success++;
            caScheduler::GetNextContext();
            caThreadContext *ctx = caScheduler::GetCurrentContext();
            ss.Clear();
            ss << "CTX: " << (const char *) ctx->name << caEnd::endl;
            Dbg::Put((const char *) ss.Str());
            if (ctx->index != 1) {
                failed++;
                ss.Clear();
                ss << "TEST SCHEDULER : FAILED TEST 03:05 " << caEnd::endl;
                Dbg::Put((const char *) ss.Str());
            } else {
                success++;
                caScheduler::GetNextContext();
                ctx = caScheduler::GetCurrentContext();
                ss.Clear();
                ss << "CTX: " << (const char *) ctx->name << caEnd::endl;
                Dbg::Put((const char *) ss.Str());
                if (ctx->index != 0) {
                    failed++;
                    ss.Clear();
                    ss << "TEST SCHEDULER : FAILED TEST 03:06 " << caEnd::endl;
                    Dbg::Put((const char *) ss.Str());
                } else {
                    success++;
                    caScheduler::GetNextContext();
                    ctx = caScheduler::GetCurrentContext();
                    ss.Clear();
                    ss << "CTX: " << (const char *) ctx->name << caEnd::endl;
                    Dbg::Put((const char *) ss.Str());
                    if (ctx->index != 1) {
                        failed++;
                        ss.Clear();
                        ss << "TEST SCHEDULER : FAILED TEST 03:07 " << caEnd::endl;
                        Dbg::Put((const char *) ss.Str());
                    } else {
                        success++;
                        caScheduler::GetNextContext();
                        ctx = caScheduler::GetCurrentContext();
                        ss.Clear();
                        ss << "CTX: " << (const char *) ctx->name << caEnd::endl;
                        Dbg::Put((const char *) ss.Str());
                        if (ctx->index != 0) {
                            failed++;
                            ss << "TEST SCHEDULER : FAILED TEST 03:08 " << caEnd::endl;
                            Dbg::Put((const char *) ss.Str());
                        } else {
                            success++;
                        }
                    }
                }
            }
        }
    } else {
        failed++;
        ss << "TEST SCHEDULER : FAILED TEST 03:09 " << caEnd::endl;
        Dbg::Put((const char *) ss.Str());
    }

}

void test_scheduler(u32 &success, u32 & failed) {
    test_01(success, failed);
    test_02(success, failed);
    test_01(success, failed);
    test_03(success, failed);
}



#endif