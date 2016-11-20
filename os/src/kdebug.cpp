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

#include "config.h"
#include "bcm2836.h"
#include "idevice.h"
#include "stream.h"
#include "miniuart.h"
#include "sysirqctrl.h"
#include "cpu.h"
#include "caos_version.h"

class caLowLevelDebug {
    /// TO DO USE STREAM....
public:
    static void Msg(const char *s);
    static void Msg(const char *s, u32 v, Dbg::kformat f = Dbg::kformat::hex, u32 cr = TRUE);
    static void Msg(const char *s, void *p, u32 v, Dbg::kformat f = Dbg::kformat::hex, u32 cr = TRUE);
    static void Hex(u32 d);
    static void Dec(s32 d);
    static void Bin(u32 d);
    static void Welcome(void);
    static void ByeBye(void);
    static void MMU(void);
    static void CACHE(void);
    static void START(void);

    static inline void uSend(u32 c) {
        caMiniUart::Send(c);
    }
};

void caLowLevelDebug::Msg(const char *s) {
    char c;
    while ((c = *s++)) uSend(c);
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
    u32 t = (u32) (p);
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
    u32 rb;
    u32 rc;
    uSend('0');
    uSend('x');
    rb = 32;
    while (1) {
        rb -= 4;
        rc = (d >> rb)&0xF;
        if (rc > 9)
            rc += 0x37;
        else
            rc += 0x30;
        uSend(rc);
        if (rb == 0) break;
    }
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

void caLowLevelDebug::Welcome(void) {
    caIrqCtrl::Init(); // start all fiq/irq disabled 
    caMiniUart::Init(115200, 8, 1, 8);
    Msg("> c.a.O.S. : [ ");
    caArmCpu::DumpCPSR();
    Msg(" ]\r\n");
}

void caLowLevelDebug::ByeBye(void) {
    Msg("bye. [ from c.a.O.S. main proc ! ]\r\n");
}

void caLowLevelDebug::MMU(void) {
    Msg("> c.a.O.S. : [ ");
    Msg("MMU : Flat Model Started Ok");
    Msg(" ]\r\n");
}

void caLowLevelDebug::CACHE(void) {
    Msg("> c.a.O.S. : [ ");
    Msg("Cache : Instruction,Data And Branch Prediction is On ");
    Msg(" ]\r\n");
}

void caLowLevelDebug::START(void) {
    Msg("> c.a.O.S. : [ ");
    Msg(caos_version);
    Msg(" ]\r\n");
}

extern "C" {

    void msgWelcome(void) {
        caLowLevelDebug::Welcome();
    }

    void msgByeBye(void) {
        caLowLevelDebug::ByeBye();
    }

    void msgMMU(void) {
        caLowLevelDebug::MMU();
    }

    void msgCACHE(void) {
        caLowLevelDebug::CACHE();
    }

    void msgSTART(void) {
        caLowLevelDebug::START();
    }

    void msgSchedule(void) {
        caLowLevelDebug::Msg("Reschedule\r\n");
        ;
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
}


