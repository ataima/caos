/*//////////////////////////////////////////////////////////////////////////////
//    Copyright (C) 2011  Angelo Coppi (angelogkcop at hotmail.com )
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
*/

.section vectors
.code 32
.balign 4

.extern ISR_Undefined
.extern ISR_Software
.extern ISR_Abort
.extern ISR_IRQ
.extern ISR_FIQ
.extern ISR_Hypervisor
.extern ISR_Prefetch
.extern msgWelcome
.extern msgByeBye
.extern switchContext
.extern msgSchedule    






.globl _start
_start:
/*
 * System entry points.
 */

        ldr     pc, _reset
        ldr     pc, _undefined
        ldr     pc, _swi
        ldr     pc, _prefetch
        ldr     pc, _abort
        ldr     pc, _hypervisor
        ldr     pc, _irq
        ldr     pc, _fiq





_reset:
        .word   ResetISR           
_undefined:
        .word   go_undefined
_swi:
        .word   go_software
_prefetch:
        .word   go_prefecth
_abort:
        .word   go_abort
_hypervisor:	
        .word   go_hypervisor
_irq:
        .word   go_irq
_fiq:
        .word   go_fiq



.section .sec_start
.code 32
.balign 4



        .set    MODE_USR, 0x10
        .set    MODE_FIQ, 0x11
        .set    MODE_IRQ, 0x12
        .set    MODE_SVC, 0x13
        .set    MODE_MON, 0x16
        .set    MODE_ABT, 0x17
        .set    MODE_HYP, 0x1A
        .set    MODE_UND, 0x1B
        .set    MODE_SYS, 0x1F

        .set    I_BIT, 0x80
        .set    F_BIT, 0x40
        .set    A_BIT, 0x100



.globl ResetISR
ResetISR:
        mrs	r0, cpsr
	and	r1, r0, #0x1f		
	cmp	r1, #MODE_HYP		
        bne     no_Monitor  
        mrs	r0, cpsr
        bic     r0,r0,#0x1F
        orr     r0,r0,#0x1F   @13 SVC
        msr     spsr_cxsf,r0
        ldr     r0,=no_hyper
        msr     ELR_hyp,r0
        eret
no_Monitor:

        /* NO IRQ - FIQ - A for entry from bootloader....*/
        mrs	r0, cpsr      
	orr	r0, r0, #(A_BIT|F_BIT|I_BIT)
	msr	cpsr,r0
        /* check Supervisor - Hypervisor mode */
        mrs	r0, cpsr
	and	r1, r0, #0x1f		
	cmp	r1, #MODE_HYP		
        bne     no_hyper
        /* set vector table in hyp mode */
        mov r0,#0x8000
        MCR p15, 4, r0, c12, c0, 0
        b retry_pos
no_hyper:        
        mrc p15, 0, r1, c12, c0, 0 ;@ get vbar
retry_pos:
        mov r0,#0x8000
        mov r1,#0x0000
        ldmia r0!,{r2,r3,r4,r5,r6,r7,r8,r9}
        stmia r1!,{r2,r3,r4,r5,r6,r7,r8,r9}
        ldmia r0!,{r2,r3,r4,r5,r6,r7,r8,r9}
        stmia r1!,{r2,r3,r4,r5,r6,r7,r8,r9}


        /*
         * Stack pointers initialization.
         */
        ldr     r0, =__und_stack_pos__
        /* Undefined */
        msr     CPSR_c, #MODE_UND | I_BIT | F_BIT
        mov     sp, r0

        
        /* Abort */
        ldr     r0, =__abt_stack_pos__
        msr     CPSR_c, #MODE_ABT | I_BIT | F_BIT
        mov     sp, r0



        /* FIQ */
        ldr     r0, =__fiq_stack_pos__
        msr     CPSR_c, #MODE_FIQ | I_BIT | F_BIT
        mov     sp, r0


        /* IRQ */
         ldr     r0, =__irq_stack_pos__
       msr     CPSR_c, #MODE_IRQ | I_BIT | F_BIT
        mov     sp, r0


        /* Supervisor */
         ldr     r0, =__svc_stack_pos__
        msr     CPSR_c, #MODE_SVC | I_BIT | F_BIT
        mov     sp, r0


        /* Monitor */
         ldr     r0, =__mon_stack_pos__
        msr     CPSR_c, #MODE_MON | I_BIT | F_BIT
        mov     sp, r0



        /* System */
         ldr     r0, =__sys_stack_pos__
        msr     CPSR_c, #MODE_SYS | I_BIT | F_BIT
        mov     sp, r0


        /* Hyp */
         ldr     r0, =__hyp_stack_pos__
        msr     CPSR_c, #MODE_HYP | I_BIT | F_BIT
        mov     sp, r0

        mov     r0, #0
        ldr     r1, =_bss_start
        ldr     r2, =_bss_end
bssloop:
        cmp     r1, r2
        strlo   r0, [r1], #4
        blo     bssloop


        /*
         * Main program invocation.
         */
        // to SUPERVISOR MODE 
        mov r0,#(F_BIT|I_BIT|MODE_SYS)
        bl  changeCPSR
        ldr r0,=0x8000;   @entry point 
        MSR LR_usr,R0
        MSR LR_svc,R0
        MSR LR_irq,R0
        MSR LR_fiq,R0
        MSR LR_und,R0
        MSR LR_abt,R0
        ldr r0,=0;
        mov r1,r0
        mov r2,r0
        mov r3,r0
        mov r4,r0
        mov r5,r0
        mov r6,r0
        mov r7,r0
        mov r8,r0
        mov r9,r0
        mov r10,r0
        mov r11,r0
        mov r12,r0
        mov r14,r0
        bl      sysInit 
        bl      _Z8hal_mainv
        bl      sysStop 
        b       _main_exit_handler
