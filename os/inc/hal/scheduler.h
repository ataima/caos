#ifndef SCHEDULER_HEADER
#define SCHEDULER_HEADER

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



#include "array.h"
#include "heaparray.h"
#include "atomiclock.h"

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
} caJobMode;

typedef enum tag_ca_thread_priority {
    caThLevel0 = 1,
    caThLevel1 = 2,
    caThLevel2 = 4,
    caThLevel3 = 8,
    caThLevel4 = 16,
    caThLevel5 = 32,
    caThLevel6 = 64,
} caJobPriority;

typedef enum tag_ca_thread_status {
    thUnknow = 0,
    thSleepForSignal = 1,
    thStop = 2,
    thSleep = 4,
    thRemove = 8,
    thRun = 0x40,
    thInit = 0x80,
    thRunning = 0xc0,
    thFreezing = 0x81,
    thStopping = 0x82,
    thSleeping = 0x84
} caJobStatus;

typedef struct tag_ca_thread_context {
    u32 pcb[32];
    u32 thid;
    u32 index; //index over taskList array
    u32 stack_start;
    u32 stack_end;
    caJobMode mode; // th mode
    caJobPriority priority; // th pripority
     u32 cur_prio; // temporary priority from priority to lowest 
     caJobStatus status; // th status
     u32 count;
     u32 sleep;
     u32 time;
     u32 result;
     u32 nswitch;    
    char name[64];
} caThreadContext;

typedef caThreadContext * (*ptrGetNextContext)( caThreadContext *current);

inline bool less( caThreadContext* a,  caThreadContext* b) {
    // TEST true SWAP TARGET
    if (a->cur_prio == b->cur_prio)
        return (a->nswitch / a->priority) > (b->nswitch / b->priority);
    else
        return a->cur_prio < b->cur_prio;
}

typedef u32(*thFunc)(u32 idx, u32 p1, u32 p2);

#define TH_MIN_STACK_BLK       0x8000



// simple ROUND ROBIN TASK MANAGER 

class caNextTaskManager {
private:
    static caArray< caThreadContext *> table;
    static u32 cur_index;
public:
    static bool Init( caThreadContext ** ebuff, s_t max_task);
    static bool Detach(void);
    static bool AddTask(caThreadContext *ctx);
    static bool RemoveTask(s_t idx);
    static caThreadContext * RoundRobinNextContext( caThreadContext *current);
    static caThreadContext * PriorityNextContext( caThreadContext *current);
    static bool IsValidContext(u32 thIdx);
    static void WakeUp(u32 thid);
    static u32 ToSleep(u32 thid, u32 tick);
    static u32 ToSleepForSignal(u32 thid);
    static bool ChangePriority(s_t thIdx, caJobPriority newPrio);
 
    static inline s_t Size(void) {
        return table.Size();
    }
};

typedef enum tag_sched_mode {
    RoundRobin,
    Priority,
} caSchedulerMode;

typedef enum _tag_fail_create {
    FailCreate = -1
} CreateThreadResult;

class caScheduler {
private:

    class caThread {

    public:

        
        static u32 CreateThread(const char *name, caJobMode mode,
                caJobPriority p, thFunc func,
                u32 par1, u32 par2, u32 stack);

        static void LaunchThread(thFunc f, u32 p1, u32 p2);

        static void Dump(caStringStream<s8> & ss,  caThreadContext *ctx);
        static void DumpPcb(caStringStream<s8> & ss,   caThreadContext *ctx);

    };
public: 
    static caThreadContext *GetContextFromIdx(u32 idx);
private:

    static caNextTaskManager mng;
    static ptrGetNextContext getnextcontext;
    static caThreadContext *taskList[MAX_TASK];
    static caThreadContext main_ctx;
    static caThreadContext *current_task;
    static u32 switch_time;
#if DEBUG_CHECK_TASK    
    static void CheckValid( caThreadContext *ctx,u32 p);
    static void Panic(void);
    static void InvalidTask(u32 idx);
    static void InfoSwitchTask(u32 oldIdx, u32 newIdx);
#else
    #define    Panic()       
    #define    CheckValid(ctx,p)
    #define    InvalidTask(idx)
    #define    InfoSwitchTask(oldidx,newidx)
#endif        
    static void EndTask(u32 result);
    static u32 StartTask(void);
    //static u32 IoCtrl(caDevicePort *port, caSchedulerDeviceCtrl *in);
public:
    static bool AddTask(caThreadContext *ctx);
    static bool Init(caSchedulerMode req);
    static bool Destroy(void);

    static void GetNextContext(void);

    static inline  caThreadContext *GetCurrentContext(void) {
        return current_task;
    }

    static u32 GetCurrentTaskId(void);
    static u32 SetSleepMode(u32 tick, u32 thIdx);
    static u32 Dump(caStringStream<s8> & ss);
    static u32 DumpPcb(caStringStream<s8> & ss);
    static inline bool ChangePriority(s_t thIdx, caJobPriority newPrio) {
        return mng.ChangePriority(thIdx, newPrio);
    }

    static inline s_t Size(void) {
        return mng.Size();
    }

    static inline void WakeUp(u32 thid) {
        mng.WakeUp(thid);
    }

    static inline u32 GetSwitchTime(void){
        return switch_time;
    }    
    
    static inline bool IsValidContext(u32 thid) {
        return mng.IsValidContext(thid);
    }

    static inline u32 AddJob(const char *name, caJobPriority p,
            thFunc func, u32 par1 = 0, u32 par2 = 0, u32 stack = 0) {
        return caThread::CreateThread(name, caJobMode::MODE_USR, p,
                func, par1, par2, stack);
    }

    static inline u32 AddSuperVisorJob(const char *name, caJobPriority p,
            thFunc func, u32 par1 = 0, u32 par2 = 0, u32 stack = 0) {
        return caThread::CreateThread(name, caJobMode::MODE_SVC, p,
                func, par1, par2, stack);
    }

    static inline u32 AddSystemJob(const char *name, caJobPriority p,
            thFunc func, u32 par1 = 0, u32 par2 = 0, u32 stack = 0) {
        return caThread::CreateThread(name, caJobMode::MODE_SYS, p,
                func, par1, par2, stack);
    }

    static bool RemoveJob(u32 idx);

    static bool RemoveAllJobs(void);

    static u32 Sleep(u32 ms);

    static u32 WaitForSignal(void) ;
};

#endif /* SCHEDULER_H */

