#ifndef _ARM_C_TYPES_HEADER_
#define  _ARM_C_TYPES_HEADER_
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


typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

typedef char s8;
typedef short s16;
typedef int s32;
typedef long long s64;

typedef u32 s_t;

#define TRUE    1
#define FALSE   0

#ifndef NULL
#define NULL   ((0))
#endif



#if __LP64__
//#warning "platform 64 bit !!"
#define _IS_64_ 1
#define _IS_32_ 0
#else
//#warning "platform 32 bit !!"
#define _IS_64_ 0
#define _IS_32_ 1
#endif

#if _IS_64_

inline u32 ptr_to_uint(void *p) {
    long long int v64 = (long long int) (p);
    u32 v = (u32) (v64 & 0xffffffff);
    return v;
}

inline void * uint_to_ptr(u32 v) {
    long long int v64 = (long long int) (v);
    return (void *) v64;
}
#endif        


#if _IS_32_

#define ptr_to_uint(p) ((u32)(p))
#define uint_to_ptr(p) ((void *)(p))

#endif      



#if !_IS_64_ && !_IS_32_
#error "You must define platform 32/64 bit on caos_c_types.h"
#endif 




/* READ ONLY !*/
#define _R_  
/* WRITE ONLY !*/
#define _W_   
/* READ AND WRITE  !*/
#define _RW_  


#endif
