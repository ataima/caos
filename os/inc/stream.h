#ifndef STREAM_H
#define STREAM_H

////////////////////////////////////////////////////////////////////////////////
//    Copyright (C) 2011  Angelo Coppi (angelogkcop at hotmail.com )
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


typedef enum tag_ss_format_req {
    dec,
    hex,
    bin
} caStringFormat;

typedef struct tag_ss_fill_req {
    s8 ch;
    size_t width;
    size_t end;

    tag_ss_fill_req(s8 c, size_t s) : ch(c), width(s), end(0) {
    }
} caStringFiller;

// TO UNIFORM ON ASINGLE OBJECT THE CR+LF ( OR  ANY IF NEED!)
// HAVE TO DO ss<<caEnd::endl o ss<<ss.Endl(Port))

typedef struct tag_ss_end_line {
    static const char *endl;
} caEnd;

template <typename T>
class caStringStream {
public:
private:
    size_t capacity;
    T *cBuff;
    T *start;
    T *stop;
    size_t size;
    bool mode_dec;
    bool mode_hex;
    bool mode_bin;
private:

    void toBase10(u32 v, size_t max_w) {
        const u32 iDiv[10] = {1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000};
        u32 fl, div;
        fl = 0;
        if (v == 0)
            Add('0');
        else {
            if (max_w > 9)
                max_w = 9;
            div = iDiv[max_w];
            while ((div != 0) && Good()) {
                u32 r = (v / div)&0xff;
                v = v % div;
                div /= 10;
                if (fl == 0) {
                    if (r != 0) {
                        Add((s8) (r + '0'));
                        fl = 1;
                    }
                } else {
                    Add((s8) (r + '0'));
                }
            }
        }
    }

    void toBase16(u32 v, size_t max_w) {
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
    }

    void toBase2(u32 v, size_t max_w) {
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
    }

    inline void Add(T v) {
        if (start != stop) {
            *start++ = v;
            size++;
        }
    }

public:

    u32 Init(T *base, size_t a_size) {
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

    void Clear(void) {
        start = cBuff;
        stop = &cBuff[capacity];
        size = 0;
        cBuff[0] = '\0';
    }

    inline size_t Size(void) {
        return size;
    }

    inline size_t Capacity(void) {
        return capacity;
    }

    inline void Forward(size_t pos) {
        size += pos;
    }

    inline T* Str(void) {
        if (size < capacity)
            cBuff[size] = '\0';
        return cBuff;
    }

    inline T* AtEnd(void) {
        cBuff[size] = '\0';
        return &cBuff[size];
    }

    inline bool Good(void) {
        return start != stop;
    }

    // by value

    caStringStream<T> & operator<<(caStringFiller t) {
        while (size < t.end && Good())
            Add(t.ch);
        return *this;
    }

    void Fix(caStringFiller & t) {
        t.end = size + t.width;
    }

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

    caStringStream<T> & operator<<(s8 t) {
        /*
        if (mode_dec) {
            if (t < 0) {
                Add('-');
            }
            toBase10(t, 2);
        } else
            if (mode_hex)toBase16(t, 2);
        else
            if (mode_bin)toBase2(t, 7);
         * */
        Add(t);
        return *this;
    }

    caStringStream<T> & operator<<(u8 t) {
        if (mode_dec)toBase10(t, 2);
        else
            if (mode_hex)toBase16(t, 2);
        else
            if (mode_bin)toBase2(t, 7);
        return *this;
    }

    caStringStream<T> & operator<<(s16 t) {
        if (mode_dec) {
            if (t < 0) {
                Add('-');
                t = -t;
            }
            toBase10(t, 4);
        } else
            if (mode_hex)toBase16(t, 4);
        else
            if (mode_bin)toBase2(t, 15);
        return *this;
    }

    caStringStream<T> & operator<<(u16 t) {
        if (mode_dec)toBase10(t, 4);
        else
            if (mode_hex)toBase16(t, 4);
        else
            if (mode_bin)toBase2(t, 15);
        return *this;
    }

    caStringStream<T> & operator<<(s32 t) {
        if (mode_dec) {
            if (t < 0) {
                Add('-');
                t = -t;
            }
            toBase10(t, 9);
        } else
            if (mode_hex)toBase16(t, 8);
        else
            if (mode_bin)toBase2(t, 31);
        return *this;
    }

    caStringStream<T> & operator<<(u32 t) {
        if (mode_dec)toBase10(t, 9);
        else
            if (mode_hex)toBase16(t, 8);
        else
            if (mode_bin)toBase2(t, 31);
        return *this;
    }

    caStringStream<T> & operator<<(caStringStream<T> & t) {
        T* base = t.Str();
        size_t t_size = t.Size();
        size_t i = 0;
        while (i < t_size && Good()) {
            Add(base[i++]);
        }
        return *this;
    }

    caStringStream<T> & operator<<(const char * t) {
        while (*t != '\0') {
            Add(*t++);
        }
        return *this;
    }

    //by pointer

    caStringStream<T> & operator<<(s8 *t) {
        Add('[');
        toBase16(t, 8);
        Add(']');
        return (*this) << *t;
    }

    caStringStream<T> & operator<<(u8 *t) {
        Add('[');
        toBase16((u32) t, 8);
        Add(']');
        return (*this) << *t;
    }

    caStringStream<T> & operator<<(s16 *t) {
        Add('[');
        toBase16(t, 8);
        Add(']');
        return (*this) << *t;
    }

    caStringStream<T> & operator<<(u16 *t) {
        Add('[');
        toBase16(t, 8);
        Add(']');
        return (*this) << *t;
    }

    caStringStream<T> & operator<<(s32 *t) {
        Add('[');
        toBase16(t, 8);
        Add(']');
        return (*this) << *t;
    }

    caStringStream<T> & operator<<(u32 *t) {
        Add('[');
        toBase16(t, 8);
        Add(']');
        return (*this) << *t;
    }

    caStringStream<T> & operator<<(caStringStream<T> *t) {
        Add('[');
        toBase16(t, 8);
        Add(']');
        return (*this) << *t;
    }

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
        Str();
        return t;
    }

};

