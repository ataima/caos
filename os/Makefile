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


info:	
	@echo "----------------------------------------------------------------------"
	@make $(V) -f $(HAL) info 
	@echo "----------------------------------------------------------------------"
	@make $(V) -f $(HW) info 
	@echo "----------------------------------------------------------------------"
	@make $(V) -f $(TEST) info 
	@echo "----------------------------------------------------------------------"
	@echo info ...done.
	@echo "----------------------------------------------------------------------"

depend:	
	@echo "----------------------------------------------------------------------"
	@make $(V) -f $(HAL) depend 
	@echo "----------------------------------------------------------------------"
	@make $(V) -f $(HW) depend
	@echo "----------------------------------------------------------------------"
	@make $(V) -f $(TEST) depend
	@echo "----------------------------------------------------------------------"
	@echo info ...done.
	@echo "----------------------------------------------------------------------"



all_obj:		
	@echo "--------------------------------------------------------------------"
	@make $(V) -f $(HAL) all 
	@echo "--------------------------------------------------------------------"
	@make $(V) -f $(HW) all
	@echo "--------------------------------------------------------------------"
	@make $(V) -f $(TEST) all
	@echo "--------------------------------------------------------------------"
	@echo all ...done.
	@echo "----------------------------------------------------------------------"

define find_objs =
ALLOBJS:= $(wildcard $1/*.o)
ALLOBJS+= $(wildcard $1/hw/*.o)	
endef
	
	
link_file: all_obj
	$(eval $(call find_objs,$(BUILDIR)/$(OBJ_OUT)))
	$(CROSS_LD)  $(ALLOBJS) $(LK_OPTS) -o $(BUILDIR)/caOS.elf > $(BUILDIR)/caOS.map
	@echo  $(EH) $(C_PURPLE)"[LINKER ]"$(C_RESET)" $(BUILDIR)/caOS.elf">/dev/stderr
	
objdump_file:
	$(CROSS_OBJDUMP) -D $(BUILDIR)/caOS.elf -S > $(BUILDIR)/caOS.list
	@echo  $(EH) $(C_GREEN)"[OBJDUMP]"$(C_RESET)" $(BUILDIR)/caOS.list">/dev/stderr
	$(CROSS_OBJCOPY) $(BUILDIR)/caOS.elf -O ihex $(BUILDIR)/caOS.hex
	@echo  $(EH) $(C_GREEN)"[OBJCOPY]"$(C_RESET)" $(BUILDIR)/caOS.hex">/dev/stderr
	$(CROSS_OBJCOPY) $(BUILDIR)/caOS.elf -O binary $(BUILDIR)/caOS.bin
	@echo  $(EH) $(C_GREEN)"[OBJCOPY]"$(C_RESET)" $(BUILDIR)/caOS.bin">/dev/stderr
	@cp $(BUILDIR)/caOS.bin kernel7.img
	@echo  $(EH) $(C_GREEN)"[OBJCOPY]"$(C_RESET)" kernel7.img">/dev/stderr
	$(CROSS_OBJDUMP) $(OBJD_CPU) -b ihex -D $(BUILDIR)/caOS.hex > $(BUILDIR)/caOS.s
	@echo  $(EH) $(C_GREEN)"[OBJDUMP]"$(C_RESET)" $(BUILDIR)/caOS.s">/dev/stderr
	$(CROSS_OBJCOPY) --only-keep-debug $(BUILDIR)/caOS.elf $(BUILDIR)/caOS.syms
	@echo  $(EH) $(C_GREEN)"[OBJDUMP]"$(C_RESET)" $(BUILDIR)/caOS.syms">/dev/stderr

dwload_1: kernel7.img
	@echo  $(EH) $(C_GREEN)"[DOWNLOAD]"$(C_RESET)" $(BUILDIR)/caOS.hex">/dev/stderr
	@$(DWLOAD) $(BUILDIR)/caOS.hex


dwload: dwload_1 	
	@echo dwload ...done.
	@echo "----------------------------------------------------------------------"


kernel7.img: link_file objdump_file
	

asm:		
	@echo "----------------------------------------------------------------------"
	@make $(V) -f $(HAL) asm 
	@echo "----------------------------------------------------------------------"
	@make $(V) -f $(HW) asm	
	@echo "----------------------------------------------------------------------"
	@make $(V) -f $(TEST) asm
	@echo "----------------------------------------------------------------------"
	@echo all ...done.
	@echo "----------------------------------------------------------------------"


clean:	
	@echo "----------------------------------------------------------------------"	
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
	@echo clean ...done.
	@echo "----------------------------------------------------------------------"
	


all:  kernel7.img doversion
	
	
qemu:
	@echo "----------------------------------------------------------------------"
	qemu-system-arm -M raspi2 -cpu bcm2836 -m 1G   -kernel kernel7.img  &
	@echo qemu ...done.
	@echo "----------------------------------------------------------------------"
		

qemu-deb:
	@echo "----------------------------------------------------------------------"	
	@qemu-system-arm -M raspi2 -cpu bcm2836 -m 1G -s  -kernel kernel7.img  &
	@echo "target remote :1234" >gdb.conf
	@echo "file ./$(BUILDIR)/caOS.elf" >>gdb.conf
	@echo  $(EH)  $(C_RED )[GDB] $(C_RESTORE)
	@$(CROSS_GDB) -x gdb.conf
	@echo qemu_deb ...done.
	@echo "----------------------------------------------------------------------"
		
jtag:
	@echo "----------------------------------------------------------------------"	
	@echo  $(EH)  $(C_RED )[GDB] $(C_RESTORE)
	@$(CROSS_GDB) ./$(BUILDIR)/caOS.elf
	@echo jtag ...done.
	@echo "----------------------------------------------------------------------"

sim:
	@echo "----------------------------------------------------------------------"
	echo "target sim" >gdb.conf
	echo "file $(BUILDIR)/caos.elf" >>gdb.conf
	@echo $(EH)  $(C_RED )[GDB]$C_RESTORE
	@$(CROSS_GDB) -x gdb.conf
	@echo sim ...done.
	@echo "----------------------------------------------------------------------"
		
	
		
distclean: clean 
	@echo "----------------------------------------------------------------------"
	@rm -rf $(BUILDIR)/$(DEPEND)
	@rm -rf $(BUILDIR)
	@echo distclean ...done	
	@echo "----------------------------------------------------------------------"



.PHONY: clean dwload qemu all all_obj distclean kernel7.img

