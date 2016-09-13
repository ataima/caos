#include "config.h"
#include "bcm2836.h"
#include "idevice.h"
#include "stream.h"
#include "atomiclock.h"
#include "interrupt.h"
#include "miniuart.h"
#include "memory.h"
#include "memaux.h"
#include "systimer.h"
#include "thread.h"
#include "cpu.h"
#include "scheduler.h"
#include "softreq.h"
#include "sysirqctrl.h"

extern u32 __heap_base__;
extern u32 __svc_stack_pos__;
extern u32 __sys_stack_pos__;

caArray<caThreadContext *> caNextTaskManager::table;

ptrGetNextContext caScheduler::getnextcontext;
caNextTaskManager caScheduler::mng;
caThreadContext *caScheduler::taskList[MAX_TASK];
caThreadContext caScheduler::main_ctx;
caThreadContext *caScheduler::current_task;




///////////////////////////////////////////////////////////////////////

bool caNextTaskManager::Init(caThreadContext ** ebuff, size_t max_task) {
    table.Init(ebuff, max_task);
    return true;
}

bool caNextTaskManager::AddTask(caThreadContext *ctx) {
    bool res = false;
    ctx->index = table.PushBack(ctx);
    res = (ctx->index != (size_t) - 1);
    if (res) {
        ctx->index += (BASE_HANDLE + 1);
        res = IsValidContext(ctx->index);
    }
    return res;
}

bool caNextTaskManager::RemoveTask(size_t idx) {
    bool res = false;
    if (IsValidContext(idx)) {
        idx -= (BASE_HANDLE + 1);
        caThreadContext *ctx = table.At(idx);
        if (ctx != NULL) {
            ctx->status = caThreadStatus::thRemove;
            res = true;
        }
    }
    return res;
}

caThreadContext * caNextTaskManager::RoundRobinNextContext(caThreadContext *current) {
    //TIN();
    //REFRESH  
    caThreadContext * tmp;
    u32 i, max;
    for (i = 0; i < table.Size(); i++) {
        tmp = table.At(i);
        if (tmp != NULL)
            tmp->time++; // always time ++ staus ignored also current
        if (tmp->status == caThreadStatus::thRemove) {
            table.Remove(i);
            caMemory::Free(tmp);
            if (table.Size() == i) {
                break; // no more context 
            } else {
                tmp = table.At(i);
                if (tmp != NULL)
                    tmp->index = i;
                i--; // re evaluate at index i
                continue;
            }
        }
        if (tmp->sleep != SLEEP_FOR_EVER && tmp->status == caThreadStatus::thSleep) {
            if (tmp->sleep != 0) {
                tmp->sleep--;
            } else {
                tmp->status = caThreadStatus::thRun;
            }
        }
    }
    //SELECT
    max = 0;
    if (current != NULL)
        i = current->index + 1;
    else
        i = 0;
    while (1) { // Round robin
        i = i % table.Size();
        tmp = table.At(i);
        if (tmp != NULL && (tmp->status & caThreadStatus::thRunning)) {
            tmp->nswitch++;
            break; // Year ... it's the next thread to
        }
        max++;
        i++;
        if (max == table.Size()) {
            tmp = NULL;
            break;
        }
    }
    //TOUT();
    return tmp;
}

caThreadContext * caNextTaskManager::PriorityNextContext(caThreadContext *current) {
    caThreadContext * tmp, *target;
    u32 i;
    //TIN();
    tmp = target = NULL;
    for (i = 0; i < table.Size(); i++) {
        tmp = table.At(i);
        if (tmp != NULL)
            tmp->time++; // always time ++ staus ignored also current
        if (tmp->status == caThreadStatus::thRemove) {
            table.Remove(i);
            caMemory::Free(tmp);
            if (table.Size() == i) {
                break; // no more context 
            } else {
                tmp = table.At(i);
                if (tmp != NULL)
                    tmp->index = i;
                i--; // re evaluate at index i
                continue;
            }
        }
        if (tmp->sleep != SLEEP_FOR_EVER && tmp->status == caThreadStatus::thSleep) {
            if (tmp->sleep != 0) {
                tmp->sleep--;
            } else {
                tmp->status = caThreadStatus::thRun;
            }
        }
        if (tmp != current && (tmp->status & caThreadStatus::thRunning)) {
            if (target == NULL) {
                target = tmp;
            } else
                if (less(target, tmp)) {
                target = tmp;
            }
        }
    }
    if (current != NULL) {
        current->cur_prio >>= 1;
        if (current->cur_prio == 0)
            current->cur_prio = current->priority;
        current->nswitch++;
    }
    if (target == NULL) {
        target = current;
    }
    //TOUT();
    return target;
}

