
#ifndef _AUXPERI_HEADER
#define _AUXPERI_HEADER
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

#if (HAVE_MINIUART || HAVE_SPI_1 || HAVE_SPI_2)

typedef union aux_irq_sel {

    struct tag_aux_irq {
        _RW_ u32 miniuart : 1;
        _RW_ u32 spi_m_1 : 1;
        _RW_ u32 spi_m_2 : 1;
    } asBit;
    _RW_ u32 asReg;
} auxIrqSel, *ptrAuxIrqSel;

typedef union aux_en_sel {

    struct tag_en_aux {
        _RW_ u32 miniuart : 1;
        _RW_ u32 spi_m_1 : 1;
        _RW_ u32 spi_m_2 : 1;
    } asBit;
    _RW_ u32 asReg;
} auxEnSel, *ptrAuxEnSel;

typedef struct aux_gen {
    _RW_ volatile auxIrqSel irq;
    _RW_ volatile auxEnSel en;
} AuxMain, * ptrAuxMain;


#define system_aux(a)  const ptrAuxMain  a=(const ptrAuxMain)(PB_BASE+0x215000 )

#endif

#if HAVE_MINIUART

typedef union mu_io_reg {

    struct tag_mu_io_reg {
        _RW_ u8 data;
    } asByteTxRx;

    struct tag_mu_iir_reg_1 {
        _RW_ u8 lsBaud;
    } asByteBaud;
    _RW_ u32 asReg;
} muIoReg, *ptrMuIoReg;

/*
The title should be AUX_MU_IER_REG. The name in the Synopsis is correct 
These are R/W bits not read only 
Bits 3:2 are marked as don't care, but are actually required in order to receive interrupts. 
Bits 1:0 are swaped. bit 0 is receive interrupt and bit 1 is transmit. 
 */
typedef union mu_ier_reg {

    struct tag_mu_ier_reg_0 {
        _RW_ u32 rxe : 1;
        _RW_ u32 txe : 1;
        _RW_ u32 errata1 : 1;
        _RW_ u32 errata2 : 1;
    } asBit;

    struct tag_mu_ier_reg_1 {
        _RW_ u8 msBaud;
    } asByte;
    _RW_ u32 asReg;
} muIerReg, *ptrMuIerReg;

typedef union mu_iir_reg {

    struct tag_mu_iir_reg_0 {
        _R_ u32 pending : 1;
        _R_ u32 irqid : 2;
    } asRdBit;

    struct tag_mu_iir_reg_1 {
        _W_ u32 none : 1;
        _W_ u32 clrrxfifo : 1;
        _W_ u32 clrtxfifo : 1;
    } aswrBit;
    _RW_ u32 asReg;
} muIirReg, *ptrMuIirReg;

//p14   Errata
//LCR register, bit 1 must be set for 8 bit mode, like a 16550 write a 3 to get 8-bit mode 

typedef union mu_lcr_reg {

    struct tag_mu_lcr_reg {
        _R_ u32 size : 1;
        _R_ u32 errata : 1; // must set to 8 bit 
        _R_ u32 dummy : 4;
        _R_ u32 sbreak : 5;
        _R_ u32 dlab : 5;
    } asBit;
    _RW_ u32 asReg;
} muLcrReg, *ptrMuLcrReg;

typedef union mu_mcr_reg {

    struct tag_mu_mcr_reg {
        _R_ u32 dummy : 1;
        _RW_ u32 rts : 1;
    } asBit;
    _RW_ u32 asReg;
} muMcrReg, *ptrMuMcrReg;

typedef union mu_lsr_reg {

    struct tag_mu_lsr_reg {
        _R_ u32 rxready : 1;
        _R_ u32 overrun : 1;
        _R_ u32 dummy : 3;
        _R_ u32 txempty : 1;
        _R_ u32 txidle : 1;
    } asBit;
    _RW_ u32 asReg;
} muLsrReg, *ptrMuLsrReg;

typedef union mu_msr_reg {

    struct tag_mu_msr_reg {
        _R_ u32 dummy : 5;
        _R_ u32 cts : 1;
    } asBit;
    _RW_ u32 asReg;
} muMsrReg, *ptrMMsrReg;

typedef union mu_ctrl_reg {

    struct tag_mu_ctrl_reg {
        _RW_ u32 enablerx : 1;
        _RW_ u32 enabletx : 1;
        _RW_ u32 autorts : 1;
        _RW_ u32 autocts : 1;
        _RW_ u32 rtslevel : 2;
        _RW_ u32 rtsassert : 1;
        _RW_ u32 ctsassert : 1;
    } asBit;
    _RW_ u32 asReg;
} muCtrlReg, *ptrMuCtrlReg;

