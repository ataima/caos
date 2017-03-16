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
#include "memory.h"
#include "memaux.h"
#include "kdebug.h"
#include "caos.h"
#include "console.h"

u32 mainTask(u32 /*thIdx*/, u32 /*p1*/, u32/*p2*/) {
    u32 st = 0;
    //u32 counter=0;
    hal_llc_reset_req.hll_leds_off();
    while (1) {
        //Dbg::Put("Main : ",counter++);
        if (st) {
            hal_llc_reset_req.hll_led_off(1);
            hal_llc_reset_req.hll_led_on(2);
        } else {
            hal_llc_reset_req.hll_led_off(2);
            hal_llc_reset_req.hll_led_on(1);
        }
        st = !st;
        caScheduler::Sleep(250);
    };
    return 0;
}

#define CONS_LINE_LENGHT 1024

u32 consoleTask(u32 thIdx, u32 /*p1*/, u32/*p2*/) {
    u8 buff_in[CONS_LINE_LENGHT];
    caComDeviceConfigure in;
    caDeviceHandle port;
    deviceError res;
    caStringStream<s8> tt;
    caConsole::Init();
    in.speed = 115200;
    in.data = 8;
    in.parity = 0;
    in.stop = 1;
    res = caOS::Open("TTY1", in, port);
    if (res == deviceError::no_error) {
        caComDeviceCtrl comCtrl;
        comCtrl.command = caComDeviceCtrl::IoComCtrlDirect::comAddSignalRx;
        comCtrl.params[0] = thIdx;
        res = caOS::IoCtrl(port, comCtrl);
        if (res == deviceError::no_error) {
            while (1) {
                port.rdBuff = buff_in;
                port.readed = 0;
                do {
                    port.rdSize = 1;
                    caScheduler::WaitForSignal();
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
            Dbg::Put("Cannot Register Rx signalling\r\n",res);
        }
    } else {
        Dbg::Put("Cannot open com1\r\n",res);
    }   
    return 0;
}

int hal_main(void) {
    caOS::Init();
    caScheduler::Init(caSchedulerMode::Priority);
    caScheduler::AddSystemJob("main",
            caJobPriority::caThLevel3,
            mainTask);
    caScheduler::AddJob("console",
            caJobPriority::caThLevel6,
            consoleTask);
    hal_llc_time_1.hll_start();
    hal_llc_int_req.hll_wait_for_interrupt();
    caScheduler::RemoveAllJobs();
    hal_llc_time_1.hll_stop();
    return 0;
}
