#ifndef SOFTREQ_HEADER
#define SOFTREQ_HEADER


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

#if SYS_SOFT_REQ_ENABLED

#include "idevice.h"

class caSoftRequest {
public:


static inline  void SVC_IOCTL(register u32 /*R0= ioctl*/, 
        register u32 */*R1=input */, 
        register u32 */*R2=output*/, 
        register u32 * /*R3=res*/)__attribute__((naked)) 
        {
        asm volatile ("DSB"); //TOTEST
        asm volatile ("ISB"); //TOTEST
        asm volatile ("SVC #1");
        asm volatile ("bx lr"); 
        asm volatile ("bx lr"); 
        asm volatile ("bx lr"); 
        asm volatile ("bx lr"); 
        asm volatile ("bx lr"); 
        asm volatile ("bx lr"); 
        asm volatile ("bx lr"); 
        asm volatile ("bx lr"); 
        asm volatile ("bx lr"); 
        asm volatile ("bx lr"); 
        asm volatile ("bx lr"); 
    }

    static void DumpSvc(const char *name,const char *file, u32 line);
};

void inline MemoryAlloc(u32 * size/*IN*/, u32* out/*OUT*/, u32 *res) {
#if SYS_SOFT_REQ_DEBUG   
    caSoftRequest::DumpSvc(__FUNCTION__,__FILE__,__LINE__);
#endif    
    return caSoftRequest::SVC_IOCTL(ioCtrlRequest::Memory |
            ioCtrlFunction::caMemoryAlloc,
            size,
            out,
            res);
}

void inline MemoryFree(u32* p/*IN*/, u32* size/*OUT*/, u32 * res) {
#if SYS_SOFT_REQ_DEBUG   
    caSoftRequest::DumpSvc(__FUNCTION__,__FILE__,__LINE__);
#endif    
    return caSoftRequest::SVC_IOCTL(ioCtrlRequest::Memory |
            ioCtrlFunction::caMemoryFree,
            p,
            size,
            res);
}

void inline MemoryList(s8 * buff, u32 size, u32 * res) {
#if SYS_SOFT_REQ_DEBUG   
    caSoftRequest::DumpSvc(__FUNCTION__,__FILE__,__LINE__);
#endif    
    return caSoftRequest::SVC_IOCTL(ioCtrlRequest::Memory |
            ioCtrlFunction::caMemoryListAll,
            (u32*) buff,
            (u32*) size,
            res);
}

void inline MemoryDump(dumpAddrReq * req, u32 * res) {
#if SYS_SOFT_REQ_DEBUG   
    caSoftRequest::DumpSvc(__FUNCTION__,__FILE__,__LINE__);
#endif    
    return caSoftRequest::SVC_IOCTL(ioCtrlRequest::Memory |
            ioCtrlFunction::caMemoryDump,
            (u32*) req,
            0,
            res);
}

void inline MemoryAsciiDump(dumpAddrReq * req, u32 * res) {
#if SYS_SOFT_REQ_DEBUG   
    caSoftRequest::DumpSvc(__FUNCTION__,__FILE__,__LINE__);
#endif    
    return caSoftRequest::SVC_IOCTL(ioCtrlRequest::Memory |
            ioCtrlFunction::caMemoryAsciiDump,
            (u32*) req,
            0,
            res);
}

void inline ThrowExceptionBadAlloc(u32 *addr, u32 *res) {
#if SYS_SOFT_REQ_DEBUG   
    caSoftRequest::DumpSvc(__FUNCTION__,__FILE__,__LINE__);
#endif    
    return caSoftRequest::SVC_IOCTL(ioCtrlRequest::Exception |
            ioCtrlFunction::caExceptionBadAlloc,
            addr,
            0,
            res);
}

void inline ThrowExceptionLenghtError(u32 *addr, u32 *res) {
#if SYS_SOFT_REQ_DEBUG   
    caSoftRequest::DumpSvc(__FUNCTION__,__FILE__,__LINE__);
#endif    
    return caSoftRequest::SVC_IOCTL(ioCtrlRequest::Exception |
            ioCtrlFunction::caExceptionLengthError,
            addr,
            0,
            res);
}

