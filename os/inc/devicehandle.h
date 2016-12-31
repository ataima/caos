#ifndef DEVICEHANDLE_H
#define DEVICEHANDLE_H

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

struct caIDeviceHandle {
};

typedef enum tag_device_action {
    caActionOpen = 0x1000,
    caActionClose,
    caActionWrite,
    caActionRead,
    caActionIoCtrl,
    caActionFlush
} caDeviceAction;

struct caDeviceHandle
: public caIDeviceHandle {
public:

    typedef enum tag_status_handle {
        Undef,
        Close,
        Open,
    } statusHandle;

public:
    // status 
    u32 handle;
    statusHandle status;
    u32 tStart;
    u32 tStop;
    u32 tLast;
    caDeviceAction tLastCmd;
    // Wr
    u8* wrBuff;
    u32 wrSize;
    u32 writed;
    u32 wrError;
    // Rd
    u8* rdBuff;
    u32 rdSize;
    u32 readed;
    u32 rdError;

public:
    static void Dump(caDeviceHandle & port, caStringStream<s8> & ss);
};


#endif /* DEVICEHANDLE_H */

