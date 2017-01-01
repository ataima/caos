#ifndef  _SYS_LOG_HEADER_
#define  _SYS_LOG_HEADER_

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


#include "hal.h"
#include "circularbuffer.h"
#include "atomiclock.h"



#ifdef HAVE_SYS_LOG

/**
 * 
 * simple log class not blocking and not interrupt safe...
 * TO DO work to add features
 **/




class caSysLog {
    // I can add strdeam of 2048 byte max at once
    static const u32 max_string = 2048;
    s_t total_size;
    // Circular buffer of size total_size attached to mn_base dinamically allocated
    caCircularBuffer<s8> cbuff;
    // string stream attached to mn_stream
    caStringStream<s8> ss;
    // circular buffer base
    s8 *mn_Base;
    // stream base
    s8 *mn_Stream;
    // log is enabled
    u32 enable;
public:
    u32 Init(s_t _total_size = 0);
    u32 Destroy();

    inline caStringStream<s8> & GetStream(void) {
        return ss;
    }

    inline caCircularBuffer<s8> & GetCBuffer(void) {
        return cbuff;
    }

    inline bool IsEnabled(void) {
        return (enable == 0x1000);
    }

    inline void Enable(void) {
        enable = 0x1000;
    }

    inline void Disable(void) {
        enable = 0;
    }

    inline caStringStream<s8> & operator<<(caStringFiller t) {
        return ss << t;
    }

    inline caStringStream<s8> & operator<<(caStringFormat t) {
        return ss << t;
    }

    inline caStringStream<s8> & operator<<(s8 t) {
        return ss << t;
    }

    inline caStringStream<s8> & operator<<(u8 t) {
        return ss << t;
    }

    inline caStringStream<s8> & operator<<(s16 t) {
        return ss << t;
    }

    inline caStringStream<s8> & operator<<(u16 t) {
        return ss << t;
    }

    inline caStringStream<s8> & operator<<(s32 t) {
        return ss << t;
    }

    inline caStringStream<s8> & operator<<(u32 t) {
        return ss << t;
    }

    inline caStringStream<s8> & operator<<(caStringStream<s8> & t) {
        return ss << t;
    }

    inline caStringStream<s8> & operator<<(const char * t) {
        return ss << t;
    }

    inline caStringStream<s8> & operator<<(const caString & t) {
        return ss << t;
    }

    inline caStringStream<s8> & operator<<(s8 *t) {
        return ss << t;
    }

    inline caStringStream<s8> & operator<<(u8 *t) {
        return ss << t;
    }

    inline caStringStream<s8> & operator<<(s16 *t) {
        return ss << t;
    }

    inline caStringStream<s8> & operator<<(u16 *t) {
        return ss << t;
    }

    inline caStringStream<s8> & operator<<(s32 *t) {
        return ss << t;
    }

    inline caStringStream<s8> & operator<<(u32 *t) {
        return ss << t;
    }

    inline caStringStream<s8> & operator<<(caStringStream<s8> *t) {
        return ss << t;
    }

    caStringStream<s8> & operator<<(caEnd & t);

};


#endif

#endif