bool caNextTaskManager::IsValidContext(u32 thIdx) {
    bool res = false;
    if (thIdx > BASE_HANDLE) {
        thIdx -= (BASE_HANDLE + 1);
        res = (thIdx < table.Size());
    }
    return res;
}

/* from interrupt service routine : must be fast!!*/
void caNextTaskManager::WakeUp(u32 thid) {
    thid -= (BASE_HANDLE + 1);
    if (thid < table.Size()) {
        caThreadContext * tmp = table.At(thid);
        if (tmp != NULL && tmp->status == caThreadStatus::thSleep)
            tmp->status = caThreadStatus::thRun;
    }
}

/* from svc iorequest sleep*/
u32 caNextTaskManager::ToSleep(u32 thid, u32 tick) {
    u32 res = deviceError::no_error;
    thid -= (BASE_HANDLE + 1);
    if (thid < table.Size()) {
        caThreadContext * tmp = table.At(thid);
        while (caIrqCtrl::LockSwitchContext() == false) {
        };
        if (tmp != NULL && tmp->status == caThreadStatus::thRun) {
            tmp->status = caThreadStatus::thSleep;
            tmp->sleep = tick;
            res = deviceError::okey;
        }
        while (caIrqCtrl::UnLockSwitchContext() == false) {
        };
    }
    return res;
}


///////////////////////////////////////////////////////////////////////

bool caScheduler::Init(caSchedulerMode req) {
    if (req == caSchedulerMode::RoundRobin) {
        getnextcontext = caNextTaskManager::RoundRobinNextContext;
    } else {
        getnextcontext = caNextTaskManager::PriorityNextContext;
    }
    caMemAux::MemZero((u32 *) taskList, sizeof (taskList) / sizeof (u32));
    caMemAux::MemZero((u32 *) & main_ctx, sizeof (caThreadContext) / sizeof (u32));
    caMemAux::StrNCpy(main_ctx.name, "Default", 64);
    main_ctx.stack_start = 0x40000000;
    main_ctx.stack_end = 0x0;
    current_task = &main_ctx;
    return mng.Init(taskList, MAX_TASK);
}

bool caScheduler::AddTask(caThreadContext *ctx) {
    bool res = false;
    while (caIrqCtrl::LockSwitchContext() == false) {
    };
    res = mng.AddTask(ctx);
    while (caIrqCtrl::UnLockSwitchContext() == false) {
    };
    return res;
}

bool caScheduler::RemoveTask(u32 idx) {
    bool res = false;
    while (caIrqCtrl::LockSwitchContext() == false) {
    };
    res = mng.RemoveTask(idx);
    while (caIrqCtrl::UnLockSwitchContext() == false) {
    };
    return res;
}

bool caScheduler::RemoveAllTask(void) {
    bool res = false;
    u32 size = mng.Size();
    u32 idx = size;
    do {
        idx--;
        res = RemoveTask(idx);
    } while (idx != 0 && res == true);
    return res;
}

/* Calling under IRQ  ISR */
void caScheduler::GetNextContext(void) {
#if DEBUG_CHECK_TASK
    CheckValid(1);
#endif
    if (current_task != &main_ctx)
        current_task = getnextcontext(current_task);
    else
        current_task = getnextcontext(NULL);

#if DEBUG_CHECK_TASK
    CheckValid(2);
#endif
    if (current_task == NULL)
        current_task = &main_ctx;
}

u32 caScheduler::SetSleepMode(u32 ms, u32 thIdx) {
    u32 res = deviceError::no_error;
    if (IsValidContext(thIdx)) {
        u32 tick = caSysTimer::ToTick(ms);
        res = mng.ToSleep(thIdx, tick);
    }
    return res;
}

u32 caScheduler::Dump(caStringStream<s8> & ss) {
    u32 i;
    for (i = 0; i < mng.Size(); i++) {
        caThread::Dump(ss, taskList[i]);
    }
    ss.Str();
    return ss.Size();
}

u32 Sleep(u32 ms) {
    u32 res = 0;
    caSoftRequest::SVC_IOCTL(ioCtrlRequest::Scheduler |
            ioCtrlFunction::caIoSleep,
            &ms,
            (u32*) caScheduler::GetCurrentTaskId(),
            &res);
    // task was blocked here...
    if (res == deviceError::okey) {
        caScheduler::SwitchContext();
    }
    return res;
}

u32 caScheduler::StartTask(void) {
    u32 res = 0xffffffff;
    while (caIrqCtrl::LockSwitchContext() == false) {
    };
    if (current_task != NULL) {
        current_task->status = caThreadStatus::thRun;
        res = current_task->index;
    }
    while (caIrqCtrl::UnLockSwitchContext() == false) {
    };
    return res;
}

