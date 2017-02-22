#ifndef MEMORY_HEADER
#define MEMORY_HEADER

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


#define BLOCKSIZE ((u32)(sizeof(caMemory::blockMem)))
#define MIN_SLICE 256-BLOCKSIZE  /* min alloC SIZE */

class caMemory {
private:

    typedef enum tag_status_block {
        start_lbl = 0x12341234,
        end_lbl = 0x43212413,
        free_lbl = 0x5aa54242,
        busy_lbl = 0xa55a8420,
    } statusBlock;

public:

    typedef struct tag_block_mem {
        tag_block_mem* addr; // this block
        tag_block_mem* prev; // this block
        tag_block_mem* next; // this block
        u32 size; // size of block        
        statusBlock status;
        // TO DO TO CHECK MEMORY LEACKS        
#if CONFIG_CHK_MEMORY_ALLOC
        s8 filename[64];
        u32 line;
#endif        
    } blockMem;


private:
    static u32* start_mem;
    static u32* end_mem;
    static u32 avail_mem;
    static hal_llc_mem_io *link;
private:
    static blockMem * GetFreePrev(blockMem *s);
    static blockMem * GetFreeNext(blockMem *s);
    static blockMem * GetBusyPrev(blockMem *s);
    static blockMem * GetBusyNext(blockMem *s);
    static blockMem *GetStartBlock(void);
    static blockMem *GetEndBlock(void);
    static void *SplitBlock(blockMem *free, u32 size);
    static void UnionBlock(blockMem * s);
public:
    static u32 List(caStringStream<s8>  & ss);
    static u32 Dump(dumpAddrReq *req);
    static u32 Ascii(dumpAddrReq *req);
    static void Dump(caStringStream<s8> & ss, blockMem *s);

    //T
    static void Init(hal_llc_mem_io *lnk);

    static void Clean(void);

    static void * Allocate(u32 size);

    static u32 Free(void * p, u32 *size = nullptr);
    static u32 FreeAll(void );

    static u32 Find(void *p);

    static s8* DumpAvail(s8* buff, s_t size);

    //T

    static inline u32* GetStartAddress() {
        return start_mem;
    }

    //T

    static inline u32* GetEndAddress() {
        return end_mem;
    }

    //T

    static inline u32 GetTotalSize() {
        return ptr_to_uint(end_mem) - ptr_to_uint(start_mem);
    }

    //T

    static inline u32 GetAvailMemory() {
        return avail_mem;
    }

    //T   

    static inline u32 Good() {
        return (avail_mem > MIN_SLICE);
    }

    //T

    static inline u32 GetHeaderBlock(void) {
        return BLOCKSIZE;
    }

};


#endif 

