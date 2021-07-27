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
//    2021-07  -> rebuild to u64 
////////////////////////////////////////////////////////////////////////////////

#include "hal.h"
#include "kdebug.h"
#include "memory.h"





u64 caMemory::start_mem;

u64 caMemory::end_mem;

u64 caMemory::avail_mem;


void caMemory::Init() {
    start_mem = hal_llc_mem.hll_user_start();
    //allign 64 
    start_mem=((start_mem/1024)+1)*1024;
    end_mem   = hal_llc_mem.hll_user_end();
    end_mem=((end_mem/1024)-1)*1024;
    avail_mem = ptr_to_uint(end_mem)  - ptr_to_uint(start_mem) ;
    if(avail_mem==0){
        hal_llc_mem.hll_memory_failure();
    }
}

void caMemory::Start(void)    {
    blockMem *start =static_cast<blockMem *> (uint_to_ptr(start_mem));
    start->addr = start;
    start->size = 0;
    start->status = statusBlock::start_lbl;
    avail_mem -= BLOCKSIZE;
    blockMem *free = static_cast<blockMem *> (uint_to_ptr(start_mem + BLOCKSIZE));
    start->next = free;
    start->prev = nullptr;
    free->addr = free;
    avail_mem -= (2*BLOCKSIZE);
    free->size = avail_mem;
    free->next = static_cast<blockMem *> (uint_to_ptr(end_mem-BLOCKSIZE));
    free->prev = start;
    free->status = statusBlock::free_lbl;
    blockMem *end = free->next;
    end->prev = free;
    end->addr = end;
    end->next = 0;
    end->size = 0;
    end->status = statusBlock::end_lbl;    
}

void caMemory::Clean(void) {
    start_mem = end_mem = 0;
    avail_mem = 0;
}

caMemory::blockMem *caMemory::GetStartBlock(void) {
    blockMem *s = static_cast<blockMem *> (uint_to_ptr(start_mem));
    return s;
}

caMemory::blockMem *caMemory::GetEndBlock(void) {
    blockMem *s = static_cast<blockMem *>( uint_to_ptr( end_mem - (2 * BLOCKSIZE)));
    return s;
}

caMemory::blockMem * caMemory::GetFreePrev(blockMem *s) {
    while (s != nullptr) {
        if (s->status == statusBlock::free_lbl)
            return s;
        s = s->prev;
    }
    return nullptr;
}

caMemory::blockMem * caMemory::GetFreeNext(blockMem *s) {
    while (s != nullptr) {
        if (s->status == statusBlock::free_lbl)
            return s;
        s = s->next;
    }
    return nullptr;
}

caMemory::blockMem * caMemory::GetBusyPrev(blockMem *s) {
    while (s != nullptr) {
        if (s->status == statusBlock::busy_lbl)
            return s;
        s = s->prev;
    }
    return nullptr;
}

caMemory::blockMem * caMemory::GetBusyNext(blockMem *s) {
    while (s != nullptr) {
        if (s->status == statusBlock::busy_lbl)
            return s;
        s = s->next;
    }
    return nullptr;
}

void *caMemory::SplitBlock(blockMem *s, u64 size) {
    void *p = nullptr;
    if (s->size > size) {
        if ((s->size - size) < MIN_SLICE) {
            s->status = statusBlock::busy_lbl;
            avail_mem -= size;
            p = static_cast<void *> (uint_to_ptr(ptr_to_uint(s->addr) + BLOCKSIZE));
        } else {
            blockMem *nb = static_cast<blockMem *>(uint_to_ptr (ptr_to_uint(s->addr)+(s->size - size)));
            nb->prev = s;
            nb->next = s->next;
            nb->addr = nb;
            nb->size = size - BLOCKSIZE;
            s->next = nb;
            s->size -= size;
            if (nb->next != nullptr) {
                nb->next->prev = nb;
            }
            nb->status = statusBlock::busy_lbl;
            avail_mem -= size;
            p = static_cast<void *> ((u8*) (nb->addr) + BLOCKSIZE);
        }
    }
    return p;
}

void caMemory::UnionBlock(blockMem * s) {
    // union of block s with prev or next if free: s is free.
    if (s->next != nullptr && s->next->status == statusBlock::free_lbl) {
        blockMem *succ = s->next;
        s->size += succ->size + BLOCKSIZE;
        succ->next->prev = s;
        s->next = succ->next;
    }
    if (s->prev != nullptr && s->prev->status == statusBlock::free_lbl)
        UnionBlock(s->prev);
}

void * caMemory::Allocate(u32 size) {
    void *p = 0;
    if (size < MIN_SLICE)
        size = MIN_SLICE;
    else {
        size = ((size / BLOCKSIZE) + 2) * BLOCKSIZE;
    }
    if (avail_mem > size) {
        blockMem *dispPrev = GetEndBlock();
        blockMem *dispNext = GetStartBlock();
        do {
            dispNext = GetFreeNext(dispNext);
            if (dispNext != nullptr) {
                if (dispNext->size > size) {
                    p = SplitBlock(dispNext, size);
                    if (p != nullptr)
                        break;
                }
            }
            dispPrev = GetFreePrev(dispPrev);
            if (dispPrev != nullptr) {
                if (dispPrev->size > size) {
                    p = SplitBlock(dispPrev, size);
                    if (p != nullptr)
                        break;
                }
            }
        } while (dispNext != nullptr || dispPrev != nullptr);
    }
    return p;
}

