#include "config.h"
#include "test.h"

#if TEST_MINI_UART && TEST
#include "bcm2836.h"
#include "idevice.h"
#include "stream.h"
#include "auxmain.h"
#include "gpio.h"
#include "miniuart.h"

static void test_01(u32 & success, u32 &failed) {
    // try to write and check interrupt
    u32 r;
    while ((caMiniUart::GetStat()&0xf) != 6);
    caMiniUart::Init(115200, 0, 0, 8);
    caMiniUart::ClearFifos();
    caMiniUart::EnableIrqTx();
    caMiniUart::Enable(0, 0);
    caMiniUart::Enable(1, 1);

    // I have write so interrupt is pending
    caMiniUart::Send('?');
    r = caMiniUart::GetIIR()&0xff;
    if (r == 0xc2)
        success++;
    else {
        failed++;
        Dbg::Put("No Irq TX\r\n");
    }
}

static void test_02(u32 & success, u32 &failed) {
    // try to write and check interrupt
    u32 i, n, r, t;
    while ((caMiniUart::GetStat()&0xf) != 6);
    caMiniUart::Init(115200, 0, 0, 8);
    caMiniUart::ClearFifos();
    i = caMiniUart::GetIIR(); // CLEAR PENDING
    caMiniUart::EnableIrqTx();
    caMiniUart::EnableIrqRx();
    t = caMiniUart::GetIER();
    r = caMiniUart::GetIIR();
    Dbg::Put("GetIER() =", t);
    Dbg::Put("GetIIR() =", r);
    Dbg::Put("type some chars...\r\n");
    do {
        r = caMiniUart::GetIIR();
    } while (r == 0xc2);
    caMiniUart::Recv();
    t = caMiniUart::GetIER();
    r = caMiniUart::GetIIR();
    n = caMiniUart::GetIIR();
    i = caMiniUart::GetRxFifo();
    Dbg::Put("GetIER() =", t);
    Dbg::Put("GetIIR() =", r);
    Dbg::Put("GetIIR() =", n);
    Dbg::Put("GetRxFifo() =", i);
    if (n == 2) {
        success++;
    } else {
        failed++;
        Dbg::Put("No Irq RX\r\n");
    }
    if (i == 1) {
        failed++;
        Dbg::Put("No Irq Pending\r\n");
    } else
        success++;
}

void test_miniuart(u32 & success, u32 &failed) {
    test_01(success, failed);
    test_02(success, failed);
}

#endif