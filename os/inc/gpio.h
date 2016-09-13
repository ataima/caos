#ifndef _GPIO_HEADER_
#define _GPIO_HEADER_

#include "bcm2836.h"

class caGpio {
public:

public:
    static u32 Set(u32 pin);
    static u32 Clr(u32 pin);
    static u32 Get(u32 pin);

    static u32 Set(u32 blk, u32 mask);
    static u32 Clr(u32 blk, u32 mask);
    static u32 Get(u32 blk, u32 mask);


    static void Delay(u32 t);

    //new functions

    static inline u32 SetOut(u32 pin) {
        return SetFunction(pin, fnOut);
    }

    static inline u32 SetIn(u32 pin) {
        return SetFunction(pin, fnInp);
    }

    static inline u32 SetAlt0(u32 pin) {
        return SetFunction(pin, fnAlt0);
    }

    static inline u32 SetAlt1(u32 pin) {
        return SetFunction(pin, fnAlt1);
    }

    static inline u32 SetAlt2(u32 pin) {
        return SetFunction(pin, fnAlt2);
    }

    static inline u32 SetAlt3(u32 pin) {
        return SetFunction(pin, fnAlt3);
    }

    static inline u32 SetAlt4(u32 pin) {
        return SetFunction(pin, fnAlt4);
    }

    static inline u32 SetAlt5(u32 pin) {
        return SetFunction(pin, fnAlt5);
    }
    static u32 GetFn(u32 pin);

    static inline u32 SetEvent(u32 pin) {
        system_gpio_control(gpio);
        return SetReg(pin, gpio->Event);
    }

    static inline u32 SetEvent(u32 blk, u32 mask) {
        system_gpio_control(gpio);
        return SetReg(blk, mask, gpio->Event);
    }

    static inline u32 ClearEvent(u32 pin) {
        system_gpio_control(gpio);
        return ClearReg(pin, gpio->Event);
    }

    static inline u32 ClearEvent(u32 blk, u32 mask) {
        system_gpio_control(gpio);
        return ClearReg(blk, mask, gpio->Event);
    }

    static inline u32 GetEvent(u32 pin) {
        system_gpio_control(gpio);
        return GetReg(pin, gpio->Event);
    }

    static inline u32 GetEvent(u32 blk, u32 mask) {
        system_gpio_control(gpio);
        return GetReg(blk, mask, gpio->Event);
    }

    static inline u32 SetDetectRisingEdge(u32 pin) {
        system_gpio_control(gpio);
        return SetReg(pin, gpio->RisingEdge);
    }

    static inline u32 SetDetectRisingEdge(u32 blk, u32 mask) {
        system_gpio_control(gpio);
        return SetReg(blk, mask, gpio->RisingEdge);
    }

    static inline u32 ClearDetectRisingEdge(u32 pin) {
        system_gpio_control(gpio);
        return ClearReg(pin, gpio->RisingEdge);
    }

    static inline u32 ClearDetectRisingEdge(u32 blk, u32 mask) {
        system_gpio_control(gpio);
        return ClearReg(blk, mask, gpio->RisingEdge);
    }

    static inline u32 GetDetectRisingEdge(u32 pin) {
        system_gpio_control(gpio);
        return GetReg(pin, gpio->RisingEdge);
    }

    static inline u32 GetDetectRisingEdge(u32 blk, u32 mask) {
        system_gpio_control(gpio);
        return GetReg(blk, mask, gpio->RisingEdge);
    }

    static inline u32 SetDetectFallingEdge(u32 pin) {
        system_gpio_control(gpio);
        return SetReg(pin, gpio->FallingEdge);
    }

    static inline u32 SetDetectFallingEdge(u32 blk, u32 mask) {
        system_gpio_control(gpio);
        return SetReg(blk, mask, gpio->FallingEdge);
    }

    static inline u32 ClearDetectFallingEdge(u32 pin) {
        system_gpio_control(gpio);
        return ClearReg(pin, gpio->FallingEdge);
    }

    static inline u32 ClearDetectFallingEdge(u32 blk, u32 mask) {
        system_gpio_control(gpio);
        return ClearReg(blk, mask, gpio->FallingEdge);
    }

    static inline u32 GetDetectFallingEdge(u32 pin) {
        system_gpio_control(gpio);
        return GetReg(pin, gpio->FallingEdge);
    }

    static inline u32 GetDetectFallingEdge(u32 blk, u32 mask) {
        system_gpio_control(gpio);
        return GetReg(blk, mask, gpio->FallingEdge);
    }

    static inline u32 SetDetectHigh(u32 pin) {
        system_gpio_control(gpio);
        return SetReg(pin, gpio->HighDetect);
    }

    static inline u32 SetDetectHigh(u32 blk, u32 mask) {
        system_gpio_control(gpio);
        return SetReg(blk, mask, gpio->HighDetect);
    }

    static inline u32 ClearDetectHigh(u32 pin) {
        system_gpio_control(gpio);
        return ClearReg(pin, gpio->HighDetect);
    }

