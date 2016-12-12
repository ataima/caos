#ifndef _SYSTEM_CONFIG_HEADER_
#define _SYSTEM_CONFIG_HEADER_
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

#define MAX_TASK        128

#define HAVE_DUMP_OBJ           0
//MINIUART 
#define HAVE_MINIUART           1
//SPIO1 
#define HAVE_SPI_1      0
//SPIO2
#define HAVE_SPI_2      0
//LEDS 
#define HAVE_LEDS               1
// ATOMIC LOCK
#define  HAVE_ATOMIC_LOCK       0
// ARM AP804 SYSTEM TIMER
#define HAVE_AP804_SYS_TIMER    1
// SYSTEM MEROY LOG
#define HAVE_SYS_LOG            1
// CLOK FOR SYSTEM TIMER USUALLY 1Mhz
#define SYS_CLOCK_TIMER 1000000
// SYSTEM TIMER FOR SCHEDULER
#define SYS_TIMER_TICK  10
// SCHEDULER FIQ OR IRQ
#define SYS_FIQ_TIMER   0
// soft request interrupt true or simulated
#define SYS_SOFT_REQ_ENABLED  0
// soft request Debug
#define SYS_SOFT_REQ_DEBUG  1

///////////////////////////// INCLUDE SEXCLUDE DEVICE FROM KERNEL

#define COM1_DEVICE             1    
#define MEM_PIPE_DEVICE         1    
#define SYS_TIMER_DEVICE        1
#define CACHE_DEVICE            1
#define SCHEDULER_DEVICE        1


// check before switch the task the validity of address to jump and size of stack
#define DEBUG_CHECK_TASK        1  
//
#define DEBUG_CHECK_SCHEDULER   0
// DUMP REQUEST OF SOFTWARE INTERRUPT
#define DEBUG_IOCTL_SVC_REQ     1
// DUMP IO COM1 DEVICE
#define DEBUG_ALLOC_MEM         0
// DUMP atomicLOck
#define DEBUG_ATOMIC_LOCK       0

// TO EXECUTE TESTS
#define TEST                    0
#endif
