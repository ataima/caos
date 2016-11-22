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

#include "config.h"
#include "bcm2836.h"
#include "idevice.h"
#include "stream.h"
#include "memaux.h"
#include "syslog.h"
#include "circularbuffer.h"
#include "memory.h"

#define a1mb (1024*1024)

#if HAVE_SYS_LOG

u32 caSysLog::Init(u32 total_size) {
    u32 res = FALSE;
    if (total_size < a1mb)
        total_size = a1mb;
    mn_Base = static_cast<s8*> (caMemory::Allocate(total_size));
    if (mn_Base != NULL) {
        mn_CBuffer.Init(mn_Base, total_size);
        res = TRUE;
    }
    return res;
}

u32 caSysLog::Destroy(void) {
    u32 res = FALSE;
    if (mn_Base != NULL) {
        mn_CBuffer.Init(NULL, 0);
        res = caMemory::Free(mn_Base);
    }
    return res;
}

void caSysLog::doLog(caStringStream<s8> & ss) {
    u32 writed = 0;
    mn_CBuffer.Insert(ss.Str(), ss.Size(), writed);
}


void caSysLog::getLog(caStringStream<s8> & ss) {
    ss<<mn_CBuffer;
}




#endif