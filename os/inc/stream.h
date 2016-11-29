#ifndef STREAM_H
#define STREAM_H

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
// Check GCC

#if __x86_64__ 
#define _IS_64_ 1
#define _IS_32_ 0
#else
#define _IS_64_ 0
#define _IS_32_ 1
#endif

#if _IS_64_

inline u32 ptr_to_uint(void *p) {
    long long int v64 = (long long int) (p);
    u32 v = (u32) (v64 & 0xffffffff);
    return v;
}
#endif        


#if _IS_32_
#define ptr_to_uint(PTR)  ((u32)(PTR))
#endif        

typedef enum tag_ss_format_req {
    dec,
    hex,
    bin
} caStringFormat;

//Tested

typedef struct tag_ss_fill_req {
    s8 ch;
    s_t width;
    s_t end;

    tag_ss_fill_req(s8 c, s_t s) : ch(c), width(s), end(0) {
    }
} caStringFiller;

// TO UNIFORM ON ASINGLE OBJECT THE CR+LF ( OR  ANY IF NEED!)
// HAVE TO DO ss<<caEnd::endl o ss<<ss.Endl(Port))

typedef struct tag_ss_end_line {
    static const char *endl;
} caEnd;

template <typename T>
class caStringStream {
protected:
    s_t capacity;
    T *cBuff;
    T *start;
    T *stop;
    s_t size;
    bool mode_dec;
    bool mode_hex;
    bool mode_bin;

protected:

    //Tested

    void toBase10(u32 v, u32 div) {
        while ((div != 0) && Good()) {
            register u32 r = (v / div)&0xff;
            v = v % div;
            div /= 10;
            Add((s8) (r + '0'));
        }
        Stopper();
    }

    //Tested

    void toBase10(u32 v) {
        u32 div;
        if (v == 0)
            Add('0');
        else {
            if (v < 1000000000) {
                if (v < 100000000) {
                    if (v < 10000000) {
                        if (v < 1000000) {
                            if (v < 100000) {
                                if (v < 10000) {
                                    if (v < 1000) {
                                        if (v < 100) {
                                            if (v < 10) {
                                                div = 1;
                                            } else
                                                div = 10;
                                        } else
                                            div = 100;
                                    } else
                                        div = 1000;
                                } else
                                    div = 10000;
                            } else
                                div = 100000;
                        } else
                            div = 1000000;
                    } else
                        div = 10000000;
                } else
                    div = 100000000;
            } else
                div = 1000000000;
            toBase10(v, div);
        }
    }

    //Tested

    void toBase10(u16 v) {
        u32 div;
        if (v == 0)
            Add('0');
        else {
            if (v < 10000) {
                if (v < 1000) {
                    if (v < 100) {
                        if (v < 10) {
                            div = 1;
                        } else
                            div = 10;
                    } else
                        div = 100;
                } else
                    div = 1000;
            } else
                div = 10000;
            toBase10((u32) v, div);
        }
    }

    //Tested

    void toBase10(u8 v) {
        u32 div;
        if (v == 0)
            Add('0');
        else {
            if (v < 100) {
                if (v < 10) {
                    div = 1;
                } else
                    div = 10;
            } else
                div = 100;
            toBase10((u32) v, div);
        }
    }

    //Tested

    void toBase16(u32 v, s_t max_w) {
        u32 rb;
        u32 rc;
        Add('0');
        Add('x');
        rb = max_w * 4;
        while (Good()) {
            rb -= 4;
            rc = (v >> rb)&0xF;
            if (rc > 9)
                rc += 0x37;
            else
                rc += 0x30;
            Add((s8) rc);
            if (rb == 0) break;
        }
        Stopper();
    }

    //Tested

    void toBase2(u32 v, s_t max_w) {
        const u32 iMask[] = {
            1, 2, 4, 8,
            0x10, 0x20, 0x40, 0x80,
            0x100, 0x200, 0x400, 0x800,
            0x1000, 0x2000, 0x4000, 0x8000,
            0x10000, 0x20000, 0x40000, 0x80000,
            0x100000, 0x200000, 0x400000, 0x800000,
            0x1000000, 0x2000000, 0x4000000, 0x8000000,
            0x10000000, 0x20000000, 0x40000000, 0x80000000
        };
        Add('0');
        Add('b');
        u32 mask = iMask[max_w];
        while (mask && Good()) {
            if (v & mask)
                Add('1');
            else
                Add('0');
            mask >>= 1;
        }
        Stopper();
    }

    //Tested

