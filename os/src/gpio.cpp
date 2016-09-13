#include "config.h"
#include "bcm2836.h"
#include "idevice.h"
#include "stream.h"
#include "systimer.h"
#include "miniuart.h"
#include "interrupt.h"
#include "gpio.h"

u32 caGpio::Set(u32 pin) {
    if (pin > MAX_GPIO_PIN)return FALSE;
    u32 bank = pin / 32;
    while (pin > 32)pin -= 32;
    system_gpio_control(gpio);
    gpio->Set[bank].asReg = BIT(pin);
    return TRUE;
}

u32 caGpio::Set(u32 blk, u32 mask) {
    if (blk > MAX_GPIO_BLK)return FALSE;
    system_gpio_control(gpio);
    gpio->Set[blk].asReg = mask;
    return TRUE;
}

u32 caGpio::Clr(u32 pin) {
    if (pin > MAX_GPIO_PIN)return FALSE;
    u32 bank = pin / 32;
    while (pin > 32)pin -= 32;
    system_gpio_control(gpio);
    gpio->Clear[bank].asReg = BIT(pin);
    return TRUE;
}

u32 caGpio::Clr(u32 blk, u32 mask) {
    if (blk > 1)return FALSE;
    system_gpio_control(gpio);
    gpio->Clear[blk].asReg = mask;
    return TRUE;
}

u32 caGpio::Get(u32 pin) {
    if (pin > MAX_GPIO_PIN)return FALSE;
    system_gpio_control(gpio);
    register u32 bank = pin / 32;
    while (pin > 32)pin -= 32;
    register u32 mask = BIT(pin);
    return ( (gpio->Level[bank].asReg & mask) != 0);
}

u32 caGpio::Get(u32 blk, u32 mask) {
    if (blk > 1)return FALSE;
    system_gpio_control(gpio);
    return (gpio->Level[blk].asReg & mask);
}

u32 caGpio::SetReg(u32 pin, volatile gpioFnReg dest[2]) {
    if (pin > MAX_GPIO_PIN)return FALSE;
    u32 bank = pin / 32;
    while (pin > 32)pin -= 32;
    u32 mask = BIT(pin);
    if ((dest[bank].asReg & mask) == 0) {
        dest[bank].asReg |= mask;
        return ((dest[bank].asReg & mask) != 0);
    }
    return TRUE;
}

u32 caGpio::SetReg(u32 blk, u32 mask, volatile gpioFnReg dest[2]) {
    if (blk > MAX_GPIO_BLK)return FALSE;
    if ((dest[blk].asReg & mask) != mask) {
        dest[blk].asReg |= mask;
        return ((dest[blk].asReg & mask) == mask);
    }
    return TRUE;
}

u32 caGpio::ClearReg(u32 pin, volatile gpioFnReg dest[2]) {
    if (pin > MAX_GPIO_PIN)return FALSE;
    u32 bank = pin / 32;
    while (pin > 32)pin -= 32;
    u32 mask = BIT(pin);
    if ((dest[bank].asReg & mask) != 0) {
        dest[bank].asReg &= ~(mask);
        return ((dest[bank].asReg & mask) != 0);
    }
    return TRUE;
}

u32 caGpio::ClearReg(u32 blk, u32 mask, volatile gpioFnReg dest[2]) {
    if (blk > MAX_GPIO_BLK)return FALSE;
    if ((dest[blk].asReg & mask) != 0) {
        dest[blk].asReg &= ~(mask);
        return (dest[blk].asReg & mask) == 0;
    }
    return TRUE;
}

u32 caGpio::GetReg(u32 pin, volatile gpioFnReg dest[2]) {
    if (pin > MAX_GPIO_PIN)return FALSE;
    u32 bank = pin / 32;
    while (pin > 32)pin -= 32;
    u32 mask = BIT(pin);
    return ((dest[bank].asReg & mask) != 0);
}

u32 caGpio::GetReg(u32 blk, u32 mask, volatile gpioFnReg dest[2]) {
    if (blk > MAX_GPIO_BLK)return FALSE;
    return (dest[blk].asReg & mask);
}

