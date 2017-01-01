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
#include "syslog.h"
#include "memaux.h"
#include "memory.h"



#if HAVE_SYS_LOG

u32 caSysLog::Init(s_t _total_size) {
    u32 res = FALSE;
    total_size = _total_size;
    mn_Base = static_cast<s8*> (caMemory::Allocate(total_size));
    if (mn_Base != NULL) {
        cbuff.Init(mn_Base, total_size);
        mn_Stream = static_cast<s8*> (caMemory::Allocate(2048));
        res = ss.Init(mn_Stream, 2048);
    }
    return res;
}

u32 caSysLog::Destroy(void) {
    u32 res = FALSE;
    if (mn_Base != NULL) {
        cbuff.Init(NULL, 0);
        res = caMemory::Free(mn_Base);
        ss.Init(NULL, 0);
        res = caMemory::Free(mn_Stream);
    }
    return res;
}

caStringStream<s8> & caSysLog::operator<<(caEnd & t) {
    u32 writed;
    if (enable == 0x1000) {
        ss << t.endl;
        cbuff.Push(ss.Str(), ss.Size(), writed);
    }
    ss.Clear();
    return ss;
}







#endif