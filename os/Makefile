include rules.mk



default: all

doversion :
	@echo $(SEP)
	$(eval BUILD_NUMBER=`./version.sh $(CAOS_VERSION_1) $(CAOS_VERSION_2) $(CAOS_VERSION_3)`)
	@echo $(EH)  $(C_GREEN)[VERSION] $(C_WHITE)= Vers. $(C_MAGENTA)$(CAOS_VERSION_1)$(C_WHITE)"."$(C_YELLOW)$(CAOS_VERSION_2)$(C_WHITE)"."$(C_PURPLE)$(CAOS_VERSION_3) $(C_WHITE)Build Number $(C_RED_LIGHT)$(BUILD_NUMBER) $(C_RESET)
	@echo $(EH) $(C_RESET)



define dump_var    
    @echo  $(EH) $(C_YELLOW) '$(shell printf "%-30s %s = " $1)' $(C_WHITE) $2 $(C_RESET)
endef

info:	
	@echo $(SEP)
	$(call dump_var,"Current HARDWARE","$(HARDWARE)")
	$(call dump_var,"Current TOOLS CHAIN","$(ARMGNU)")
	$(call dump_var,"Current TOOLS CHAIN PATH","$(ARMPATH)")
	$(call dump_var,"Current TOOLS CHAIN CC","$(CROSS_CC)")
	$(call dump_var,"Current TOOLS CHAIN CPP","$(CROSS_CPP)") 
	$(call dump_var,"Current TOOLS CHAIN AS","$(CROSS_AS)") 
	$(call dump_var,"Current TOOLS CHAIN LD","$(CROSS_LD)")
	$(call dump_var,"Current TOOLS CHAIN OBJDUMP","$(CROSS_OBJDUMP)") 
	$(call dump_var,"Current TOOLS CHAIN OBJCOPY","$(CROSS_OBJCOPY)")
	$(call dump_var,"Current TOOLS CHAIN LD","$(CROSS_GDB)")
	$(call dump_var,"Current Debug level","$(DBG)")
	$(call dump_var,"Current Options for depend  files","$(DEP_OPTS)")
	$(call dump_var,"Current Options for C(*.c) files","$(C_OPTS)")
	$(call dump_var,"Current Options for CPP(*.cpp) files","$(CPP_OPTS)")
	$(call dump_var,"Current Options for ASM(*.s) files","$(ASM_OPTS)")
	$(call dump_var,"Current Options for LINKER files","$(LK_OPTS)")
	$(call dump_var,"Current Download Program","$(DWLOAD)")
	$(call dump_var,"Current Terminal Program","$(PUTTY)")
	$(call dump_var,"MAIN ROOT","$(ROOT_DIR)")
	$(call dump_var,"OUTPUT BUILD DIR","$(BUILD_DIR)")
	$(call dump_var,"All objs","$(OBJS)")
	@echo $(SEP)
	@$(MAKE) $(V) -C $(HAL) info 
	@echo $(SEP)

	
	

depend:	
	@echo $(SEP)
	@$(MAKE) $(V) -C $(HAL) depend 
	@echo $(SEP)

	



all_obj:		
	@echo $(SEP)
	@$(MAKE) $(V) -C $(HAL) all 	
	
	
	
link_file: all_obj
	 @$(MAKE) $(V) -C $(HAL) link_file
	
	

caOS.elf: link_file

dwload_1: kernel7.img
	$(call add_msg,"DOWNLOAD",$(BUILD_DIR)/caOS.hex)
	$(DWLOAD) $(BUILD_DIR)/caOS.hex


dwload: dwload_1 		
	@echo $(SEP)


kernel7.img: link_file
	
	

asm:		
	@echo $(SEP)
	@$(MAKE) $(V) -C $(HAL) asm 
	@echo $(SEP)
	@$(MAKE) $(V) -C $(HW) asm	
	@echo $(SEP)

	
clean:	
	@echo $(SEP)	
	@rm -rf  $(BUILD_DIR)/$(OBJ_OUT)
	@rm -rf  $(BUILD_DIR)
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(BUILD_DIR)/$(OBJ_OUT)
	@rm -f $(BUILD_DIR)/*.bin
	@rm -f $(BUILD_DIR)/*.hex
	@rm -f $(BUILD_DIR)/*.elf
	@rm -f $(BUILD_DIR)/*.lst
	@rm -f $(BUILD_DIR)/*.list
	@rm -f $(BUILD_DIR)/*.img
	@rm -f $(BUILD_DIR)/*.s
	@rm -f  trace-*
	@rm -f kernel7.img

	


all:  kernel7.img doversion
	
	
qemu:
	@echo $(SEP)
	qemu-system-arm -M raspi2 -cpu bcm2836 -m 1G   -kernel kernel7.img  &
	@echo $(SEP)
		

qemu-deb:
	@echo $(SEP)	
	@qemu-system-arm -M raspi2 -cpu bcm2836 -m 1G -s  -kernel kernel7.img  &
	@echo "target remote :1234" >gdb.conf
	@echo "file ./$(BUILD_DIR)/caOS.elf" >>gdb.conf
	@echo  $(EH)  $(C_RED )[GDB] $(C_RESET)
	@$(CROSS_GDB) -x gdb.conf
	@echo $(SEP)
		
jtag:   all
	@openocd -f ./raspi2.cfg &
	@sleep 1
	@echo "file $(BUILD_DIR)/caOS.elf" >gdb.conf
	@echo "target extended-remote :3333" >>gdb.conf
	#@echo "monitor reset init" >>gdb.conf
	@echo "monitor halt" >>gdb.conf
	@echo "monitor targets bcm2836.cpu0" >> gdb.conf
	@echo "load" >> gdb.conf
	@echo "continue" >> gdb.conf
	@echo  $(EH)  $(C_RED )[GDB] $(C_RESET)
	@echo  $(EH)  $(C_RESET)
	@$(CROSS_GDB) -x gdb.conf
	@killall openocd
	@echo jtag ...done.
	@echo $(SEP)

start_openocd: all
	@openocd -f ./raspi2.cfg &

stop_openocd:
	@killall openocd

sim:
	@echo $(SEP)
	echo "target sim" >gdb.conf
	echo "file $(BUILD_DIR)/caOS.elf" >>gdb.conf
	@echo $(EH)  $(C_RED )[GDB]$C_RESET
	@$(CROSS_GDB) -x gdb.conf
	@echo $(SEP)
		
	
		
distclean: clean 
	@rm -rf $(BUILD_DIR)/$(DEPEND)
	@rm -rf $(BUILD_DIR)




.PHONY: clean dwload qemu all all_obj distclean kernel7.img

