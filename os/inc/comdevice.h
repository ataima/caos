#ifndef CCOMDEVICE_HEADER
#define CCOMDEVICE_HEADER


#include "idevice.h"
#include "circularbuffer.h"
#include "stream.h"
#include "atomiclock.h"

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

