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


#include "bcm2836.h"
#include "stream.h"
#include "circularbuffer.h"
#include "atomiclock.h"



#ifdef HAVE_SYS_LOG

typedef caCircularBuffer<s8> caLogStream;

class caSysLog {
    static caLogStream mn_CBuffer;
    static s8 *mn_Base;
    static caAtomicLock mn_Lock;
public:
    static u32 Init(u32 total_size);    
    static void DoLog(caStringStream<s8> & ss);
    static u32 Destroy();    
};




#endif

#endif