    inline void Add(T v) {
        if (start != stop) {
            *start++ = v;
            size++;
        }
    }

    //Tested

    inline void Add(T *ptr, s_t num) {
        s_t avail = Available();
        if (num > avail)num = avail;
        memcpy(start, ptr, num * sizeof (T));
        start += num;
        size += num;
    }

    //Tested

    inline void Stopper(void) {
        if (start != stop) {
            *start = 0;
        }
    }

public:

    //Tested

    caStringStream() {
        capacity = 0;
        cBuff = start = stop = NULL;
        size = 0;
        mode_dec = mode_hex = mode_bin = false;
    }

    //Tested

    u32 Init(T *base, s_t a_size) {
        u32 res = FALSE;
        capacity = a_size - 1;
        cBuff = base;
        start = cBuff;
        stop = &cBuff[capacity];
        size = 0;
        mode_dec = true;
        mode_hex = false;
        mode_bin = false;
        if (cBuff != NULL && capacity > 0) {
            cBuff[0] = '\0';
            res = TRUE;
        }
        return res;
    }

    //Tested

    void Clear(void) {
        start = cBuff;
        stop = &cBuff[capacity];
        size = 0;
        cBuff[0] = '\0';
    }

    //Tested

    inline s_t Size(void) {
        return size;
    }

    //Tested

    inline s_t Capacity(void) {
        return capacity;
    }


    //Tested

    inline bool Empty(void) {
        return size == 0;
    }

    //Tested

    inline s_t Available(void) {
        return (capacity - size);
    }

    inline void Forward(s_t pos) {
        size += pos;
    }

    //Tested

    inline T* Str(void) {
        if (size < capacity)
            cBuff[size] = '\0';
        return cBuff;
    }

    //Tested

    inline bool Good(void) {
        return start != stop;
    }

    //Tested

    caStringStream<T> & operator<<(caStringFiller t) {
        while (size < t.end && Good())
            Add(t.ch);
        Stopper();
        return *this;
    }

    //Tested

    void Fix(caStringFiller & t) {
        t.end = size + t.width;
    }

    //Tested

    caStringStream<T> & operator<<(caStringFormat t) {
        mode_dec = mode_hex = mode_bin = false;
        if (t == caStringFormat::dec) {
            mode_dec = true;
        } else
            if (t == caStringFormat::hex) {
            mode_hex = true;
        } else
            if (t == caStringFormat::bin) {
            mode_bin = true;
        }
        return *this;
    }

    //Tested

    caStringStream<T> & operator<<(s8 t) {
        Add(t);
        return *this;
    }

    //Tested

    caStringStream<T> & operator<<(u8 t) {
        if (mode_dec)toBase10(t);
        else
            if (mode_hex)toBase16(t, 2);
        else
            if (mode_bin)toBase2(t, 7);
        return *this;
    }

    //Tested

    caStringStream<T> & operator<<(s16 t) {
        register u16 n;
        if (mode_dec) {
            if (t < 0) {
                Add('-');
                n = (u16) (-t);
            } else
                n = (u16) (t);
            toBase10(n);
        } else
            if (mode_hex)toBase16(t, 4);
        else
            if (mode_bin)toBase2(t, 15);
        return *this;
    }

    //Tested

    caStringStream<T> & operator<<(u16 t) {
        if (mode_dec)toBase10(t);
        else
            if (mode_hex)toBase16(t, 4);
        else
            if (mode_bin)toBase2(t, 15);
        return *this;
    }

    //Tested

    caStringStream<T> & operator<<(s32 t) {
        u32 n;
        if (mode_dec) {
            if (t < 0) {
                Add('-');
                n = (u32) (-t);
            } else
                n = (u32) (t);
            toBase10(n);
        } else
            if (mode_hex)toBase16(t, 8);
        else
            if (mode_bin)toBase2(t, 31);
        return *this;
    }

    //Tested

    caStringStream<T> & operator<<(u32 t) {
        if (mode_dec)toBase10(t);
        else
            if (mode_hex)toBase16(t, 8);
        else
            if (mode_bin)toBase2(t, 31);
        return *this;
    }

    //Tested

    inline caStringStream<T> & operator<<(caStringStream<T> & t) {
        Add(t.Str(), t.Size());
        return *this;
    }

    //Tested

    caStringStream<T> & operator<<(const char * t) {
        while (*t != '\0') {
            Add(*t++);
        }
        Stopper();
        return *this;
    }

    //by pointer
    //Tested

    caStringStream<T> & operator<<(s8 *t) {
        Add('[');
        toBase16(ptr_to_uint(t), 8);
        Add(']');
        if (t != NULL)
            (*this) << *t;
        return (*this);
    }

