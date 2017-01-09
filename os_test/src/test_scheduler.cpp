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

#include "CPPtester.h"
#include "docMacro.h"
#include "hal.h"
#include <iostream>



static struct timespec start_time = {0, 0};
static struct timespec c_spec = {0, 0};

static bool isvalidcontext(u32 num) {
    if (num == 12345678)
        return false;
    if (num == 0)
        return false;
    return true;
}

static u32 hll_time(void) {
    u32 us;
    struct timespec diff;
    clock_gettime(CLOCK_REALTIME, &c_spec);
    diff.tv_sec = (time_t) difftime(c_spec.tv_sec, start_time.tv_sec);
    if (c_spec.tv_nsec > start_time.tv_nsec)
        diff.tv_nsec = c_spec.tv_nsec - start_time.tv_nsec;
    else {
        c_spec.tv_sec--;
        diff.tv_nsec = c_spec.tv_nsec - start_time.tv_nsec + 1000000000;
    }
    us = (u32) (diff.tv_sec * 1000000 + diff.tv_nsec / 1000);
    if (us == 0)
        us = 1;
    return us;
}

static u32 hll_totick(u32 ms) {
    return 100 + ms;
}

static bool hll_enable_lock(void) {
    std::cout<<"SPINLOCK "<<std::endl;
    return true;
}

static bool hll_enable_unlock(void) {
    std::cout<<"SPINUNLOCK "<<std::endl;    
    return true;
}

static void hll_req_schedule(void) {
    std::cout<<"REQ SCHEDULER "<<std::endl;
}

hal_llc_scheduler_io hal_llc_scheduler = {
    hll_time,
    hll_totick,
    isvalidcontext,
    hll_enable_lock,
    hll_enable_unlock,
    hll_req_schedule
};

void start_time_scheduler(void) {
    clock_gettime(CLOCK_REALTIME, &start_time);
}
