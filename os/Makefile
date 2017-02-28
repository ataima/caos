include rules.mk
V=$(SILENT)

HAL:=src/hal/do.mk
ifeq ($(HARDWARE),RASPI2)
    HW:=src/hw/rasp2/do.mk
    TEST:=test/rasp2/do.mk
endif
ifeq ($(HARDWARE),686)
    HW:=src/hw/686/do.mk
    TEST:=test/686/do.mk
endif
ifeq ($(HARDWARE),SIMULA)
    HW:=src/hw/simula/do.mk
    TEST:=test/simula/do.mk
endif


default: all

doversion :
	$(eval BUILD_NUMBER=`./version.sh $(CAOS_VERSION_1) $(CAOS_VERSION_2) $(CAOS_VERSION_3)`)
	@echo $(EH)  $(C_GREEN)[VERSION] $(C_WHITE)= Vers. $(C_MAGENTA)$(CAOS_VERSION_1)$(C_WHITE)"."$(C_YELLOW)$(CAOS_VERSION_2)$(C_WHITE)"."$(C_PURPLE)$(CAOS_VERSION_3) $(C_WHITE)Build Number $(C_RED)$(BUILD_NUMBER) $(C_RESTORE)
	@echo $(EH) $(C_RESTORE)


info:	
	@echo $(SEP)
	@make $(V) -f $(HAL) info 
	@echo $(SEP)
	@make $(V) -f $(HW) info 
	@echo $(SEP)
	@make $(V) -f $(TEST) info 
	@echo $(SEP)
	

depend:	
	@echo $(SEP)
	@make $(V) -f $(HAL) depend 
	@echo $(SEP)
	@make $(V) -f $(HW) depend
	@echo $(SEP)
	@make $(V) -f $(TEST) depend
	@echo $(SEP)
	



all_obj:		
	@echo $(SEP)
	@make $(V) -f $(HAL) all 
	@echo $(SEP)
	@make $(V) -f $(HW) all
	@echo $(SEP)
	@make $(V) -f $(TEST) all
	@echo $(SEP)
	

