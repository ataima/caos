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

u32 caSysLog::Init(s_t total_size,loglevels reqlev) {
    u32 res = FALSE;
    caMemAux<s8>::MemZero((s8*)mn_Base, sizeof (mn_Base));
    s_t i;
    if(reqlev>end_log_lev )reqlev=end_log_lev;
    curlev=reqlev;
    for (i = 0; i < curlev; i++) {
        mn_Base[i] = static_cast<s8*> (caMemory::Allocate(total_size));
        if (mn_Base[i] != NULL) {
            res = ss[i].Init(mn_Base[i], total_size);
        }
        if (res == FALSE)break;
    }
    enable=false;
    return res;
}

u32 caSysLog::Destroy(void) {
    u32 res = FALSE;
    s_t i;
    for (i = 0; i < curlev; i++) {
        if (mn_Base[i] != NULL) {
            ss[i].Init(NULL, 0);
            res = caMemory::Free(mn_Base[i]);
            mn_Base[i] = NULL;
        }
    }
    enable=false;
    curlev=panic;
    return res;
}









#endif