#ifndef CCOMDEVICE_HEADER
#define CCOMDEVICE_HEADER

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

#include "idevice.h"
#include "circularbuffer.h"
#include "stream.h"
#include "atomiclock.h"
#include "syslog.h"

struct caComDeviceConfigure
: public caIDeviceConfigure {
public:
    u32 speed;
    u32 stop;
    u32 parity;
    u32 data;

    void Dump(caStringStream<s8> & ss) {
        ss << "SETUP SPEED = " << speed << caEnd::endl;
        ss << "SETUP STOP = " << stop << caEnd::endl;
        ss << "SETUP PARITY = " << parity << caEnd::endl;
        ss << "SETUP DATA = " << data << caEnd::endl;
        ss.Str();
    }


};

struct caComDeviceCtrl
: public caIDeviceCtrl {
public:

    typedef enum tag_io_ctrl_specific_request {
        comFlush = 0x5000,
        comStop,
        comStart,
        comListHardware,
        comStatusBuffer,
        comAddSignalRx,
        comAddSignalTx,
        comRemoveSignalRx,
        comRemoveSignalTx,
        comStartLog,
        comStopLog,
        comGetLog
    } IoCtrlDirect;

    IoCtrlDirect command;
    caStringStream<s8> *ss;
    u32 st_rx;
    u32 st_tx;
};

class caComDevice {
private:
    static const u32 QUEUESIZE = 0x4000;
    static u32 guid;
    static u32 isOpen;
    static u32 eOverrun;
    static u32 signalRx;
    static u32 signalTx;
    static caCircularBuffer<u8> Rx;
    static caCircularBuffer<u8> Tx;
    static u8 RxBuffer[QUEUESIZE];
    static u8 TxBuffer[QUEUESIZE];
    static caAtomicLock RxLock;
    static caAtomicLock TxLock;
    static u32 Flush(caDevicePort *port);
    static bool IsValidHandle(u32 handle);
    static caSysLog caLog;
public:
    static u32 Open(caComDeviceConfigure *in, caDevicePort *out);
    static u32 Close(caDevicePort *port);
    static u32 Write(caDevicePort *port);
    static u32 Read(caDevicePort *port);
    static u32 IoCtrl(caDevicePort *port, caComDeviceCtrl *in);
    static u32 IoctlReq(ioCtrlFunction request, u32 *p1, u32 *p2);
    static void IrqService(void);
};


#endif 

