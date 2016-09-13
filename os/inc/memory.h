#ifndef MEMORY_HEADER
#define MEMORY_HEADER

#include "circularbuffer.h"


#define BLOCKSIZE sizeof(blockMem)
#define MIN_SLICE 256-BLOCKSIZE  /* min alloC SIZE */

class caMemory {
private:

    typedef enum tag_status_block {
        start_lbl = 0x12341234,
        end_lbl = 0x43212413,
        free_lbl = 0x5aa54242,
        busy_lbl = 0xa55a8420,
    } statusBlock;

    typedef struct tag_block_mem {
        tag_block_mem* addr; // this block
        tag_block_mem* prev; // this block
        tag_block_mem* next; // this block
        u32 size; // size of block        
        statusBlock status;
    } blockMem;


private:
    static u32 start_mem;
    static u32 end_mem;
    static u32 avail_mem;

private:
    static blockMem * GetFreePrev(blockMem *s);
    static blockMem * GetFreeNext(blockMem *s);
    static blockMem *GetStartBlock(void);
    static blockMem *GetEndBlock(void);
    static void *SplitBlock(blockMem *free, u32 size);
    static void UnionBlock(blockMem * s);

    static u32 List(s8 *buff, u32 size);
    static u32 Dump(dumpAddrReq *req);
    static u32 Ascii(dumpAddrReq *req);
    static void Dump(caStringStream<s8> & ss, blockMem *s);
public:

    static void Init(void);

    static inline u32 GetHeaderBlock(void) {
        return BLOCKSIZE;
    }

    static void * Allocate(u32 size);

    static u32 Free(void * p, u32 *size = NULL);

    static void DumpAvail(void);

    static u32 IoctlReq(ioCtrlFunction request, u32 *p1, u32 *p2);

    static inline u32 GetStartAddress() {
        return start_mem;
    }

    static inline u32 GetEndAddress() {
        return end_mem;
    }

    static inline u32 GetTotalSize() {
        return end_mem - start_mem;
    }

    static inline u32 GetAvailMemory() {
        return avail_mem;
    }
#ifdef TEST_SVC_MEMORY
    static void TestMemoryIoctl(void);
#else

    static inline void TestMemoryIoctl(void) {
    }
#endif
};



#if MEM_PIPE_DEVICE  

#define MAX_SHARED_MEM_BLOCK    32
#define MAX_SHARED_MASK_ID      MAX_SHARED_MEM_BLOCK-1
#define MAX_SHARED_BLOCK_NAME   64

struct caMemDeviceConfigure
: public caIDeviceConfigure {
public:

    typedef enum tag_request_mode {
        host,
        guest
    } requestMode;
    char name[MAX_SHARED_BLOCK_NAME];
    u32 size; // 1000-> u32[1000]
    requestMode host_guest; // 0 host 1 guest
};

struct caMemDeviceCtrl
: public caIDeviceCtrl {
public:

    typedef enum tag_io_ctrl_specific_request {
        Resize = 0x5000,
        Dump,
        Reset
    } IoCtrlDirect;

    IoCtrlDirect command;
};

struct caMemResizeDeviceCtrl
: public caMemDeviceCtrl {
public:
    u32 new_size;
};

struct caMemDumpDeviceCtrl
: public caMemDeviceCtrl {
public:

    caStringStream<s8> *ss;
};

struct caMemDeviceDescriptor {

    typedef enum tag_action_shared_mem {
        unknow = 0,
        writed = 0x1000,
        readed = 0x2000,
        opened = 0x4000
    } actionMem;
    char name[MAX_SHARED_BLOCK_NAME];
    u32 size;
    u32 index;
    u32 host;
    u32 guest;
    actionMem action;
    caCircularBuffer<u32> queue;
};

// device to share global allocate mem ....

class caMemDevice {
private:
    static caMemDeviceDescriptor descriptors[MAX_SHARED_MEM_BLOCK];
    static u32 n_desc;
    static u32 guid;
    static u32 isOpen;
private:
    static caMemDeviceDescriptor * FindDescriptor(caMemDeviceConfigure* setup);

    static bool IsValidHandle(u32 handle);
    static u32 Resize(caDevicePort *port, u32 size);
    static u32 Reset(caDevicePort *port);
    static u32 Dump(caDevicePort *port, caStringStream<s8> *ss);
    static caMemDeviceDescriptor *GetDescriptor(u32 handle);
public:
    static u32 Open(caMemDeviceConfigure *in, caDevicePort *out);
    static u32 Close(caDevicePort *port);
    static u32 Write(caDevicePort *port);
    static u32 Read(caDevicePort *port);
    static u32 IoCtrl(caDevicePort *port, caMemDeviceCtrl *in);
    static u32 IoctlReq(ioCtrlFunction request, u32 *p1, u32 *p2);
};

#endif

#endif /* SYSTEM_H */

