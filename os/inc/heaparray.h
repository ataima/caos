#ifndef HEAPARRAY_H
#define HEAPARRAY_H

#include "array.h"

template <typename T>
class caHeapArray
: public caArray<T> {
private:

    void SortTopDown(void) {
        size_t j, k = 0;
        while (2 * k < this->size) {
            j = 2 * k;
            if (j < this->size && less(this->buff[j], this->buff[j + 1])) j++;
            if (!less(this->buff[k], this->buff[j]))break;
            this->Swap(k, j);
            k = j;
        }
    }

    void SortBottomUp(void) {
        size_t k = this->size - 1;
        size_t h = k / 2;
        while (k >= 0 && less(this->buff[h], this->buff[k])) {
            this->Swap(h, k);
            k = h;
            h = k / 2;
        }
    }
public:

    bool Add(T obj) {
        bool res = (this->PushBack(obj) != (size_t) - 1);
        if (res) {
            SortBottomUp();
        }
        return res;
    }

    bool Remove(size_t index) {
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

