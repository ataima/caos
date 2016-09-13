////////////////////////////////////////////////////////////////////////////////
//    Copyright (C) 2011  Angelo Coppi (angelogkcop at hotmail.com )
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
#include "arm_c_types.h"
#include "memaux.h"

void* caMemAux::MemCpy(u32 *dest, u32 *src, u32 size) {
    if (!dest)
        return dest;
    if (!src)
        return dest;
    while (size > 0) {
        *dest++ = *src++;
        size--;
    }
    return dest;
}

void* caMemAux::MemSet(u32 *dest, u32 pat, u32 size) {
    if (!dest)
        return FALSE;
    while (size > 0) {
        *dest++ = pat;
        size--;
    }
    return dest;
}

void* caMemAux::MemZero(u32 *dest, u32 size) {
    if (!dest)
        return dest;
    while (size > 0) {
        *dest++ = 0;
        size--;
    }
    return dest;
}

void* caMemAux::MemMove(u32 *dest, u32 *src, u32 size) {
    while (size != 0) {
        *dest++ = *src++;
        size--;
    }
    return (void *) dest;
}

void* caMemAux::MemMove(u16 *dest, u16 *src, u32 size) {
    while (size != 0) {
        *dest++ = *src++;
        size--;
    }
    return (void *) dest;
}

void* caMemAux::MemMove(u8 *dest, u8 *src, u32 size) {
    while (size != 0) {
        *dest++ = *src++;
        size--;
    }
    return (void *) dest;
}

#define TOUPPER(CH)  (((CH) >= 'a' && (CH) <= 'z') ? ((CH) - 'a' + 'A') : (CH))

u32 caMemAux::StrICmp(const char *s1, const char *s2) {
    while (*s2 != 0 && TOUPPER(*s1) == TOUPPER(*s2)) {
        s1++, s2++;
    }
    return (u32) (TOUPPER(*s1) - TOUPPER(*s2));
}

u32 caMemAux::StrNCmp(char *s1, const char *s2, u32 max) {

    while (max--) {
        if (*s1 != *s2)
            return (u32) (*s1 - *s2);
        s1++, s2++;
    }
    return 0;
}

u32 caMemAux::StrCmp(const char *s1, const char *s2) {
    while (*s1 != '\0' && (*s1++ == *s2++)) {
    }
    return (u32) (*s1 - *s2);
}

s8* caMemAux::StrCpy(char *s1, const char *s2) {
    while (*s2 != '\0')
        *s1++ = *s2++;
    *s1 = '\0';
    return s1;
}

s8* caMemAux::StrNCpy(char *s1, const char *s2, u32 max) {
    u32 len = 0;
    max--;
    while (*s2 != '\0' && len++<max)
        *s1++ = *s2++;
    *s1 = '\0';
    return s1;
}

u32 caMemAux::StrLen(const char *s1) {
    u32 len = 0;
    while (*s1++ != '\0')
        len++;
    return len;
}