void inline ThrowExceptionOutOfRange(u32 *addr, const char *s, u32 *res) {
#if SYS_SOFT_REQ_DEBUG   
    caSoftRequest::DumpSvc(__FUNCTION__,__FILE__,__LINE__);
#endif    
    return caSoftRequest::SVC_IOCTL(ioCtrlRequest::Exception |
            ioCtrlFunction::caExceptionOutOfRange,
            addr,
            (u32 *) s,
            res);
}

void inline ThrowExceptionOInvalidObject(u32 *addr, const char *s, u32 *res) {
#if SYS_SOFT_REQ_DEBUG   
    caSoftRequest::DumpSvc(__FUNCTION__,__FILE__,__LINE__);
#endif    
    return caSoftRequest::SVC_IOCTL(ioCtrlRequest::Exception |
            ioCtrlFunction::caExceptionInvalidObject,
            addr,
            (u32 *) s,
            res);
}

void inline ThrowExceptionObjectFull(u32 *addr, const char *s, u32 *res) {
#if SYS_SOFT_REQ_DEBUG   
    caSoftRequest::DumpSvc(__FUNCTION__,__FILE__,__LINE__);
#endif    
    return caSoftRequest::SVC_IOCTL(ioCtrlRequest::Exception |
            ioCtrlFunction::caExceptionObjectFull,
            addr,
            (u32 *) s,
            res);
}

void inline ThrowExceptionObjectEmpty(u32 *addr, const char *s, u32 *res) {
#if SYS_SOFT_REQ_DEBUG   
    caSoftRequest::DumpSvc(__FUNCTION__,__FILE__,__LINE__);
#endif    
    return caSoftRequest::SVC_IOCTL(ioCtrlRequest::Exception |
            ioCtrlFunction::caExceptionObjectEmpty,
            addr,
            (u32 *) s,
            res);
}

// COM1 DEVICE IOCTRL USER TO SUPERVISOR MODE
#if COM1_DEVICE         

void inline Com1OpenDevice(caIDeviceConfigure *in, caIDevicePort *out, u32 *res) {
#if SYS_SOFT_REQ_DEBUG   
    caSoftRequest::DumpSvc(__FUNCTION__,__FILE__,__LINE__);
#endif    
    return caSoftRequest::SVC_IOCTL(ioCtrlRequest::Com1 |
            ioCtrlFunction::caOpenDevice,
            reinterpret_cast<u32 *> (in),
            reinterpret_cast<u32 *> (out),
            res);
}

void inline Com1CloseDevice(caIDevicePort *p, u32 *res) {
#if SYS_SOFT_REQ_DEBUG   
    caSoftRequest::DumpSvc(__FUNCTION__,__FILE__,__LINE__);
#endif    
    return caSoftRequest::SVC_IOCTL(ioCtrlRequest::Com1 |
            ioCtrlFunction::caCloseDevice,
            reinterpret_cast<u32 *> (p),
            0,
            res);
}

void inline Com1WriteDevice(caIDevicePort *p, u32 *res) {
#if SYS_SOFT_REQ_DEBUG   
    caSoftRequest::DumpSvc(__FUNCTION__,__FILE__,__LINE__);
#endif    
    return caSoftRequest::SVC_IOCTL(ioCtrlRequest::Com1 |
            ioCtrlFunction::caWriteDevice,
            reinterpret_cast<u32 *> (p),
            0,
            res);
}

void inline Com1ReadDevice(caIDevicePort *p, u32 *res) {
#if SYS_SOFT_REQ_DEBUG   
    caSoftRequest::DumpSvc(__FUNCTION__,__FILE__,__LINE__);
#endif    
    return caSoftRequest::SVC_IOCTL(ioCtrlRequest::Com1 |
            ioCtrlFunction::caReadDevice,
            reinterpret_cast<u32 *> (p),
            0,
            res);
}

