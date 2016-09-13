#ifndef CACHEDEVICE_H
#define CACHEDEVICE_H

#if CACHE_DEVICE

#include "idevice.h"

struct caCacheDeviceConfigure
: public caIDeviceConfigure {
public:
    //TO DO
};

struct caCacheDeviceCtrl
: public caIDeviceCtrl {
public:

    typedef enum tag_io_ctrl_specific_request {
        cacheInvalidate = 0x5000,
        cacheStart,
        cacheStop,
    } IoCtrlDirect;
    IoCtrlDirect command;
};

class caCacheDevice {
private:
    static u32 guid;
    static u32 isOpen;
    static bool IsValidHandle(u32 handle);
public:
    static u32 Open(caCacheDeviceConfigure *in, caDevicePort *out);
    static u32 Close(caDevicePort *port);
    static u32 Write(caDevicePort *port);
    static u32 Read(caDevicePort *port);
    static u32 IoCtrl(caDevicePort *port, caCacheDeviceCtrl *in);
    static u32 IoctlReq(ioCtrlFunction request, u32 *p1, u32 *p2);
};


#endif 


#endif 