template <typename T>
class TokenString {
public:
    T * ptr;
    size_t size;

    TokenString() {
        ptr = NULL;
        size = 0;
    }
};

// to do BASE CLASS STREAM TO DERIVE OSTREAMN ISTREAM SSTREAM

template <typename T>
class caTokenizeSStream {
private:
    size_t capacity;
    T *cBuff;
    T *pos;
    T *stop;
    size_t size;
    bool good;
private:

    bool inline Next(void) {
        return (pos <= stop);
    }

    T inline Get(void) {
        return *pos++;
    }

    bool inline IsSpace(T u) {
        return (u < ' ');
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
        good = Next();
        if (good) {
            while (Next()) {
                u = Get();
                if (!IsSpace(u))break;
            }
            if (IsMinus(u))
                sign = -1;
            if (IsNumber(u) && Next()) {
                res = res * base + GetNumber(u);
                u = Get();
                if (res == 0) {
                    if (u == 'X' || u == 'x') {
                        base = 16;
                        u = ToUpper(Get());
                        while (Next()) {
                            if (IsNumber(u))
                                res = res * base + GetNumber(u);
                            else
                                if (IsHexSuffix(u))
                                res = res * base + GetHexNumber(u);
                            else
                                break;
                            u = Get();
                        }
                    } else
                        if (u == 'b' || u == 'b') {
                        base = 2;
                        u = Get();
                        while ((u == '0' || u == '1') && Next()) {
                            res = res * base + GetNumber(u);
                            u = Get();
                        }
                    }
                } else {
                    while (IsNumber(u) && Next()) {
                        res = res * base + GetNumber(u);
                        u = Get();
                    }
                }
                while (IsSpace(u) && Next()) {
                    u = Get();
                }
                res *= sign;
            } else {
                good = false;
            }
        }
        return res;
    }

    void tokenize(T *&res, size_t & rsize) {
        T u = 0;
        rsize = 0;
        good = Next();
        if (good) {
            while (Next()) {
                res = pos;
                u = Get();
                if (!IsSpace(u))break;
            }
            while (IsAscii(u)) {
                rsize++;
                u = Get();
                if (!Next())break;
            }
        } else {
            res = NULL;
        }
    }
public:

    u32 Init(T *base, size_t a_size) {
        u32 res = FALSE;
        capacity = a_size - 1;
        cBuff = base;
        pos = cBuff;
        stop = &cBuff[capacity];
        size = 0;
        if (cBuff != NULL && capacity > 0) {
            res = TRUE;
        }
        good = (res == TRUE);
        return res;
    }

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
        good = (res == TRUE);
        return res;
    }

    void Clear(void) {
        pos = cBuff;
        stop = &cBuff[capacity];
        size = 0;
        cBuff[0] = '\0';
        good = (capacity > 2);
    }

    inline size_t Size(void) {
        return size;
    }

    inline bool Empty(void) {
        return size == 0;
    }

    inline size_t Capacity(void) {
        return capacity;
    }

    inline size_t Good(void) {
        return good;
    }

    inline void Forward(size_t npos) {
        size += npos;
        stop = &cBuff[size];
        good = Next();
    }

    inline T* Str(void) {
        if (size < capacity)
            cBuff[size] = '\0';
        return cBuff;
    }

    inline T* Position(void) {
        if (pos < stop)
            return pos;
        else
            return stop;
    }

    inline size_t Remain(void) {
        return (size_t) (stop - pos);
    }

    caTokenizeSStream<T> & operator>>(s8 & c) {
        good = Next();
        if (good)
            c = (s8) Get();
        return (*this);
    }

    caTokenizeSStream<T> & operator>>(u8 & c) {
        good = Next();
        if (good)
            c = (u8) Get();
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


