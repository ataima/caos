#ifndef THREAD_CA_HEADER
#define THREAD_CA_HEADER

////////////////////////////////////////////////////////////////////////////////
//    Copyright (C) 2011  Angelo Coppi (angelogkcop at hotmail.com )
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


#include "bcm2836.h"
#include "cpu.h"

typedef enum tag_ca_thread_mode {
    MODE_USR = 0x10,
    MODE_FIQ = 0x11,
    MODE_IRQ = 0x12,
    MODE_SVC = 0x13,
    MODE_MON = 0x16,
    MODE_ABT = 0x17,
    MODE_HYP = 0x1A,
    MODE_UND = 0x1B,
    MODE_SYS = 0x1F,
} caThreadMode;

typedef enum tag_ca_thread_priority {
    caThLevel0 = 1,
    caThLevel1 = 2,
    caThLevel2 = 4,
    caThLevel3 = 8,
    caThLevel4 = 16,
    caThLevel5 = 32,
    caThLevel6 = 64,
} caThreadPriority;

typedef enum tag_ca_thread_status {
    thUnknow = 0,
    //thFreeze = 1,
    thStop = 2,
    thSleep = 4,
    thRemove = 8,
    thRun = 0x40,
    thInit = 0x80,
    thRunning = 0xc0
} caThreadStatus;

typedef struct tag_ca_thread_context {
    volatile u32 pcb[32];
    u32 thid;
    u32 index; //index over taskList array
    u32 stack_start;
    u32 stack_end;
    caThreadMode mode; // th pripority
    caThreadPriority priority; // th pripority
    volatile u32 cur_prio; // temporary priority from priority to lowest 
    volatile caThreadStatus status; // th status
    volatile u32 count;
    volatile u32 sleep;
    volatile u32 time;
    volatile u32 result;
    volatile u32 nswitch;
    char name[64];
} caThreadContext;

inline bool less(caThreadContext* a, caThreadContext* b) {
    if ((a != NULL) &&
            (b != NULL)/* &&
            ((a->status & caThreadStatus::thRunning)) &&
            ((b->status & caThreadStatus::thRunning))*/) {
        if (a->cur_prio == b->cur_prio)
            return (a->nswitch / a->priority) > (b->nswitch / b->priority);
        else
            return a->cur_prio < b->cur_prio;
    } else
        return false;
}

typedef u32(*thFunc)(u32 idx, u32 p1, u32 p2);

#define TH_MIN_STACK_BLK       4096

class caThread {
private:
    static u32 CreateThread(const char *name, caThreadMode mode,
            caThreadPriority p, thFunc func,
            u32 par1, u32 par2, u32 stack);

    static void LaunchThread(thFunc f, u32 p1, u32 p2);

public:

    static inline u32 CreateThread(const char *name, caThreadPriority p,
            thFunc func, u32 par1 = 0, u32 par2 = 0, u32 stack = 0) {
        return CreateThread(name, caThreadMode::MODE_USR, p,
                func, par1, par2, stack);
    }

    static inline u32 CreateSuperVisorThread(const char *name, caThreadPriority p,
            thFunc func, u32 par1 = 0, u32 par2 = 0, u32 stack = 0) {
        return CreateThread(name, caThreadMode::MODE_SVC, p,
                func, par1, par2, stack);
    }

    static inline u32 CreateSystemThread(const char *name, caThreadPriority p,
            thFunc func, u32 par1 = 0, u32 par2 = 0, u32 stack = 0) {
        return CreateThread(name, caThreadMode::MODE_SYS, p,
                func, par1, par2, stack);
    }

    static u32 DestroyThread(u32 idx);
    static void Dump(caStringStream<s8> & ss, caThreadContext *ctx);

    static inline void ReqSchedule(void) {
        //asm volatile ("DSB"); //TOTEST
        //asm volatile ("ISB"); //TOTEST
        //asm volatile ("SVC 7961"); /// SWITCH CONTEXT
    }
};

#endif 

