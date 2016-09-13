#ifndef LOCKER_HEADER
#define LOCKER_HEADER

#define LOCK_V 0x07091961

#define LOCK_FREE 0

class caAtomicLock {
    u32 key;

    static inline u32 AtomicSwap(u32 *ptr, u32 v_old, u32 v_new) {
        u32 oldval;
        u32 res;

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

        return oldval;
    }

public:

    inline caAtomicLock() {
        key = 0;
    }

    inline u32 Lock(void) {
        u32 register res = AtomicSwap(&key, 0, LOCK_V);
#if DEBUG_ATOMIC_LOCK         
        if (res != 0) {
            Dbg::Put("LOCK fail! = ", key);
        }
#endif        
        return res == 0;
    }

    inline u32 UnLock(void) {
        u32 register res = AtomicSwap(&key, LOCK_V, 0);
#if DEBUG_ATOMIC_LOCK         
        if (res != LOCK_V) {
            Dbg::Put("UNLOCK fail! = ", key);
        }
#endif        
        return res == LOCK_V;
    }

    inline u32 Get(void) {
        return key;
    }
};

#endif 

