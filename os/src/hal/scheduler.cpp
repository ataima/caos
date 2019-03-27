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
#include "atomiclock.h"
#include "memory.h"
#include "memaux.h"
#include "scheduler.h"
#include "kdebug.h"


extern u32 __svc_stack_pos__;
extern u32 __sys_stack_pos__;

caArray< caThreadContext *> caNextTaskManager::table;
u32 caNextTaskManager::cur_index;

ptrGetNextContext caScheduler::getnextcontext;
caNextTaskManager caScheduler::mng;
caThreadContext *caScheduler::current_task;
u32 caScheduler::switch_time;
caThreadContext caScheduler::main_ctx;
caThreadContext *caScheduler::taskList[MAX_TASK];

///////////////////////////////////////////////////////////////////////
//////    THREAD  MANAGER
///////////////////////////////////////////////////////////////////////

/*
 |---------------------------------------------------------------|
 | ?64/                  FREE STACK                 sp| 64 | CTX |
 |---------------------------------------------------------------|     
 */


u32 caScheduler::caThread::CreateThread(const char * name, caJobMode mode, caJobPriority p, thFunc func, u32 par1, u32 par2, u32 stack) {
    caThreadContext * ctx = nullptr;
    u32 pst, a_stk, base;
    u32 header_mem_alloc = caMemory::GetHeaderBlock();
    if (stack <= TH_MIN_STACK_BLK)
        a_stk = TH_MIN_STACK_BLK - header_mem_alloc;
    else
        a_stk = (((stack) / TH_MIN_STACK_BLK) + 1) * TH_MIN_STACK_BLK;
    base = ptr_to_uint(caMemory::Allocate(a_stk));
    if (base != 0) {
        ctx = static_cast<caThreadContext *> ((void *) uint_to_ptr(a_stk + base - sizeof (caThreadContext)));
        pst = ptr_to_uint(ctx) - 64; //64 guard
        ctx->thid = ptr_to_uint(ctx);
        ctx->status = caJobStatus::thInit;
        ctx->priority = p;
        ctx->mode = mode;
        caMemAux<u32>::MemSet((u32 *) ctx->pcb, 0, sizeof (u32)*32);
        ctx->pcb[0] = mode; //SPSR
        ctx->pcb[1] = ptr_to_uint((void *) caThread::LaunchThread);
        ctx->pcb[2] = ptr_to_uint((void *) func); //r0
        ctx->pcb[3] = par1;
        ctx->pcb[4] = par2;
        ctx->pcb[15] = pst; //r13
        ctx->pcb[16] = ptr_to_uint((void *) caThread::LaunchThread); //r14
        ctx->count = hal_llc_scheduler.hll_tick();
        ctx->stack_start = (u32) pst;
        ctx->stack_end = base + 64; // 64 guard
        ctx->time = 0;
        ctx->sleep = 0;
        ctx->result = -1;
        ctx->cur_prio = ctx->priority;
        ctx->nswitch = 0;
        caStrAux::StrNCpy(ctx->name, name, 64);
        if (hal_llc_scheduler.hll_scheduler_add_atsk(ctx)) {
            return ctx->index;
        }
    }
    return CreateThreadResult::FailCreate;
}

void caScheduler::caThread::LaunchThread(thFunc f, u32 p1, u32 p2) {
    u32 res, idx;
    idx = caScheduler::StartTask();
    res = f(idx, p1, p2);
    caScheduler::EndTask(res);
    while (1); // wait cancellation from scheduler
}

