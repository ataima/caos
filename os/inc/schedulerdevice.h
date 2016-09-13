#ifndef CA_SCHEDULER_DEVICE_HEADER
#define CA_SCHEDULER_DEVICE_HEADER

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