void inline Com1IoCtrlDevice(caIDevicePort *p1, caIDeviceCtrl *p2, u32 *res) {
#if SYS_SOFT_REQ_DEBUG   
    caSoftRequest::DumpSvc(__FUNCTION__,__FILE__,__LINE__);
#endif    
    return caSoftRequest::SVC_IOCTL(ioCtrlRequest::Com1 |
            ioCtrlFunction::caIoCtrlDevice,
            reinterpret_cast<u32 *> (p1),
            reinterpret_cast<u32 *> (p2),
            res);
}
#endif
// MEMPIPE DEVICE IOCTRL USER TO SUPERVISOR MODE
#if  MEM_PIPE_DEVICE

void inline MemPipeOpenDevice(caIDeviceConfigure *in, caIDevicePort *out, u32 *res) {
#if SYS_SOFT_REQ_DEBUG   
    caSoftRequest::DumpSvc(__FUNCTION__,__FILE__,__LINE__);
#endif    
    return caSoftRequest::SVC_IOCTL(ioCtrlRequest::MemPipe |
            ioCtrlFunction::caOpenDevice,
            reinterpret_cast<u32 *> (in),
            reinterpret_cast<u32 *> (out),
            (res));
}

void inline MemPipeCloseDevice(caIDevicePort *p, u32 *res) {
#if SYS_SOFT_REQ_DEBUG   
    caSoftRequest::DumpSvc(__FUNCTION__,__FILE__,__LINE__);
#endif    
    return caSoftRequest::SVC_IOCTL(ioCtrlRequest::MemPipe |
            ioCtrlFunction::caCloseDevice,
            reinterpret_cast<u32 *> (p),
            0,
            (res));
}

void inline MemPipeWriteDevice(caIDevicePort *p, u32 *res) {
#if SYS_SOFT_REQ_DEBUG   
    caSoftRequest::DumpSvc("MemPipeWriteDevice",__FUNCTION__,__LINE__);
#endif    
    caSoftRequest::DumpSvc(__FUNCTION__,__FILE__,__LINE__);
    return caSoftRequest::SVC_IOCTL(ioCtrlRequest::MemPipe |
            ioCtrlFunction::caWriteDevice,
            reinterpret_cast<u32 *> (p),
            0,
            (res));
}

void inline MemPipeReadDevice(caIDevicePort *p, u32 *res) {
#if SYS_SOFT_REQ_DEBUG   
    caSoftRequest::DumpSvc(__FUNCTION__,__FILE__,__LINE__);
#endif    
    return caSoftRequest::SVC_IOCTL(ioCtrlRequest::MemPipe |
            ioCtrlFunction::caReadDevice,
            reinterpret_cast<u32 *> (p),
            0,
            (res));
}

void inline MemPipeIoCtrlDevice(caIDevicePort *p1, caIDeviceCtrl *p2, u32 *res) {
#if SYS_SOFT_REQ_DEBUG   
    caSoftRequest::DumpSvc(__FUNCTION__,__FILE__,__LINE__);
#endif    
    return caSoftRequest::SVC_IOCTL(ioCtrlRequest::MemPipe |
            ioCtrlFunction::caIoCtrlDevice,
            reinterpret_cast<u32 *> (p1),
            reinterpret_cast<u32 *> (p2),
            (res));
}
#endif

void inline SchedulerDump(s8 * buff, u32 size, u32 * res) {
#if SYS_SOFT_REQ_DEBUG   
    caSoftRequest::DumpSvc(__FUNCTION__,__FILE__,__LINE__);
#endif    
    return caSoftRequest::SVC_IOCTL(ioCtrlRequest::Scheduler |
            ioCtrlFunction::caSchedulerListAll,
            (u32*) buff,
            (u32*) size,
            res);
}

#if SYS_TIMER_DEVICE  

