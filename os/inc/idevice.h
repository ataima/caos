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

#define BASE_HANDLE  0x1000

struct caIDeviceConfigure {
};

struct caIDeviceCtrl {
};

struct caIDevicePort {
};

typedef enum tag_device_action {
    caActionOpen = 0x1000,
    caActionClose,
    caActionWrite,
    caActionRead,
    caActionIoCtrl,
} caDeviceAction;

struct caDevicePort
: public caIDevicePort {
public:

    typedef enum tag_status_port {
        Undef,
        Close,
        Open,
    } statusPort;

public:
    // status 
    u32 handle;
    statusPort status;
    u32 error;
    u32 tStart;
    u32 tStop;
    u32 tLast;
    caDeviceAction tLastCmd;
    // Wr
    u8* wrBuff;
    u32 wrSize;
    u32 writed;
    // rd
    u8* rdBuff;
    u32 rdSize;
    u32 readed;


public:
    bool IsValidHandle(void);
};

#define IO_UID(a)    ((a)<<20)

typedef enum tag_ioctrl_request {
    no_ioctrl = 0,
    Memory = IO_UID(1),
    Scheduler = IO_UID(2),
    Exception = IO_UID(3),
    Task = IO_UID(4),
    Com1 = IO_UID(5),
    MemPipe = IO_UID(6),
    SysTimer = IO_UID(7),
    Cache = IO_UID(8),
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
    size_t size;
    size_t addr;
} dumpAddrReq;

/* AL DEVICE CLASSES ARE STATIC ... BUT IMPLEMENT ALL SAME METHOD AS VIRTUAL CLASSES*/

class IDevice {
public:
    // METHOD TO SYSTEM MODE
    virtual u32 Open(caIDeviceConfigure *in, caIDevicePort *out) = 0;
    virtual u32 Close(caIDevicePort *port) = 0;
    virtual u32 Write(caIDevicePort *port) = 0;
    virtual u32 Read(caIDevicePort *port) = 0;
    virtual u32 IoCtrl(caIDevicePort *port, caIDeviceCtrl *in) = 0;
    // METHOD LAUNCHED BY SVC FROM USER MODE
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
    error_log_empthy
} deviceError;

class caDevice {
private:
    static ioCtrlRequest GetIoCtrlRequest(const char * name);
    static ioCtrlRequest GetIoCtrlRequest(u32 handle);
public:
    static deviceError Open(const char * device, caIDeviceConfigure & in, caDevicePort & out);
    static deviceError Close(caDevicePort & port);
    static deviceError Write(caDevicePort & port);
    static deviceError Read(caDevicePort & port);
    static deviceError IoCtrl(caDevicePort & port, caIDeviceCtrl &in);
    static bool IsValidHandle(caDevicePort & port, ioCtrlRequest & req);
};




#endif 

