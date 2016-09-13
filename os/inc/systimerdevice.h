#ifndef SYSTIMERDEVICE_H
#define SYSTIMERDEVICE_H

#if SYS_TIMER_DEVICE
#include "idevice.h"

struct caSysTimerDeviceConfigure
: public caIDeviceConfigure {
public:
    //TO DO
};

struct caSysTimerDeviceCtrl
: public caIDeviceCtrl {
public:

    typedef enum tag_io_ctrl_specific_request {
        sysTimerFlush = 0x5000,
        sysTimerListHardware,
    } IoCtrlDirect;
    IoCtrlDirect command;
    caStringStream<s8> *ss;
};

class caSysTimerDevice {
private:
    static u32 guid;
    static u32 isOpen;
    static u32 Flush(caDevicePort *port);
    static bool IsValidHandle(u32 handle);
public:
    static u32 Open(caSysTimerDeviceConfigure *in, caDevicePort *out);
    static u32 Close(caDevicePort *port);
    static u32 Write(caDevicePort *port);
    static u32 Read(caDevicePort *port);
    static u32 IoCtrl(caDevicePort *port, caSysTimerDeviceCtrl *in);
    static u32 IoctlReq(ioCtrlFunction request, u32 *p1, u32 *p2);
};


#endif 
#endif 

