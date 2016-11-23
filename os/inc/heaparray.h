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
        while (2 * k < this->size) {
            j = 2 * k;
            if (j < this->size && less(this->buff[j], this->buff[j + 1])) j++;
            if (!less(this->buff[k], this->buff[j]))break;
            this->Swap(k, j);
            k = j;
        }
    }

    void SortBottomUp(void) {
        s_t k = this->size - 1;
        s_t h = k / 2;
        while (k >= 0 && less(this->buff[h], this->buff[k])) {
            this->Swap(h, k);
            k = h;
            h = k / 2;
        }
    }
public:

    bool Add(T obj) {
        bool res = (this->PushBack(obj) != (s_t) - 1);
        if (res) {
            SortBottomUp();
        }
        return res;
    }

    bool Remove(s_t index) {
        bool res = caArray<T>::Remove(index);
        if (res && this->Size() > 0) {
            SortTopDown();
            res = true;
        }
        return res;
    }

    T dequeue(void) {
        T res = NULL;
        if (this->Size() > 0) {
            this->Swap(0, this->Size() - 1);
            res = this->PopBack();
            if (this->Size())
                SortTopDown();
        }
        return res;
    }

    T Max(void) {
        T res = NULL;
        if (this->Size() > 0) {
            res = this->PopFront();
        }
        return res;
    }

};

#endif /* HEAPARRAY_H */

