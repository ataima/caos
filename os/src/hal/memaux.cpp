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

#define TOUPPER(CH)  (((CH) >= 'a' && (CH) <= 'z') ? ((CH) - 'a' + 'A') : (CH))

u32 caStrAux::StrICmp(const s8 *s1, const s8 *s2) {
    while (*s2 != 0 && TOUPPER(*s1) == TOUPPER(*s2)) {
        s1++, s2++;
    }
    return (u32) (TOUPPER(*s1) - TOUPPER(*s2));
}

u32 caStrAux::StrNCmp(const s8 *s1, const s8 *s2, s_t max) {

    while (max--) {
        if (*s1 != *s2)
            return (u32) (*s1 - *s2);
        s1++, s2++;
    }
    return 0;
}

u32 caStrAux::StrCmp(const s8 *s1, const s8 *s2) {
    while (*s1 != '\0' && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return (u32) (*s1 - *s2);
}

s8* caStrAux::StrCpy(s8 *s1, const s8 *s2) {
    while (*s2 != '\0')
        *s1++ = *s2++;
    *s1 = '\0';
    return s1;
}

s8* caStrAux::StrNCpy(s8 *s1, const s8 *s2, s_t max) {
    u32 len = 0;
    while (*s2 != '\0' && len++<max)
        *s1++ = *s2++;
    *s1 = '\0';
    return s1;
}

u32 caStrAux::StrLen(const s8 *s1) {
    u32 len = 0;
    while (*s1++ != '\0')
        len++;
    return len;
}
