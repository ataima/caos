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

class caLowLevelDebug {
    /// TO DO USE STREAM....
public:
    static void Msg(const char *s);
    static void Msg(const char *s, u32 v, Dbg::kformat f = Dbg::kformat::hex, u32 cr = true);
    static void Msg(const char *s, void *p, u32 v, Dbg::kformat f = Dbg::kformat::hex, u32 cr = true);
    static void Hex(u32 d);
    static void Dec(s32 d);
    static void Bin(u32 d);


    static inline void uSend(u32 c) {
        hal_llc_com1.hll_send(c);
    }
};

void caLowLevelDebug::Msg(const char *s) {
    u32 c;
    do{
        c=(u32)*s;
        s++;
        uSend(c);
    }
    while (*s!=0);
}

void caLowLevelDebug::Msg(const char *s, u32 v, Dbg::kformat f, u32 cr) {
    caLowLevelDebug::Msg(s);
    if (f == Dbg::kformat::dec)
        caLowLevelDebug::Dec(v);
    else
        if (f == Dbg::kformat::bin)
        caLowLevelDebug::Bin(v);
    else
        caLowLevelDebug::Hex(v);
    if (cr)
        caLowLevelDebug::Msg("\r\n");
}

void caLowLevelDebug::Msg(const char *s, void *p, u32 v, Dbg::kformat f, u32 cr) {
    u32 t = ptr_to_uint(p);
    caLowLevelDebug::Msg("[");
    caLowLevelDebug::Hex(t);
    caLowLevelDebug::Msg("] - ");
    caLowLevelDebug::Msg(s);
    caLowLevelDebug::Msg(" = ");
    if (f == Dbg::kformat::dec)
        caLowLevelDebug::Dec(v);
    else
        if (f == Dbg::kformat::bin)
        caLowLevelDebug::Bin(v);
    else
        caLowLevelDebug::Hex(v);
    if (cr)
        caLowLevelDebug::Msg("\r\n");
}

void caLowLevelDebug::Hex(u32 d) {
    s32 rb;
    u32 rc;
    uSend('0');
    uSend('x');
    rb = 28;
    do{
        rc = (d >> rb)&0xF;
        if (rc > 9)
            rc += 0x37;
        else
            rc += 0x30;
        uSend(rc);
        rb=rb-4;
    }while (rb >= 0);
}

void caLowLevelDebug::Bin(u32 d) {
    uSend('0');
    uSend('b');
    u32 mask = 0x80000000;
    while (mask) {
        if (d & mask)
            uSend('1');
        else
            uSend('0');
        mask >>= 1;
    }
}

void caLowLevelDebug::Dec(s32 d) {
    s32 rb, rc;
    if (d == 0) {
        uSend('0');
        return;
    }
    if (d < 0) {
        uSend('-');
        d = -d;
    }
    rc = 1000000000;
    while (rc > d) {
        rc /= 10;
        if (rc == 0)break;
    }
    while (rc > 0) {
        rb = d / rc;
        d = d - (rb * rc);
        rc /= 10;
        rb += 0x30;
        uSend(rb);
    }
}




namespace Dbg {

    void Put(const char *s, void *p, u32 v, kformat f, u32 cr) {
        caLowLevelDebug::Msg(s, p, v, f, cr);
    }

    void Put(const char *msg, u32 val, kformat format, u32 cr) {
        caLowLevelDebug::Msg(msg, val, format, cr);
    }

    void Put(const char *msg) {
        caLowLevelDebug::Msg(msg);
    }
    
    void Put(const char c) {
        caLowLevelDebug::uSend(c);
    }
    
    void PutA(const char *msg, u32 val, const char *fin) {
        caLowLevelDebug::Msg(msg, val, Dbg::kformat::hex, 0);
        caLowLevelDebug::Msg(fin);
    }
    
    
    void Put(const char *msg, const char *info, u32 cr) {
        caLowLevelDebug::Msg(msg);
        caLowLevelDebug::Msg(info);
        if(cr)caLowLevelDebug::Msg("\r\n");
    }
    
    
}