void inline SysTimerOpenDevice(caIDeviceConfigure *in, caIDevicePort *out, u32 *res) {
#if SYS_SOFT_REQ_DEBUG   
    caSoftRequest::DumpSvc(__FUNCTION__,__FILE__,__LINE__);
#endif    
    return caSoftRequest::SVC_IOCTL(ioCtrlRequest::SysTimer |
            ioCtrlFunction::caOpenDevice,
            reinterpret_cast<u32 *> (in),
            reinterpret_cast<u32 *> (out),
            res);
}

void inline SysTimerCloseDevice(caIDevicePort *p, u32 *res) {
#if SYS_SOFT_REQ_DEBUG   
    caSoftRequest::DumpSvc(__FUNCTION__,__FILE__,__LINE__);
#endif    
    return caSoftRequest::SVC_IOCTL(ioCtrlRequest::SysTimer |
            ioCtrlFunction::caCloseDevice,
            reinterpret_cast<u32 *> (p),
            0,
            res);
}

void inline SysTimerWriteDevice(caIDevicePort *p, u32 *res) {
#if SYS_SOFT_REQ_DEBUG   
    caSoftRequest::DumpSvc(__FUNCTION__,__FILE__,__LINE__);
#endif    
    return caSoftRequest::SVC_IOCTL(ioCtrlRequest::SysTimer |
            ioCtrlFunction::caWriteDevice,
            reinterpret_cast<u32 *> (p),
            0,
            res);
}

void inline SysTimerReadDevice(caIDevicePort *p, u32 *res) {
#if SYS_SOFT_REQ_DEBUG   
    caSoftRequest::DumpSvc(__FUNCTION__,__FILE__,__LINE__);
#endif    
    return caSoftRequest::SVC_IOCTL(ioCtrlRequest::SysTimer |
            ioCtrlFunction::caReadDevice,
            reinterpret_cast<u32 *> (p),
            0,
            res);
}

void inline SysTimerIoCtrlDevice(caIDevicePort *p1, caIDeviceCtrl *p2, u32 *res) {
#if SYS_SOFT_REQ_DEBUG   
    caSoftRequest::DumpSvc(__FUNCTION__,__FILE__,__LINE__);
#endif    
    return caSoftRequest::SVC_IOCTL(ioCtrlRequest::SysTimer |
            ioCtrlFunction::caIoCtrlDevice,
            reinterpret_cast<u32 *> (p1),
            reinterpret_cast<u32 *> (p2),
            res);
}
#endif

#if CACHE_DEVICE            

void inline CacheOpenDevice(caIDeviceConfigure *in, caIDevicePort *out, u32 *res) {
#if SYS_SOFT_REQ_DEBUG   
    caSoftRequest::DumpSvc(__FUNCTION__,__FILE__,__LINE__);
#endif    
    return caSoftRequest::SVC_IOCTL(ioCtrlRequest::Cache |
            ioCtrlFunction::caOpenDevice,
            reinterpret_cast<u32 *> (in),
            reinterpret_cast<u32 *> (out),
            res);
}

void inline CacheCloseDevice(caIDevicePort *p, u32 *res) {
#if SYS_SOFT_REQ_DEBUG   
    caSoftRequest::DumpSvc(__FUNCTION__,__FILE__,__LINE__);
#endif    
    return caSoftRequest::SVC_IOCTL(ioCtrlRequest::Cache |
            ioCtrlFunction::caCloseDevice,
            reinterpret_cast<u32 *> (p),
            0,
            res);
}

void inline CacheWriteDevice(caIDevicePort *p, u32 *res) {
#if SYS_SOFT_REQ_DEBUG   
    caSoftRequest::DumpSvc(__FUNCTION__,__FILE__,__LINE__);
#endif    
    return caSoftRequest::SVC_IOCTL(ioCtrlRequest::Cache |
            ioCtrlFunction::caWriteDevice,
            reinterpret_cast<u32 *> (p),
            0,
            res);
}

void inline CacheReadDevice(caIDevicePort *p, u32 *res) {
#if SYS_SOFT_REQ_DEBUG   
    caSoftRequest::DumpSvc(__FUNCTION__,__FILE__,__LINE__);
#endif    
    return caSoftRequest::SVC_IOCTL(ioCtrlRequest::Cache |
            ioCtrlFunction::caReadDevice,
            reinterpret_cast<u32 *> (p),
            0,
            res);
}

