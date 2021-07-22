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

#include "bcm2836.h"

#include "hw_func.h"
#include "interrupt.h"
#include "miniuart.h"
#include "cpu.h"
#include "coprocessor.h"
#include "cache.h"
#include "caos_version.h"
#include "memory.h"
#include "sysirqctrl.h"
#include "systimer.h"
#include "smp.h"
#include "mmu.h"



extern "C" {

    void sysInit(void) {
        s8 buff[256];
        caIrqCtrl::Init(); // start all fiq/irq disabled 
        caMiniUart::Init(115200, 8, 1, 8);
        caMiniUart::DisableIrqRx();
        caMiniUart::DisableIrqTx();
        caMiniUart::Enable(1, 1);
        Dbg::Put("> c.a.O.S. : [ ");
        caArmCpu::DumpCPSR();
        Dbg::Put(" ]\r\n");
        caMemory::Init(&hal_llc_mem);
        caMemory::DumpAvail(buff, sizeof (buff));
        Dbg::Put(buff);
        caArmCpu::GetMainIdCpuInfo();
#if CACHE_DEVICE    
        if (caCache::Start()) {
            Dbg::Put("> c.a.O.S. : [ ");
            Dbg::Put("MMU : Flat Model");
            Dbg::Put(" ]\r\n");
            Dbg::Put("> c.a.O.S. : [ ");
            Dbg::Put("Cache : Instruction,Data And Branch Prediction is On ");
            Dbg::Put(" ]\r\n");
        }
#endif
        Dbg::Put("> c.a.O.S. : [ ");
        Dbg::Put(caos_version);
        Dbg::Put(" ]\r\n");
    }

    void sysStop(void) {
        Dbg::Put("bye. [ from c.a.O.S. main proc ! ]\r\n");
        caArmCpu::WaitForEver();
    }

    void msgSchedule(void) {
        Dbg::Put("Reschedule\r\n");
    }

    void sysReset(void) {
        Dbg::Put("system Reset : TODO ");
        caSysTimer::Stop();
        caCache::Stop();
        caSMP::Disable();
        caMMU::Disable();
        caCache::SetBPIALL(0);
        caCache::SetICIALLU(0);
        caCache::SetBPIALL(0);
        u32 *ptr = (u32 *) 0x8000;
        *ptr = 0xea1fbfff;
        jump_to(0x8000);
    }

    void sysShutDown(void) {
        Dbg::Put("system ShutDown : TODO ");
        sysReset();
    }

    void sysRestart(void) {
        Dbg::Put("system Restart : TODO ");
        sysReset();
    }
}