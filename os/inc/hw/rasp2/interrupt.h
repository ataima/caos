
#ifndef _INTERRUPT_HEADER_
#define _INTERRUPT_HEADER_


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

//IRQ, FIQ, SWI, ABORT and UNDEF
extern "C" {
    // to do port in class caARMspecific
    extern void changeCPSR(u32 mode);

    void ISR_Undefined(u32 lr_usr, u32 lr_svc, u32 lr_irq, u32 lr_und);
    void ISR_Software(u32 ioctl, u32 *p1, u32 *p2, u32 *res);
    void ISR_Abort(u32 lr_usr, u32 lr_svc, u32 lr_irq, u32 lr_abt);
    u32 ISR_IRQ(void);
    u32 ISR_FIQ(void);
    void ISR_Prefetch(u32 lr_usr, u32 lr_svc, u32 lr_irq, u32 lr_abt);
    void ISR_Hypervisor(void);
    void switchContext(void) __attribute__ ((naked));
}

class caInterruptRequest {
public:
    static void Undefined(u32 lr_usr, u32 lr_svc, u32 lr_irq, u32 ld_und);
    static void Software(u32 ioctl, u32 *p1,
            u32 *p2, u32 *res);
    static void Abort(u32 lr_usr, u32 lr_svc, u32 lr_irq, u32 lr_abt);
    static u32 IRQ(void);
    static u32 FIQ(void);
    static void Prefetch(u32 lr_usr, u32 lr_svc, u32 lr_irq, u32 lr_fiq);
    static void Hypervisor(void);



};


#endif 

