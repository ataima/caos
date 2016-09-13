#ifndef BCM2835_IQR_HEADER
#define BCM2835_IQR_HEADER

typedef union irq_basic_pending {

    struct tag_irq_basic_pending_reg {
        _RW_ u32 timer : 1;
        _RW_ u32 mailbox : 1;
        _RW_ u32 doorbell0 : 1;
        _RW_ u32 doorbell1 : 1;
        _RW_ u32 gpu0halt : 1;
        _RW_ u32 gpu1halt : 1;
        _RW_ u32 illegalaccess1 : 1;
        _RW_ u32 illegalaccess0 : 1;
        _RW_ u32 pending1 : 1;
        _RW_ u32 pending2 : 1;
        _RW_ u32 gpuirq7 : 1;
        _RW_ u32 gpuirq9 : 1;
        _RW_ u32 gpuirq10 : 1;
        _RW_ u32 gpuirq18 : 1;
        _RW_ u32 gpuirq19 : 1;
        _RW_ u32 gpuirq53 : 1;
        _RW_ u32 gpuirq54 : 1;
        _RW_ u32 gpuirq55 : 1;
        _RW_ u32 gpuirq56 : 1;
        _RW_ u32 gpuirq57 : 1;
        _RW_ u32 gpuirq62 : 1;
    } asBit;
    _RW_ u32 asReg;
} irqBasicPending, *ptrIrqBasicPending;

typedef union irq_gpu_0_pending {

    struct tag_irq_gpu_0_pending_reg {
        _RW_ u32 irqsource0 : 1;
        _RW_ u32 irqsource1 : 1;
        _RW_ u32 irqsource2 : 1;
        _RW_ u32 irqsource3 : 1;
        _RW_ u32 irqsource4 : 1;
        _RW_ u32 irqsource5 : 1;
        _RW_ u32 irqsource6 : 1;
        _RW_ u32 irqsource7 : 1;
        _RW_ u32 irqsource8 : 1;
        _RW_ u32 irqsource9 : 1;
        _RW_ u32 irqsource10 : 1;
        _RW_ u32 irqsource11 : 1;
        _RW_ u32 irqsource12 : 1;
        _RW_ u32 irqsource13 : 1;
        _RW_ u32 irqsource14 : 1;
        _RW_ u32 irqsource15 : 1;
        _RW_ u32 irqsource16 : 1;
        _RW_ u32 irqsource17 : 1;
        _RW_ u32 irqsource18 : 1;
        _RW_ u32 irqsource19 : 1;
        _RW_ u32 irqsource20 : 1;
        _RW_ u32 irqsource21 : 1;
        _RW_ u32 irqsource22 : 1;
        _RW_ u32 irqsource23 : 1;
        _RW_ u32 irqsource24 : 1;
        _RW_ u32 irqsource25 : 1;
        _RW_ u32 irqsource26 : 1;
        _RW_ u32 irqsource27 : 1;
        _RW_ u32 irqsource28 : 1;
        _RW_ u32 irqaux29 : 1;
        _RW_ u32 irqsource30 : 1;
        _RW_ u32 irqsource31 : 1;
    } asBit;
    _RW_ u32 asReg;
} irqGpu0Pending, *ptrIrqGpu0Pending;

typedef union irq_gpu_1_pending {

    struct tag_irq_gpu_1_pending_reg {
        _RW_ u32 irqsource32 : 1;
        _RW_ u32 irqsource33 : 1;
        _RW_ u32 irqsource34 : 1;
        _RW_ u32 irqsource35 : 1;
        _RW_ u32 irqsource36 : 1;
        _RW_ u32 irqsource37 : 1;
        _RW_ u32 irqsource38 : 1;
        _RW_ u32 irqsource39 : 1;
        _RW_ u32 irqsource40 : 1;
        _RW_ u32 irqsource41 : 1;
        _RW_ u32 irqsource42 : 1;
        _RW_ u32 irqsource43 : 1;
        _RW_ u32 irqsource44 : 1;
        _RW_ u32 irqsource45 : 1;
        _RW_ u32 irqsource46 : 1;
        _RW_ u32 irqsource47 : 1;
        _RW_ u32 irqsource48 : 1;
        _RW_ u32 irqsource49 : 1;
        _RW_ u32 irqsource50 : 1;
        _RW_ u32 irqsource51 : 1;
        _RW_ u32 irqsource52 : 1;
        _RW_ u32 irqsource53 : 1;
        _RW_ u32 irqsource54 : 1;
        _RW_ u32 irqsource55 : 1;
        _RW_ u32 irqsource56 : 1;
        _RW_ u32 irqsource57 : 1;
        _RW_ u32 irqsource58 : 1;
        _RW_ u32 irqsource59 : 1;
        _RW_ u32 irqsource60 : 1;
        _RW_ u32 irqsource61 : 1;
        _RW_ u32 irqsource62 : 1;
        _RW_ u32 irqsource63 : 1;
    } asBit;
    _RW_ u32 asReg;
} irqGpu1Pending, *ptrIrqGpu1Pending;

