#ifndef CA_SCHEDULER_DEVICE_HEADER
#define CA_SCHEDULER_DEVICE_HEADER


////////////////////////////////////////////////////////////////////////////////
//    Copyright (C) 2011  Angelo Coppi (angelogkcop at hotmail.com )
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


#if SCHEDULER_DEVICE

#include "idevice.h"

struct caSchedulerDeviceConfigure
: public caIDeviceConfigure {
public:
    //TO DO
};

struct caSchedulerDeviceCtrl
: public caIDeviceCtrl {
public:

    typedef enum tag_io_ctrl_specific_request {
        schGetThid = 0x5000,
    } IoCtrlDirect;

    IoCtrlDirect command;
    u32 io;
};

class caSchedulerDevice {
private:
    static u32 guid;
    static u32 isOpen;
    static bool IsValidHandle(u32 handle);
public:
    static u32 Open(caSchedulerDeviceConfigure *in, caDevicePort *out);
    static u32 Close(caDevicePort *port);
    static u32 Write(caDevicePort *port);
    static u32 Read(caDevicePort *port);
    static u32 IoCtrl(caDevicePort *port, caSchedulerDeviceCtrl *in);
    static u32 IoctlReq(ioCtrlFunction request, u32 *p1, u32 *p2);
};


#endif 


#endif 