void caScheduler::caThread::Dump(caStringStream<s8> & ss, caThreadContext *ctx) {
    if (ctx != nullptr) {
        ss << caStringFormat::hex << ctx->index << caStringFormat::dec << ") ";
        caStringFiller p(' ', 18);
        ss.Fix(p);
        ss << (const char *) ctx->name << p;
        ss << " : ";
        switch (ctx->mode) {
            case caJobMode::MODE_USR:
                ss << "USR : ";
                break;
            case caJobMode::MODE_FIQ:
                ss << "FIQ : ";
                break;
            case caJobMode::MODE_IRQ:
                ss << "IRQ : ";
                break;
            case caJobMode::MODE_SVC:
                ss << "SVC : ";
                break;
            case caJobMode::MODE_MON:
                ss << "MON : ";
                break;
            case caJobMode::MODE_ABT:
                ss << "ABT : ";
                break;
            case caJobMode::MODE_HYP:
                ss << "HYP : ";
                break;
            case caJobMode::MODE_UND:
                ss << "UND : ";
                break;
            case caJobMode::MODE_SYS:
                ss << "SYS : ";
                break;
        }
        switch (ctx->priority) {
            case caJobPriority::caThLevel0:
                ss << "Pri0 : ";
                break;
            case caJobPriority::caThLevel1:
                ss << "Pri1 : ";
                break;
            case caJobPriority::caThLevel2:
                ss << "Pri2 : ";
                break;
            case caJobPriority::caThLevel3:
                ss << "Pri3 : ";
                break;
            case caJobPriority::caThLevel4:
                ss << "Pri4 : ";
                break;
            case caJobPriority::caThLevel5:
                ss << "Pri5 : ";
                break;
            case caJobPriority::caThLevel6:
                ss << "Pri6 : ";
                break;
            default:
                ss << "Pri" << (u32) ctx->priority << ": ";
                break;
        }
        p.width = 12;
        ss.Fix(p);
        switch (ctx->status) {
            case caJobStatus::thInit:
                ss << "INIT";
                break;
            case caJobStatus::thStop:
                ss << "STOP";
                break;
            case caJobStatus::thRun:
                ss << "RUN";
                break;
            case caJobStatus::thSleep:
                ss << "SLEEP (" << ctx->sleep << ")";
                break;
            default:
                ss << "?";
                ss << (u32) (ctx->status);
                ss << " : ";
                break;
        }
        ss << p << ": CPU : ";
        u32 cpu;
        if (ctx->nswitch < 0x00020000)
            cpu = (ctx->nswitch * 10000) / ctx->time;
        else
            cpu = ((ctx->nswitch >> 8) * 10000) / (ctx->time >> 8);
        u32 p1 = cpu / 100;
        u32 p2 = cpu % 100;
        ss << caStringFormat::dec << p1 << "." << p2 << "%\r\n";
    }
}

void caScheduler::caThread::DumpPcb(caStringStream<s8> & ss, caThreadContext *ctx) {
    u32 i;
    if (ctx != nullptr) {
        ss << "----------------------------------" << caEnd::endl;
        ss << "name=" << (const char *) ctx->name << caEnd::endl;
        ss << "ctx=" << caStringFormat::hex << ptr_to_uint(ctx) << caEnd::endl;
        for (i = 0; i < 32; i++) {
            ss << "PCB [ " << caStringFormat::dec << i << " ] = " <<
                    caStringFormat::hex << ctx->pcb[i] << caEnd::endl;
        }
        ss << "thid=" << caStringFormat::dec << ctx->thid << caEnd::endl;
        ss << "index=" << caStringFormat::dec << ctx->index << caEnd::endl;
        ss << "stack_start=" << caStringFormat::hex << ctx->stack_start << caEnd::endl;
        ss << "stack_end=" << caStringFormat::hex << ctx->stack_end << caEnd::endl;
        ss << "mode=" << caStringFormat::dec << (u32) ctx->mode << caEnd::endl;
        ss << "priority=" << caStringFormat::dec << (u32) ctx->priority << caEnd::endl;
        ss << "cur_prio=" << caStringFormat::dec << (u32) ctx->cur_prio << caEnd::endl;
        ss << "status=" << caStringFormat::dec << (u32) ctx->status << caEnd::endl;
        ss << "count=" << caStringFormat::dec << (u32) ctx->count << caEnd::endl;
        ss << "sleep=" << caStringFormat::dec << (u32) ctx->sleep << caEnd::endl;
        ss << "time=" << caStringFormat::dec << (u32) ctx->time << caEnd::endl;
        ss << "result=" << caStringFormat::dec << (u32) ctx->result << caEnd::endl;
        ss << "nswitch=" << caStringFormat::dec << (u32) ctx->nswitch << caEnd::endl;
    }
}



///////////////////////////////////////////////////////////////////////

bool caNextTaskManager::Init(caThreadContext ** ebuff, s_t max_task) {
    table.Init(ebuff, max_task);
    cur_index = -1;
    return true;
}

bool caNextTaskManager::Detach(void) {
    table.Detach();
    return true;
}

bool caNextTaskManager::AddTask(caThreadContext *ctx) {
    bool res = false;
    ctx->index = table.PushBack(ctx) - 1;
    res = (ctx->index != (s_t) - 1);
    if (res) {
        res = IsValidContext(ctx->index);
    }
    return res;
}

