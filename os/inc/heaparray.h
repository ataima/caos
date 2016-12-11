#ifndef HEAPARRAY_H
#define HEAPARRAY_H
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


#include "array.h"




template <typename T>
class caHeapArray
: public caArray<T> {
private:

    void SortTopDown(void) {
        s_t j, k = 0;
        while (2 * k < this->Size()) {
            j = 2 * k;
            if (j < this->Size() && (this->buff[j]<this->buff[j + 1])) j++;
            if (this->buff[k]>this->buff[j])break;
            this->Swap(k, j);
            k = j;
        }
    }

    void SortBottomUp(void) {
        s_t k = this->Size() - 1;
        s_t h = k / 2;
        while (k >= 0 && (this->buff[h]<this->buff[k])) {
            this->Swap(h, k);
            k = h;
            h = k / 2;
        }
    }
public:
    
    

    bool inqueue(T obj) {
        bool res = ( this->PushBack(obj) != (s_t) - 1);
        if (res) {
            SortBottomUp();
        }
        return res;
    }


    s_t outqueue(T & out) {
        s_t res = (s_t)(-1);
        if (this->Size() > 0) {
            this->Swap(0, this->Size() - 1);
            res = this->PopBack(out);
            if (res!=(s_t)(-1) && this->Size())
                SortTopDown();
        }
        return res;
    }


};

#endif /* HEAPARRAY_H */

