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
#include "interrupt.h"

#include "interrupt.h"
#include "softreq.h"


#if SYS_SOFT_REQ_ENABLED

void caSoftRequest::SVC_IOCTL(register u32 /*R0= ioctl*/, register u32 */*R1=input */,
        register u32 */*R2=output*/, register u32 * /*R3=res*/) {
        asm volatile ("DSB"); //TOTEST
        asm volatile ("ISB"); //TOTEST
        asm volatile ("SVC #1");
        asm volatile ("bx lr"); 
        asm volatile ("bx lr"); 
        asm volatile ("bx lr"); 
        asm volatile ("bx lr"); 
        asm volatile ("bx lr"); 
        asm volatile ("bx lr"); 
        asm volatile ("bx lr"); 
        asm volatile ("bx lr"); 
        asm volatile ("bx lr"); 
        asm volatile ("bx lr"); 
        asm volatile ("bx lr"); 
    }
#else
void caSoftRequest::SVC_IOCTL(register u32 ioctl, register u32 *input ,
        register u32 *output, register u32 *res ) {
    ISR_Software(ioctl,input,output,res);
    }
#endif        

void caSoftRequest::DumpSvc(const char *name,const char *file, u32 line){
    Dbg::Put("SVC : > ");
    Dbg::Put(name);
    Dbg::Put(" < - ");
    Dbg::Put(file);
    Dbg::Put(" : ",line,Dbg::dec,1);    
}
