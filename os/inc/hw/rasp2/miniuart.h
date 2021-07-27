#ifndef _MINI_UART_HEADER_
#define _MINI_UART_HEADER_

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

#if   HAVE_MINIUART 

class caMiniUart {
private:
    // temp rx buffer for irq rx
    static u8 rxBuff[32];
    // buffer rx index
    static u32 rxPos;
    // temp tx buffer for irq tx
    static u8 txBuff[32];
    // buffer tx index
    static u32 txPos;
    // error tx
    static u32 txOverrun;
    // error rx 
    static u32 rxOverrun;

    static void IrqServiceTx(void);

    static void IrqServiceRx(void);

public:

    static inline u32 Enable(bool rx, bool tx) {
        system_aux_mini_uart(mu);
        mu->ctrl.asReg |= (rx | (2 * tx));
        return (mu->ctrl.asReg & 3);
    }

    static inline u32 EnableIrq(void) {
        system_aux_mini_uart(mu);
        mu->ier.asReg = 0xf;
        return 1;
    }

    static inline u32 EnableIrqRx(void) {
        system_aux_mini_uart(mu);
        mu->ier.asReg |= 0xd;
        return mu->ier.asBit.rxe;
    }

    static inline u32 DisableIrqRx(void) {
        system_aux_mini_uart(mu);
        mu->ier.asReg &= 0xe;
        return mu->ier.asBit.rxe;
    }

    static inline u32 DisableIrqTx(void) {
        system_aux_mini_uart(mu);
        mu->ier.asReg &= 0xd;
        return mu->ier.asBit.rxe;
    }

    static inline u32 EnableIrqTx(void) {
        system_aux_mini_uart(mu);
        mu->ier.asReg |= 0xe;
        return mu->ier.asBit.txe;
    }

    static inline u8 GetIO(void) {
        system_aux_mini_uart(mu);
        return (mu->io.asReg & 0xff);
    }

    static inline void SetIO(u8 c) {
        system_aux_mini_uart(mu);
        mu->io.asReg = c;
    }

    static inline u32 GetIER(void) {
        system_aux_mini_uart(mu);
        return mu->ier.asReg;
    }

    static inline u32 GetIIR(void) {
        system_aux_mini_uart(mu);
        return mu->iir.asReg;
    }

    static inline u32 GetLsr(void) {
        system_aux_mini_uart(mu);
        return mu->lsr.asReg;
    }

    static inline u32 GetStat(void) {
        system_aux_mini_uart(mu);
        return mu->stat.asReg;
    }

    static inline u32 ClearFifos(void) {
        system_aux_mini_uart(mu);
        mu->iir.asReg = 0xC6;
        return 1;
    }

    static inline u32 GetRxFifo(void) {
        system_aux_mini_uart(mu);
        return mu->stat.asBit.rxfifo;
    }

    static inline u32 GetTxFifo(void) {
        system_aux_mini_uart(mu);
        return mu->stat.asBit.txfifo;
    }

    static u32 Recv(void) {
        system_aux_mini_uart(mu);
        while (!mu->lsr.asBit.rxready);
        return mu->io.asReg;
    }

    static void Send(u32 c) {
        system_aux_mini_uart(mu);
        while (!mu->lsr.asBit.txempty){};
        mu->io.asReg = c;
        while (!mu->lsr.asBit.txempty){};        
    }

    static inline void Ready(void) {
        system_aux_mini_uart(mu);
        while (!mu->lsr.asBit.txempty);
    }

    static u32 Dump(caStringStream<s8> * ss);

    static u32 Configure(u32 speed, u32 stop, u32 parity, u32 data);

    static u32 EnableInt(void);

    static void IrqService(void);

    static u32 Stop(void);

    static u32 Init(u32 vel, u32 stop, u32 parity, u32 data);

    static u32 GetErrors(u32 &rxError, u32 & txError);

};

#endif
#endif
