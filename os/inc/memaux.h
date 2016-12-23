#ifndef MEMAUX_HEADER
#define MEMAUX_HEADER

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

class caMemAux {
public:

    //T
    static void* MemCpy(u32 *dest, u32 *src, u32 size);
    //T
    static void* MemCpy(u16 *dest, u16 *src, u32 size);
    //T
    static void* MemCpy(u8 *dest, u8 *src, u32 size);
    //T
    static void* MemSet(u32 *dest, u32 pat, u32 size);
    //T
    static void* MemSet(u16 *dest, u16 pat, u32 size);
    //T    
    static void* MemSet(u8 *dest, u8 pat, u32 size);
    //
};

class caStrAux {
public:

    static u32 StrCmp(const s8 *s1, const s8 *s2);

    static u32 StrICmp(const s8 *s1, const s8 *s2);

    static u32 StrNCmp(const s8 *s1, const s8 *s2, u32 max);

    static s8* StrCpy(s8 *s1, const s8 *s2);

    static s8* StrNCpy(s8 *s1, const s8 *s2, u32 max);

    static u32 StrLen(const s8 *s1);
};



#endif 

