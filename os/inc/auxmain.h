#ifndef AUXMAIN_H
#define AUXMAIN_H


#include "bcm2836.h"

#if (HAVE_MINIUART || HAVE_SPI_1 || HAVE_SPI_2)

class caAuxMain {
public:

    static inline u32 EnableMiniUart(void) {
        system_aux(aux);
        auxEnSel ctrl;
        ctrl.asReg = aux->en.asReg;
        if (!ctrl.asBit.miniuart) {
            ctrl.asBit.miniuart = 1;
            aux->en.asReg = ctrl.asReg;
            return TRUE;
        }
        return FALSE;
    }

    static inline u32 DisableMiniUart(void) {
        system_aux(aux);
        auxEnSel ctrl;
        ctrl.asReg = aux->en.asReg;
        if (ctrl.asBit.miniuart) {
            ctrl.asBit.miniuart = 0;
            aux->en.asReg = ctrl.asReg;
            return TRUE;
        }
        return FALSE;
    }

    static inline u32 EnableSpi1(void) {
        system_aux(aux);
        auxEnSel ctrl;
        ctrl.asReg = aux->en.asReg;
        if (!ctrl.asBit.spi_m_1) {
            ctrl.asBit.spi_m_1 = 1;
            aux->en.asReg = ctrl.asReg;
            return TRUE;
        }
        return FALSE;
    }

    static inline u32 DisableSpi1(void) {
        system_aux(aux);
        auxEnSel ctrl;
        ctrl.asReg = aux->en.asReg;
        if (ctrl.asBit.spi_m_1) {
            ctrl.asBit.spi_m_1 = 0;
            aux->en.asReg = ctrl.asReg;
            return TRUE;
        }
        return FALSE;
    }

    static inline u32 EnableSpi2(void) {
        system_aux(aux);
        auxEnSel ctrl;
        ctrl.asReg = aux->en.asReg;
        if (!ctrl.asBit.spi_m_2) {
            ctrl.asBit.spi_m_2 = 1;
            aux->en.asReg = ctrl.asReg;
            return TRUE;
        }
        return FALSE;

    }

    static inline u32 DisableSpi2(void) {
        system_aux(aux);
        auxEnSel ctrl;
        ctrl.asReg = aux->en.asReg;
        if (ctrl.asBit.spi_m_2) {
            ctrl.asBit.spi_m_2 = 0;
            aux->en.asReg = ctrl.asReg;
            return TRUE;
        }
        return FALSE;
    }

    static inline u32 GetIrqPending(void) {
        system_aux(aux);
        return aux->irq.asReg;
    }

    static inline u32 CheckMiniUartIrq(void) {
        system_aux(aux);
        return aux->irq.asBit.miniuart;
    }

    static inline u32 CheckSpi1Irq(void) {
        system_aux(aux);
        return aux->irq.asBit.spi_m_1;
    }

    static inline u32 CheckSpi2Irq(void) {
        system_aux(aux);
        return aux->irq.asBit.spi_m_2;
    }
};

#endif
#endif /* AUXMAIN_H */

