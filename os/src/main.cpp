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

#include "hal.h"
#include "bcm2836.h"
#include "sysleds.h"
#include "gpio.h"
#include "miniuart.h"
#include "interrupt.h"
#include "sysirqctrl.h"
#include "systimer.h"
#include "memory.h"
#include "cpu.h"
#include "thread.h"
#include "scheduler.h"
#include "schedulerdevice.h"
#include "console.h"
#include "comdevice.h"
#include "memaux.h"
#include "cache.h"
#include "test.h"
#include "cachedevice.h"
#include "syslog.h"
#include "softreq.h"
#include "caos.h"

u32 start_system_timer(void) {
    u32 res = FALSE;
    if (caSysTimer::Init(SYS_CLOCK_TIMER, SYS_TIMER_TICK)) {
        if (caSysTimer::EnableCounter(1)) {
            if (caSysTimer::EnableTimer(1)) {
                Dbg::Put("> c.a.O.S. : [ Start Schedule interrupt ... ]\r\n");
                res = caSysTimer::IrqEnable();
            }
        }
    }
    caArmCpu::EnableIrqFiq();
    return res;
}

u32 stop_system_timer(void) {
    u32 res = FALSE;
    if (caSysTimer::EnableCounter(0)) {
        if (caSysTimer::EnableTimer(0)) {
            res = caSysTimer::IrqDisable();
        }
    }
    caArmCpu::DisableIrqFiq();
    return res;
}

u32 nullTask(u32 /*thIdx*/, u32 /*p1*/, u32/*p2*/) {
    Dbg::Put("Idle live..\r\n");
    u32 idleCount = 1;
    for (;;) {
        idleCount++;
        //if (idleCount == 0)
        //Dbg::Put("Idle live..\r\n");
    }
}

u32 mainTask(u32 /*thIdx*/, u32 /*p1*/, u32/*p2*/) {
    Dbg::Put("Main live..\r\n");
    u32 st = 0;
    caSysLed::LedsOff();
    while (1) {
        if (st) {
            caSysLed::LedGreenOff();
            caSysLed::LedRedOn();
        } else {
            caSysLed::LedGreenOn();
            caSysLed::LedRedOff();
        }
        st = !st;
        Sleep(250);
    };
    return 0;
}

#define CONS_LINE_LENGHT 1024

u32 consoleTask(u32 thIdx, u32 /*p1*/, u32/*p2*/) {
    Dbg::Put("Console live..\r\n");
    u8 buff_in[CONS_LINE_LENGHT];
    caComDeviceConfigure in;
    caDeviceHandle port;
    deviceError res;
    caConsole::Init();
    in.speed = 115200;
    in.data = 8;
    in.parity = 0;
    in.stop = 1;
    res = caOS::Open("COM1", in, port);
    if (res == deviceError::no_error) {
        caComDeviceCtrl comCtrl;
        comCtrl.command = caComDeviceCtrl::IoCtrlDirect::comAddSignalRx;
        comCtrl.st_rx = thIdx;
        res = caOS::IoCtrl(port, comCtrl);
        if (res == deviceError::no_error) {
            while (1) {
                port.rdBuff = buff_in;
                port.readed = 0;
                do {
                    port.rdSize = 1;
                    WaitForSignal();
                    res = caOS::Read(port);
                    if (res == deviceError::no_error) {
                        if (port.readed > 0) {
                            //echo to sender
                            port.wrBuff = port.rdBuff - 1;
                            port.writed = 0;
                            port.wrSize = 1;
                            res = caOS::Write(port);
                        }
                        if (port.readed > 0 && buff_in[port.readed - 1] == '\r') {
                            port.wrBuff = (u8*) "\n";
                            port.writed = 0;
                            port.wrSize = 1;
                            res = caOS::Write(port);
                            break;
                        }
                    } else {
                        port.wrBuff[0] = '*';
                        port.writed = 0;
                        port.wrSize = 1;
                        res = caOS::Write(port);
                    }
                } while (port.readed < (CONS_LINE_LENGHT - 1));
                if (port.readed > 0) {
                    u32 endC = port.readed - 1;
                    buff_in[endC] = '\0';
                    caTokenizeSStream<u8> iss;
                    iss.Init(buff_in, endC, 0);
                    iss.Forward(endC);
                    caConsole::Execute(iss, port);
                }
            }
            res = caOS::Close(port);
        } else {
            Dbg::Put("Cannot Register Rx signalling\r\n");
        }
    }
    return 0;
}

int main(void) {
    s8 buff[256];
    u32 thMainTask;
    u32 thNullTask;
    u32 thConsoleTask;
    caIrqCtrl::Init();
    caMemory::Init();
    caMemory::DumpAvail(buff, sizeof (buff));
    Dbg::Put(buff);
    caArmCpu::GetMainIdCpuInfo();
#if CACHE_DEVICE    
    caCache::Start();
#endif
    msgSTART();
    caScheduler::Init(caSchedulerMode::Priority);

#if TEST
    tmain();
#endif    
    // MAIN TASK SVC MODE TO RUN  ALL SERVICES    
    thMainTask = caThread::CreateSystemThread("main",
            caThreadPriority::caThLevel3,
            mainTask);
    thConsoleTask = caThread::CreateThread("TTY",
            caThreadPriority::caThLevel6,
            consoleTask, 0, 0, 0x4000);
    // idle task always two task in scheduler : alway nullTASK running
    // will be  the priority scheduler to unswitcjh to null task with 
    // have the lowest priority of all threads: no cpu load for null task
    thNullTask = caThread::CreateThread("idle",
            caThreadPriority::caThLevel0,
            nullTask);
    // GO scheduler 
    start_system_timer();
    //

    //caInterruptRequest::WaitForInterrupt();
    while (1) {
        //leave this throw irq/fiq scheduler interrupt
        Dbg::Put("Wait\r\n");
    };
    //lbl_shutdown:
    stop_system_timer();
    caThread::DestroyThread(thMainTask);
    caThread::DestroyThread(thConsoleTask);
    caThread::DestroyThread(thNullTask);
    return 0;
}
