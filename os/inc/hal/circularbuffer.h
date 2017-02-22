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
    bool error;

protected:

    bool add(T *v, s_t requested, s_t & writed) {
        bool res = false;
        writed = 0;
        s_t i = Available();
        s_t request = requested;
        if (request > i)
            request = i;
        for (i = 0; i < request; i++) {
            if (Push(v[i]))
                writed++;
            else
                break;
        }
        res = (writed == requested);
        return res;
    }

    bool remove(T *ptr, s_t requested, s_t & removed) {
        bool res = false;
        removed = 0;
        s_t i = Size();
        s_t request = requested;
        if (request > i)
            request = i;
        for (i = 0; i < request; i++) {
            T v;
            if (Pop(v)) {
                ptr[i] = v;
                removed++;
            } else
                break;
        }
        res = (removed == requested);
        return res;
    }



public:

    caCircularBuffer() {
        start = stop = WIDTH = 0;
        cBuff = nullptr;
    }

    void Init(T * buff, s_t assigned_width) {
        WIDTH = assigned_width;
        start = stop = 0;
        cBuff = buff;
    }

    inline bool Good(void) {
        bool res;
        s_t diff;
        if (start == stop)
            res = true;
        else {
            if (start > stop)
                diff = start - stop;
            else
                diff = stop - start;
            res = (diff != WIDTH);
        }
        return res;
    }

    inline bool Empty(void) {
        return (start == stop);
    }

    inline s_t Capacity(void) {
        return WIDTH;
    }

    inline bool Push(T v) {
        if (stop == WIDTH)
            stop = 0;
        if (start == WIDTH)
            start = 0;
        cBuff[stop] = v;
        stop += 1;
        //stop = stop % WIDTH;        
        return true;
    }

    inline bool Pop(T & V) {
        bool res = false;
        if (Empty() == false) {
            if (start == WIDTH)
                start = 0;
            V = cBuff[start];
            start += 1;
            res = true;
        }
        return res;
    }

    inline bool Push(T *ptr, s_t requested, s_t & writed) {
        bool res = false;
        if (ptr != nullptr) {
            res = add(ptr, requested, writed);
        }
        return res;
    }

    inline bool Pop(T *ptr, s_t requested, s_t & readed) {
        bool res = false;
        if (ptr != nullptr) {
            res = remove(ptr, requested, readed);
        }
        return res;
    }

    s_t Size(void) {
        s_t size;
        if (stop >= start)
            size = (stop - start);
        else
            size = (WIDTH - start + stop);
        return size;
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

    inline T * GetBase(void) {
        return cBuff;
    }

};



#endif /* _CCIRCULARBUFFER_H */

