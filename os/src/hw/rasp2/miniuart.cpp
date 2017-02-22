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

#include "auxmain.h"
#include "gpio.h"
#include "miniuart.h"
#include "interrupt.h"
#include "sysirqctrl.h"
#include "cpu.h"
#include "memaux.h"





#if HAVE_MINIUART
#define RATE_DIV(baudrate) ((BCM2836_CLOCK_FREQ / (8 * (baudrate))) - 1)
#define BAUD_1M  RATE_DIV(1000000)
#define BAUD_115200  RATE_DIV(115200)
#define BAUD_57600  RATE_DIV(57600)
#define BAUD_9600  RATE_DIV(9600)

// temp rx buffer for irq rx
u8 caMiniUart::rxBuff[32];
// buffer rx index
u32 caMiniUart::rxPos = 0;
// temp tx buffer for irq tx
u8 caMiniUart::txBuff[32];
// buffer tx index
u32 caMiniUart::txPos = 0;
// error tx overrun
u32 caMiniUart::txOverrun = 0;
// error rx overrun
u32 caMiniUart::rxOverrun = 0;

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
    caMemAux<u32>::MemSet((u32*) rxBuff, 0, sizeof (rxBuff));
    caMemAux<u32>::MemSet((u32*) txBuff, 0, sizeof (txBuff));
    rxPos = txPos = rxOverrun = txOverrun = 0;
    return true;
}

u32 caMiniUart::Stop(void) {
    caIrqCtrl::DisableIrqAux();
    Enable(0, 0);
    caAuxMain::DisableMiniUart();
    caGpio::SetIn(14);
    caGpio::SetIn(15);
    caMemAux<u32>::MemSet((u32*) rxBuff, 0, sizeof (rxBuff));
    caMemAux<u32>::MemSet((u32*) txBuff, 0, sizeof (txBuff));
    rxPos = txPos = rxOverrun = txOverrun = 0;
    return true;
}

u32 caMiniUart::GetErrors(u32 &rxError, u32 & txError) {
    rxError = rxOverrun;
    txError = txOverrun;
    return true;
}

u32 caMiniUart::Dump(caStringStream<s8> * ptr_ss) {
    caStringStream<s8> & ss = *ptr_ss;
    u32 res = 0;
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
    res = ss.Size();
    return res;
}

u32 caMiniUart::Configure(u32 speed, u32 stop, u32 parity, u32 data) {
    u32 res = deviceError::error_hal_configure;
    if (caMiniUart::Init(speed, stop, parity, data)) {
        caMiniUart::ClearFifos();
        caMiniUart::Enable(0, 0);
        caMiniUart::Enable(1, 1);
        res = deviceError::no_error;
    }
    return res;
}

u32 caMiniUart::EnableInt(void) {
    u32 res = 0;
    if (caMiniUart::EnableIrqRx() != 0 &&
            caIrqCtrl::EnableIrqAux())
        res = 1;
    return res;
}

void caMiniUart::IrqServiceTx(void) {
    muStatReg stat;
    muLsrReg lsr;
    stat.asReg = caMiniUart::GetStat();
    u32 writed, symbol = 7 - stat.asBit.txfifo;
    hal_llc_com1.hll_irq_tx(hal_llc_com1.hll_lnk_obj, txBuff, symbol, writed);
    if (writed) {
        for (txPos = 0; txPos < writed; txPos++) {
            lsr.asReg = caMiniUart::GetLsr();
            if (lsr.asBit.overrun)txOverrun++;
            if (lsr.asBit.txempty == 0)break;
            caMiniUart::SetIO(txBuff[txPos]);
        }
    } else {
        caMiniUart::DisableIrqTx();
    }
}

void caMiniUart::IrqServiceRx(void) {

    muLsrReg lsr;
    muStatReg stat;
    u32 symbol, readed;
    stat.asReg = caMiniUart::GetStat();
    symbol = stat.asBit.rxfifo;
    while (symbol != 0) {
        lsr.asReg = caMiniUart::GetLsr();
        if (lsr.asBit.overrun)rxOverrun++;
        if (lsr.asBit.rxready == 0) break;
        rxBuff[rxPos++] = caMiniUart::GetIO();
        symbol--;
    }
    hal_llc_com1.hll_irq_rx(hal_llc_com1.hll_lnk_obj, rxBuff, rxPos, readed);
    rxPos -= readed;
    if (rxPos) { // ERROR INCOMPLETE COPY : TRY TO REALIGN BUFFER AT NEXT READ
        symbol = 0;
        while (symbol < rxPos) {
            rxBuff[symbol] = rxBuff[symbol + readed];
            symbol++;
        }
    }
}

void caMiniUart::IrqService(void) {
    muIirReg iir;
    iir.asReg = caMiniUart::GetIIR();
    if (iir.asRdBit.pending == 0) {
        register u32 irq = iir.asRdBit.irqid;
        if ((irq & 1) == 1) // TX
        {
            caMiniUart::IrqServiceTx();
        }
        if ((irq & 2) == 2) // RX
        {
            caMiniUart::IrqServiceRx();
        }
    }
}


#endif