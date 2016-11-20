#ifndef CA_KDEBUG_HEADER
#define CA_KDEBUG_HEADER


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

// DIRECT TO MINIUART LOW LEVEL DEBUG
namespace Dbg {

    typedef enum _format_ {
        hex, dec, bin
    } kformat;


    extern void Put(const char *s, void *p, u32 v, kformat f = kformat::hex, u32 cr = TRUE);
    extern void Put(const char *msg, u32 val, kformat format = hex, u32 cr = TRUE);
    extern void Put(const char *msg);
}

extern "C" {
    void msgWelcome(void);
    void msgByeBye(void);
    void msgMMU(void);
    void msgCACHE(void);
    void msgSTART(void);
}

#if TRACE 


#define TIN()       u32 trace;\
                    Dbg::Put("\r\nIN : ");\
                    Dbg::Put(__func__);\
                    Dbg::Put("\r\n");\
                    trace=caSysTimer::ReadFreeCounter();\

#define TOUT()      trace=caSysTimer::ReadFreeCounter()-trace;\
                    Dbg::Put("\r\nOUT : ");\
                    Dbg::Put(__func__);\
                    Dbg::Put(" : uSec =",trace,Dbg::kformat::dec);

#else
#define TIN()
#define TOUT()
#endif


#endif 