bool caNextTaskManager::RemoveTask(s_t idx) {
    bool res = false;
    if (IsValidContext(idx)) {
        caThreadContext *ctx = nullptr;
        if (table.At(ctx, idx) && (ctx != nullptr)) {
            ctx->status = caJobStatus::thRemove;
            res = true;
        }
    }
    return res;
}

bool caNextTaskManager::ChangePriority(s_t thIdx, caJobPriority newPrio) {
    bool res = 0;
    if (IsValidContext(thIdx)) {
        caThreadContext *ctx = nullptr;
        if (table.At(ctx, thIdx) && (ctx != nullptr)) {
            while (hal_llc_scheduler.hll_lock() == false) {
            };
            ctx->priority = newPrio;
            ctx->cur_prio = newPrio;
            while (hal_llc_scheduler.hll_lock() == false) {
            };
        } else {
            res = 1;
        }
    }
    return res;
}

caThreadContext * caNextTaskManager::RoundRobinNextContext(caThreadContext *) {
    //TIN();
    //REFRESH  
    caThreadContext * tmp = nullptr;
    u32 i, max = 0;
    for (i = 0; i < table.Size(); i++) {
        if (table.At(tmp, i) && (tmp != nullptr)) {
            tmp->time++; // always time ++ staus ignored also current
            if (tmp->status == caJobStatus::thRemove) {
                table.Remove(i);
                caMemory::Free((void *) tmp);
                if (table.Size() == i) {
                    break; // no more context 
                } else {
                    if (table.At(tmp, i) && (tmp != nullptr))
                        tmp->index = i;
                    i--; // re evaluate at index i :  remove also this task if is mask removable
                    continue;
                }
            } else
                if (tmp->status == caJobStatus::thSleep) {
                if (tmp->sleep != 0) {
                    tmp->sleep--;
                } else {
                    tmp->status = caJobStatus::thRun;
                }
            } else
                if (tmp->status == caJobStatus::thSleepForSignal) {
                tmp->sleep++; // time waiting for signal wake up
            }
        }
    }
    //SELECT
    if (table.Size() == 0) {
        cur_index = -1;
        return nullptr;
    }
    cur_index++;
    if (cur_index >= table.Size())
        cur_index = 0;
    i = cur_index;
    while (1) { // Round robin
        i = i % table.Size();
        if (table.At(tmp, i) && (tmp != nullptr) && (tmp->status & caJobStatus::thRunning)) {
            tmp->nswitch++;
            break; // Year ... it's the next thread to
        }
        max++;
        i++;
        if (max == table.Size()) {
            tmp = nullptr;
            break;
        }
    }
    //TOUT();
    return (caThreadContext *) tmp;
}

/*
 How WORK
 * FOR N SWITCH 
 * N= Th0+2*Th1+4*Th2+8*Th3+16*Th4+32*TH5+64*TH6=127 
 * n=PRIORITY 0  = ntotal/127           ->0,787%    CPU Time
 * n=PRIORITY 1  = ( ntotal/127)*2      ->%1,57     CPU Time
 * n=PRIORITY 2  = ( ntotal/127)*4      ->%3.1496...CPU Time
 * n=PRIORITY 3  = ( ntotal/127)*8      ->6.28%     CPU Time
 * n=PRIORITY 4  = ( ntotal/127)*16     ->12,56%    CPU Time
 * n=PRIORITY 5  = ( ntotal/127)*32     ->25,12%    CPU Time
 * n=PRIORITY 6  = ( ntotal/127)*64     ->50,24%    CPU Time
 */
caThreadContext * caNextTaskManager::PriorityNextContext(caThreadContext *current) {
    caThreadContext * tmp, *target;
    u32 i;
    tmp = target = nullptr;
    for (i = 0; i < table.Size(); i++) {
        if (table.At(tmp, i) && (tmp != nullptr)) {
            tmp->time++; // always time ++ staus ignored also current
            if (tmp->status == caJobStatus::thRemove) {
                table.Remove(i);
                caMemory::Free((void *) tmp);
                if (table.Size() == i) {
                    break; // no more context 
                } else {
                    if (table.At(tmp, i) && (tmp != nullptr))
                        tmp->index = i;
                    i--; // re evaluate at index i
                    continue;
                }
            } else
                if (tmp->status == caJobStatus::thSleep) {
                if (tmp->sleep != 0) {
                    tmp->cur_prio = 0;
                    tmp->sleep--;
                } else {
                    tmp->status = caJobStatus::thRun;
                    tmp->cur_prio = tmp->priority;
                }
            } else
                if (tmp != current && (tmp->status & caJobStatus::thRunning)) {
                if (target == nullptr) {
                    target = tmp;
                } else
                    if (less(target, tmp)) {
                    target = tmp;
                }
            }
        }
    }
    if (current != nullptr) {
        current->cur_prio >>= 1;
        if (target == nullptr)
            target = current;
        else
            if (less(target, current)) {
            target = current;
        }
        if (current->cur_prio == 0)
            current->cur_prio = current->priority;
        current->nswitch++;
    }
    return (caThreadContext *) target;
}

