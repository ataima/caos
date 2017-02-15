#ifndef _HAL_PIPE_DEVICE_H
#define _HAL_PIPE_DEVICE_H

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
#include "syslog.h"


#define MAX_PIPE_BLOCK    32         //aligned 2^n 2,4,8,16
#define MAX_PIPE_MASK_ID_BLOCK  (MAX_PIPE_BLOCK-1)
#define MAX_PIPE_BLOCK_NAME   32

struct caPipeDeviceCtrl
: public caIDeviceCtrl {
public:
   typedef enum tag_io_ctrl_specific_request {
        Resize = 0x5000,
        Dump,
        Reset
    } IoPipeCtrlDirect;
};

struct caPipeDeviceConfigure
: public caIDeviceConfigure {
public:

    typedef enum tag_request_mode {
        host,
        guest
    } requestMode;
    char name[MAX_PIPE_BLOCK_NAME];
    u32 size; // 1000-> u32[1000]
    requestMode host_guest; // 0 host 1 guest
};

struct caPipeDeviceDescriptor {

    typedef enum tag_action_shared_mem {
        unknow = 0,
        writed = 0x1000,
        readed = 0x2000,
        opened = 0x4000
    } actionMem;
    char name[MAX_PIPE_BLOCK_NAME];
    u32 size;
    u32 index;
    u32 host;
    u32 guest;
    actionMem action;
    caCircularBuffer<u32> queue;
};



class caHalPipeDevice
: public IDevice {
private:
    caPipeDeviceDescriptor descriptors[MAX_PIPE_BLOCK];
    u32 mask_guid;
    u32 handle_guid;
    u32 isOpen;
    caSysLog caLog;
    hal_llc_mem_io *link;
protected:
    caPipeDeviceDescriptor * FindDescriptor(caPipeDeviceConfigure* setup);
    caPipeDeviceDescriptor * GetDescriptor(u32 handle);
    u32 Resize(caDeviceHandle *port, u32 size);
    u32 Dump(caDeviceHandle *port, caStringStream<s8> *ss);
public:
    caHalPipeDevice(hal_llc_mem_io *iface, u32 mask_handle);
    u32 Open(caIDeviceConfigure *conf, caDeviceHandle *port);
    u32 Close(caDeviceHandle *port);
    u32 Write(caDeviceHandle *port);
    u32 Read(caDeviceHandle *port);
    u32 IoCtrl(caDeviceHandle *port, caIDeviceCtrl *in);
    u32 Flush(caDeviceHandle *port);
    u32 IrqService1(u8 * buff, s_t size, s_t & iosize);
    u32 IrqService2(u8 * buff, s_t size, s_t & iosize);
    u32 IrqService3(u8 * buff, s_t size, s_t & iosize);
    u32 IrqService4(u8 * buff, s_t size, s_t & iosize);
    u32 IrqService5(u8 * buff, s_t size, s_t & iosize);
    u32 IrqService6(u8 * buff, s_t size, s_t & iosize);
    u32 IrqService7(u8 * buff, s_t size, s_t & iosize);
    u32 IrqService8(u8 * buff, s_t size, s_t & iosize);

    inline caSysLog * GetDeviceLog(void) {
        return &caLog;
    }

    inline u32 GetOpenFlag(void) {
        return isOpen;
    }

    inline const char * toString(void) {
        return "Hal Memory driver ('Mem') :";
    }

};



#endif  //_HAL_PIPE_DEVICE_H

