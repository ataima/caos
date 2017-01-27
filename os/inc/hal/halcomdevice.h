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

struct caComDeviceConfigure
: public caIDeviceConfigure {
public:
    u32 speed;
    u32 stop;
    u32 parity;
    u32 data;

    void Dump(caStringStream<s8> & ss) {
        caCSTR(cs_speed, "SETUP SPEED  = ");
        caCSTR(cs_stop, "SETUP STOP   = ");
        caCSTR(cs_parity, "SETUP PARITY = ");
        caCSTR(cs_data, "SETUP DATA   = ");
        ss << cs_speed << speed << caEnd::endl;
        ss << cs_stop << stop << caEnd::endl;
        ss << cs_parity << parity << caEnd::endl;
        ss << cs_data << data << caEnd::endl;
        ss.Str();
    }


};

struct caComDeviceCtrl
: public caIDeviceCtrl {
public:

    typedef enum tag_io_ctrl_com_specific_request {
        comFlush = 0x5000,
        comStop,
        comStart,
        comListHardware,
        comStatusBuffer,
        comAddSignalRx,
        comAddSignalTx,
        comRemoveSignalRx,
        comRemoveSignalTx,
        comGetSignalRx,
        comGetSignalTx,
    } IoComCtrlDirect;

};

class caHalComDevice
: public IDevice {
private:
    static const u32 QUEUESIZE = 0x4000;
    u32 mask_guid;
    u32 handle_guid;
    u32 isOpen;
    u32 signalRx;
    u32 signalTx;
    caCircularBuffer<u8> Rx;
    caCircularBuffer<u8> Tx;
    u8 RxBuffer[QUEUESIZE];
    u8 TxBuffer[QUEUESIZE];
    caAtomicLock RxLock;
    caAtomicLock TxLock;
    caSysLog caLog;
    hal_llc_com_io *link;

public:
    caHalComDevice(hal_llc_com_io *com, u32 mask_handle);
    u32 IoctlReq(ioCtrlFunction request, u32 *p1, u32 *p2);
    u32 Open(caIDeviceConfigure *conf, caDeviceHandle *port);
    u32 Close(caDeviceHandle *port);
    u32 Write(caDeviceHandle *port);
    u32 Read(caDeviceHandle *port);
    u32 IoCtrl(caDeviceHandle *port, caIDeviceCtrl *in);
    u32 Flush(caDeviceHandle *port);
    u32 IrqService1(u8 * txbuff, s_t size, s_t & writed);
    u32 IrqService2(u8 * rxbuff, s_t size, s_t & readed);
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


};



#endif /* HALCOMDEVICE_H */