u32 caMemory::Free(void * p, u32 *size) {
    u32 res = false;
    if (size != nullptr)
        *size = 0;
    if (ptr_to_uint(p) > hal_llc_mem.hll_mem_min_phy() && ptr_to_uint(p) < hal_llc_mem.hll_mem_max_phy()) {
        blockMem *s = static_cast<blockMem *>(uint_to_ptr (ptr_to_uint(p) - BLOCKSIZE));
        if (s != nullptr && s->status == statusBlock::busy_lbl) {
            // Ok
            s->status = statusBlock::free_lbl;
            if (size != nullptr) {
                *size = ptr_to_uint(s->size);
            }
            avail_mem += s->size + BLOCKSIZE;
            UnionBlock(s);
            res = true;
        }
    }
    return res;
}

u32 caMemory::FreeAll(void) {
    caMemory::Init();
    return true;
}

u32 caMemory::Find(void *p) {
    u32 res = 0;
    if (ptr_to_uint(p) > hal_llc_mem.hll_mem_min_phy() && ptr_to_uint(p) < hal_llc_mem.hll_mem_max_phy()) {
        blockMem *s = static_cast<blockMem *> (uint_to_ptr(ptr_to_uint(p) - BLOCKSIZE));
        if (s != nullptr && s->status == statusBlock::busy_lbl) {
            // Ok
            res = ptr_to_uint(s->size);
        }
    }
    return res;
}

void caMemory::Dump(caStringStream<s8> & ss, blockMem *start) {
    ss << ":" << ptr_to_uint(start->addr);
    ss << " Size :";
    caStringFiller p(' ', 12);
    ss << caStringFormat::dec;
    ss.Fix(p);
    ss << (u32) start->size << p << caStringFormat::hex;
    p.width = 10;
    ss.Fix(p);
    switch (start->status) {
        case statusBlock::start_lbl:
            ss << " START";
            break;
        case statusBlock::end_lbl:
            ss << " END";
            break;
        case statusBlock::free_lbl:
            ss << " FREE";
            break;
        case statusBlock::busy_lbl:
            ss << " BUSY";
            break;
    }
    ss << p << caEnd::endl;
}

u32 caMemory::List(caStringStream<s8> & ss) {
    caStringFiller p(' ', 12);
    u32 i = 1;
    ss << " --- MEMORY LIST ---\r\n";
    blockMem *start = static_cast<blockMem *>( uint_to_ptr(start_mem));
    while (start != nullptr) {
        ss << caStringFormat::dec << i << caStringFormat::hex;
        Dump(ss, start);
        start = start->next;
        i++;
    }
    ss << caStringFormat::dec;
    ss << "Free Memory  : ";
    ss.Fix(p);
    ss << ptr_to_uint(GetAvailMemory()) << p << " bytes" << caEnd::endl;
    ss << "Busy Memory  : ";
    ss.Fix(p);
    ss << ptr_to_uint(GetTotalSize()) - ptr_to_uint(GetAvailMemory()) << p << " bytes" << caEnd::endl;
    ss << "Total Memory : ";
    ss.Fix(p);
    ss << ptr_to_uint(GetTotalSize()) << p << " bytes" << caEnd::endl;
    ss.Str();
    return ss.Size();
}


void caMemory::DumpAll(void) {
    Dbg::Put("MEMORY START = ",ptr_to_uint(hal_llc_mem.hll_mem_min_phy()));
    Dbg::Put("MEMORY STOP  = ",ptr_to_uint(hal_llc_mem.hll_mem_max_phy()));
    Dbg::Put("MEMORY SIZE  = ",ptr_to_uint(hal_llc_mem.hll_mem_size_phy()));
    Dbg::Put("HEAP   START = ",ptr_to_uint(hal_llc_mem.hll_heap_start()));
    Dbg::Put("HEAP   STOP  = ",ptr_to_uint(hal_llc_mem.hll_heap_end()));
    Dbg::Put("HEAP   SIZE  = ",ptr_to_uint(hal_llc_mem.hll_heap_end())-ptr_to_uint(hal_llc_mem.hll_heap_start()));    
}

u32 caMemory::Dump(dumpAddrReq *req) {
    caStringStream<s8> ss;
    if (req != nullptr) {
        u32 i, u, *ptr = static_cast<u32 *> (uint_to_ptr(req->addr));
        ss.Init(req->buffo, req->size);
        ss << " --- MEMORY DUMP ---\r\n";
        ss << caStringFormat::hex;
        for (i = 0; i < 16; i++) {
            if (!ss.Good())break;
            ss << ptr_to_uint(ptr) << " - ";
            for (u = 0; u < 4; u++) {
                if (!ss.Good())break;
                ss << *ptr++ << "  ";
            }
            ss << caEnd::endl;
        }
        ss << caEnd::endl;
        ss.Str();
        return ss.Size();
    }
    return 0;
}

u32 caMemory::Ascii(dumpAddrReq *req) {
    caStringStream<s8> ss;
    if (req != nullptr) {
        u32 i, u;
        s8 *ptr = static_cast<s8 *> (uint_to_ptr(req->addr));
        ss.Init(req->buffo, req->size);
        ss << " --- MEMORY ASCII DUMP ---\r\n";
        ss << caStringFormat::hex;
        for (i = 0; i < 32; i++) {
            if (!ss.Good())break;
            ss << ptr_to_uint(ptr) << " - ";
            for (u = 0; u < 8; u++) {
                if (!ss.Good())break;
                u32 ch = (u32) * ptr++;
                ss << (u8) ch << "[";
                if (ch < 32 || ch > 127)
                    ss << '.';
                else
                    ss << (s8) ch;
                ss << "] ";
            }
            ss << caEnd::endl;
        }
        ss << caEnd::endl;
        ss.Str();
        return ss.Size();
    }
    return 0;
}