typedef union fiq_control {

    struct tag_fiq_control_reg {
        _RW_ u32 source : 7;
        _RW_ u32 enable : 1;
    } asBit;
    _RW_ u32 asReg;
} fiqControl, *ptrFiqControl;

typedef union irq_enable_0 {

    struct tag_irq_enable_0_reg {
        _RW_ u32 unused0 : 1;
        _RW_ u32 unused1 : 1;
        _RW_ u32 unused2 : 1;
        _RW_ u32 unused3 : 1;
        _RW_ u32 unused4 : 1;
        _RW_ u32 unused5 : 1;
        _RW_ u32 unused6 : 1;
        _RW_ u32 unused7 : 1;
        _RW_ u32 unused8 : 1;
        _RW_ u32 unused9 : 1;
        _RW_ u32 unused10 : 1;
        _RW_ u32 unused11 : 1;
        _RW_ u32 unused12 : 1;
        _RW_ u32 unused13 : 1;
        _RW_ u32 unused14 : 1;
        _RW_ u32 unused15 : 1;
        _RW_ u32 unused16 : 1;
        _RW_ u32 unused17 : 1;
        _RW_ u32 unused18 : 1;
        _RW_ u32 unused19 : 1;
        _RW_ u32 unused20 : 1;
        _RW_ u32 unused21 : 1;
        _RW_ u32 unused22 : 1;
        _RW_ u32 unused23 : 1;
        _RW_ u32 unused24 : 1;
        _RW_ u32 unused25 : 1;
        _RW_ u32 unused26 : 1;
        _RW_ u32 unused27 : 1;
        _RW_ u32 unused28 : 1;
        _RW_ u32 aux_int : 1;
        _RW_ u32 unused30 : 1;
        _RW_ u32 unused31 : 1;
    } asBit;
    _RW_ u32 asReg;
} irqEnable0, *ptrIrqEnable0;

typedef union irq_enable_1 {

    struct tag_irq_enable_1_reg {
        _RW_ u32 unused32 : 1;
        _RW_ u32 unused33 : 1;
        _RW_ u32 unused34 : 1;
        _RW_ u32 unused35 : 1;
        _RW_ u32 unused36 : 1;
        _RW_ u32 unused37 : 1;
        _RW_ u32 unused38 : 1;
        _RW_ u32 unused39 : 1;
        _RW_ u32 unused40 : 1;
        _RW_ u32 unused41 : 1;
        _RW_ u32 unused42 : 1;
        _RW_ u32 i2c_spi_slv_int : 1;
        _RW_ u32 unused44 : 1;
        _RW_ u32 pwa0 : 1;
        _RW_ u32 pwa1 : 1;
        _RW_ u32 unused47 : 1;
        _RW_ u32 smi : 1;
        _RW_ u32 gpio_int0 : 1;
        _RW_ u32 gpio_int1 : 1;
        _RW_ u32 gpio_int2 : 1;
        _RW_ u32 gpio_int3 : 1;
        _RW_ u32 i2c_int : 1;
        _RW_ u32 spi_int : 1;
        _RW_ u32 pcm_int : 1;
        _RW_ u32 unused56 : 1;
        _RW_ u32 uart_int : 1;
        _RW_ u32 unused58 : 1;
        _RW_ u32 unused59 : 1;
        _RW_ u32 unused60 : 1;
        _RW_ u32 unused61 : 1;
        _RW_ u32 unused62 : 1;
        _RW_ u32 unused63 : 1;
    } asBit;
    _RW_ u32 asReg;
} irqEnable1, *ptrIrqEnable1;

typedef union irq_base_enable {

    struct tag_irq_base_enable_reg {
        _RW_ u32 timerirq : 1;
        _RW_ u32 mailboxirq : 1;
        _RW_ u32 dorbell0irq : 1;
        _RW_ u32 dorbell1irq : 1;
        _RW_ u32 gpu0 : 1;
        _RW_ u32 gpu1 : 1;
        _RW_ u32 accesserror0 : 1;
        _RW_ u32 accesserror1 : 1;
    } asBit;
    _RW_ u32 asReg;
} irqBaseEnable, *ptrIrqBaseEnable;

// DISABLE 

