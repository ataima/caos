#ifndef IDEVICE_HEADER
#define IDEVICE_HEADER

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

#include "devicehandle.h"
#include "memaux.h"

struct caIDeviceConfigure {
};

struct caIDeviceCtrl {

    typedef enum tag_io_ctrl_specific_request {
        ctrl_none = 0,
        ctrl_LogCreate,
        ctrl_LogDestroy,
        ctrl_LogStart,
        ctrl_LogStop,
        ctrl_LogGet
    } IoCtrlDirect;

    caIDeviceCtrl() {
        caMemAux<s_t>::MemZero(params, sizeof (params));
        ss = NULL;
        command = 0;
    }

    s_t command;
    caStringStream<s8> *ss;
    s_t params[8];
};

/*  utility to convert to any struct from params ex:
 * struct dummy {  int a,u32 b... } only to to change param name...
 * struct dummy *p=caAssociate<struct dummy>(caIDeviceCtrl &t);
 */
template <typename T>
class caAttach {
public:

    static T* link(caIDeviceCtrl &t) {
        return reinterpret_cast<T*> (&t.params);
    }

    static T* link(caIDeviceCtrl *t) {
        return reinterpret_cast<T*> (&t->params);
    }

};

typedef enum tag_id_device {
    id_Mem = 0x100,
    id_Task = 0x200,
    id_Pipe = 0x300,
    id_SysTimer = 0x400,
    id_Cache = 0x500,
    id_Com = 0x600,
} id_device;


#define IO_UID(a)    (((u32)(a))<<16)
#define IO_CTRL_UID(a)   (((u32)(a))<<8)

typedef enum tag_ioctrl_request {
    no_ioctrl = 0,
    Memory = IO_UID(id_device::id_Mem),
    Task = IO_UID(id_device::id_Task),
    MemPipe = IO_UID(id_device::id_Pipe),
    SysTimer1 = IO_UID(id_device::id_SysTimer),
    SysTimer2 = IO_UID(id_device::id_SysTimer + 1),
    SysTimer3 = IO_UID(id_device::id_SysTimer + 2),
    SysTimer4 = IO_UID(id_device::id_SysTimer + 3),
    SysTimer5 = IO_UID(id_device::id_SysTimer + 4),
    SysTimer6 = IO_UID(id_device::id_SysTimer + 5),
    SysTimer7 = IO_UID(id_device::id_SysTimer + 6),
    SysTimer8 = IO_UID(id_device::id_SysTimer + 7),
    Cache = IO_UID(id_device::id_Cache),
    //COMS
    Com1 = IO_UID(id_device::id_Com),
    Com2 = IO_UID(id_device::id_Com + 1),
    Com3 = IO_UID(id_device::id_Com + 2),
    Com4 = IO_UID(id_device::id_Com + 3),
    Com5 = IO_UID(id_device::id_Com + 4),
    Com6 = IO_UID(id_device::id_Com + 5),
    Com7 = IO_UID(id_device::id_Com + 6),
    Com8 = IO_UID(id_device::id_Com + 7),
    maskIoCtrl = 0xffff0000,
    maskHandle = 0x0000ffff
} ioCtrlRequest;

typedef enum tag_software_request {
    // IOCTRL MEMORY SPECIFIC
    caMemoryAlloc = 1000,
    caMemoryFree,
    caMemoryListAll,
    caMemoryDump,
    caMemoryAsciiDump,
    // generic driver        
    caOpenDevice,
    caCloseDevice,
    caReadDevice,
    caWriteDevice,
    caIoCtrlDevice,
    // task
    caIOCreate,
    caIoDestroy,
    // scheduler        
    caIoSleep,
    caSchedulerListAll
} ioCtrlFunction;

typedef struct tag_mem_dump_addr {
    s8 *buffo;
    s_t size;
    s_t addr;
} dumpAddrReq;



class caSysLog;

class IDevice {
public:
    // METHOD TO SYSTEM MODE

    virtual u32 Open(caIDeviceConfigure *conf, caDeviceHandle *port) = 0;

    virtual u32 Close(caDeviceHandle *port) = 0;

    virtual u32 Write(caDeviceHandle *port) = 0;

    virtual u32 Read(caDeviceHandle *port) = 0;

    virtual u32 Flush(caDeviceHandle *port) = 0;

    virtual u32 IoCtrl(caDeviceHandle *port, caIDeviceCtrl *ctrl) = 0;

    virtual u32 GetOpenFlag(void) = 0;

    virtual caSysLog * GetDeviceLog(void) = 0;

