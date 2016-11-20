#ifndef ARRAY_H
#define ARRAY_H

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

#include "softreq.h"

template <typename T>
class caArray {
protected:
    size_t capacity;
    size_t size;
    T *buff;
public:

    void Init(T* ebuff, size_t esize) {
        if (ebuff)
            buff = ebuff;
        capacity = esize;
        if (capacity < 2) {
            buff = NULL;
            capacity = 0;
            // TO DO ThrowExceptionLittlebuffer
        }
        size = 0;
    }

    inline size_t Size(void) {
        return size;
    }

    inline size_t Capacity(void) {
        return capacity;
    }

    inline bool Empty(void) {
        return size == 0;
    }

    size_t PushBack(T obj) {
        size_t res = -1; // check !=-1
        if (size < capacity) {
            res = size;
            buff[size++] = obj;
        } else {
            u32 pc, tres;
            asm("mov %0,pc" : "=r" (pc));
            ThrowExceptionObjectFull(&pc, __func__, &tres);
        }
        return res;
    }

    T PopBack(void) {
        T obj;
        if (Empty() == false) {
            size--;
            obj = buff[size];
        } else {
            u32 pc, res;
            asm("mov %0,pc" : "=r" (pc));
            ThrowExceptionObjectEmpty(&pc, __func__, &res);
        }
        return obj;
    }

    T PopFront(void) {
        T obj;
        if (Empty() == false) {
            size--;
            obj = buff[0];
            buff[0] = buff[size];
        } else {
            u32 pc, res;
            asm("mov %0,pc" : "=r" (pc));
            ThrowExceptionObjectEmpty(&pc, __func__, &res);
        }
        return obj;
    }

    T At(size_t index) {
        T obj = NULL;
        if (index < size) {
            obj = buff[index];
        } else {
            u32 pc, res;
            asm("mov %0,pc" : "=r" (pc));
            ThrowExceptionOutOfRange(&pc, __func__, &res);
        }
        return obj;
    }

    T AtEnd(void) {
        return buff[size - 1];
    }

    T operator[](const size_t index) {
        T obj = NULL;
        if (index < size) {
            obj = buff[index];
        } else {
            u32 pc, res;
            asm("mov %0,pc" : "=r" (pc));
            ThrowExceptionOutOfRange(&pc, __func__, &res);
        }
        return obj;
    }

    bool SetAt(T obj, size_t index) {
        bool res = false;
        if (index < size) {
            buff[index] = obj;
            res = true;
        } else {
            u32 pc, tres;
            asm("mov %0,pc" : "=r" (pc));
            ThrowExceptionOutOfRange(&pc, __func__, &tres);
        }
        return res;
    }

    bool Swap(size_t h, size_t k) {
        bool res = false;
        if (h < size && k < size) {
            if (h != k) {
                T p = buff[h];
                buff[h] = buff[k];
                buff[k] = p;
            }
            res = true;
        } else {
            u32 pc, tres;
            asm("mov %0,pc" : "=r" (pc));
            ThrowExceptionOutOfRange(&pc, __func__, &tres);
        }
        return res;
    }

    bool Remove(size_t index) {
        // caARRAY : remove invalidate Sort...
        bool res = false;
        if (index < size) {
            buff[index] = AtEnd();
            size--;
            res = true;
        }
        return res;
    }
};


#endif /* ARRAY_H */

