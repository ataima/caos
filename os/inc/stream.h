#ifndef STREAM_H
#define STREAM_H

#include "circularbuffer.h"
#include "memaux.h"

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


// CSTR simple and quick string

struct caString {
    const s8* str;
    const u32 len;

    caString(const s8* msg, s_t _len)
    : str(msg), len(_len) {
    }
};

// WARNING : sizeof(MSG)==strlen(MSG)+1
#define caCSTR(a,MSG) const caString a(MSG,sizeof(MSG))

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

typedef struct tag_ss_end_line {
    static const char* endl;
    static const char* tab;
    static const char* newl;
    static const char* cret;
    static const char* bel;
    static tag_ss_end_line obj;
} caEnd;

template <typename T>
class caIFaceStringStream {
protected:

    virtual void Add(T v) = 0;

    virtual void Stopper(void) = 0;

    virtual bool Good(void) = 0;

    static void toBase10(caIFaceStringStream<T> *obj, u32 v, u32 div) {
        while ((div != 0) && obj->Good()) {
            register u32 r = (v / div)&0xff;
            v = v % div;
            div /= 10;
            obj->Add((s8) (r + '0'));
        }
        obj->Stopper();
    }

    static void toBase10(caIFaceStringStream<T> *obj, u32 v) {
        u32 div;
        if (v == 0)
            obj->Add('0');
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
            toBase10(obj, v, div);
        }
    }

    static void toBase10(caIFaceStringStream<T> *obj, u16 v) {
        u32 div;
        if (v == 0)
            obj->Add('0');
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
            toBase10(obj, (u32) v, div);
        }
    }

    static void toBase10(caIFaceStringStream<T> *obj, u8 v) {
        u32 div;
        if (v == 0)
            obj->Add('0');
        else {
            if (v < 100) {
                if (v < 10) {
                    div = 1;
                } else
                    div = 10;
            } else
                div = 100;
            toBase10(obj, (u32) v, div);
        }
    }

    static void toBase16(caIFaceStringStream<T> *obj, u32 v, s_t max_w) {
        u32 rb;
        u32 rc;
        obj->Add('0');
        obj->Add('x');
        rb = max_w * 4;
        while (obj->Good()) {
            rb -= 4;
            rc = (v >> rb)&0xF;
            if (rc > 9)
                rc += 0x37;
            else
                rc += 0x30;
            obj->Add((s8) rc);
            if (rb == 0) break;
        }
        obj->Stopper();
    }

    static void toBase2(caIFaceStringStream<T> *obj, u32 v, s_t max_w) {
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
        obj->Add('0');
        obj->Add('b');
        u32 mask = iMask[max_w];
        while (mask && obj->Good()) {
            if (v & mask)
                obj->Add('1');
            else
                obj->Add('0');
            mask >>= 1;
        }
        obj->Stopper();
    }
};