void inline CacheIoCtrlDevice(caIDevicePort *p1, caIDeviceCtrl *p2, u32 *res) {
#if SYS_SOFT_REQ_DEBUG   
    caSoftRequest::DumpSvc(__FUNCTION__,__FILE__,__LINE__);
#endif    
    return caSoftRequest::SVC_IOCTL(ioCtrlRequest::Cache |
            ioCtrlFunction::caIoCtrlDevice,
            reinterpret_cast<u32 *> (p1),
            reinterpret_cast<u32 *> (p2),
            res);
}
#endif

// GENERIC DEVICE A STUB FOR ALL IOCTRL   ????


#if SCHEDULER_DEVICE            

void inline SchedulerOpenDevice(caIDeviceConfigure *in, caIDevicePort *out, u32 *res) {
#if SYS_SOFT_REQ_DEBUG   
    caSoftRequest::DumpSvc(__FUNCTION__,__FILE__,__LINE__);
#endif    
    return caSoftRequest::SVC_IOCTL(ioCtrlRequest::Scheduler |
            ioCtrlFunction::caOpenDevice,
            reinterpret_cast<u32 *> (in),
            reinterpret_cast<u32 *> (out),
            res);
}

void inline SchedulerCloseDevice(caIDevicePort *p, u32 *res) {
#if SYS_SOFT_REQ_DEBUG   
    caSoftRequest::DumpSvc(__FUNCTION__,__FILE__,__LINE__);
#endif    
    return caSoftRequest::SVC_IOCTL(ioCtrlRequest::Scheduler |
            ioCtrlFunction::caCloseDevice,
            reinterpret_cast<u32 *> (p),
            0,
            res);
}

void inline SchedulerWriteDevice(caIDevicePort *p, u32 *res) {
#if SYS_SOFT_REQ_DEBUG   
    caSoftRequest::DumpSvc(__FUNCTION__,__FILE__,__LINE__);
#endif    
    return caSoftRequest::SVC_IOCTL(ioCtrlRequest::Scheduler |
            ioCtrlFunction::caWriteDevice,
            reinterpret_cast<u32 *> (p),
            0,
            res);
}

void inline SchedulerReadDevice(caIDevicePort *p, u32 *res) {
#if SYS_SOFT_REQ_DEBUG   
    caSoftRequest::DumpSvc(__FUNCTION__,__FILE__,__LINE__);
#endif    
    return caSoftRequest::SVC_IOCTL(ioCtrlRequest::Scheduler |
            ioCtrlFunction::caReadDevice,
            reinterpret_cast<u32 *> (p),
            0,
            res);
}

void inline SchedulerIoCtrlDevice(caIDevicePort *p1, caIDeviceCtrl *p2, u32 *res) {
#if SYS_SOFT_REQ_DEBUG   
    caSoftRequest::DumpSvc(__FUNCTION__,__FILE__,__LINE__);
#endif    
    return caSoftRequest::SVC_IOCTL(ioCtrlRequest::Scheduler |
            ioCtrlFunction::caIoCtrlDevice,
            reinterpret_cast<u32 *> (p1),
            reinterpret_cast<u32 *> (p2),
            res);
}

void inline SchedulerIoCtrlSleep(u32 *ms, u32 *taskid, u32 *res) {
#if SYS_SOFT_REQ_DEBUG   
    caSoftRequest::DumpSvc(__FUNCTION__,__FILE__,__LINE__);
#endif    
return caSoftRequest::SVC_IOCTL(ioCtrlRequest::Scheduler |
            ioCtrlFunction::caIoSleep,
            ms,
            taskid,
            res);
}


#endif










#endif 



#define SLEEP_FOR_EVER 0xffffffff

u32 Sleep(u32 ms);

inline u32 WaitForSignal(void) {
    return Sleep(SLEEP_FOR_EVER);
}

#endif /* SOFTREQ_H */

