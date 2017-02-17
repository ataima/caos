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
#include "kdebug.h"
#include "memory.h"





u32* caMemory::start_mem;
u32* caMemory::end_mem;
u32 caMemory::avail_mem;

void caMemory::Init(hal_llc_mem_io *lnk) {
    if (lnk != NULL) {
        start_mem =static_cast<u32*>(uint_to_ptr(lnk->hll_heap_start()));
        end_mem = static_cast<u32*>(uint_to_ptr(lnk->hll_heap_end()));
    } else {
        start_mem = 0;
        end_mem = 0;
    }
    avail_mem = (ptr_to_uint(end_mem) - ptr_to_uint(start_mem));
    blockMem *start = reinterpret_cast<blockMem *> (start_mem);
    start->addr = start;
    start->size = 0;
    start->status = statusBlock::start_lbl;
    avail_mem -= BLOCKSIZE;
    blockMem *free = reinterpret_cast<blockMem *> (start_mem + BLOCKSIZE);
    start->next = free;
    start->prev = NULL;
    free->addr = free;
    avail_mem -= (2 * BLOCKSIZE);
    free->size = avail_mem;
    free->next = reinterpret_cast<blockMem *> ((u8*) (free->addr) + avail_mem);
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
    start_mem = end_mem = NULL;
    avail_mem = 0;
}

caMemory::blockMem *caMemory::GetStartBlock(void) {
    blockMem *s = reinterpret_cast<blockMem *> (start_mem);
    return s;
}

caMemory::blockMem *caMemory::GetEndBlock(void) {
    blockMem *s = reinterpret_cast<blockMem *> ((u8*) end_mem - (2 * BLOCKSIZE));
    return s;
}

caMemory::blockMem * caMemory::GetFreePrev(blockMem *s) {
    while (s != NULL) {
        if (s->status == statusBlock::free_lbl)
            return s;
        s = s->prev;
    }
    return NULL;
}

caMemory::blockMem * caMemory::GetFreeNext(blockMem *s) {
    while (s != NULL) {
        if (s->status == statusBlock::free_lbl)
            return s;
        s = s->next;
    }
    return NULL;
}

void *caMemory::SplitBlock(blockMem *s, u32 size) {
    void *p = NULL;
    if (s->size > size) {
        if ((s->size - size) < MIN_SLICE) {
            s->status = statusBlock::busy_lbl;
            avail_mem -= size;
            p = reinterpret_cast<void *> (ptr_to_uint(s->addr) + BLOCKSIZE);
        } else {
            blockMem *nb = reinterpret_cast<blockMem *> ((u8*) (s->addr)+(s->size - size));
            nb->prev = s;
            nb->next = s->next;
            nb->addr = nb;
            nb->size = size - BLOCKSIZE;
            s->next = nb;
            s->size -= size;
            if (nb->next != NULL) {
                nb->next->prev = nb;
            }
            nb->status = statusBlock::busy_lbl;
            avail_mem -= size;
            p = reinterpret_cast<void *> ((u8*) (nb->addr) + BLOCKSIZE);
        }
    }
    return p;
}

void caMemory::UnionBlock(blockMem * s) {
    // union of block s with prev or next if free: s is free.
    if (s->next != NULL && s->next->status == statusBlock::free_lbl) {
        blockMem *succ = s->next;
        s->size += succ->size + BLOCKSIZE;
        succ->next->prev = s;
        s->next = succ->next;
    }
    if (s->prev != NULL && s->prev->status == statusBlock::free_lbl)
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
            if (dispNext != NULL) {
                if (dispNext->size > size) {
                    p = SplitBlock(dispNext, size);
                    if (p != NULL)
                        break;
                }
            }
            dispPrev = GetFreePrev(dispPrev);
            if (dispPrev != NULL) {
                if (dispPrev->size > size) {
                    p = SplitBlock(dispPrev, size);
                    if (p != NULL)
                        break;
                }
            }
        } while (dispNext != NULL || dispPrev != NULL);
    }
    return p;
}

u32 caMemory::Free(void * p, u32 *size) {
    u32 res = FALSE;
    if (size != NULL)
        *size = 0;
    if (ptr_to_uint(p) > hal_llc_mem.hll_mem_min_phy() && ptr_to_uint(p) < hal_llc_mem.hll_mem_max_phy()) {
        blockMem *s = reinterpret_cast<blockMem *> ((u8*) (p) - BLOCKSIZE);
        if (s != NULL && s->status == statusBlock::busy_lbl) {
            // Ok
            s->status = statusBlock::free_lbl;
            if (size != NULL) {
                *size = s->size;
            }
            avail_mem += s->size + BLOCKSIZE;
            UnionBlock(s);
            res = TRUE;
        }
    }
    return res;
}

u32 caMemory::Find(void *p) {
    u32 res = 0;
    if (ptr_to_uint(p) > hal_llc_mem.hll_mem_min_phy() && ptr_to_uint(p) < hal_llc_mem.hll_mem_max_phy()) {
        blockMem *s = reinterpret_cast<blockMem *> ((u8*) (p) - BLOCKSIZE);
        if (s != NULL && s->status == statusBlock::busy_lbl) {
            // Ok
            res = s->size;
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
    blockMem *start = reinterpret_cast<blockMem *> (start_mem);
    while (start != NULL) {
        ss << caStringFormat::dec << i << caStringFormat::hex;
        Dump(ss, start);
        start = start->next;
        i++;
    }
    ss << caStringFormat::dec;
    ss << "Free Memory  : ";
    ss.Fix(p);
    ss << GetAvailMemory() << p << " bytes" << caEnd::endl;
    ss << "Busy Memory  : ";
    ss.Fix(p);
    ss << (ptr_to_uint(end_mem) - ptr_to_uint(start_mem)) - GetAvailMemory() << p << " bytes" << caEnd::endl;
    ss << "Total Memory : ";
    ss.Fix(p);
    ss << (ptr_to_uint(end_mem) - ptr_to_uint(start_mem)) << p << " bytes" << caEnd::endl;
    ss.Str();
    return ss.Size();
}

s8* caMemory::DumpAvail(s8 * buff, s_t size) {
    caStringStream<s8> ss;
    ss.Init(buff, size);
    ss << "> c.a.O.S. : [ Avaiable memory  " <<
            caMemory::GetAvailMemory() << " bytes ]\r\n";
    return buff;
}

u32 caMemory::Dump(dumpAddrReq *req) {
    caStringStream<s8> ss;
    if (req != NULL) {
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
    if (req != NULL) {
        u32 i, u;
        s8 *ptr = static_cast<s8 *>(uint_to_ptr(req->addr));
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