.loop1: b       .loop1




go_undefined:
    STMFD    sp!,  {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
    MRS   r0,LR_usr
    MRS   r1,LR_svc
    MRS   r2,LR_irq
    MRS   r3,LR_und
    MRS     r4,SPSR        
    STMFD   sp!,{r4}
    BL ISR_Undefined
    LDMFD   sp!,{r0}          
    MSR     SPSR_cxsf,r0    
    LDMFD    sp!,   {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
    eret

switch_svc:
    MSR     SPSR_cxsf,R2    
    LDMFD   SP!,  {r0,r1,r2}
    MRC P15, 0, R1, C1, C1, 0
    BIC R1, R1, #1
    MCR P15, 0, R1, C1, C1, 0
    ERET

go_software:
    STMFD   SP!,  {r0,r1,r2}
    MRS     R2,SPSR
    MRS     R0,CPSR
    AND     R1,R0, #0x1f		
    CMP     R1,#MODE_HYP		
    BEQ     switch_svc
    LDR     R1,[LR,#-4]          
    BIC     R1,R1,#0xff000000 
    LDR     R0 , =7961
    CMP     R1,R0		
    BEQ     req_switch_context
    MSR     SPSR_cxsf,R2  
    LDMFD   SP!,  {r0,r1,r2}
    STMFD    sp!,  {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
    MRS     r10,SPSR        
    STMFD   sp!,{r10}
    bl ISR_Software
    LDMFD   sp!,{r0}          
    MSR     SPSR_cxsf,r0    
    LDMFD    sp!,  {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
    eret



req_switch_context:
    MSR     SPSR_cxsf,R2  
    LDMFD   SP!,  {r0,r1,r2}
    B switchContext 
    eret

go_prefecth:
    STMFD    sp!,  {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
    MRS   r0,LR_usr
    MRS   r1,LR_svc
    MRS   r2,LR_irq
    MRS   r3,LR_abt
    MRS     r4,SPSR        
    STMFD   sp!,{r4}
    BL       ISR_Prefetch
    LDMFD   sp!,{r0}          
    MSR     SPSR_cxsf,r0    
    LDMFD    sp!,   {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
    eret


go_abort:
    STMFD    sp!,  {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}        
    MRS   r0,LR_usr
    MRS   r1,LR_svc
    MRS   r2,LR_irq
    MRS   r3,LR_abt
    MRS     r4,SPSR        
    STMFD   sp!,{r4}
    BL       ISR_Abort
    LDMFD   sp!,{r0}          
    MSR     SPSR_cxsf,r0    
    LDMFD    sp!,   {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
    eret


go_hypervisor:
    STMFD    sp!,  {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
    MRS     r4,SPSR        
    STMFD   sp!,{r4}
    BL       ISR_Hypervisor
    LDMFD   sp!,{r0}          
    MSR     SPSR_cxsf,r0    
    LDMFD    sp!,   {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
    eret


go_irq:
    STMFD    sp!,  {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
    MRS     r4,SPSR        
    STMFD   sp!,{r4}
    BL       ISR_IRQ   @ return non zero need schedule
    cmp r0,#0
    bne  irq_scheduler
    LDMFD   sp!,{r1}          
    MSR     SPSR_cxsf,r1    
    LDMFD    sp!,   {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
    eret
    
irq_scheduler:
    LDMFD   sp!,{r1}          
    MSR     SPSR_cxsf,r1    
    LDMFD    sp!,   {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
    B switchContext 
    eret



go_fiq:
    STMFD    sp!,  {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}    
    MRS     r4,SPSR        
    STMFD   sp!,{r4}
    BL       ISR_FIQ   @ return non zero need schedule
    cmp r0,#0
    bne  fiq_scheduler
    LDMFD   sp!,{r1}          
    MSR     SPSR_cxsf,r1    
    LDMFD    sp!,   {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
    eret

fiq_scheduler:
    LDMFD   sp!,{r1}          
    MSR     SPSR_cxsf,r1    
    LDMFD    sp!,   {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
    B switchContext   
    eret



/*
 * Default main function exit handler.
 */
.global _main_exit_handler
_main_exit_handler:
.loop:  b       .loop








.global jump_to
jump_to:
    mov r12,#0
    mcr p15, 0, r12, c7, c10, 1
    dsb
    mov r12, #0
    mcr p15, 0, r12, c7, c5, 0
    mov r12, #0
    mcr p15, 0, r12, c7, c5, 6
    dsb
    isb
    bx r0



@ ----------------------------------------------------------------------
@ Description  : modify the control bits of the CPSR 
@ Parameters   : r0 - new modification
@ Return       : none... 
@ ----------------------------------------------------------------------
.global changeCPSR
changeCPSR:

     STMFD   r13!,{r3}
     MOV     r3,r13	
     ADD     r13,r13,#4      
     STMFD   r3!,{r1,lr}

@ ----------------------------------------------------------------------
@ Currently on the stack: r3,r1,lr
@ Modify the control part of the CPSR  
@ ----------------------------------------------------------------------

     MRS     r1,CPSR
     BIC     r1,r1,#0xff 
     ORR     r1,r1,r0 
     MSR     CPSR_c,r1

@ ----------------------------------------------------------------------
@ Restore from the stack: r3,r1,lr
@ Return to the callee
@ ----------------------------------------------------------------------

     LDMFD   r3!,{r1,lr}
     LDMFD   r3,{r3}
     MOV     pc,lr

.data

    .global Base_TTB
Base_TTB:
	.align 14		@ L1 Translation table must be aligned to 16kB boundary
	.skip  0x00004000	@ L1 Translation table takes up 16kB (4k entries X 4bytes each)
