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

struct caIDeviceConfigure {
};

struct caIDeviceCtrl {
};





#define IO_UID(a)    ((a)<<20)

typedef enum tag_ioctrl_request {
    no_ioctrl = 0,
    Memory = IO_UID(1),
    Scheduler = IO_UID(2),
    Exception = IO_UID(3),
    Task = IO_UID(4),
    MemPipe = IO_UID(5),
    SysTimer = IO_UID(6),
    Cache = IO_UID(7),
    //COMS
    Com1 = IO_UID(8),
    Com2 = IO_UID(9),
    Com3 = IO_UID(10),
    Com4 = IO_UID(11),
    Com5 = IO_UID(12),
    Com6 = IO_UID(13),
    Com7 = IO_UID(14),
    Com8 = IO_UID(15),

    maskIoCtrl = 0xfff00000,
    maskHandle = 0x000fffff
} ioCtrlRequest;

typedef enum tag_software_request {
    // IOCTRL MEMORY SPECIFIC
    caMemoryAlloc = 1000,
    caMemoryFree,
    caMemoryListAll,
    caMemoryDump,
    caMemoryAsciiDump,
    // IOCTRL EXCEPTIONS
    caExceptionBadAlloc,
    caExceptionLengthError,
    caExceptionOutOfRange,
    caExceptionInvalidObject,
    caExceptionObjectFull,
    caExceptionObjectEmpty,
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

class IDevice {
public:
    // METHOD TO SYSTEM MODE

    virtual u32 Open(caIDeviceConfigure *conf, caDeviceHandle *port) = 0;

    virtual u32 Close(caDeviceHandle *port) = 0; 

    virtual u32 Write(caDeviceHandle *port)  = 0; 

    virtual u32 Read(caDeviceHandle *port)  = 0; 

    virtual u32 Flush(caDeviceHandle *port) = 0; 

    virtual u32 IoCtrl(caDeviceHandle *port, caIDeviceCtrl *ctrl) = 0; 

    virtual u32 GetOpenFlag(void) = 0; 

    // METHOD LAUNCHED BY SVC FROM USER MODE
};

class caHalDeviceRules {
public:
    static u32 Open(IDevice *dev, caIDeviceConfigure * setup, caDeviceHandle *port, u32 guid);
    static u32 Close(IDevice *dev, caDeviceHandle *port, u32 guid);
    static u32 Write(IDevice *dev, caDeviceHandle *port, u32 guid);
    static u32 Read(IDevice *dev, caDeviceHandle *port, u32 guid);
    static u32 IoCtrl(IDevice *dev, caDeviceHandle *port, caIDeviceCtrl *inp, u32 guid);
    static u32 IoctlReq(IDevice *dev, ioCtrlFunction request, u32 *p1, u32 *p2);
    static u32 Flush(IDevice *dev, caDeviceHandle *port, u32 guid);
    static bool IsValidHandle(u32 handle, u32 mask);
    static u32 isOpen(IDevice *device);
};

typedef enum tag_device_error {
    no_error = 0,
    okey = 1,
    error_unknow_device_name = 1000,
    error_unknow_device_ioctrl,
    error_open_device,
    error_close_device,
    error_write_device,
    error_read_device,
    error_Ioctrl_device,
    error_generic_fail_device,
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
    error_device_not_opened,
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
    error_log_already_set,
    error_log_not_set,
    error_log_empthy,
    error_stream_no_good,
    error_hal_configure
} deviceError;






#endif 

