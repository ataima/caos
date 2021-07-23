
ARMGNU:=arm-none-eabi
ARMPATH:=$(shell pwd)/../$(ARMGNU)/bin/
CROSS_CC:=$(ARMPATH)$(ARMGNU)-gcc
CROSS_CPP:=$(ARMPATH)$(ARMGNU)-g++
CROSS_AS:=$(ARMPATH)$(ARMGNU)-as
CROSS_LD:=$(ARMPATH)$(ARMGNU)-ld
CROSS_OBJDUMP:=$(ARMPATH)$(ARMGNU)-objdump
CROSS_OBJCOPY:=$(ARMPATH)$(ARMGNU)-objcopy
CROSS_GDB:=$(ARMPATH)$(ARMGNU)-gdb


COPS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding 

all : kernel7.img 

clean :
	rm -f *.o
	rm -f *.bin
	rm -f *.hex
	rm -f *.elf
	rm -f *.lst
	rm -f *.list
	rm -f *.img
	rm -f *.bc
	rm -f *.clang.s
	rm -f all_code_ok.s
	rm -f bootloader07.s
	rm -f periph.s
	rm all_code.s dis_code.s 	


vectors.o : vectors.s
	$(CROSS_AS)  vectors.s -o vectors.o

bootloader07.o : bootloader07.c
	$(CROSS_CC)  $(COPS) -c bootloader07.c -o bootloader07.o

bootloader07.s : bootloader07.c
	$(CROSS_CC)  $(COPS) -c bootloader07.c -S -o bootloader07.s

bootloader07.lst : bootloader07.c
	$(CROSS_CC)  $(COPS) -Wa,-adhln -g bootloader07.c  > bootloader07.lst

periph.s : periph.c 
	$(CROSS_CC)  $(COPS) -c periph.c -S -o periph.s

periph.lst : periph.c 
	$(CROSS_CC)  $(COPS) -Wa,-adhln -g periph.c  > periph.lst

periph.o : periph.c 
	$(CROSS_CC)  $(COPS) -c periph.c -o periph.o

kernel7.img : asm  loader vectors.o periph.o bootloader07.o 
	$(CROSS_LD) vectors.o periph.o bootloader07.o -T loader -o bootloader07.elf
	$(CROSS_OBJDUMP) -D bootloader07.elf > bootloader07.list
	$(CROSS_OBJCOPY)  bootloader07.elf -O ihex bootloader07.hex
	$(CROSS_OBJCOPY) bootloader07.elf -O binary kernel7.img
	$(CROSS_OBJDUMP) -m arm -b ihex -D bootloader07.hex > dis_code.s
	
asm:	periph.s bootloader07.s
	@rm -f all_code.s
	@cat vectors.s > all_code.s
	@cat periph.s >> all_code.s
	@cat bootloader07.s >> all_code.s


lst:    periph.lst booloader07.lst
	@rm  -f all_code.lst
	@cat vectors.s > all_code.lst
	@cat periph.lst >> all_code.lst
	@cat bootloader07.lst >> all_code.lst


