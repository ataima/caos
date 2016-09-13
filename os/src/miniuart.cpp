#include "config.h"
#include "bcm2836.h"
#include "idevice.h"
#include "stream.h"
#include "auxmain.h"
#include "gpio.h"
#include "miniuart.h"
#include "interrupt.h"
#include "sysirqctrl.h"
#include "cpu.h"



#if HAVE_MINIUART
#define RATE_DIV(baudrate) ((BCM2836_CLOCK_FREQ / (8 * (baudrate))) - 1)
#define BAUD_1M  RATE_DIV(1000000)
#define BAUD_115200  RATE_DIV(115200)
#define BAUD_57600  RATE_DIV(57600)
#define BAUD_9600  RATE_DIV(9600)

u32 caMiniUart::Init(u32 vel, u32 /*stop*/, u32 /*parity*/, u32 data) {
    system_aux_mini_uart(mu);
    caGpio::SetAlt5(14);
    caGpio::SetAlt5(15);
    caGpio::RemovePullDown(0, BIT(14) | BIT(15));
    caAuxMain::EnableMiniUart();
    mu->iir.asReg = 0;
    Enable(0, 0);
    if (data == 8)
        mu->lcr.asReg = 3; // Bit 1 must be set
    else
        mu->lcr.asReg = 2; // Bit 1 must be set
    mu->mcr.asReg = 0;
    mu->iir.asReg = 0xc6; // FIFO ON + FIFO CLEAR 
    switch (vel) {
        case 9600: mu->baud = BAUD_9600;
            break;
        case 57600: mu->baud = BAUD_57600;
            break;
        case 115200: mu->baud = BAUD_115200;
            break;
        case 1000000: mu->baud = BAUD_1M;
            break;
        default: mu->baud = RATE_DIV(vel);
    }
    Enable(1, 1);
    return TRUE;
}

u32 caMiniUart::Stop(void) {
    caIrqCtrl::DisableIrqAux();
    Enable(0, 0);
    caAuxMain::DisableMiniUart();
    caGpio::SetIn(14);
    caGpio::SetIn(15);
    return TRUE;
}

u32 caMiniUart::Dump(caStringStream<s8> & ss) {
    u32 res = deviceError::no_error;
    system_aux_mini_uart(mu);
    ss << caStringFormat::hex;
    ss << "bcm 2835/6 Aux MiniUart :" << caEnd::endl;
    ss << "IO  [" << (u32) & mu->io << "]=" << mu->io.asReg << caEnd::endl;
    ss << "IER [" << (u32) & mu->ier << "]=" << mu->ier.asReg << caEnd::endl;
    ss << "IIR [" << (u32) & mu->iir << "]=" << mu->iir.asReg << caEnd::endl;
    ss << "LCR [" << (u32) & mu->lcr << "]=" << mu->lcr.asReg << caEnd::endl;
    ss << "MCR [" << (u32) & mu->mcr << "]=" << mu->mcr.asReg << caEnd::endl;
    ss << "LSR [" << (u32) & mu->lsr << "]=" << mu->lsr.asReg << caEnd::endl;
    ss << "MSR [" << (u32) & mu->msr << "]=" << mu->msr.asReg << caEnd::endl;
    ss << "SCR [" << (u32) & mu->scratch << "]=" << mu->scratch << caEnd::endl;
    ss << "CTRL[" << (u32) & mu->ctrl << "]=" << mu->ctrl.asReg << caEnd::endl;
    ss << "STAT[" << (u32) & mu->stat << "]=" << mu->stat.asReg << caEnd::endl;
    ss << "BAUD[" << (u32) & mu->baud << "]=" << mu->baud << caEnd::endl;
    ss.Str();
    if (!ss.Good())
        res = deviceError::error_generic_fail_device;
    return res;
}


#endif