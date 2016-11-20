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

#ifndef _CIRCULARBUFFER_H
#define _CIRCULARBUFFER_H

template <typename T>
class caCircularBuffer {
    size_t WIDTH;
    size_t start;
    size_t stop;
    T *cBuff;
public:

    void Init(T * buff, size_t assigned_width) {
        WIDTH = assigned_width;
        start = stop = 0;
        cBuff = buff;
    }

    inline bool Empty(void) {
        return start == stop;
    }

    inline T* GetData(void) {
        return cBuff;
    }

    bool Insert(T *ptr, size_t requested, size_t & writed) {
        bool res = false;
        size_t i;
        size_t p;
        for (i = 0; i < requested; i++) {
            p = (stop + i) % WIDTH;
            if (Empty() == false && p == start) {
                i--;
                break;
            }
            T v = ptr[i];
            cBuff[p] = v;
        }
        if (i == requested)
            res = true;
        writed = i;
        stop = (stop + i) % WIDTH;
        return res;
    }

    bool Remove(T *ptr, size_t requested, size_t & removed) {
        bool res = false;
        if (start != stop) {
            size_t i;
            size_t p;
            for (i = 0; i < requested; i++) {
                p = (start + i) % WIDTH;
                if (Empty() == false && p == stop) {
                    i--;
                    break;
                }
                T v = cBuff[p];
                ptr[i] = v;
            }
            if (i == requested)
                res = true;
            removed = i;
            start = (start + i) % WIDTH;
        }
        return res;
    }

    size_t Size(void) {
        if (stop >= start)
            return (stop - start);
        else
            return (WIDTH - start + stop);
    }

    inline size_t Available(void) {
        return WIDTH - Size();
    }

    void Clean(void) {
        size_t i;
        for (i = 0; i < WIDTH; i++) {
            cBuff[i] = 0;
        }
        start = stop = 0;
    }

};


#endif /* _CCIRCULARBUFFER_H */