    static inline u32 ClearDetectHigh(u32 blk, u32 mask) {
        system_gpio_control(gpio);
        return ClearReg(blk, mask, gpio->HighDetect);
    }

    static inline u32 GetDetectHigh(u32 pin) {
        system_gpio_control(gpio);
        return GetReg(pin, gpio->HighDetect);
    }

    static inline u32 GetDetectHigh(u32 blk, u32 mask) {
        system_gpio_control(gpio);
        return GetReg(blk, mask, gpio->HighDetect);
    }

    static inline u32 SetDetectLow(u32 pin) {
        system_gpio_control(gpio);
        return SetReg(pin, gpio->LowDetect);
    }

    static inline u32 SetDetectLow(u32 blk, u32 mask) {
        system_gpio_control(gpio);
        return SetReg(blk, mask, gpio->LowDetect);
    }

    static inline u32 ClearDetectLow(u32 pin) {
        system_gpio_control(gpio);
        return ClearReg(pin, gpio->LowDetect);
    }

    static inline u32 ClearDetectLow(u32 blk, u32 mask) {
        system_gpio_control(gpio);
        return ClearReg(blk, mask, gpio->LowDetect);
    }

    static inline u32 GetDetectLow(u32 pin) {
        system_gpio_control(gpio);
        return GetReg(pin, gpio->LowDetect);
    }

    static inline u32 GetDetectLow(u32 blk, u32 mask) {
        system_gpio_control(gpio);
        return GetReg(blk, mask, gpio->LowDetect);
    }

    static inline u32 SetDetectAsyncRisingEdge(u32 pin) {
        system_gpio_control(gpio);
        return SetReg(pin, gpio->AsyncRisingEdge);
    }

    static inline u32 SetDetectAsyncRisingEdge(u32 blk, u32 mask) {
        system_gpio_control(gpio);
        return SetReg(blk, mask, gpio->AsyncRisingEdge);
    }

    static inline u32 ClearDetectAsyncRisingEdge(u32 pin) {
        system_gpio_control(gpio);
        return ClearReg(pin, gpio->AsyncRisingEdge);
    }

    static inline u32 ClearDetectAsyncRisingEdge(u32 blk, u32 mask) {
        system_gpio_control(gpio);
        return ClearReg(blk, mask, gpio->AsyncRisingEdge);
    }

    static inline u32 GetDetectAsyncRisingEdge(u32 pin) {
        system_gpio_control(gpio);
        return GetReg(pin, gpio->AsyncRisingEdge);
    }

    static inline u32 GetDetectAsyncRisingEdge(u32 blk, u32 mask) {
        system_gpio_control(gpio);
        return GetReg(blk, mask, gpio->AsyncRisingEdge);
    }

    static inline u32 SetDetectAsyncFallingEdge(u32 pin) {
        system_gpio_control(gpio);
        return SetReg(pin, gpio->AsyncFallingEdge);
    }

    static inline u32 SetDetectAsyncFallingEdge(u32 blk, u32 mask) {
        system_gpio_control(gpio);
        return SetReg(blk, mask, gpio->AsyncFallingEdge);
    }

    static inline u32 ClearDetectAsyncFallingEdge(u32 pin) {
        system_gpio_control(gpio);
        return ClearReg(pin, gpio->AsyncFallingEdge);
    }

    static inline u32 ClearDetectAsyncFallingEdge(u32 blk, u32 mask) {
        system_gpio_control(gpio);
        return ClearReg(blk, mask, gpio->AsyncFallingEdge);
    }

    static inline u32 GetDetectAsyncFallingEdge(u32 pin) {
        system_gpio_control(gpio);
        return GetReg(pin, gpio->AsyncFallingEdge);
    }

    static inline u32 GetDetectAsyncFallingEdge(u32 blk, u32 mask) {
        system_gpio_control(gpio);
        return GetReg(blk, mask, gpio->AsyncFallingEdge);
    }

    static u32 SetPullUp(u32 pin);
    static u32 SetPullUp(u32 blk, u32 mask);

    static u32 SetPullDown(u32 pin);
    static u32 SetPullDown(u32 blk, u32 mask);

    static u32 RemovePullDown(u32 pin);
    static u32 RemovePullDown(u32 blk, u32 mask);

#if HAVE_DUMP_OBJ    
    static void Dump(void);
    static void DumpFn(u32 pin);
#else

    static inline void Dump(void) {
    };

    static inline void DumpFn(u32) {
    }
#endif    
private:
    static u32 SetFunction(u32 pin, u32 fn);
    static u32 SetReg(u32 pin, volatile gpioFnReg dest[2]);
    static u32 SetReg(u32 blk, u32 mask, volatile gpioFnReg dest[2]);
    static u32 ClearReg(u32 pin, volatile gpioFnReg dest[2]);
    static u32 ClearReg(u32 blk, u32 mask, volatile gpioFnReg dest[2]);
    static u32 GetReg(u32 pin, volatile gpioFnReg dest[2]);
    static u32 GetReg(u32 blk, u32 mask, volatile gpioFnReg dest[2]);
};




#endif
