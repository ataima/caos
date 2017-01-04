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


#ifdef HAVE_SYS_LOG





typedef enum tag_device_log_levels {
    irq_rx,    
    irq_tx,
    device,
    error,
    info,
    end_device_log_lev
} deviceloglevels;

class caSysLog {
    caCircularStringStream<s8> ss[deviceloglevels::end_device_log_lev];
    // circular buffer base
    s8 *mn_Base[deviceloglevels::end_device_log_lev];
    // log is enabled
    bool enable;
    // curr level for log ( l<=cur lev -> log )
    deviceloglevels curlev;
public:
    
    caSysLog(){
        enable=false;
        curlev=deviceloglevels::end_device_log_lev;
        caMemAux<s8>::MemZero((s8*)mn_Base,sizeof(mn_Base));
    }
    
    u32 Init(s_t _total_size , deviceloglevels reqlev);
    u32 Destroy();

    
    inline bool IsValid(void) {
        return (curlev>0 && mn_Base[curlev-1]!=NULL);
    }

    inline bool IsEnabled(void) {
        return enable ;
    }
    
    inline void Enable(void) {
        enable = true;
    }

    inline void Disable(void) {
        enable = false;
    }
    
    
    inline s32 GetCurLogLevel(void) {
        return (s32)(curlev);
    }
    
    inline caCircularStringStream<s8> & Stream(deviceloglevels l){
        if(l>deviceloglevels::info)l=deviceloglevels::info;
        return ss[l];
    }
    
    inline s8* GetBase(deviceloglevels l){
        if(l>deviceloglevels::info)l=deviceloglevels::info;
        return mn_Base[l];
    }
};


extern hal_ll_sys_time hal_ll_time;

#define LOGGIN 1

#if LOGGIN

#define LOG(LOG,LEVEL) if(LOG.IsEnabled() && (s32)(LEVEL)<=LOG.GetCurLogLevel()) \
                        LOG.Stream(LEVEL)<<"["<<hal_ll_time.hll_tick()<<"] : "<<#LEVEL<<" : "<<__func__<<" : "


#else
#define LOG(LOG,LEVEL)

#endif 




#endif

#endif