bool caNextTaskManager::IsValidContext(u32 thIdx) {
    return ( thIdx != 0xffffffff && thIdx < table.Size());
}

/* from interrupt service routine : must be fast!!*/
void caNextTaskManager::WakeUp(u32 thid) {
    if (thid < table.Size()) {
        caThreadContext * tmp = nullptr;
        if (table.At(tmp, thid) && tmp != nullptr && tmp->status == caJobStatus::thSleep) {
            tmp->status = caJobStatus::thRun;
        }
    }
}

u32 caNextTaskManager::ToSleep(u32 thid, u32 tick) {
    u32 res = 0;
    if (thid <= table.Size()) {
        caThreadContext * tmp = nullptr;
        while (hal_llc_scheduler.hll_lock() == false) {
        };
        table.At(tmp, thid);
        if (tmp != nullptr && (tmp->status & caJobStatus::thRunning)) {
            tmp->status = caJobStatus::thSleep;
            tmp->sleep = tick;
        } else {
            res = 1;
        }
        while (hal_llc_scheduler.hll_unlock() == false) {
        };
    }
    return res;
}

u32 caNextTaskManager::ToSleepForSignal(u32 thid) {
    u32 res = 0;
    if (thid <= table.Size()) {
        caThreadContext * tmp = nullptr;
        while (hal_llc_scheduler.hll_lock() == false) {
        };
        table.At(tmp, thid);
        if (tmp != nullptr && (tmp->status & caJobStatus::thRunning)) {
            tmp->status = caJobStatus::thSleepForSignal;
        } else {
            res = 1;
        }
        while (hal_llc_scheduler.hll_unlock() == false) {
        };
    }
    return res;
}

///////////////////////////////////////////////////////////////////////

static u32 nullTask(u32 /*thIdx*/, u32 /*p1*/, u32/*p2*/) {
    while(1) {
        hal_llc_int_req.hll_wait_for_interrupt(caScheduler::GetContextFromIdx(0));
    }
}

bool caScheduler::Init(caSchedulerMode req) {
    bool res = false;
    if (req == caSchedulerMode::RoundRobin) {
        getnextcontext = caNextTaskManager::RoundRobinNextContext;
    } else {
        getnextcontext = caNextTaskManager::PriorityNextContext;
    }
    caMemAux<u32>::MemSet((u32 *) taskList, 0, sizeof (taskList));
    caMemAux<u32>::MemSet((u32 *) & main_ctx, 0, sizeof (caThreadContext));
    caStrAux::StrNCpy(main_ctx.name, "Default Main Context", 64);
    main_ctx.stack_start = 0x40000000;
    main_ctx.stack_end = 0x0;
    current_task = &main_ctx;
    res = mng.Init(taskList, MAX_TASK);
    // idle task always RUN task in scheduler , low priority     
    caScheduler::AddJob("idle",
            caJobPriority::caThLevel0,
            nullTask);
    return res;
}

bool caScheduler::Destroy(void) {
    bool res = false;
    if (caScheduler::RemoveAllJobs()) {
        caMemAux<u32>::MemSet((u32 *) taskList, 0, sizeof (taskList));
        caMemAux<u32>::MemSet((u32 *) & main_ctx, 0, sizeof (caThreadContext));
        current_task = nullptr;
        mng.Detach();
        res = true;
    }
    return res;
}


caThreadContext *caScheduler::GetContextFromIdx(u32 idx){
    if(idx>=MAX_TASK)return nullptr;
    return taskList[idx];
}

bool caScheduler::AddTask(caThreadContext *ctx) {
    bool res = false;
    while (hal_llc_scheduler.hll_lock() == false) {
    };
    res = mng.AddTask(ctx);
    while (hal_llc_scheduler.hll_unlock() == false) {
    };
    return res;
}

