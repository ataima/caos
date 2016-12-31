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

template <typename T>
class caMemAux {
public:

    //T

    static void* MemCpy(T *dest, const T *src, s_t size) {
        if (!dest)
            return dest;
        if (!src)
            return dest;
        size/=(s_t)(sizeof(T));
        while (size > 0) {
            *dest++ = *src++;
            size--;
        }
        return dest;
    }
    //T

    static void* MemSet(T *dest, T pat, s_t size) {
        if (!dest)
            return FALSE;
        size/=(s_t)(sizeof(T));
        while (size > 0) {
            *dest++ = pat;
            size--;
        }
        return dest;
    }
    //

    static void* MemZero(T *dest, s_t size) {
        if (!dest)
            return FALSE;
        size/=(s_t)(sizeof(T));
        while (size > 0) {
            *dest++ = 0;
            size--;
        }
        return dest;
    }

    static s32 MemCmp(const T *dest, const T *src, s_t size) {
        if (!dest)
            return -1;
        if (!src)
            return -2;
        size/=(s_t)(sizeof(T));
        while (size > 0 && *dest == *src) {
            dest++;
            src++;
            size--;
        }
        return (s32) (size == 0 ? 0 : (*dest - *src));
    }

};

class caStrAux {
public:

    static u32 StrCmp(const s8 *s1, const s8 *s2);

    static u32 StrICmp(const s8 *s1, const s8 *s2);

    static u32 StrNCmp(const s8 *s1, const s8 *s2, s_t max);

    static s8* StrCpy(s8 *s1, const s8 *s2);

    static s8* StrNCpy(s8 *s1, const s8 *s2, s_t max);

    static u32 StrLen(const s8 *s1);
};



#endif 