void caScheduler::EndTask(u32 result) {
    while (caIrqCtrl::LockSwitchContext() == false) {
    };
    if (current_task != NULL) {
        current_task->result = result;
        current_task->status = caThreadStatus::thStop;
    }
    while (caIrqCtrl::UnLockSwitchContext() == false) {
    };
}


#if DEBUG_CHECK_TASK

void caScheduler::Panic(void) {
    s8 buff[512];
    caStringStream<s8> ss;
    ss.Init(buff, 512);
    if (current_task != NULL)
        caThread::Dump(ss, current_task);
    Dbg::Put((const char *) ss.Str());
    ss.Clear();
    caScheduler::Dump(ss);
    Dbg::Put((const char *) ss.Str());
    ss.Clear();
    Dbg::Put("> c.a.O.S. : [ panic error : stop ! ]\r\n");
    caInterruptRequest::DisableInt();
    caArmCpu::DisableIrqFiq();
    while (1);
}

void caScheduler::CheckValid(u32 p) {
    if (p == 1 && current_task == &main_ctx)
        return;
    if (current_task == NULL) {
        Dbg::Put("ERROR : NULL CONTEXT : ", p);
        Panic();
    }
    if (current_task->pcb[1] != current_task->pcb[16]) {
        if (current_task->pcb[16] > __heap_base__) {
            Dbg::Put("ERROR : TASK JUMP ADDRESS FAULT : HIGHER", p);
            Dbg::Put("TASK :");
            Dbg::Put((const char *) current_task->name);
            Dbg::Put("\r\n");
            Dbg::Put("pcb[16] = ", current_task->pcb[16]);
            Dbg::Put("pcb[1] = ", current_task->pcb[1]);
            current_task->pcb[16] = current_task->pcb[1];
            Dbg::Put("pcb[15] = ", current_task->pcb[15]);
            Dbg::Put("stack_start = ", current_task->stack_start);
            Dbg::Put("stack_end = ", current_task->stack_end);
        }
        if (current_task->pcb[16] < 0x8000) {
            Dbg::Put("TASK :");
            Dbg::Put((const char *) current_task->name);
            Dbg::Put("\r\n");
            Dbg::Put("ERROR : TASK JUMP ADDRESS FAULT : LOWER ", p);
            Dbg::Put("pcb[16] = ", current_task->pcb[16]);
            Dbg::Put("pcb[1] = ", current_task->pcb[1]);
            Dbg::Put("pcb[15] = ", current_task->pcb[15]);
            Dbg::Put("stack_start = ", current_task->stack_start);
            Dbg::Put("stack_end = ", current_task->stack_end);
            current_task->pcb[16] = current_task->pcb[1];
        }
    }
    if (current_task->pcb[15] > current_task->stack_start)// pcb 1 = r14
    {
        Dbg::Put("ERROR : TASK STACK POINTER TOO BIGGER : ", p);
        Dbg::Put("TASK :");
        Dbg::Put((const char *) current_task->name);
        Dbg::Put("\r\n");
        Dbg::Put("pcb[15] = ", current_task->pcb[15]);
        Dbg::Put("stack_start = ", current_task->stack_start);
        Dbg::Put("stack_end = ", current_task->stack_end);
        Panic();
    }
    if (current_task->pcb[15] < current_task->stack_end)// pcb 1 = r14
    {
        Dbg::Put("ERROR : TASK STACK POINTER TOO LOWER : ", p);
        Dbg::Put("TASK :");
        Dbg::Put((const char *) current_task->name);
        Dbg::Put("\r\n");
        Dbg::Put("pcb[15] = ", current_task->pcb[15]);
        Dbg::Put("stack_start = ", current_task->stack_start);
        Dbg::Put("stack_end = ", current_task->stack_end);
        Panic();
    }
    if ((u32) (current_task) != current_task->stack_start + 64) {
        Dbg::Put("ERROR : TASK UNALIGNED TO STACK POINTER : ", p);
        Dbg::Put("TASK :");
        Dbg::Put((const char *) current_task->name);
        Dbg::Put("\r\n");
        Dbg::Put("pcb[15] = ", current_task->pcb[15]);
        Dbg::Put("stack_start = ", current_task->stack_start);
        Dbg::Put("stack_end = ", current_task->stack_end);
        Panic();
    }
}



#endif

void caScheduler::SwitchContext(void) {
    caThread::ReqSchedule();
    while (current_task->status != caThreadStatus::thRun) {
    };
}

u32 caScheduler::GetCurrentTaskId(void) {
    if (current_task == NULL || current_task == &main_ctx) {
        return -1;
    } else {
        return current_task->index;
    }
}