    //Tested

    caStringStream<T> & operator<<(u8 *t) {
        Add('[');
        toBase16(ptr_to_uint(t), 8);
        Add(']');
        if (t != NULL)
            (*this) << *t;
        return (*this);
    }

    //Tested

    caStringStream<T> & operator<<(s16 *t) {
        Add('[');
        toBase16(ptr_to_uint(t), 8);
        Add(']');
        if (t != NULL)
            (*this) << *t;
        return (*this);
    }

    //Tested

    caStringStream<T> & operator<<(u16 *t) {
        Add('[');
        toBase16(ptr_to_uint(t), 8);
        Add(']');
        if (t != NULL)
            (*this) << *t;
        return (*this);
    }

    //Tested

    caStringStream<T> & operator<<(s32 *t) {
        Add('[');
        toBase16(ptr_to_uint(t), 8);
        Add(']');
        if (t != NULL)
            (*this) << *t;
        return (*this);
    }

    //Tested

    caStringStream<T> & operator<<(u32 *t) {
        Add('[');
        toBase16(ptr_to_uint(t), 8);
        Add(']');
        if (t != NULL)
            (*this) << *t;
        return (*this);
    }

    //Tested

    caStringStream<T> & operator<<(caStringStream<T> *t) {
        if (t != NULL)
            (*this) << *t;
        return (*this);
    }

    //TO DO REMOVE IT ! to -> caDevicePort operator << (caStringStream...)

    caStringStream<T> & operator<<(caDevicePort & port) {
        if (port.IsValidHandle()) {
            //Write Out + Clear Stream;
            deviceError res;
            port.wrBuff = (u8 *)this->Str();
            port.wrSize = this->Size();
            port.writed = 0;
            res = caDevice::Write(port);
            if (res == deviceError::no_error) {
                this->Clear();
            }
        }
        return (*this);
    }

    caDevicePort & Endl(caDevicePort &t) {
        (*this) << caEnd::endl;
        Stopper();
        return t;
    }

};

template <typename T>
class TokenString {
public:
    T * ptr;
    s_t size;

    TokenString() {
        ptr = NULL;
        size = 0;
    }
};

// to do BASE CLASS STREAM TO DERIVE OSTREAMN ISTREAM SSTREAM

template <typename T>
class caTokenizeSStream {
private:
    s_t capacity;
    T *cBuff;
    T *pos;
    T *stop;
    s_t size;
    bool good;
private:

    bool inline Next(void) {
        pos++;
        return (pos != stop);
    }

    T inline Get(void) {
        return *pos;
    }

    bool inline IsSpace(T u) {
        return (u <= ' ');
    }

    bool inline IsNumber(T u) {
        return (u >= '0' && u <= '9');
    }

    bool inline IsHexSuffix(T u) {
        return (u >= 'A' && u <= 'F');
    }

    T inline ToUpper(T u) {
        if (u >= 'a' && u <= 'z')u = (T) (u + ('A' - 'a'));
        return u;
    }

    bool inline IsMinus(T u) {
        return (u == '-');
    }

    bool inline IsAscii(T u) {
        return (u > ' ' && u < 128);
    }

    u32 inline GetNumber(T u) {
        return u - '0';
    }

    u32 inline GetHexNumber(T u) {
        return u - 'A' + 10;
    }

    u32 ToNumber(void) {
        T u;
        u32 res = 0;
        u32 base = 10;
        s32 sign = 1;
        Good();
        if (good) {
            u = Get();
            if (IsSpace(u)) {
                Next();
                while (Good()) {
                    u = Get();
                    if (!IsSpace(u))break;
                    Next();
                }
            }
            if (IsMinus(u)) {
                sign = -1;
                if (Good()) {
                    Next();
                    u = Get();
                    if (!IsNumber(u)) {
                        good = false;
                        res = 0;
                        return res;
                    }
                } else {
                    good = false;
                    res = 0;
                    return res;
                }
            }
            if (IsNumber(u) && Good()) {
                Next();
                if (u == '0') {
                    u = Get();
                    Next();
                    if (u == 'X' || u == 'x') {
                        base = 16;
                        u = ToUpper(Get());
                        while (Good()) {
                            if (IsNumber(u))
                                res = res * base + GetNumber(u);
                            else
                                if (IsHexSuffix(u))
                                res = res * base + GetHexNumber(u);
                            else
                                break;
                            Next();
                            u = ToUpper(Get());
                        }
                    } else
                        if (u == 'b' || u == 'b') {
                        base = 2;
                        u = Get();
                        Next();
                        if (u == '0' || u == '1') {
                            while (Good()) {
                                res = res * base + GetNumber(u);
                                u = Get();
                                if (u == '0' || u == '1')
                                    Next();
                                else
                                    break;
                            }
                        }
                    }
                } else {
                    while (IsNumber(u) && Good()) {
                        res = res * base + GetNumber(u);
                        u = Get();
                        if (!IsNumber(u))break;
                        Next();
                    }
                }
                if (u != '\0' && IsSpace(u)) {
                    while (Good()) {
                        u = Get();
                        if (!IsSpace(u))break;
                        Next();
                    }
                }
                res *= sign;
            } else {
                good = false;
            }
        }
        return res;
    }

