#include "hal.h"
#include "auxmain.h"
#include "gpio.h"
#include "miniuart.h"
#include "test.h"
#if TEST
extern void test_miniuart(u32 &success, u32 & failed);
extern void test_circular_buffer(u32 &success, u32 & failed);
extern void test_mem_pipe(u32 &success, u32 & failed);
extern void test_scheduler(u32 &success, u32 & failed);
extern void test_Tokenize_sstream(u32 &success, u32 & failed);
extern void test_systimer_device(u32 & success, u32 &failed);
extern void test_com1_device(u32 & success, u32 &failed);
extern void test_locking(u32 & success, u32 &failed);

void tmain(void) {
    Dbg::Put("\r\nSTART TESTS :");
    Dbg::Put("\r\n");
    Dbg::Put("\r\n");
    Dbg::Put("\r\n");
    u32 success = 0;
    u32 failed = 0;
#if TEST_MINI_UART    
    test_miniuart(success, failed);
#endif    
#if TEST_CIRCULAR_BUFFER    
    test_circular_buffer(success, failed);
#endif    
#if TEST_MEM_PIPE && MEM_PIPE_DEVICE   
    test_mem_pipe(success, failed);
#endif    
#if TEST_SCHEDULER    
    test_scheduler(success, failed);
#endif    
#if TEST_TOKENIZE_SSTREAM    
    test_Tokenize_sstream(success, failed);
#endif   
#if TEST_COM1_DEVICE
    test_com1_device(success, failed);
#endif   
#if TEST_SYSTIMER_DEVICE && SYS_TIMER_DEVICE
    test_systimer_device(success, failed);
#endif
#if TEST_LOCKING
    test_locking(success, failed);
#endif
    Dbg::Put("RESULT\r\n");
    Dbg::Put("\tSUCCESS=", success);
    Dbg::Put("\tFAILED =", failed);
    Dbg::Put("\tTOTAL  =", success + failed);
    Dbg::Put("STOP\r\n");
    while (1);
}
#endif