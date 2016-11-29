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
    s_t WIDTH;
    s_t start;
    s_t stop;
    T *cBuff;
public:

    caCircularBuffer(){
        start=stop=WIDTH = 0;
        cBuff=NULL;
    }
    
    void Init(T * buff, s_t assigned_width) {
        WIDTH = assigned_width;
        start = stop = 0;
        cBuff = buff;
    }

    inline bool Empty(void) {
        return start == stop;
    }

    inline s_t Capacity(void) {
        return WIDTH;
    }

    bool Insert(T *ptr, s_t requested, s_t & writed) {
        bool res = false;
        s_t i;
        s_t p;
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

    bool Remove(T *ptr, s_t requested, s_t & removed) {
        bool res = false;
        if (start != stop) {
            s_t i;
            s_t p;
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

    s_t Size(void) {
        if (stop >= start)
            return (stop - start);
        else
            return (WIDTH - start + stop);
    }

    inline s_t Available(void) {
        return WIDTH - Size();
    }

    void Clean(void) {
        s_t i;
        for (i = 0; i < WIDTH; i++) {
            cBuff[i] = 0;
        }
        start = stop = 0;
    }

};


#endif /* _CCIRCULARBUFFER_H */