    void tokenize(T *&res, s_t & rsize) {
        T u = 0;
        rsize = 0;
        Good();
        if (good) {
            while (Good()) {
                res = pos;
                u = Get();
                if (!IsSpace(u))break;
                Next();
            }
            while (IsAscii(u)) {
                rsize++;
                u = Get();
                if (!Good())break;
                Next();
            }
        } else {
            res = NULL;
        }
    }

public:

    //Tested 

    caTokenizeSStream() {
        capacity = 0;
        cBuff = pos = stop = NULL;
        size = 0;
        good = false;
    }

    //Tested 

    u32 Init(T *base, s_t total_size, s_t s_size) {
        u32 res = FALSE;
        capacity = total_size - 1;
        cBuff = base;
        pos = cBuff;
        size = s_size;
        stop = &cBuff[size];
        if (cBuff != NULL && capacity > 0) {
            res = TRUE;
        }
        good = ((res == TRUE) && Good());
        return res;
    }

    //Tested 

    u32 Init(caStringStream<T> & ss) {
        u32 res = FALSE;
        capacity = ss.Capacity();
        cBuff = ss.Str();
        size = ss.Size();
        pos = cBuff;
        stop = &cBuff[size];
        if (cBuff != NULL && capacity > 0) {
            res = TRUE;
        }
        good = ((res == TRUE) && Good());
        return res;
    }

    //Tested 

    void Clear(void) {
        pos = cBuff;
        stop = &cBuff[capacity];
        size = 0;
        cBuff[0] = '\0';
        good = (capacity > 2);
    }

    //Tested 

    inline s_t Size(void) {
        return size;
    }

    //Tested

    inline bool Empty(void) {
        return pos == &cBuff[size];
    }

    //Tested 

    inline s_t Capacity(void) {
        return capacity;
    }

    //Tested 

    inline s_t Available(void) {
        return capacity - size;
    }

    //Tested 

    inline s_t Good(void) {
        return good = (pos <= stop && pos != NULL);
    }

    inline void Forward(s_t npos) {
        size += npos;
        stop = &cBuff[size];
        good = Next();
    }

    //Tested 

    inline T* Str(void) {
        if (stop != NULL)
            *stop = '\0';
        return pos;
    }

    inline T* Position(void) {
        if (pos < stop)
            return pos;
        else
            return stop;
    }

    inline s_t Remain(void) {
        return (s_t) (ptr_to_uint(stop) - ptr_to_uint(pos));
    }

    caTokenizeSStream<T> & operator>>(s8 & c) {
        Good();
        if (good) {
            c = (s8) Get();
            Next();
        }
        return (*this);
    }

    caTokenizeSStream<T> & operator>>(u8 & c) {
        Good();
        if (good) {
            c = (u8) Get();
            Next();
        }
        return (*this);
    }

    caTokenizeSStream<T> & operator>>(s16 & c) {
        c = (s16) ToNumber();
        return (*this);
    }

    caTokenizeSStream<T> & operator>>(u16 & c) {
        c = (u16) ToNumber();
        return (*this);
    }

    caTokenizeSStream<T> & operator>>(s32 & c) {
        c = (s32) ToNumber();
        return (*this);
    }

    caTokenizeSStream<T> & operator>>(u32 & c) {
        c = ToNumber();
        return (*this);
    }

    //TOKENIZE

    caTokenizeSStream<T> & operator>>(TokenString<T> & t) {
        tokenize(t.ptr, t.size);
        return (*this);
    }

    void Dump(caStringStream<s8> & ss) {
        ss << caStringFormat::hex;
        while (Next()) {
            T v = Get();
            ss << "[";
            ss << (u8) v;
            ss << "] ";
        }
    }
};
#endif /* STREAM_H */


