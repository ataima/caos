#ifndef BCM2835GPIO_H
#define BCM2835GPIO_H

typedef union gpio_func_sel {

    struct tag_func_block {
        _RW_ u32 b0 : 3;
        _RW_ u32 b1 : 3;
        _RW_ u32 b2 : 3;
        _RW_ u32 b3 : 3;
        _RW_ u32 b4 : 3;
        _RW_ u32 b5 : 3;
        _RW_ u32 b6 : 3;
        _RW_ u32 b7 : 3;
        _RW_ u32 b8 : 3;
        _RW_ u32 b9 : 3;
        _RW_ u32 dummy : 2;
    } asBlock;
    _RW_ u32 asReg;
} gpioFnSel;

typedef union gpio_func_reg {

    struct tag_reg_bits {
        _RW_ u32 bit0 : 1;
        _RW_ u32 bit1 : 1;
        _RW_ u32 bit2 : 1;
        _RW_ u32 bit3 : 1;
        _RW_ u32 bit4 : 1;
        _RW_ u32 bit5 : 1;
        _RW_ u32 bit6 : 1;
        _RW_ u32 bit7 : 1;
        _RW_ u32 bit8 : 1;
        _RW_ u32 bit9 : 1;
        _RW_ u32 bit10 : 1;
        _RW_ u32 bit11 : 1;
        _RW_ u32 bit12 : 1;
        _RW_ u32 bit13 : 1;
        _RW_ u32 bit14 : 1;
        _RW_ u32 bit15 : 1;
        _RW_ u32 bit16 : 1;
        _RW_ u32 bit17 : 1;
        _RW_ u32 bit18 : 1;
        _RW_ u32 bit19 : 1;
        _RW_ u32 bit20 : 1;
        _RW_ u32 bit21 : 1;
        _RW_ u32 bit22 : 1;
        _RW_ u32 bit23 : 1;
        _RW_ u32 bit24 : 1;
        _RW_ u32 bit25 : 1;
        _RW_ u32 bit26 : 1;
        _RW_ u32 bit27 : 1;
        _RW_ u32 bit28 : 1;
        _RW_ u32 bit29 : 1;
        _RW_ u32 bit30 : 1;
        _RW_ u32 bit31 : 1;
    } asBit;
    _RW_ u32 asReg;
} gpioFnReg;

typedef struct tag_gpio_control {
    _RW_ volatile gpioFnSel Sel[6];
    u32 unused_0;
    _RW_ volatile gpioFnReg Set[2];
    u32 unused_1;
    _RW_ volatile gpioFnReg Clear[2];
    u32 unused_2;
    _RW_ volatile gpioFnReg Level[2];
    u32 unused_3;
    _RW_ volatile gpioFnReg Event[2];
    u32 unused_4;
    _RW_ volatile gpioFnReg RisingEdge[2];
    u32 unused_5;
    _RW_ volatile gpioFnReg FallingEdge[2];
    u32 unused_6;
    _RW_ volatile gpioFnReg HighDetect[2];
    u32 unused_7;
    _RW_ volatile gpioFnReg LowDetect[2];
    u32 unused_8;
    _RW_ volatile gpioFnReg AsyncRisingEdge[2];
    u32 unused_9;
    _RW_ volatile gpioFnReg AsyncFallingEdge[2];
    u32 unused_10;
    _RW_ volatile u32 PullResistor;
    _RW_ volatile gpioFnReg PullClock[2];
    u32 unused_11;
} GpioCntl, *ptrGpioCtrl;

#define system_gpio_control(a)  const ptrGpioCtrl a=(const ptrGpioCtrl)(PB_BASE+0x200000)

#define     fnInp       0
#define     fnOut       1
#define     fnAlt0      4
#define     fnAlt1      5
#define     fnAlt2      6
#define     fnAlt3      7
#define     fnAlt4      3
#define     fnAlt5      2

#define     MAX_GPIO_PIN   53
#define     MAX_GPIO_BLK   1
#endif /* BCM2835GPIO_H */

