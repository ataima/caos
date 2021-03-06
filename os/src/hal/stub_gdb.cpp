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
#include "caos_c_types.h"
#include "memaux.h"
#include "kdebug.h"

// stub for memcpy & more missing function . gcc with -O0 and -ggdb reuire memcpy 
// in ctor and virtual pure default functions...


extern "C" void * memcpy(void  *dest,const void *src,s_t len){
    s8* p1=reinterpret_cast<s8*>(dest);
    const s8* p2=reinterpret_cast<const s8*>(src);
 return caMemAux<s8>::MemCpy(p1,p2,len);
}

extern "C" void __cxa_pure_virtual() { 
    Dbg::Put("> c.a.O.S. : PANIC ERROR [ call to virtual pure function !! ]\r\n");
    while(1){
    }
}
