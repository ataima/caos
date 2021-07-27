__und_stack_size__	= 0x10000;
__abt_stack_size__	= 0x10000;
__fiq_stack_size__	= 0x10000;
__irq_stack_size__	= 0x10000;
__svc_stack_size__	= 0x10000;
__sys_stack_size__	= 0x10000;
__hyp_stack_size__	= 0x10000;
__mon_stack_size__	= 0x10000;
__user_stack_size__	= 0x1000000;
__stacks_total_size__	= __und_stack_size__ + __abt_stack_size__ + __fiq_stack_size__ + __irq_stack_size__ + __svc_stack_size__ + __sys_stack_size__ + __hyp_stack_size__ + __mon_stack_size__ + __user_stack_size__;



MEMORY
{
    ram : org = 0x8000, len = 0x3f000000-0x8000 
}

__ram_start__		= ORIGIN(ram);
__ram_size__		= LENGTH(ram);
__ram_end__		= __ram_start__ + __ram_size__;


__und_stack_pos__	= ABSOLUTE(__ram_end__);
__abt_stack_pos__	= ABSOLUTE(__und_stack_pos__ - __und_stack_size__);
__fiq_stack_pos__	= ABSOLUTE(__abt_stack_pos__ - __abt_stack_size__);
__irq_stack_pos__	= ABSOLUTE(__fiq_stack_pos__ - __fiq_stack_size__);
__svc_stack_pos__	= ABSOLUTE(__irq_stack_pos__ - __irq_stack_size__);
__sys_stack_pos__	= ABSOLUTE(__svc_stack_pos__ - __svc_stack_size__);
__hyp_stack_pos__	= ABSOLUTE(__sys_stack_pos__ - __sys_stack_size__);
__mon_stack_pos__	= ABSOLUTE(__hyp_stack_pos__ - __hyp_stack_size__);
__user_stack_pos__	= ABSOLUTE(__mon_stack_pos__ - __mon_stack_size__);
__user_stack_pos__end   = ABSOLUTE(__user_stack_pos__ - __user_stack_size__);


SECTIONS
{
	. = 0;

	.text : ALIGN(16) SUBALIGN(16)
	{
        _text = .;
        KEEP(*(vectors))
        *(.sec_start)
        *(.sec_start.*)
        *(.text)
        *(.text.*)
        *(.rodata)
        *(.rodata.*)
        *(.glue_7t)
        *(.glue_7)
        *(.gcc*)
        *(.ctors)
        *(.dtors)
    } > ram

    __extab_start = .;
    .ARM.extab : {*(.ARM.extab* .gnu.linkonce.armextab.*)}> ram
    __extab_end = .;

    __exidx_start = .;
    .ARM.exidx : {*(.ARM.exidx* .gnu.linkonce.armexidx.*)} > ram
    __exidx_end = .;

    .eh_frame_hdr : {*(.eh_frame_hdr)}

    .eh_frame : ONLY_IF_RO {*(.eh_frame)}

    . = ALIGN(4);
    _etext = .;
    _textdata = _etext;

    .data :
    {
        _data = .;
        *(.data)
        . = ALIGN(4);
        *(.data.*)
        . = ALIGN(4);
        *(.ramtext)
        . = ALIGN(4);
        _edata = .;
    } > ram 

    .bss :
    {
        _bss_start = .;
        *(.bss)
        . = ALIGN(4);
        *(.bss.*)
        . = ALIGN(4);
        *(COMMON)
        . = ALIGN(4);
        _bss_end = .;
    } > ram    
}

/* heap + thread stack for os  todo 2 memory allocator  */
__heap_base__              = .;
__heap_end__             = __heap_base__ + 0x4000000;
__main_thread_stack_base__ = __heap_base__ +0x4000000;
__user_ram_start__ = __main_thread_stack_base__ +0x200 ;
__user_ram_end__ = __ram_end__ - __stacks_total_size__  ;
PROVIDE(end = .);
_end = .;