bool caScheduler::RemoveJob(u32 idx) {
    bool res = false;
    while (hal_llc_scheduler.hll_lock() == false) {
    };
    res = mng.RemoveTask(idx);
    while (hal_llc_scheduler.hll_unlock() == false) {
    };
    return res;
}

bool caScheduler::RemoveAllJobs(void) {
    bool res = false;
    u32 size = mng.Size();
    u32 idx = size;
    do {
        idx--;
        res = RemoveJob(idx);
    } while (idx != 0 && res == true);
    return res;
}
/* Calling under IRQ  ISR or simulated */
void caScheduler::GetNextContext(void) {
    hal_llc_scheduler.hll_stop_scheduler();
    CheckValid(current_task, 1);
    u32 curIdx=0xffffffff,nextIdx=0xffffffff;
    u32 start_c = hal_llc_time_1.hll_free_counter();
    if(current_task!=nullptr)curIdx=current_task->index;
    if (current_task != &main_ctx)
        current_task = getnextcontext(current_task);
    else
        current_task = getnextcontext(nullptr);
    if (current_task == nullptr)
        current_task = &main_ctx;
    if(current_task!=nullptr)nextIdx=current_task->index;
    
    u32 stop_c = hal_llc_time_1.hll_free_counter();
    if (start_c > stop_c) {
        switch_time = 0xffffffff - start_c + stop_c;
    } else {
        switch_time = stop_c - start_c;
    }    
    CheckValid(current_task, 2);
    InfoSwitchTask(curIdx,nextIdx);
    hal_llc_scheduler.hll_start_scheduler();
}

