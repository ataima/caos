#ifndef LOCKER_HEADER
#define LOCKER_HEADER
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


#include "kdebug.h"

#define LOCK_V 0x07091961

#define LOCK_FREE 0

class caAtomicLock {
    u32 key;

    static inline u32 AtomicSwap(u32 *ptr, u32 v_old, u32 v_new) {
        u32 oldval;
        u32 res;
#ifdef HW_RASPI2         
        asm volatile("pldw %a0"::"p" (ptr));

        do {
            __asm__ __volatile__("@ AtomicSwap\n"
                    "ldrex	%1, [%3]\n"
                    "mov	%0, #0\n"
                    "teq	%1, %4\n"
                    "strexeq %0, %5, [%3]\n"
                    : "=&r" (res), "=&r" (oldval), "+Qo" (*ptr)
                    : "r" (ptr), "Ir" (v_old), "r" (v_new)
                    : "cc");
        } while (res);
#else        
        *ptr = v_new;
        res = oldval = v_old;
        return res;
#endif // ARM

        return oldval;
    }

public:

    inline caAtomicLock() {
        key = 0;
    }

    inline u32 Lock(void) {
        u32 register res = AtomicSwap(&key, 0, LOCK_V);
        if (res != LOCK_V && res != 0)            
            Dbg::Put("Key=",key);
        return res == 0;
    }

    inline u32 UnLock(void) {
        u32 register res = AtomicSwap(&key, LOCK_V, 0);
          if (res != LOCK_V && res != 0)            
            Dbg::Put("Key=",key);
        return res == LOCK_V;
    }

    inline u32 Get(void) {
        if (key == LOCK_V)
        if (key != LOCK_V && key != 0)            
            Dbg::Put("Key=",key);
        return key;
    }
};

#endif 

