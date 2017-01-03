#ifndef AP804TIMER_H
#define AP804TIMER_H
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


#if HAVE_AP804_SYS_TIMER

/*ARM TIMER AS DEFINED IN BCM2836 DATA SHEET */
typedef union ap804_control_tag {

    struct tag_control_bits {
        _RW_ u32 unused0 : 1; // bit0
        _RW_ u32 counter_mode : 1; //bit 1
        _RW_ u32 clkscale : 2; //bit 2-3
        _RW_ u32 unused1 : 1; // bit4
        _RW_ u32 enableirq : 1; // bit5
        _RW_ u32 unused2 : 1; // bit6
        _RW_ u32 enable : 1; // bit7
        _RW_ u32 debug : 1; // bit8
        _RW_ u32 enablecnt : 1; // bit9
        _RW_ u32 unused3 : 6; // bit10-15
        _RW_ u32 prescaler : 8; // bit16-23
        _RW_ u32 unused4 : 8; // bit24-31
    } asBit;
    _RW_ u32 asReg;
} Ap804Control;

typedef struct arm_ap804_timer_tag {
    _RW_ u32 Load; /// Value to load into timer count
    _R_ u32 Value; /// current value of timer count
    _RW_ Ap804Control Control; /// timer and free runnig counter control
    _W_ u32 AckIrq; /// Ack irq  timer
    _R_ u32 RawIrq; /// timer irq perndin state
    _R_ u32 MaskIrq; /// timer irq status
    _R_ u32 Reload; /// reload value   
    _RW_ u32 Predivider; /// sys clk predivider 1-16-256
    _R_ u32 Counter; /// free running counter 
} Ap804ArmSysTimer, *ptrAp804ArmSysTimer;

#define system_ap804_timer(a)  const ptrAp804ArmSysTimer a=(const ptrAp804ArmSysTimer)(PB_BASE+0x00B400)
#endif


#endif /* AP804TIMER_H */