u32 caScheduler::SetSleepMode(u32 ms, u32 thIdx) {
    u32 tick, res = 0;
    if (IsValidContext(thIdx)) {
        tick = hal_llc_scheduler.hll_to_tick(ms);
        res = mng.ToSleep(thIdx, tick);
    } else {
        InvalidTask(thIdx);
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

u32 caScheduler::DumpPcb(caStringStream<s8> & ss) {
    u32 i;
    for (i = 0; i < mng.Size(); i++) {
        caThread::DumpPcb(ss, taskList[i]);
    }
    ss.Str();
    return ss.Size();
}

u32 caScheduler::Sleep(u32 ms) {
    u32 res = 0;
    if (current_task != nullptr) {
        u32 idx = caScheduler::GetCurrentTaskId();
        if (IsValidContext(idx)) {
            res = caScheduler::SetSleepMode(ms, idx);
            if (res == 0) {
                while ((current_task->status & caJobStatus::thRunning) == 0) {
                    hal_llc_int_req.hll_wait_for_interrupt(current_task);
                };
            }
        }
    }
    return res;
}

u32 caScheduler::StartTask(void) {
    u32 res = 0xffffffff;
    if (current_task != nullptr) {
        current_task->status = caJobStatus::thRun;
        res = current_task->index;
    }
    return res;
}

void caScheduler::EndTask(u32 result) {
    if (current_task != nullptr) {
        current_task->result = result;
        current_task->status = caJobStatus::thStop;
    }
}


#if DEBUG_CHECK_TASK

void caScheduler::Panic(void) {
    s8 buff[512];
    caStringStream<s8> ss;
    hal_llc_int_req.hll_disable();
    ss.Init(buff, 512);
    if (current_task != nullptr)
        caThread::Dump(ss, current_task);
    Dbg::Put((const char *) ss.Str());
    ss.Clear();
    caScheduler::Dump(ss);
    Dbg::Put((const char *) ss.Str());
    ss.Clear();
    Dbg::Put("> c.a.O.S. : [ panic error : stop ! ]\r\n");
    while (1);
}

void caScheduler::InvalidTask(u32 idx){
        Dbg::Put("caSheduler: Invalid thread context : ",idx,Dbg::kformat::dec,true);
        caStringStream<s8> ss;
        Dump(ss);
        Dbg::Put(ss.Str());
        Panic(); 
}


void caScheduler::InfoSwitchTask(u32 oldIdx,u32 newIdx){
    if(oldIdx!=0xffffffff && oldIdx < MAX_TASK){
        Dbg::Put("SCHEDULER : old task = ");Dbg::Put(taskList[oldIdx]->name);Dbg::Put("\n");
    } else {
        Dbg::Put("SCHEDULER : old task = unassigned !\n");
    }
    if(newIdx!=0xffffffff && newIdx < MAX_TASK ){
        Dbg::Put("SCHEDULER : new task = ");Dbg::Put(taskList[newIdx]->name);Dbg::Put("\n");
    } else {
        Dbg::Put("SCHEDULER : new task = unassigned !\n");
    }
}

void caScheduler::CheckValid(caThreadContext *ctx, u32 p) {
    if (p == 1 && ctx == &main_ctx)
        return;
    if (ctx == nullptr) {
        Dbg::Put("ERROR : nullptr CONTEXT : ", p);
        Panic();
    }
    if (ctx->pcb[1] != ctx->pcb[16]) {
        if (ctx->pcb[16] > ptr_to_uint(hal_llc_mem.hll_heap_start())) {
            Dbg::Put("ERROR : TASK JUMP ADDRESS FAULT : HIGHER", p);
            Dbg::Put("TASK :");
            Dbg::Put((const char *) ctx->name);
            Dbg::Put("\r\n");
            Dbg::Put("pcb[16] = ", ctx->pcb[16]);
            Dbg::Put("pcb[1] = ", ctx->pcb[1]);
            ctx->pcb[16] = ctx->pcb[1];
            Dbg::Put("pcb[15] = ", ctx->pcb[15]);
            Dbg::Put("stack_start = ", ctx->stack_start);
            Dbg::Put("stack_end = ", ctx->stack_end);
        }
        if (ctx->pcb[16] < 0x8000) {
            Dbg::Put("TASK :");
            Dbg::Put((const char *) ctx->name);
            Dbg::Put("\r\n");
            Dbg::Put("ERROR : TASK JUMP ADDRESS FAULT : LOWER ", p);
            Dbg::Put("pcb[16] = ", ctx->pcb[16]);
            Dbg::Put("pcb[1] = ", ctx->pcb[1]);
            Dbg::Put("pcb[15] = ", ctx->pcb[15]);
            Dbg::Put("stack_start = ", ctx->stack_start);
            Dbg::Put("stack_end = ", ctx->stack_end);
            ctx->pcb[16] = ctx->pcb[1];
        }
    }
    if (ctx->pcb[15] > ctx->stack_start)// pcb 1 = r14
    {
        Dbg::Put("ERROR : TASK STACK POINTER TOO BIGGER : ", p);
        Dbg::Put("TASK :");
        Dbg::Put((const char *) ctx->name);
        Dbg::Put("\r\n");
        Dbg::Put("pcb[15] = ", ctx->pcb[15]);
        Dbg::Put("stack_start = ", ctx->stack_start);
        Dbg::Put("stack_end = ", ctx->stack_end);
        Panic();
    }
    if (ctx->pcb[15] < ctx->stack_end)// pcb 1 = r14
    {
        Dbg::Put("ERROR : TASK STACK POINTER TOO LOWER : ", p);
        Dbg::Put("TASK :");
        Dbg::Put((const char *) ctx->name);
        Dbg::Put("\r\n");
        Dbg::Put("pcb[15] = ", ctx->pcb[15]);
        Dbg::Put("stack_start = ", ctx->stack_start);
        Dbg::Put("stack_end = ", ctx->stack_end);
        Panic();
    }
    if (ptr_to_uint(ctx) != ctx->stack_start + 64) {
        Dbg::Put("ERROR : TASK UNALIGNED TO STACK POINTER : ", p);
        Dbg::Put("TASK :");
        Dbg::Put((const char *) ctx->name);
        Dbg::Put("\r\n");
        Dbg::Put("pcb[15] = ", ctx->pcb[15]);
        Dbg::Put("stack_start = ", ctx->stack_start);
        Dbg::Put("stack_end = ", ctx->stack_end);
        Panic();
    }
}


#endif

u32 caScheduler::GetCurrentTaskId(void) {
    if (current_task == nullptr) {
        return -1;
    } else {
        return current_task->index;
    }
}



// nuova implementazione per simulazione con pthread

u32 caScheduler::WaitForSignal(void) {
    u32 res = 0;
    if (current_task != nullptr) {
        u32 idx = caScheduler::GetCurrentTaskId();
        if (IsValidContext(idx)) {
            res = mng.ToSleepForSignal(idx);
            if (res == 0) {
                hal_llc_int_req.hll_wait_for_interrupt(current_task);
            }
        } else {
            InvalidTask(idx);
        }
    } else {
        InvalidTask(-1);
    }
    return res;
}
