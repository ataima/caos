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

template <typename T>
class caArray {
protected:
    s_t capacity;
    s_t size;
    bool good;
    T *buff;
public:
    //T

    caArray(void) {
        buff = nullptr;
        good = false;
        size = capacity = 0;
    }

    //T

    void Init(T* ebuff, s_t esize) {
        if (ebuff)
            buff = ebuff;
        capacity = esize;
        if (capacity < 2) {
            buff = nullptr;
            capacity = 0;
            good = false;
        } else
            good = true;
        size = 0;
    }

    void Detach(void) {
        buff = nullptr;
        good = false;
        size = capacity = 0;
    }

    //T

    inline s_t Size(void) {
        return size;
    }

    //T

    inline s_t Capacity(void) {
        return capacity;
    }

    //T

    inline bool Good(void) {
        return good;
    }

    //T

    inline T* Start(void) {
        return buff;
    }

    //T

    inline T* Stop(void) {
        if (buff)
            return &buff[capacity - 1];
        else
            return nullptr;
    }

    //T

    inline bool Empty(void) {
        return size == 0;
    }

    //T

    s_t PushBack(T obj) {
        s_t res = -1; // check !=-1
        if (size < capacity) {                        
            buff[size] = obj;
            size++;
            res = size;
            good = true;
        } else {
            good = false;
        }
        return res;
    }

    //T    

    s_t PopBack(T & obj) {
        s_t res = -1; // check !=-1
        if (Empty() == false) {
            size--;
            obj = buff[size];
            good = true;
            res = size;
        } else {
            good = false;
        }
        return res;
    }

    // T 

    bool At(T & obj, s_t index) {
        bool res = false;
        if (index < size) {
            obj = buff[index];
            res = true;
        }
        good = res;
        return res;
    }

    //T 

    bool SetAt(T obj, s_t index) {
        bool res = false;
        if (index < size) {
            buff[index] = obj;
            res = true;
        }
        good = res;
        return res;
    }

    //T

    T End(void) {
        return buff[size - 1];
    }

    // T

    bool Swap(s_t h, s_t k) {
        bool res = false;
        if (h < size && k < size) {
            if (h != k) {
                T p = buff[h];
                buff[h] = buff[k];
                buff[k] = p;
            }
            res = true;
        }
        good = res;
        return res;
    }

    // T

    bool Remove(s_t index) {
        // caARRAY : remove invalidate Sort...
        bool res = false;
        if (index < size) {
            buff[index] = End();
            size--;
            res = true;
        }
        good = res;
        return res;
    }
};


#endif /* ARRAY_H */