void caGpio::Delay(u32 n) {

    struct dummy {

        dummy(u32 v) {
            int a = v;
            int b = a;
            v = b;
        }
    };
    volatile u32 i;
    for (i = 0; i < n; i++) {
        struct dummy a(i);
    }

}

u32 caGpio::SetFunction(u32 pin, u32 fn) {
    if (pin > MAX_GPIO_PIN)return FALSE;
    if (fn != GetFn(pin)) {
        system_gpio_control(gpio);
        register u32 bank = pin / 10;
        register u32 off = (pin - (bank * 10))*3;
        register u32 mask = BLOCK(off);
        register u32 fnmask = FNBLOCK(fn, off);
        gpio->Sel[bank].asReg &= ~(mask);
        gpio->Sel[bank].asReg |= fnmask;
        return (fn == GetFn(pin));
    }
    return TRUE;
}

u32 caGpio::GetFn(u32 pin) {
    if (pin > MAX_GPIO_PIN)return 0;
    system_gpio_control(gpio);
    register u32 bank = pin / 10;
    register u32 off = (pin - (bank * 10))*3;
    register u32 mask = BLOCK(off);
    register u32 st = (gpio->Sel[bank].asReg & mask);
    st >>= off;
    return st;
}

u32 caGpio::SetPullUp(u32 pin) {
    if (pin > MAX_GPIO_PIN)return FALSE;
    system_gpio_control(gpio);
    gpio->PullResistor = 2;
    Delay(20);
    SetReg(pin, gpio->PullClock);
    Delay(20);
    ClearReg(pin, gpio->PullClock);
    return TRUE;
}

u32 caGpio::SetPullUp(u32 blk, u32 mask) {
    if (blk > MAX_GPIO_BLK)return FALSE;
    system_gpio_control(gpio);
    gpio->PullResistor = 2;
    Delay(20);
    SetReg(blk, mask, gpio->PullClock);
    Delay(20);
    ClearReg(blk, mask, gpio->PullClock);
    return TRUE;
}

u32 caGpio::SetPullDown(u32 pin) {
    if (pin > MAX_GPIO_PIN)return FALSE;
    system_gpio_control(gpio);
    gpio->PullResistor = 1;
    Delay(20);
    SetReg(pin, gpio->PullClock);
    Delay(20);
    ClearReg(pin, gpio->PullClock);
    return TRUE;
}

u32 caGpio::SetPullDown(u32 blk, u32 mask) {
    if (blk > MAX_GPIO_BLK)return FALSE;
    system_gpio_control(gpio);
    gpio->PullResistor = 1;
    Delay(20);
    SetReg(blk, mask, gpio->PullClock);
    Delay(20);
    ClearReg(blk, mask, gpio->PullClock);
    return TRUE;
}

u32 caGpio::RemovePullDown(u32 pin) {
    if (pin > MAX_GPIO_PIN)return FALSE;
    system_gpio_control(gpio);
    gpio->PullResistor = 0;
    Delay(20);
    SetReg(pin, gpio->PullClock);
    Delay(20);
    ClearReg(pin, gpio->PullClock);
    return TRUE;
}

u32 caGpio::RemovePullDown(u32 blk, u32 mask) {
    if (blk > MAX_GPIO_BLK)return FALSE;
    system_gpio_control(gpio);
    gpio->PullResistor = 0;
    Delay(20);
    SetReg(blk, mask, gpio->PullClock);
    Delay(20);
    ClearReg(blk, mask, gpio->PullClock);
    return TRUE;
}

#if HAVE_DUMP_OBJ

