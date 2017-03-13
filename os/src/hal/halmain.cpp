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


u32 mainTask(u32 /*thIdx*/, u32 /*p1*/, u32/*p2*/) {
    u32 st = 0;
    u32 counter=0;
    hal_llc_reset_req.hll_leds_off();
    while (1) {
        Dbg::Put("Main : ",counter++);
        if (st) {
            hal_llc_reset_req.hll_led_off(1);
            hal_llc_reset_req.hll_led_on(2);
        } else {
            hal_llc_reset_req.hll_led_off(2);
            hal_llc_reset_req.hll_led_on(1);
        }
        st = !st;
        caScheduler::Sleep(1000);
    };
    return 0;
}



int hal_main(void) {
    caOS::Init();
    caScheduler::Init(caSchedulerMode::Priority);   
    caScheduler::AddSystemJob("main",
            caJobPriority::caThLevel3,
            mainTask);
    //caScheduler::AddJob("TTY",
    //        caJobPriority::caThLevel6,
    //        consoleTask );
    hal_llc_time_1.hll_start();
    hal_llc_int_req.hll_wait_for_interrupt();
    hal_llc_time_1.hll_stop();
    caScheduler::RemoveAllJobs();
    return 0;
}
