#include "config.h"
#include "caos_c_types.h"
#include "test.h"

#if TEST_CIRCULAR_BUFFER && TEST
#include "bcm2836.h"
#include "idevice.h"
#include "stream.h"
#include "auxmain.h"
#include "gpio.h"
#include "miniuart.h"
#include "circularbuffer.h"

static void test_01(u32 & success, u32 &failed) {
    u8 buff[100];
    u8 c = '?';
    caCircularBuffer<u8> CB;
    CB.Init(buff, 100);
    u32 insert;
    CB.Push(&c, 1, insert);
    if (CB.Size() == 1 && insert == 1)
        success++;
    else {
        Dbg::Put("FAILED CIRCULAR_BUFFER::TEST_01 1 SIZE!\r\n");
        failed++;
    }
    c = 0;
    u32 remove;
    CB.Pop(&c, 1, remove);
    if (c == '?' && remove == 1 && CB.Size() == 0)
        success++;
    else {
        Dbg::Put("FAILED CIRCULAR_BUFFER::TEST_01 Remove!\r\n");
        failed++;
    }
    if (CB.Size() == 0)
        success++;
    else {
        Dbg::Put("FAILED CIRCULAR_BUFFER::TEST_01 2 SIZE!\r\n");
        failed++;
    }
}

void test_circular_buffer(u32 & success, u32 &failed) {
    test_01(success, failed);
}


#endif