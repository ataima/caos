#ifndef HALCOMDEVICE_H
#define HALCOMDEVICE_H

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
#include "circularbuffer.h"
#include "atomiclock.h"
#include "syslog.h"

class caHalComDevice
: public IDevice {
private:
    static const u32 QUEUESIZE = 0x4000;
    static u32 guid;
    u32 isOpen;
    u32 eOverrun;
    u32 signalRx;
    u32 signalTx;
    caCircularBuffer<u8> Rx;
    caCircularBuffer<u8> Tx;
    u8 RxBuffer[QUEUESIZE];
    u8 TxBuffer[QUEUESIZE];
    caAtomicLock RxLock;
    caAtomicLock TxLock;
    bool IsValidHandle(u32 handle);
    caSysLog caLog;
    const hal_ll_com_io *link;
private:
    u32 addHandle(void);
public:
    caHalComDevice(const hal_ll_com_io *com);
    u32 IoctlReq(ioCtrlFunction request, u32 *p1, u32 *p2);    
    u32 Open(caIDeviceConfigure *conf, caDeviceHandle *port);
    u32 Close(caDeviceHandle *port);
    u32 Write(caDeviceHandle *port);
    u32 Read(caDeviceHandle *port);
    u32 IoCtrl(caDeviceHandle *port, caIDeviceCtrl *in);
    u32 Flush(caDeviceHandle *port);
    inline u32 GetOpenFlag(void) {return isOpen;}
};



#endif /* HALCOMDEVICE_H */