typedef union mu_stat_reg {

    struct tag_mu_stat_reg {
        _R_ u32 rxsymbol : 1;
        _R_ u32 txspace : 1;
        _R_ u32 rxidle : 1;
        _R_ u32 txidle : 1;
        _R_ u32 rxoverrun : 1;
        _R_ u32 txfull : 1;
        _R_ u32 rts : 1;
        _R_ u32 cts : 1;
        _R_ u32 txempty : 1;
        _R_ u32 txdone : 1;
        _R_ u32 none_0 : 6;
        _R_ u32 rxfifo : 4;
        _R_ u32 none_1 : 4;
        _R_ u32 txfifo : 4;
    } asBit;
    _RW_ u32 asReg;
} muStatReg, *ptrMuStatReg;

typedef struct aux_mini_uart {
    _RW_ volatile muIoReg io;
    _RW_ volatile muIerReg ier;
    _RW_ volatile muIirReg iir;
    _RW_ volatile muLcrReg lcr;
    _RW_ volatile muMcrReg mcr;
    _RW_ volatile muLsrReg lsr;
    _RW_ volatile muMsrReg msr;
    _RW_ volatile u32 scratch;
    _RW_ volatile muCtrlReg ctrl;
    _RW_ volatile muStatReg stat;
    _RW_ volatile u32 baud;
} AuxMiniUart, * ptrAuxMiniUart;

#define system_aux_mini_uart(a)  const ptrAuxMiniUart  a=(const  ptrAuxMiniUart)(PB_BASE+0x215040 )

#endif


#if ( HAVE_SPI_1 || HAVE_SPI_2)

typedef union spi_ctrl_0_reg {

    struct tag_spi_ctrl_0_reg {
        _RW_ u32 lenght : 6;
        _RW_ u32 msfirst : 1;
        _RW_ u32 invertclk : 1;
        _RW_ u32 risingout : 1;
        _RW_ u32 clearfifo : 1;
        _RW_ u32 risingin : 1;
        _RW_ u32 enable : 1;
        _RW_ u32 holdtime : 2;
        _RW_ u32 varwidth : 1;
        _RW_ u32 varcs : 1;
        _RW_ u32 postimputmode : 1;
        _RW_ u32 chipselect : 3;
        _RW_ u32 speed : 12;
    } asBit;
    _RW_ u32 asReg;
} spiCtrl0Reg, *ptrSpiCtrl0Reg;

typedef union spi_ctrl_1_reg {

    struct tag_spi_ctrl_1_reg {
        _RW_ u32 keepinput : 1;
        _RW_ u32 msfirst : 1;
        _RW_ u32 dummy : 4;
        _RW_ u32 doneirq : 1;
        _RW_ u32 txemptyirq : 1;
        _RW_ u32 cshtime : 3;
    } asBit;
    _RW_ u32 asReg;
} spiCtrl1Reg, *ptrSpiCtrl1Reg;

typedef union spi_stat_reg {

    struct tag_spi_stat_reg {
        _RW_ u32 bcount : 6;
        _RW_ u32 busy : 1;
        _RW_ u32 rxempty : 1;
        _RW_ u32 txempty : 1;
        _RW_ u32 txfull : 1;
        _RW_ u32 reserver : 6;
        _RW_ u32 rxfifocount : 8;
        _RW_ u32 txfifocount : 8;
    } asBit;
    _RW_ u32 asReg;
} spiStatReg, *ptrSpiStatReg;

typedef union spi_peek_reg {

    struct tag_spi_peek_reg {
        _RW_ u32 data : 16;
    } asBit;
    _RW_ u32 asReg;
} spiPeekReg, *ptrSpiPeekReg;

typedef union spi_io_reg {

    struct tag_spi_io_reg {
        _RW_ u32 data : 16;
    } asBit;
    _RW_ u32 asReg;
} spiIoReg, *ptrSpiIoReg;

typedef union spi_txhold_reg {

    struct tag_spi_txhold_reg {
        _RW_ u32 data : 16;
    } asBit;
    _RW_ u32 asReg;
} spiTxHoldReg, *ptrSpiTxHoldReg;

typedef struct aux_spi {
    _RW_ volatile spiCtrl0Reg ctrl0;
    _RW_ volatile spiCtrl1Reg ctrl1;
    _RW_ volatile spiStatReg stat;
    _RW_ volatile spiPeekReg peek;
    _RW_ volatile spiIoReg io;
    _RW_ volatile spiTxHoldReg txhold;
} AuxSpi, * ptrAuxSpi;



#endif





#if  HAVE_SPI_1
#define system_aux_spi_1(a)  const ptrAuxSpi  a=(const  ptrAuxSpi)(PB_BASE+0x215080 )
#endif

#if  HAVE_SPI_2
#define system_aux_spi_2(a)  const ptrAuxSpi  a=(const  ptrAuxSpi)(PB_BASE+0x2150C0 )
#endif




#endif /* AUXPERI_H */