    virtual const char * toString(void) = 0;

    virtual u32 IrqService1(u8 * buff, s_t size, s_t & iosize) = 0;

    virtual u32 IrqService2(u8 * buff, s_t size, s_t & iosize) = 0;

    virtual u32 IrqService3(u8 * buff, s_t size, s_t & iosize) = 0;

    virtual u32 IrqService4(u8 * buff, s_t size, s_t & iosize) = 0;

    virtual u32 IrqService5(u8 * buff, s_t size, s_t & iosize) = 0;

    virtual u32 IrqService6(u8 * buff, s_t size, s_t & iosize) = 0;

    virtual u32 IrqService7(u8 * buff, s_t size, s_t & iosize) = 0;

    virtual u32 IrqService8(u8 * buff, s_t size, s_t & iosize) = 0;
    // METHOD LAUNCHED BY SVC FROM USER MODE
};

class caHalDeviceRules {
public:
    static u32 Open(IDevice *dev, caIDeviceConfigure * setup,
            caDeviceHandle *port, u32 guid);
    static u32 Close(IDevice *dev, caDeviceHandle *port, u32 guid);
    static u32 Write(IDevice *dev, caDeviceHandle *port, u32 guid);
    static u32 Read(IDevice *dev, caDeviceHandle *port, u32 guid);
    static u32 IoCtrl(IDevice *dev, caDeviceHandle *port, caIDeviceCtrl *inp,
            u32 guid);
    static u32 IoctlReq(IDevice *dev, ioCtrlFunction request, u32 *p1, u32 *p2);
    static u32 Flush(IDevice *dev, caDeviceHandle *port, u32 guid);
    static bool IsValidHandle(u32 handle, u32 mask);
    static u32 isOpen(IDevice *device);
    static u32 IrqService1(void *obj, u8 * buff, s_t size, s_t & iosize);
    static u32 IrqService2(void *obj, u8 * buff, s_t size, s_t & iosize);
    static u32 IrqService3(void *obj, u8 * buff, s_t size, s_t & iosize);
    static u32 IrqService4(void *obj, u8 * buff, s_t size, s_t & iosize);
    static u32 IrqService5(void *obj, u8 * buff, s_t size, s_t & iosize);
    static u32 IrqService6(void *obj, u8 * buff, s_t size, s_t & iosize);
    static u32 IrqService7(void *obj, u8 * buff, s_t size, s_t & iosize);
    static u32 IrqService8(void *obj, u8 * buff, s_t size, s_t & iosize);

    static inline u32 addHandle(u32 & guid, u32 & mask) {
        guid++;
        return mask | guid;
    }
};

typedef enum tag_device_error {
    no_error = 0, // false OK
    error_generic = 1, // else error
    error_unknow_device_name = 1000,
    error_unknow_device_ioctrl,
    error_open_device,
    error_close_device,
    error_write_device,
    error_read_device,
    error_ioctrl_device,
    error_device_config_param,
    error_port_out_config,
    error_pipe_no_memory,
    error_pipe_invalid_name,
    error_pipe_not_exist,
    error_pipe_no_free_resize,
    error_pipe_queue_full,
    error_pipe_port_not_host,
    error_pipe_port_not_guest,
    error_pipe_invalid_descriptor,
    error_invalid_null_port,
    error_invalid_null_buffer_port,
    error_invalid_null_device,
    error_invalid_handle_port,
    error_invalid_handle_port_wr_size,
    error_invalid_handle_port_rd_size,
    error_device_not_opened,
    error_device_already_opened,
    error_read_less_data,
    error_write_less_data,
    error_configure_serial_port,
    error_configure_irq_serial_port,
    error_stop_serial_port,
    error_tx_queue_full,
    error_rx_queue_empty,
    error_invalid_null_destination,
    error_device_is_busy,
    error_signal_already_set,
    error_cannot_create_log,
    error_cannot_destroy_log,
    error_log_already_set,
    error_log_not_set,
    error_log_empthy,
    error_log_null,
    error_stream_no_good,
    error_com_dump,
    error_systimer_configure_not_valid,
    error_systimer_configure_error,
    error_systimer_dump,
    error_ioctrl_command_error,
    error_hal_configure,
    error_hal_job_remove,
    error_hal_jobs_remove_all,
    error_hal_job_dump,
    error_hal_job_sv_create,
    error_hal_job_sys_create,
    error_hal_job_user_create,
    error_hal_job_wait_for_signal,
    error_hal_job_sleep,
    error_hal_job_change_priority,
} deviceError;




#endif 