typedef union irq_disable_0 {

    struct tag_irq_disable_0_reg {
        _RW_ u32 unused0 : 1;
        _RW_ u32 unused1 : 1;
        _RW_ u32 unused2 : 1;
        _RW_ u32 unused3 : 1;
        _RW_ u32 unused4 : 1;
        _RW_ u32 unused5 : 1;
        _RW_ u32 unused6 : 1;
        _RW_ u32 unused7 : 1;
        _RW_ u32 unused8 : 1;
        _RW_ u32 unused9 : 1;
        _RW_ u32 unused10 : 1;
        _RW_ u32 unused11 : 1;
        _RW_ u32 unused12 : 1;
        _RW_ u32 unused13 : 1;
        _RW_ u32 unused14 : 1;
        _RW_ u32 unused15 : 1;
        _RW_ u32 unused16 : 1;
        _RW_ u32 unused17 : 1;
        _RW_ u32 unused18 : 1;
        _RW_ u32 unused19 : 1;
        _RW_ u32 unused20 : 1;
        _RW_ u32 unused21 : 1;
        _RW_ u32 unused22 : 1;
        _RW_ u32 unused23 : 1;
        _RW_ u32 unused24 : 1;
        _RW_ u32 unused25 : 1;
        _RW_ u32 unused26 : 1;
        _RW_ u32 unused27 : 1;
        _RW_ u32 unused28 : 1;
        _RW_ u32 aux_int : 1;
        _RW_ u32 unused30 : 1;
        _RW_ u32 unused31 : 1;
    } asBit;
    _RW_ u32 asReg;
} irqDisable0, *ptrIrqDisable0;

typedef union irq_disable_1 {

    struct tag_irq_disable_1_reg {
        _RW_ u32 unused32 : 1;
        _RW_ u32 unused33 : 1;
        _RW_ u32 unused34 : 1;
        _RW_ u32 unused35 : 1;
        _RW_ u32 unused36 : 1;
        _RW_ u32 unused37 : 1;
        _RW_ u32 unused38 : 1;
        _RW_ u32 unused39 : 1;
        _RW_ u32 unused40 : 1;
        _RW_ u32 unused41 : 1;
        _RW_ u32 unused42 : 1;
        _RW_ u32 i2c_spi_slv_int : 1;
        _RW_ u32 unused44 : 1;
        _RW_ u32 pwa0 : 1;
        _RW_ u32 pwa1 : 1;
        _RW_ u32 unused47 : 1;
        _RW_ u32 smi : 1;
        _RW_ u32 gpio_int0 : 1;
        _RW_ u32 gpio_int1 : 1;
        _RW_ u32 gpio_int2 : 1;
        _RW_ u32 gpio_int3 : 1;
        _RW_ u32 i2c_int : 1;
        _RW_ u32 spi_int : 1;
        _RW_ u32 pcm_int : 1;
        _RW_ u32 unused56 : 1;
        _RW_ u32 uart_int : 1;
        _RW_ u32 unused58 : 1;
        _RW_ u32 unused59 : 1;
        _RW_ u32 unused60 : 1;
        _RW_ u32 unused61 : 1;
        _RW_ u32 unused62 : 1;
        _RW_ u32 unused63 : 1;
    } asBit;
    _RW_ u32 asReg;
} irqDisable1, *ptrIrqDisable1;

typedef union irq_base_disable {

    struct tag_irq_base_disable_reg {
        _RW_ u32 timerirq : 1;
        _RW_ u32 mailboxirq : 1;
        _RW_ u32 dorbell0irq : 1;
        _RW_ u32 dorbell1irq : 1;
        _RW_ u32 gpu0 : 1;
        _RW_ u32 gpu1 : 1;
        _RW_ u32 accesserror0 : 1;
        _RW_ u32 accesserror1 : 1;
    } asBit;
    _RW_ u32 asReg;
} irqBaseDisable, *ptrIrqBaseDisable;

typedef struct irq_control_tag {
    _RW_ volatile irqBasicPending basepending;
    _RW_ volatile irqGpu0Pending gpu0;
    _RW_ volatile irqGpu1Pending gpu1;
    _RW_ volatile fiqControl fiq;
    _RW_ volatile irqEnable0 enable0;
    _RW_ volatile irqEnable1 enable1;
    _RW_ volatile irqBaseEnable baseenable;
    _RW_ volatile irqDisable0 disable0;
    _RW_ volatile irqDisable1 disable1;
    _RW_ volatile irqBaseDisable basedisable;
} irqControl, *ptrIrqControl;


#define system_irq_control(a)  const ptrIrqControl a=(const ptrIrqControl)(PB_BASE+0xB200)

#endif 

