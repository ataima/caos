#ifndef _BCM2836_H_
#define _BCM2836_H_
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

#include "armv7.h"


#define REG(x) (*(volatile u32 *)(x))
#define BIT(n) (1 << (n))
#define BLOCK(n) (7 << (n))
#define FNBLOCK(fn,n) (fn << (n))
#define BCM2836_CLOCK_FREQ 250000000
#define PB_BASE 0x3f000000

// GPIO STRUCT
#include "bcm2835gpio.h"
// AP804 TIMER STRUCT 
#include "ap804timer.h"
// AUX STRUCT
#include "bcm2835aux.h"
// IRQ STRUCT
#include "bcm2835irq.h"
// CPU SPECIFIC 
#include "cortexv7.h"
// *****************************************************************************
//                        System Timer
// *****************************************************************************

#define SYSTIMER_CS         REG(PB_BASE+0x003000)
#define SYSTIMER_CLO        REG(PB_BASE+0x003004)
#define SYSTIMER_CHI        REG(PB_BASE+0x003008)

#define SYSTIMER_CMP0       REG(PB_BASE+0x00300C)
#define SYSTIMER_CMP1       REG(PB_BASE+0x003010)
#define SYSTIMER_CMP2       REG(PB_BASE+0x003014)
#define SYSTIMER_CMP3       REG(PB_BASE+0x003018)

#define SYSTIMER_CS_MATCH0  0x01
#define SYSTIMER_CS_MATCH1  0x02
#define SYSTIMER_CS_MATCH2  0x04
#define SYSTIMER_CS_MATCH3  0x08

#define SYSTIMER_IRQEN0     0x01
#define SYSTIMER_IRQEN1     0x02
#define SYSTIMER_IRQEN2     0x04
#define SYSTIMER_IRQEN3     0x08

#define SYSTIMER_CLOCK_FREQ 1000000

// *****************************************************************************
//                        Interrupts
// *****************************************************************************

#define IRQ_BASIC         REG(PB_BASE+0x00B200)
#define IRQ_PEND1         REG(PB_BASE+0x00B204)
#define IRQ_PEND2         REG(PB_BASE+0x00B208)
#define IRQ_FIQ_CONTROL   REG(PB_BASE+0x00B20C)
#define IRQ_ENABLE1       REG(PB_BASE+0x00B210)
#define IRQ_ENABLE2       REG(PB_BASE+0x00B214)
#define IRQ_ENABLE_BASIC  REG(PB_BASE+0x00B218)
#define IRQ_DISABLE1      REG(PB_BASE+0x00B21C)
#define IRQ_DISABLE2      REG(PB_BASE+0x00B220)
#define IRQ_DISABLE_BASIC REG(PB_BASE+0x00B224)

#define SPI_IRQ           BIT(22)

// *****************************************************************************
//                 Broadcom Serial Controllers (BSC/I2C)
// *****************************************************************************

struct bscdevice_t {
    volatile unsigned int control;
    volatile unsigned int status;
    volatile unsigned int dataLength;
    volatile unsigned int slaveAddress;
    volatile unsigned int dataFifo;
    volatile unsigned int clockDivider;
    volatile unsigned int dataDelay;
    volatile unsigned int clockStretchTimeout;
};

typedef struct bscdevice_t bscdevice_t;

/* Only BSC0 is accessible from the RPi pi header.*/
#define BSC0_ADDR ((bscdevice_t *)PB_BASE+0x205000)
#define BSC1_ADDR ((bscdevice_t *)PB_BASE+0x804000)
#define BSC2_ADDR ((bscdevice_t *)PB_BASE+0x805000)

#define BSC_CLOCK_FREQ 150000000

/* I2C control flags */
#define BSC_I2CEN BIT(15)
#define BSC_INTR  BIT(10)
#define BSC_INTT  BIT(9)
#define BSC_INTD  BIT(8)
#define BSC_ST    BIT(7)
#define BSC_CLEAR BIT(4)
#define BSC_READ  BIT(0)

/* I2C status flags */
#define BSC_TA   BIT(0) /** @brief Transfer active.*/
#define BSC_DONE BIT(1) /** @brief Transfer done.*/
#define BSC_TXW  BIT(2) /** @brief FIFO needs writing.*/
#define BSC_RXR  BIT(3) /** @brief FIFO needs reading.*/
#define BSC_TXD  BIT(4) /** @brief FIFO can accept data.*/
#define BSC_RXD  BIT(5) /** @brief FIFO contains data.*/
#define BSC_TXE  BIT(6) /** @brief FIFO empty.*/
#define BSC_RXF  BIT(7) /** @brief FIFO full.*/
#define BSC_ERR  BIT(8) /** @brief ACK error.*/
#define BSC_CLKT BIT(9) /** @brief Clock stretch timeout.*/

/* Rising/Falling Edge Delay Defaults.*/
#define BSC_DEFAULT_FEDL       0x30
#define BSC_DEFAULT_REDL       0x30

/* Clock Stretch Timeout Defaults.*/
#define BSC_DEFAULT_CLKT       0x40

#define CLEAR_STATUS  BSC_CLKT|BSC_ERR|BSC_DONE

#define START_READ    BSC_I2CEN|BSC_ST|BSC_CLEAR|BSC_READ
#define START_WRITE   BSC_I2CEN|BSC_ST