define find_objs =
ALLOBJS:= $(wildcard $1/*.o)
ALLOBJS+= $(wildcard $1/hw/*.o)	
endef

define width_s40
    $(shell printf "%-35s %s" "$1")
endef
	
define add_msg
    @echo $(EH) $(C_GREEN)"[$1] :" $(C_WHITE)"$(call width_s40, $2)"$(C_RED)$2$(C_RESET) >/dev/stderr
endef
	
	
link_file: all_obj
	$(eval $(call find_objs,$(BUILDIR)/$(OBJ_OUT)))
	@$(CROSS_LD)  $(ALLOBJS) $(LK_OPTS) -o $(BUILDIR)/caOS.elf > $(BUILDIR)/caOS.map
	$(call add_msg,"LINKER ",$(BUILDIR)/caOS.elf)
	
	
objdump_file:
	@$(CROSS_OBJDUMP) -D $(BUILDIR)/caOS.elf -S > $(BUILDIR)/caOS.list
	$(call add_msg,"OBJDUMP",$(BUILDIR)/caOS.list)
	@$(CROSS_OBJCOPY) $(BUILDIR)/caOS.elf -O ihex $(BUILDIR)/caOS.hex
	$(call add_msg,"OBJCOPY",$(BUILDIR)/caOS.hex)
	@$(CROSS_OBJCOPY) $(BUILDIR)/caOS.elf -O binary $(BUILDIR)/caOS.bin
	$(call add_msg,"OBJCOPY",$(BUILDIR)/caOS.bin)
	@cp $(BUILDIR)/caOS.bin kernel7.img
	$(call add_msg,"OBJCOPY",kernel7.img)
	@$(CROSS_OBJDUMP) $(OBJD_CPU) -b ihex -D $(BUILDIR)/caOS.hex > $(BUILDIR)/caOS.s
	$(call add_msg,"OBJDUMP",$(BUILDIR)/caOS.s)
	@$(CROSS_OBJCOPY) --only-keep-debug $(BUILDIR)/caOS.elf $(BUILDIR)/caOS.syms
	$(call add_msg,"OBJDUMP",$(BUILDIR)/caOS.syms)

dwload_1: kernel7.img
	$(call add_msg,"DOWNLOAD",$(BUILDIR)/caOS.hex)
	@$(DWLOAD) $(BUILDIR)/caOS.hex


dwload: dwload_1 		
	@echo $(SEP)


kernel7.img: link_file objdump_file
	

asm:		
	@echo $(SEP)
	@make $(V) -f $(HAL) asm 
	@echo $(SEP)
	@make $(V) -f $(HW) asm	
	@echo $(SEP)
	@make $(V) -f $(TEST) asm
	@echo $(SEP)
	
clean:	
	@echo $(SEP)	
	@rm -rf  $(BUILDIR)/$(OBJ_OUT)
	@rm -rf  $(BUILDIR)
	@mkdir -p $(BUILDIR)
	@mkdir -p $(BUILDIR)/$(OBJ_OUT)
	@rm -f $(BUILDIR)/*.bin
	@rm -f $(BUILDIR)/*.hex
	@rm -f $(BUILDIR)/*.elf
	@rm -f $(BUILDIR)/*.lst
	@rm -f $(BUILDIR)/*.list
	@rm -f $(BUILDIR)/*.img
	@rm -f $(BUILDIR)/*.s
	@rm -f  trace-*
	@rm -f kernel7.img
	@echo $(SEP)
	


all:  kernel7.img doversion
	
	
qemu:
	@echo $(SEP)
	qemu-system-arm -M raspi2 -cpu bcm2836 -m 1G   -kernel kernel7.img  &
	@echo $(SEP)
		

qemu-deb:
	@echo $(SEP)	
	@qemu-system-arm -M raspi2 -cpu bcm2836 -m 1G -s  -kernel kernel7.img  &
	@echo "target remote :1234" >gdb.conf
	@echo "file ./$(BUILDIR)/caOS.elf" >>gdb.conf
	@echo  $(EH)  $(C_RED )[GDB] $(C_RESTORE)
	@$(CROSS_GDB) -x gdb.conf
	@echo $(SEP)
		
jtag:
	@openocd -f ./raspi2.cfg &
	@sleep 1
	@echo "file ./$(BUILDIR)/caOS.elf" >gdb.conf
	@echo "target remote :3333" >>gdb.conf
	@echo "monitor halt" >>gdb.conf
	@echo "monitor reset_config srst_only" >>gdb.conf
	@echo "monitor reset" >>gdb.conf
	@echo "monitor targets rpi2.cpu0" >> gdb.conf
	@echo "monitor reg cpsr 0" >> gdb.conf 	
	@echo "monitor targets rpi2.cpu1" >> gdb.conf
	@echo "monitor reg cpsr 0" >> gdb.conf
	@echo "monitor targets rpi2.cpu2" >> gdb.conf
	@echo "monitor reg cpsr 0" >> gdb.conf
	@echo "monitor targets rpi2.cpu3" >> gdb.conf
	@echo "monitor reg cpsr 0" >> gdb.conf
	@echo "monitor targets rpi2.cpu0" >> gdb.conf
	@echo "load" >> gdb.conf
	@echo "monitor targets" >> gdb.conf
	@echo  $(EH)  $(C_RED )[GDB] $(C_RESTORE)
	@$(CROSS_GDB) -x gdb.conf
	@killall openocd
	@echo jtag ...done.
	@echo $(SEP)


sim:
	@echo $(SEP)
	echo "target sim" >gdb.conf
	echo "file $(BUILDIR)/caos.elf" >>gdb.conf
	@echo $(EH)  $(C_RED )[GDB]$C_RESTORE
	@$(CROSS_GDB) -x gdb.conf
	@echo $(SEP)
		
	
		
distclean: clean 
	@echo $(SEP)
	@rm -rf $(BUILDIR)/$(DEPEND)
	@rm -rf $(BUILDIR)
	@echo $(SEP)



.PHONY: clean dwload qemu all all_obj distclean kernel7.img