void caGpio::Dump(void) {
    system_gpio_control(gpio);
    kPut("bcm 2835/6 GPIOs :\r\n");
    kPut("SEL0", (void*) &gpio->Sel[0], gpio->Sel[0].asReg);
    kPut("SEL1", (void*) &gpio->Sel[1], gpio->Sel[1].asReg);
    kPut("SEL2", (void*) &gpio->Sel[2], gpio->Sel[2].asReg);
    kPut("SEL3", (void*) &gpio->Sel[3], gpio->Sel[3].asReg);
    kPut("SEL4", (void*) &gpio->Sel[4], gpio->Sel[4].asReg);
    kPut("SEL5", (void*) &gpio->Sel[5], gpio->Sel[5].asReg);
    kPut("SET0", (void*) &gpio->Set[0], gpio->Set[0].asReg);
    kPut("SET1", (void*) &gpio->Set[1], gpio->Set[1].asReg);
    kPut("CLR0", (void*) &gpio->Clear[0], gpio->Clear[0].asReg);
    kPut("CLR1", (void*) &gpio->Clear[1], gpio->Clear[1].asReg);
    kPut("LEV0", (void*) &gpio->Level[0], gpio->Level[0].asReg);
    kPut("LEV1", (void*) &gpio->Level[1], gpio->Level[1].asReg);
    kPut("EDS0", (void*) &gpio->Event[0], gpio->Event[0].asReg);
    kPut("EDS1", (void*) &gpio->Event[1], gpio->Event[1].asReg);
    kPut("REN0", (void*) &gpio->RisingEdge[0], gpio->RisingEdge[0].asReg);
    kPut("REN1", (void*) &gpio->RisingEdge[1], gpio->RisingEdge[1].asReg);
    kPut("FEN0", (void*) &gpio->FallingEdge[0], gpio->FallingEdge[0].asReg);
    kPut("FEN1", (void*) &gpio->FallingEdge[1], gpio->FallingEdge[1].asReg);
    kPut("HIG0", (void*) &gpio->HighDetect[0], gpio->HighDetect[0].asReg);
    kPut("HIG1", (void*) &gpio->HighDetect[1], gpio->HighDetect[1].asReg);
    kPut("LOW0", (void*) &gpio->LowDetect[0], gpio->LowDetect[0].asReg);
    kPut("LOW1", (void*) &gpio->LowDetect[1], gpio->LowDetect[1].asReg);
    kPut("ARE0", (void*) &gpio->AsyncRisingEdge[0], gpio->AsyncRisingEdge[0].asReg);
    kPut("ARE1", (void*) &gpio->AsyncRisingEdge[1], gpio->AsyncRisingEdge[1].asReg);
    kPut("AFE0", (void*) &gpio->AsyncFallingEdge[0], gpio->AsyncFallingEdge[0].asReg);
    kPut("AFE1", (void*) &gpio->AsyncFallingEdge[1], gpio->AsyncFallingEdge[1].asReg);
    kPut("PULL", (void*) &gpio->PullResistor, gpio->PullResistor);
    kPut("CLK0", (void*) &gpio->PullClock[0], gpio->PullClock[0].asReg);
    kPut("CLK1", (void*) &gpio->PullClock[1], gpio->PullClock[1].asReg);
    kPut("\r\n");
}

void caGpio::DumpFn(u32 pin) {
    if (pin > MAX_GPIO_PIN)return;
    u32 st = GetFn(pin);
    switch (st) {
        case 0:
            kPut("GPIO[");
            caMiniUart::Dec(pin);
            kPut("] as Input\r\n");
            break;
        case 1:
            kPut("GPIO[");
            caMiniUart::Dec(pin);
            kPut("] as Output\r\n");
            break;
        case 2:
            kPut("GPIO[");
            caMiniUart::Dec(pin);
            kPut("] as Alt5\r\n");
            break;
        case 3:
            kPut("GPIO[");
            caMiniUart::Dec(pin);
            kPut("] as Alt4\r\n");
            break;
        case 4:
            kPut("GPIO[");
            caMiniUart::Dec(pin);
            kPut("] as Alt0\r\n");
            break;
        case 5:
            kPut("GPIO[");
            caMiniUart::Dec(pin);
            kPut("] as Alt1\r\n");
            break;
        case 6:
            kPut("GPIO[");
            caMiniUart::Dec(pin);
            kPut("] as Alt2\r\n");
            break;
        case 7:
            kPut("GPIO[");
            caMiniUart::Dec(pin);
            kPut("] as Alt3\r\n");
            break;
    }
}

#endif
