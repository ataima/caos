# set environment variables
include rules.mk
#define includes 
INC=-I inc 

#dependences and objs 

#SOURCE FILES
SRC_CPP:=$(wildcard $(SRC)/*.cpp)

SRC_C:=$(wildcard $(SRC)/*.c)

SRC_ASM:=$(wildcard $(SRC)/*.s)

#test
TEST_CPP:=$(wildcard $(TEST)/*.cpp)

#OBJ FILES
OBJ_CPP:= $(patsubst $(SRC)/%.cpp,$(BUILDIR)/$(OBJ_OUT)/%.o,$(SRC_CPP))

OBJ_C:=$(patsubst $(SRC)/%.c,$(BUILDIR)/$(OBJ_OUT)/%.o,$(SRC_C))

OBJ_ASM:=$(patsubst $(SRC)/%.s,$(BUILDIR)/$(OBJ_OUT)/%.o,$(SRC_ASM))

TEST_OBJ_CPP:= $(patsubst $(TEST)/%.cpp,$(BUILDIR)/$(OBJ_OUT)/%.o,$(TEST_CPP))


#DEPEND FILES
DEP_CPP:= $(patsubst $(SRC)/%.cpp,$(BUILDIR)/$(DEPEND)/%.d,$(SRC_CPP))

DEP_C:=$(patsubst $(SRC)/%.c,$(BUILDIR)/$(DEPEND)/%.d,$(SRC_C))

TEST_DEP_CPP:= $(patsubst $(TEST)/%.cpp,$(BUILDIR)/$(DEPEND)/%.d,$(TEST_CPP))

#GENERATED ASM FILES
ASM_CPP:= $(patsubst $(SRC)/%.cpp,$(BUILDIR)/$(GE_ASM)/%.s,$(SRC_CPP))

ASM_C:=$(patsubst $(SRC)/%.c,$(BUILDIR)/$(GE_ASM)/%.s,$(SRC_C))

ASM_S:=$(patsubst $(SRC)/%.s,$(BUILDIR)/$(GE_ASM)/%.s,$(SRC_ASM))

TEST_ASM_CPP:= $(patsubst $(TEST)/%.cpp,$(BUILDIR)/$(GE_ASM)/%.s,$(TEST_CPP))

ASM_OBJ:= $(ASM_CPP)  $(ASM_C) $(ASM_S) $(TEST_ASM_CPP)

#ALL OBJS
OBJS:= $(OBJ_C) $(OBJ_CPP) $(TEST_OBJ_CPP) $(OBJ_ASM) 

#ALL DEPEND
DEP_OBJ:= $(DEP_CPP)  $(DEP_C) $(TEST_DEP_CPP)

$(BUILDIR)/$(DEPEND)/%.d:$(SRC)/%.cpp
	@mkdir -p $(BUILDIR)
	@mkdir -p $(BUILDIR)/$(DEPEND)
	@mkdir -p $(BUILDIR)/$(OBJ_OUT)
	$(CROSS_CPP) $(CPP_OPTS) $(DEP_OPTS) $(BUILDIR)/$(OBJ_OUT)/$(patsubst %.d,%.o,$(@F))  $(INC) -o $(BUILDIR)/$(DEPEND)/$(@F) -c $<
	@echo "	"$(CROSS_CPP) $(INC)   $(CPP_OPTS)  -o $(BUILDIR)/$(OBJ_OUT)/$(patsubst %.d,%.o,$(@F)) $< >>$(BUILDIR)/$(DEPEND)/$(@F)
	@echo "	@echo "$(EH)  $(I_CRED)"[CPP]"$(I_RESET)" $<"$(I_RED)$(I_TAB)": "$(I_RESET) $(I_GREEN)"$(BUILDIR)/$(OBJ_OUT)/$(patsubst %.d,%.o,$(@F))"$(I_RESET)">/dev/stderr">>$@
	@echo>>$@

$(BUILDIR)/$(DEPEND)/%.d:$(TEST)/%.cpp
	@mkdir -p $(BUILDIR)
	@mkdir -p $(BUILDIR)/$(DEPEND)
	@mkdir -p $(BUILDIR)/$(OBJ_OUT)
	$(CROSS_CPP) $(CPP_OPTS) $(DEP_OPTS) $(BUILDIR)/$(OBJ_OUT)/$(patsubst %.d,%.o,$(@F))  $(INC) -o $(BUILDIR)/$(DEPEND)/$(@F) -c $<
	@echo "	"$(CROSS_CPP) $(INC)   $(CPP_OPTS)  -o $(BUILDIR)/$(OBJ_OUT)/$(patsubst %.d,%.o,$(@F)) $< >>$(BUILDIR)/$(DEPEND)/$(@F)
	@echo "	@echo "$(EH)  $(I_CPURPLE)"[TEST]"$(I_RESET)" $<"$(I_RED)$(I_TAB)": "$(I_RESET) $(I_GREEN)"$(BUILDIR)/$(OBJ_OUT)/$(patsubst %.d,%.o,$(@F))"$(I_RESET)">/dev/stderr">>$@
	@echo>>$@




$(BUILDIR)/$(DEPEND)/%.d:$(SRC)/%.c
	@mkdir -p $(BUILDIR)
	@mkdir -p $(BUILDIR)/$(DEPEND)
	@mkdir -p $(BUILDIR)/$(OBJ_OUT)
	$(CROSS_C) $(C_OPTS) $(DEP_OPTS) $(BUILDIR)/$(OBJ_OUT)/$(patsubst %.d,%.o,$(@F))  $(INC) -o $(BUILDIR)/$(DEPEND)/$(@F) -c $<
	@echo "	"$(CROSS_C) $(INC)   $(C_OPTS)  -o $(BUILDIR)/$(OBJ_OUT)/$(patsubst %.d,%.o,$(@F)) $< >>$(BUILDIR)/$(DEPEND)/$(@F)
	@echo "	@echo "$(EH)  $(I_CRED)"[ C ]"$(I_RESET)" $<"$(I_RED)$(I_TAB)": "$(I_RESET) $(I_GREEN)"$(BUILDIR)/$(OBJ_OUT)/$(patsubst %.d,%.o,$(@F))"$(I_RESET)">/dev/stderr">>$@
	@echo>>$@

#double pass : first create dependences with compile command so build it

-include $(DEP_OBJ) 

$(BUILDIR)/$(GE_ASM)/%.s:$(SRC)/%.cpp
	@mkdir -p $(BUILDIR)
	@mkdir -p $(BUILDIR)/$(GE_ASM)
	$(CROSS_CPP) $(CPP_OPTS)  $(INC) -S -fverbose-asm -g -O2 -o $(BUILDIR)/$(GE_ASM)/$(@F) $<
	@echo $(EH) $(C_CYAN)"[LST]"$(C_RESET)" $<"$(C_CYAN)"\t: "$(C_GREEN)"$(BUILDIR)/$(OBJ_OUT)/$(patsubst %.cpp,%.s,$(@F))" $(C_RESET) >/dev/stderr

$(BUILDIR)/$(GE_ASM)/%.s:$(TEST)/%.cpp
	@mkdir -p $(BUILDIR)
	@mkdir -p $(BUILDIR)/$(GE_ASM)
	$(CROSS_CPP) $(CPP_OPTS)  $(INC) -S -fverbose-asm -g -O2 -o $(BUILDIR)/$(GE_ASM)/$(@F)  $<
	@echo  $(EH) $(C_CYAN)"[LST]"$(C_RESET)" $<"$(C_CYAN)"\t: "$(C_GREEN)"$(BUILDIR)/$(OBJ_OUT)/$(patsubst %.cpp,%.s,$(@F))" $(C_RESET) >/dev/stderr


$(BUILDIR)/$(GE_ASM)/%.s:$(SRC)/%.c
	@mkdir -p $(BUILDIR)
	@mkdir -p $(BUILDIR)/$(GE_ASM)
	$(CROSS_C) $(C_OPTS)  $(INC) -S -o $(BUILDIR)/$(GE_ASM)/$(@F) -c $<
	@echo  $(EH) $(C_CYAN)"[LST]"$(C_RESET)" $<"$(C_CYAN)"\t: "$(C_GREEN)"$(BUILDIR)/$(OBJ_OUT)/$(patsubst %.c,%.s,$(@F))" $(C_RESET) >/dev/stderr

$(BUILDIR)/$(GE_ASM)/%.s:$(SRC)/%.s
	@mkdir -p $(BUILDIR)
	@mkdir -p $(BUILDIR)/$(GE_ASM)
	cp $< $(BUILDIR)/$(GE_ASM)/$(@F) 
	@echo  $(EH) $(C_CYAN)"[LST]"$(C_RESET)" $<"$(C_CYAN)"\t: "$(C_GREEN)"$(BUILDIR)/$(OBJ_OUT)/$(patsubst %.s,%.s,$(@F))" $(C_RESET) >/dev/stderr

$(BUILDIR)/$(OBJ_OUT)/%.o:$(SRC)/%.s
	@mkdir -p $(BUILDIR)
	@mkdir -p $(BUILDIR)/$(OBJ_OUT)
	$(CROSS_AS) -o $(BUILDIR)/$(OBJ_OUT)/$(@F)  $(ASM_OPTS)  $<
	@echo  $(EH) $(C_BLUE)"[AS ]"$(C_RESET)" $<"$(C_BLUE)"\t: "$(C_GREEN)"$(BUILDIR)/$(OBJ_OUT)/$(patsubst %.s,%.o,$(@F))" $(C_RESET) >/dev/stderr


.PHONY: doversion all clean info dwload qemu asm

all: doversion kernel7.img

distclean: clean 
	@rm -rf $(BUILDIR)/$(DEPEND)
	@rm -rf $(BUILDIR)

clean :
	@rm -rf $(BUILDIR)/$(OBJ_OUT)
	@rm -f $(BUILDIR)/*.bin
	@rm -f $(BUILDIR)/*.hex
	@rm -f $(BUILDIR)/*.elf
	@rm -f $(BUILDIR)/*.lst
	@rm -f $(BUILDIR)/*.list
	@rm -f $(BUILDIR)/*.img
	@rm -f $(BUILDIR)/*.s
	@rm -f  trace-*
	@rm -f kernel7.img
	


depend: $(DEP_OBJ)

all_asm_file:	$(OBJ_ASM)

all_c_file:  	$(OBJ_C)

all_cpp_file: 	$(OBJ_CPP)
	
all_test_file: 	$(TEST_OBJ_CPP)	



link_file:
	$(CROSS_LD) $(TEST_OBJ_CPP) $(OBJ_CPP)  $(OBJ_C) $(OBJ_ASM) -M $(LK_OPT) -T ld_conf/BCM2836.ld -o $(BUILDIR)/caOS.elf > $(BUILDIR)/caOS.map
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
	$(CROSS_OBJDUMP) -m arm -b ihex -D $(BUILDIR)/caOS.hex > $(BUILDIR)/caOS.s
	@echo  $(EH) $(C_GREEN)"[OBJDUMP]"$(C_RESET)" $(BUILDIR)/caOS.s">/dev/stderr

dwload: kernel7.img
	@echo  $(EH) $(C_GREEN)"[DOWNLOAD]"$(C_RESET)" $(BUILDIR)/caOS.hex">/dev/stderr
	@$(DWLOAD) $(BUILDIR)/caOS.hex
	@$(PUTTY)  

kernel7.img :   depend all_asm_file all_c_file all_cpp_file all_test_file link_file objdump_file


info:	
	@echo  $(EH) $(C_YELLOW)"Current TOOLS CHAIN = "$(C_RESET)$(ARMGNU) 
	@echo  $(EH) $(C_YELLOW)"Current TOOLS CHAIN PATH= "$(C_RESET)$(ARMPATH)
	@echo  $(EH) $(C_YELLOW)"Current TOOLS CHAIN CC= "$(C_RESET)$(CROSS_CC)
	@echo  $(EH) $(C_YELLOW)"Current TOOLS CHAIN CPP= "$(C_RESET)$(CROSS_CPP) 
	@echo  $(EH) $(C_YELLOW)"Current TOOLS CHAIN AS= "$(C_RESET)$(CROSS_AS) 
	@echo  $(EH) $(C_YELLOW)"Current TOOLS CHAIN LD= "$(C_RESET)$(CROSS_LD)
	@echo  $(EH) $(C_YELLOW)"Current TOOLS CHAIN OBJDUMP= "$(C_RESET)$(CROSS_OBJDUMP) 
	@echo  $(EH) $(C_YELLOW)"Current TOOLS CHAIN OBJCOPY= "$(C_RESET)$(CROSS_OBJCOPY)
	@echo  $(EH) $(C_YELLOW)"Current TOOLS CHAIN LD= "$(C_RESET)$(CROSS_GDB)
	@echo  $(EH) $(C_YELLOW)"Current Debug level = " $(C_RESET)$(DBG)
	@echo  $(EH) $(C_YELLOW)"Current Options for depend  files = " $(C_RESET)$(DEP_OPTS)
	@echo  $(EH) $(C_YELLOW)"Current Options for C(*.c) files = " $(C_RESET)$(C_OPTS)
	@echo  $(EH) $(C_YELLOW)"Current Options for CPP(*.cpp) files = " $(C_RESET)$(CPP_OPTS)
	@echo  $(EH) $(C_YELLOW)"Current Options for ASM(*.s) files = " $(C_RESET)$(ASM_OPTS)
	@echo  $(EH) $(C_YELLOW)"Current Options for LINKER files = " $(C_RESET)$(LK_OPT)
	@echo  $(EH) $(C_YELLOW)"Current Download Program = " $(C_RESET)$(DWLOAD)
	@echo  $(EH) $(C_YELLOW)"Current Terminal Program = " $(C_RESET)$(PUTTY)
	@echo  $(EH) $(C_YELLOW)"Assembler source = "$(C_RESET)$(SRC_ASM)
	@echo  $(EH) $(C_YELLOW)"C source = "$(C_RESET)$(SRC_C)
	@echo  $(EH) $(C_YELLOW)"CPP source = "$(C_RESET)$(SRC_CPP) $(TEST_CPP)
	@echo  $(EH) $(C_YELLOW)"Assembler objs = "$(C_RESET)$(OBJ_ASM)
	@echo  $(EH) $(C_YELLOW)"C objs = "$(C_RESET)$(OBJ_C)
	@echo  $(EH) $(C_YELLOW)"CPP objs = "$(C_RESET)$(OBJ_CPP) $(TEST_OBJ_CPP)
	@echo  $(EH) $(C_YELLOW)"All objs = "$(C_RESET)$(OBJS)
	@echo  $(EH) $(C_YELLOW)"All dep = "$(C_RESET)$(DEP_OBJ)
	@echo  $(EH) $(C_YELLOW)"Asm = "$(C_RESET)$(ASM_OBJ)
	@echo  $(EH) $(C_YELLOW)"MAIN ROOT = "$(C_RESET)$(ROOT)
	@echo  $(EH) $(C_YELLOW)"OUTPUT BUILD DIR = " $(C_RESET)$(BUILDIR)
	

qemu:
	qemu-system-arm.exe -M raspi2 -cpu bcm2836 -m 128M   -kernel $(BUILDIR)/caOS.bin  &



qemu_deb:
	qemu-system-arm.exe -M raspi2 -cpu bcm2836 -m 128M -s  -kernel $(BUILDIR)/caOS.bin  &
	echo "target remote :1234" >gdb.conf
	echo "file $(BUILDIR)/caos.elf" >>gdb.conf
	@echo  $(EH)  $(C_RED )[GDB] $(C_RESTORE)
	@$(CROSS_GDB) -x gdb.conf


sim:
	echo "target sim" >gdb.conf
	echo "file $(BUILDIR)/caos.elf" >>gdb.conf
	@echo $(EH)  $(C_RED )[GDB]$C_RESTORE
	@$(CROSS_GDB) -x gdb.conf

asm:    $(ASM_OBJ)


doversion :
	$(eval BUILD_NUMBER=`./version.sh $(CAOS_VERSION_1) $(CAOS_VERSION_2) $(CAOS_VERSION_3)`)
	@echo $(EH)  $(C_GREEN)[VERSION] $(C_WHITE)= Vers. $(C_MAGENTA)$(CAOS_VERSION_1)$(C_WHITE)"."$(C_YELLOW)$(CAOS_VERSION_2)$(C_WHITE)"."$(C_PURPLE)$(CAOS_VERSION_3) $(C_WHITE)Build Number $(C_RED)$(BUILD_NUMBER) $(C_RESTORE) 