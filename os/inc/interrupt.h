
#ifndef _INTERRUPT_HEADER_
#define _INTERRUPT_HEADER_

//IRQ, FIQ, SWI, ABORT and UNDEF
extern "C" {
    // to do port in class caARMspecific
    extern void changeCPSR(u32 mode);

    void ISR_Undefined(u32 lr_usr, u32 lr_svc, u32 lr_irq, u32 lr_und);
    void ISR_Software(u32 ioctl, u32 *p1, u32 *p2, u32 *res);
    void ISR_Abort(u32 lr_usr, u32 lr_svc, u32 lr_irq, u32 lr_abt);
    u32 ISR_IRQ(void);
    u32 ISR_FIQ(void);
    void ISR_Prefetch(u32 lr_usr, u32 lr_svc, u32 lr_irq, u32 lr_abt);
    void ISR_Hypervisor(void);
    void Welcome(void);
    void ByeBye(void);
    void switchContext(void) __attribute__((naked));
}

class caInterruptRequest {
public:
    static void Undefined(u32 lr_usr, u32 lr_svc, u32 lr_irq, u32 ld_und);
    static void Software(u32 ioctl, u32 *p1,
            u32 *p2, u32 *res);
    static void Abort(u32 lr_usr, u32 lr_svc, u32 lr_irq, u32 lr_abt);
    static u32 IRQ(void);
    static u32 FIQ(void);
    static void Prefetch(u32 lr_usr, u32 lr_svc, u32 lr_irq, u32 lr_fiq);
    static void Hypervisor(void);

    static inline void EnableInt(void) {
        asm volatile("cpsie iaf");
    }

    static inline void DisableInt(void) {
        asm volatile("cpsid iaf");
    }

    static inline void WaitForInterrupt(void) {
        asm volatile("wfi");
    }


};


#endif 

