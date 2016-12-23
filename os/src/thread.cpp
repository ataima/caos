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

#include "config.h"
#include "interrupt.h"
#include "atomiclock.h"
#include "miniuart.h"
#include "thread.h"
#include "cpu.h"
#include "systimer.h"
#include "scheduler.h"
#include "memory.h"
#include "memaux.h"
#include "comdevice.h"

/*
 |---------------------------------------------------------------|
 |                       FREE STACK                 sp| 64 | CTX |
 |---------------------------------------------------------------|     
 */


u32 caThread::CreateThread(const char * name, caThreadMode mode, caThreadPriority p, thFunc func, u32 par1, u32 par2, u32 stack) {
    caThreadContext * ctx = NULL;
    u32 pst, a_stk, base;
    u32 header_mem_alloc = caMemory::GetHeaderBlock();
    if (stack <= TH_MIN_STACK_BLK)
        a_stk = TH_MIN_STACK_BLK - header_mem_alloc;
    else
        a_stk = (((stack - header_mem_alloc) / TH_MIN_STACK_BLK) + 1) * TH_MIN_STACK_BLK;
    base = (u32) (caMemory::Allocate(a_stk));
    if (base != 0) {
        a_stk += base;
        a_stk -= sizeof (caThreadContext);
        ctx = static_cast<caThreadContext *> ((void *) a_stk);
        pst = ((u32) ctx - 64); //64 guard
        ctx->thid = (u32) ctx;
        ctx->status = caThreadStatus::thInit;
        ctx->priority = p;
        ctx->mode = mode;
        caMemAux::MemSet((u32 *) ctx->pcb, 0, 32);
        ctx->pcb[0] = mode; //SPSR
        ctx->pcb[1] = (u32) caThread::LaunchThread;
        ctx->pcb[2] = (u32) func; //r0
        ctx->pcb[3] = par1;
        ctx->pcb[4] = par2;
        ctx->pcb[15] = (u32) pst; //r13
        ctx->pcb[16] = (u32) caThread::LaunchThread; //r14
        ctx->count = caSysTimer::GetCount();
        ctx->stack_start = (u32) pst;
        ctx->stack_end = (u32) base + 64; // 64 guard
        ctx->time = 0;
        ctx->sleep = 0;
        ctx->result = -1;
        ctx->cur_prio = ctx->priority;
        ctx->nswitch = 0;
        caStrAux::StrNCpy(ctx->name, name, 64);
        caScheduler::AddTask(ctx);
    }
    return ctx->index;
}

void caThread::LaunchThread(thFunc f, u32 p1, u32 p2) {
    u32 res, idx;
    idx = caScheduler::StartTask();
    res = f(idx, p1, p2);
    caScheduler::EndTask(res);
    while (1); // wait cancellation from scheduler
}

u32 caThread::DestroyThread(u32 idx) {
    u32 res = FALSE;
    caScheduler::RemoveTask(idx);
    return res;
}

void caThread::Dump(caStringStream<s8> & ss, caThreadContext *ctx) {
    if (ctx != NULL) {
        ss << caStringFormat::hex << ctx->index << caStringFormat::dec << ") ";
        caStringFiller p(' ', 18);
        ss.Fix(p);
        ss << (const char *) ctx->name << p;
        ss << " : ";
        switch (ctx->mode) {
            case caThreadMode::MODE_USR:
                ss << "USR : ";
                break;
            case caThreadMode::MODE_FIQ:
                ss << "FIQ : ";
                break;
            case caThreadMode::MODE_IRQ:
                ss << "IRQ : ";
                break;
            case caThreadMode::MODE_SVC:
                ss << "SVC : ";
                break;
            case caThreadMode::MODE_MON:
                ss << "MON : ";
                break;
            case caThreadMode::MODE_ABT:
                ss << "ABT : ";
                break;
            case caThreadMode::MODE_HYP:
                ss << "HYP : ";
                break;
            case caThreadMode::MODE_UND:
                ss << "UND : ";
                break;
            case caThreadMode::MODE_SYS:
                ss << "SYS : ";
                break;
        }
        switch (ctx->priority) {
            case caThreadPriority::caThLevel0:
                ss << "Pri0 : ";
                break;
            case caThreadPriority::caThLevel1:
                ss << "Pri1 : ";
                break;
            case caThreadPriority::caThLevel2:
                ss << "Pri2 : ";
                break;
            case caThreadPriority::caThLevel3:
                ss << "Pri3 : ";
                break;
            case caThreadPriority::caThLevel4:
                ss << "Pri4 : ";
                break;
            case caThreadPriority::caThLevel5:
                ss << "Pri5 : ";
                break;
            case caThreadPriority::caThLevel6:
                ss << "Pri6 : ";
                break;
            default:
                ss << "Pri" << (u32) ctx->priority << ": ";
                break;
        }
        p.width = 12;
        ss.Fix(p);
        switch (ctx->status) {
            case caThreadStatus::thInit:
                ss << "INIT";
                break;
            case caThreadStatus::thStop:
                ss << "STOP";
                break;
            case caThreadStatus::thRun:
                ss << "RUN";
                break;
            case caThreadStatus::thSleep:
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