// *****************************************************************************
//                  Serial Peripheral Interface (SPI)
// *****************************************************************************

/// See 10.5 SPI Register Map
#define SPI0_CS        REG(PB_BASE+0x204000) /* @brief SPI Master Control and Status.*/
#define SPI0_FIFO      REG(PB_BASE+0x204004) /* @brief SPI Master TX and RX FIFOs.*/
#define SPI0_CLK       REG(PB_BASE+0x204008) /* @brief SPI Master Clock Divider.*/
#define SPI0_DLEN      REG(PB_BASE+0x20400C) /* @brief SPI Master Data Length.*/
#define SPI0_LTOH      REG(PB_BASE+0x204010) /* @brief SPI LOSSI mode TOH.*/
#define SPI0_DC        REG(PB_BASE+0x204014) /* @brief SPI DMA DREQ Controls.*/

// Register masks for SPI0_CS
#define SPI_CS_LEN_LONG             0x02000000 /* @brief Enable Long data word in Lossi mode if DMA_LEN is set.*/
#define SPI_CS_DMA_LEN              0x01000000 /* @brief Enable DMA mode in Lossi mode.*/
#define SPI_CS_CSPOL2               0x00800000 /* @brief Chip Select 2 Polarity.*/
#define SPI_CS_CSPOL1               0x00400000 /* @brief Chip Select 1 Polarity.*/
#define SPI_CS_CSPOL0               0x00200000 /* @brief Chip Select 0 Polarity.*/
#define SPI_CS_RXF                  0x00100000 /* @brief RXF - RX FIFO Full.*/
#define SPI_CS_RXR                  0x00080000 /* @brief RXR RX FIFO needs Reading ( full).*/
#define SPI_CS_TXD                  0x00040000 /* @brief TXD TX FIFO can accept Data.*/
#define SPI_CS_RXD                  0x00020000 /* @brief RXD RX FIFO contains Data.*/
#define SPI_CS_DONE                 0x00010000 /* @brief Done transfer Done.*/
#define SPI_CS_TE_EN                0x00008000 /* @brief Unused.*/
#define SPI_CS_LMONO                0x00004000 /* @brief Unused.*/
#define SPI_CS_LEN                  0x00002000 /* @brief LEN LoSSI enable.*/
#define SPI_CS_REN                  0x00001000 /* @brief REN Read Enable.*/
#define SPI_CS_ADCS                 0x00000800 /* @brief ADCS Automatically Deassert Chip Select.*/
#define SPI_CS_INTR                 0x00000400 /* @brief INTR Interrupt on RXR.*/
#define SPI_CS_INTD                 0x00000200 /* @brief INTD Interrupt on Done.*/
#define SPI_CS_DMAEN                0x00000100 /* @brief DMAEN DMA Enable.*/
#define SPI_CS_TA                   0x00000080 /* @brief Transfer Active.*/
#define SPI_CS_CSPOL                0x00000040 /* @brief Chip Select Polarity.*/
#define SPI_CS_CLEAR                0x00000030 /* @brief Clear FIFO Clear RX and TX.*/
#define SPI_CS_CLEAR_RX             0x00000020 /* @brief Clear FIFO Clear RX .*/
#define SPI_CS_CLEAR_TX             0x00000010 /* @brief Clear FIFO Clear TX .*/
#define SPI_CS_CPOL                 0x00000008 /* @brief Clock Polarity.*/
#define SPI_CS_CPHA                 0x00000004 /* @brief Clock Phase.*/
#define SPI_CS_CS                   0x00000003 /* @brief Chip Select.*/

// *****************************************************************************
//                  Pulse Width Modulation (PWM)
// *****************************************************************************

#define PWM_CTL        REG(PB_BASE+0x20C000)
#define PWM_STATUS     REG(PB_BASE+0x20C004)

#define PWM0_RANGE     REG(PB_BASE+0x20C010)
#define PWM0_DATA      REG(PB_BASE+0x20C014)

#define PWM1_RANGE     REG(PB_BASE+0x20C020)
#define PWM1_DATA      REG(PB_BASE+0x20C024)

#define PWM0_ENABLE    BIT(0)
#define PWM0_MODE_MS   BIT(7)

#define PWM1_ENABLE    BIT(8)
#define PWM1_MODE_MS   BIT(15)

#define PWM_MODE_MS    0xFF

#define GPIO_CLK_PWD   0x5a000000

#define GPIO0_CLK_CTL  REG(PB_BASE+0x1010A0)
#define GPIO0_CLK_DIV  REG(PB_BASE+0x1010A4)

// *****************************************************************************
//       Power Management, Reset controller and Watchdog registers 
// *****************************************************************************

#define PM_BASE                  (PB_BASE+0x100000) 
#define PM_RSTC                  REG(PM_BASE+0x1c)
#define PM_WDOG                  REG(PM_BASE+0x24)

#define PM_WDOG_RESET            0000000000
#define PM_PASSWORD              0x5a000000
#define PM_WDOG_TIME_SET         0x000fffff
#define PM_RSTC_WRCFG_CLR        0xffffffcf
#define PM_RSTC_WRCFG_SET        0x00000030
#define PM_RSTC_WRCFG_FULL_RESET 0x00000020
#define PM_RSTC_RESET            0x00000102

#include "kdebug.h"

#endif
