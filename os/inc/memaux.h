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
    static void* MemCpy(u32 *dest, u32 *src, u32 size);
    static void* MemSet(u32 *dest, u32 pat, u32 size);
    static void* MemZero(u32 *dest, u32 size);
    static void* MemMove(u32 *dest, u32 *src, u32 size);
    static void* MemMove(u16 *dest, u16 *src, u32 size);
    static void* MemMove(u8 *dest, u8 *src, u32 size);
    static u32 StrICmp(const char *s1, const char *s2);
    static u32 StrCmp(const char *s1, const char *s2);
    static u32 StrNCmp(char *s1, const char *s2, u32 max);
    static s8* StrCpy(char *s1, const char *s2);
    static s8* StrNCpy(char *s1, const char *s2, u32 max);
    static u32 StrLen(const char *s1);
};




#endif 

