#ifndef _SYSTEM_CONFIG_HEADER_
#define _SYSTEM_CONFIG_HEADER_

#define MAX_TASK        128

#define HAVE_DUMP_OBJ  0
//MINIUART 
#define HAVE_MINIUART   1
//SPIO1 
#define HAVE_SPI_1      1
//SPIO2
#define HAVE_SPI_2      1
//LEDS 
#define HAVE_LEDS   1
// ARM AP804 SYSTEM TIMER
#define HAVE_AP804_SYS_TIMER  1
// CLOK FOR SYSTEM TIMER USUALLY 1Mhz
#define SYS_CLOCK_TIMER 1000000
// SYSTEM TIMER FOR SCHEDULER
#define SYS_TIMER_TICK  100
// SCHEDULER FIQ OR IRQ
#define SYS_FIQ_TIMER   0

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
#define DEBUG_IOCTL_SVC_REQ     0
// DUMP IO COM1 DEVICE
#define DEBUG_ALLOC_MEM         0
// DUMP atomicLOck
#define DEBUG_ATOMIC_LOCK       0

// TO EXECUTE TESTS
#define TEST                    0
#endif