template <typename T>
class caStringStream
: public caIFaceStringStream<T> {
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

    inline void Add(T v) {
        if (start != stop) {
            *start++ = v;
            size++;
        }
    }

    //Tested

    void Add(const T *ptr, s_t num) {
        s_t i, avail = Available();
        if (num > avail)num = avail;
        i = num;
        while (i) {
            *start++ = *ptr++;
            i--;
        }
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
        if (base != NULL) {
            if (a_size > 0)
                capacity = a_size - 1;
            else
                capacity = 0;
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
        } else {
            start = stop = cBuff = NULL;
            size = capacity = 0;
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

    bool operator==(const s8* msg) {
        return caMemAux<s8>::MemCmp(Str(), msg, Size()) == 0;
    }
    //Tested

    bool operator==(caString & msg) {
        if (Size() != msg.len)
            return false;
        else
            return caMemAux<s8>::MemCmp(Str(), msg.str, Size()) == 0;
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
        if (mode_dec)caIFaceStringStream<T>::toBase10(this, t);
        else
            if (mode_hex)caIFaceStringStream<T>::toBase16(this, t, 2);
        else
            if (mode_bin)caIFaceStringStream<T>::toBase2(this, t, 7);
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
            caIFaceStringStream<T>::toBase10(this, n);
        } else
            if (mode_hex)caIFaceStringStream<T>::toBase16(this, t, 4);
        else
            if (mode_bin)caIFaceStringStream<T>::toBase2(this, t, 15);
        return *this;
    }

    //Tested

    caStringStream<T> & operator<<(u16 t) {
        if (mode_dec)caIFaceStringStream<T>::toBase10(this, t);
        else
            if (mode_hex)caIFaceStringStream<T>::toBase16(this, t, 4);
        else
            if (mode_bin)caIFaceStringStream<T>::toBase2(this, t, 15);
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
            caIFaceStringStream<T>::toBase10(this, n);
        } else
            if (mode_hex)caIFaceStringStream<T>::toBase16(this, t, 8);
        else
            if (mode_bin)caIFaceStringStream<T>::toBase2(this, t, 31);
        return *this;
    }

    //Tested

    caStringStream<T> & operator<<(u32 t) {
        if (mode_dec)caIFaceStringStream<T>::toBase10(this, t);
        else
            if (mode_hex)caIFaceStringStream<T>::toBase16(this, t, 8);
        else
            if (mode_bin)caIFaceStringStream<T>::toBase2(this, t, 31);
        return *this;
    }

    //Tested

    inline caStringStream<T> & operator<<(caStringStream<T> & t) {
        Add(t.Str(), t.Size());
        return *this;
    }

    //Tested

    caStringStream<T> & operator<<(const char * msg) {
        if (msg) {
            while (*msg) {
                Add(*msg++);
            }
            Stopper();
        }
        return *this;
    }

    //Tested

    caStringStream<T> & operator<<(const caString & t) {
        Add(t.str, t.len - 1);
        Stopper();
        return *this;
    }


    //by pointer
    //Tested

    caStringStream<T> & operator<<(s8 *t) {
        Add('[');
        caIFaceStringStream<T>::toBase16(this, ptr_to_uint(t), 8);
        Add(']');
        if (t != NULL)
            (*this) << *t;
        return (*this);
    }

    //Tested

    caStringStream<T> & operator<<(u8 *t) {
        Add('[');
        caIFaceStringStream<T>::toBase16(this, ptr_to_uint(t), 8);
        Add(']');
        if (t != NULL)
            (*this) << *t;
        return (*this);
    }

    //Tested

    caStringStream<T> & operator<<(s16 *t) {
        Add('[');
        caIFaceStringStream<T>::toBase16(this, ptr_to_uint(t), 8);
        Add(']');
        if (t != NULL)
            (*this) << *t;
        return (*this);
    }

    //Tested

    caStringStream<T> & operator<<(u16 *t) {
        Add('[');
        caIFaceStringStream<T>::toBase16(this, ptr_to_uint(t), 8);
        Add(']');
        if (t != NULL)
            (*this) << *t;
        return (*this);
    }

    //Tested

    caStringStream<T> & operator<<(s32 *t) {
        Add('[');
        caIFaceStringStream<T>::toBase16(this, ptr_to_uint(t), 8);
        Add(']');
        if (t != NULL)
            (*this) << *t;
        return (*this);
    }

    //Tested

    caStringStream<T> & operator<<(u32 *t) {
        Add('[');
        caIFaceStringStream<T>::toBase16(this, ptr_to_uint(t), 8);
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

};

template <typename T>
class caCircularStringStream
: public caIFaceStringStream<T> {
protected:
    s_t capacity;
    T *cBuff;
    s_t start;
    s_t stop;
    bool mode_dec;
    bool mode_hex;
    bool mode_bin;

protected:

    //Tested

    inline void Add(T v) {
        if (start == capacity)
            start = 0;
        cBuff[stop] = v;
        stop++;
        if (stop == capacity)
            stop = 0;
        if (stop == start)
            start++;
    }

    inline bool Remove(T & out) {
        bool res = start != stop;
        if (res) {
            out = cBuff[start];
            start++;
            if (start == capacity)
                start = 0;
        }
        return res;
    }

    inline bool Get(T & out, s_t & index) {
        s_t pos = index + start;
        if (pos >= capacity)
            pos -= capacity;
        bool res = ((pos) != stop);
        if (res) {
            out = cBuff[pos];
            index++;
        }
        return res;
    }
    //Tested

    void Add(const T *ptr, s_t num) {
        while (num) {
            Add(*ptr++);
            num--;
        }
    }



    //Tested

    inline void Stopper(void) {
        if (stop == capacity)
            stop = 0;
        cBuff[stop] = '\0';
    }

public:

    //Tested

    caCircularStringStream() {
        start = stop = capacity = 0;
        cBuff = NULL;
        mode_dec = mode_hex = mode_bin = false;
    }

    //Tested

    u32 Init(T *base, s_t a_size) {
        u32 res = FALSE;
        if (base != NULL && a_size > 0) {
            capacity = a_size - 1;
            cBuff = base;
            start = stop = 0;
            mode_dec = true;
            mode_hex = false;
            mode_bin = false;
            if (cBuff != NULL && capacity > 0) {
                cBuff[0] = '\0';
                res = TRUE;
            }
        } else {
            cBuff = NULL;
            start = stop = capacity = 0;
        }
        return res;
    }

    //Tested

    void Clear(void) {
        start = stop = 0;
        cBuff[0] = '\0';
    }

    //Tested

    s_t Size(void) {
        s_t size;
        if (stop >= start)
            size = (stop - start);
        else
            size = (capacity - start + stop);
        return size;
    }

    inline s_t Capacity(void) {
        return capacity;
    }

    inline s_t Available(void) {
        return capacity;
    }

    //Tested

    inline bool Empty(void) {
        return start == stop;
    }

    s_t Remove(T *ptr, s_t num) {
        s_t removed = 0;
        bool res;
        T out;
        do {
            if (Remove(out)) {
                *ptr++ = out;
                removed++;
            } else {
                break;
            }
            num--;
        } while (num > 0);
        return removed;
    }

    s_t Str(T * buff, s_t size) {
        s_t index, writed = 0;
        s_t w = Size();
        if (w > 0 && buff != NULL && size > 0) {
            if (w > size)
                w = size;
            index = 0;
            T out;
            do {
                if (Get(out, index)) {
                    buff[writed] = out;
                    writed++;
                } else {
                    break;
                }
                w--;
            } while (w > 0);
            buff[writed] = '\0';
        }
        return writed;
    }

    inline s_t Str(caStringStream<T> & out) {
        s_t wr = Str(out.Str(), out.Capacity());
        out.Forward(wr);
        return wr;
    }
    //Tested

    inline bool Good(void) {
        return capacity > 0;
    }

    //Tested

    caCircularStringStream<T> & operator<<(caStringFiller t) {
        while (Size() < t.end)
            Add(t.ch);
        Stopper();
        return *this;
    }

    //Tested

    void Fix(caStringFiller & t) {
        t.end = Size() + t.width;
    }


    //Tested

    caCircularStringStream<T> & operator<<(caStringFormat t) {
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

    caCircularStringStream<T> & operator<<(s8 t) {
        Add(t);
        return *this;
    }

    //Tested

    caCircularStringStream<T> & operator<<(u8 t) {
        if (mode_dec)caIFaceStringStream<T>::toBase10(this, t);
        else
            if (mode_hex)caIFaceStringStream<T>::toBase16(this, t, 2);
        else
            if (mode_bin)caIFaceStringStream<T>::toBase2(this, t, 7);
        return *this;
    }

    //Tested

    caCircularStringStream<T> & operator<<(s16 t) {
        register u16 n;
        if (mode_dec) {
            if (t < 0) {
                Add('-');
                n = (u16) (-t);
            } else
                n = (u16) (t);
            caIFaceStringStream<T>::toBase10(this, n);
        } else
            if (mode_hex)caIFaceStringStream<T>::toBase16(this, t, 4);
        else
            if (mode_bin)caIFaceStringStream<T>::toBase2(this, t, 15);
        return *this;
    }

    //Tested

    caCircularStringStream<T> & operator<<(u16 t) {
        if (mode_dec)caIFaceStringStream<T>::toBase10(this, t);
        else
            if (mode_hex)caIFaceStringStream<T>::toBase16(this, t, 4);
        else
            if (mode_bin)caIFaceStringStream<T>::toBase2(this, t, 15);
        return *this;
    }

    //Tested

    caCircularStringStream<T> & operator<<(s32 t) {
        u32 n;
        if (mode_dec) {
            if (t < 0) {
                Add('-');
                n = (u32) (-t);
            } else
                n = (u32) (t);
            caIFaceStringStream<T>::toBase10(this, n);
        } else
            if (mode_hex)caIFaceStringStream<T>::toBase16(this, t, 8);
        else
            if (mode_bin)caIFaceStringStream<T>::toBase2(this, t, 31);
        return *this;
    }

    //Tested

    caCircularStringStream<T> & operator<<(u32 t) {
        if (mode_dec)caIFaceStringStream<T>::toBase10(this, t);
        else
            if (mode_hex)caIFaceStringStream<T>::toBase16(this, t, 8);
        else
            if (mode_bin)caIFaceStringStream<T>::toBase2(this, t, 31);
        return *this;
    }

    //Tested

    inline caCircularStringStream<T> & operator<<(caCircularStringStream<T> & t) {
        s_t old,index,total = t.Size();
        T out;
        index=0;
        do {
            if (t.Get(out,index))
            {
                old=start;
                Add(out);
                if(old!=start)
                    index--;  // a<<a if start++ index-- 
            }
            else
                break;
            total--;
        } while (total > 0);
        return *this;
    }

    //Tested

    caCircularStringStream<T> & operator<<(const char * msg) {
        if (msg) {
            while (*msg) {
                Add(*msg++);
            }
            Stopper();
        }
        return *this;
    }

    //Tested

    caCircularStringStream<T> & operator<<(const caString & t) {
        Add(t.str, t.len - 1);
        Stopper();
        return *this;
    }


    //by pointer
    //Tested

    caCircularStringStream<T> & operator<<(s8 *t) {
        Add('[');
        caIFaceStringStream<T>::toBase16(this, ptr_to_uint(t), 8);
        Add(']');
        if (t != NULL)
            (*this) << *t;
        return (*this);
    }

    //Tested

    caCircularStringStream<T> & operator<<(u8 *t) {
        Add('[');
        caIFaceStringStream<T>::toBase16(this, ptr_to_uint(t), 8);
        Add(']');
        if (t != NULL)
            (*this) << *t;
        return (*this);
    }

    //Tested

    caCircularStringStream<T> & operator<<(s16 *t) {
        Add('[');
        caIFaceStringStream<T>::toBase16(this, ptr_to_uint(t), 8);
        Add(']');
        if (t != NULL)
            (*this) << *t;
        return (*this);
    }

    //Tested

    caCircularStringStream<T> & operator<<(u16 *t) {
        Add('[');
        caIFaceStringStream<T>::toBase16(this, ptr_to_uint(t), 8);
        Add(']');
        if (t != NULL)
            (*this) << *t;
        return (*this);
    }

    //Tested

    caCircularStringStream<T> & operator<<(s32 *t) {
        Add('[');
        caIFaceStringStream<T>::toBase16(this, ptr_to_uint(t), 8);
        Add(']');
        if (t != NULL)
            (*this) << *t;
        return (*this);
    }

    //Tested

    caCircularStringStream<T> & operator<<(u32 *t) {
        Add('[');
        caIFaceStringStream<T>::toBase16(this, ptr_to_uint(t), 8);
        Add(']');
        if (t != NULL)
            (*this) << *t;
        return (*this);
    }

    //Tested

    caCircularStringStream<T> & operator<<(caCircularStringStream<T> *t) {
        if (t != NULL)
            (*this) << *t;
        return (*this);
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
        if (pos != NULL && pos == stop && *pos == '\0') {
            res = NULL;
        } else {
            if (Good()) {
                while (Good()) {
                    res = pos;
                    u = Get();
                    if (!IsSpace(u))break;
                    Next();
                }
                if (Good()) {
                    do {
                        u = Get();
                        if (!IsAscii(u))break;
                        rsize++;
                        if (!Good())break;
                        Next();
                    } while (1);
                }
            } else {
                res = NULL;
            }
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
        size = npos;
        stop = &cBuff[size];
        good = (pos != stop);
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


