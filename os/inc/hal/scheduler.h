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






// simple ROUND ROBIN TASK MANAGER 

class caNextTaskManager {
private:
    static caArray<caThreadContext *> table;
public:
    static bool Init(caThreadContext ** ebuff, s_t max_task);
    static bool AddTask(caThreadContext *ctx);
    static bool RemoveTask(s_t idx);
    static caThreadContext * RoundRobinNextContext(caThreadContext *current);
    static caThreadContext * PriorityNextContext(caThreadContext *current);
    static bool IsValidContext(u32 thIdx);
    static void WakeUp(u32 thid);
    static u32 ToSleep(u32 thid, u32 tick);

    static inline s_t Size(void) {
        return table.Size();
    }
};

typedef enum tag_sched_mode {
    RoundRobin,
    Priority,
} caSchedulerMode;


typedef caThreadContext * (*ptrGetNextContext)(caThreadContext *current);

class caScheduler {
private:
    static caNextTaskManager mng;
    static ptrGetNextContext getnextcontext;
    static caThreadContext *taskList[MAX_TASK];
    static caThreadContext main_ctx;
    static caThreadContext *current_task;
#if DEBUG_CHECK_TASK    
    static void CheckValid(u32 p);
    static void Panic(void);
#endif    
    //static u32 IoCtrl(caDevicePort *port, caSchedulerDeviceCtrl *in);
public:
    static bool Init(caSchedulerMode req);
    static bool AddTask(caThreadContext *ctx);
    static bool RemoveTask(u32 idx);
    static bool RemoveAllTask(void);
    static void GetNextContext(void);
    static void SwitchContext(void);
    static u32 GetCurrentTaskId(void);
    static u32 SetSleepMode(u32 tick, u32 thIdx);
    static u32 Dump(caStringStream<s8> & ss);
    static void EndTask(u32 result);

    static inline s_t Size(void) {
        return mng.Size();
    }
    static u32 StartTask(void);

    static inline void WakeUp(u32 thid) {
        mng.WakeUp(thid);
    }

    static inline bool IsValidContext(u32 thid) {
        return mng.IsValidContext(thid);
    }
    
#define SLEEP_FOR_EVER 0xffffffff

static u32 Sleep(u32 ms);

static inline u32 WaitForSignal(void) {
    return Sleep(SLEEP_FOR_EVER);
}
};

#endif /* SCHEDULER_H */

