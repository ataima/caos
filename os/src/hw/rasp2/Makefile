#define includes 
INC+= -I $(ROOT_DIR)/inc/hw/rasp2 
 
SRC:=.

#SOURCE FILES
SRC_CPP:=$(wildcard $(SRC)/*.cpp) 

SRC_C:=$(wildcard $(SRC)/*.c)

SRC_ASM:=$(wildcard $(SRC)/*.s)


#OBJ FILES
OBJ_CPP:= $(patsubst $(SRC)/%.cpp,$(BUILD_DIR)/$(OBJ_OUT)/hw/%.o,$(SRC_CPP))

OBJ_C:=$(patsubst $(SRC)/%.c,$(BUILD_DIR)/$(OBJ_OUT)/hw/%.o,$(SRC_C))

OBJ_ASM:=$(patsubst $(SRC)/%.s,$(BUILD_DIR)/$(OBJ_OUT)/hw/%.o,$(SRC_ASM))



#DEPEND FILES
DEP_CPP:= $(patsubst $(SRC)/%.cpp,$(BUILD_DIR)/$(DEPEND)/hw/%.d,$(SRC_CPP))

DEP_C:=$(patsubst $(SRC)/%.c,$(BUILD_DIR)/$(DEPEND)/hw/%.d,$(SRC_C))


#GENERATED ASM FILES
ASM_CPP:= $(patsubst $(SRC)/%.cpp,$(BUILD_DIR)/$(GE_ASM)/hw/%.s,$(SRC_CPP))

ASM_C:=$(patsubst $(SRC)/%.c,$(BUILD_DIR)/$(GE_ASM)/hw/%.s,$(SRC_C))

ASM_S:=$(patsubst $(SRC)/%.s,$(BUILD_DIR)/$(GE_ASM)/hw/%.s,$(SRC_ASM))


ASM_OBJ:= $(ASM_CPP)  $(ASM_C) $(ASM_S) $(TEST_ASM_CPP)

#ALL OBJS
OBJS+= $(OBJ_C) $(OBJ_CPP)  $(OBJ_ASM) 

#ALL DEPEND
DEP_OBJ:= $(DEP_CPP)  $(DEP_C)  


define width_s40
    $(shell printf "%-35s %s" "$1")
endef


define add_cpp_msg_rules
    @echo "	@echo "$(EH)  $(I_GREEN)"[CPP]    :"$(I_WHITE)""\""$(call width_s40, $1)"\"" "$(I_CRED)$2$(I_RESET)">/dev/stderr">>$3
endef

define add_c_msg_rules
    @echo "	@echo "$(EH)  $(I_GREEN)"[C  ]    :"$(I_WHITE)""\""$(call width_s40, $1)"\"" "$(I_CRED)$2$(I_RESET)">/dev/stderr">>$3
endef

define add_list_msg
    @echo $(EH) $(C_GREEN)"[LST] :" $(C_WHITE)"$(call width_s40, $1)"$(C_RED)$2$(C_RESET) >/dev/stderr
endef

define add_asm_msg
    @echo $(EH) $(C_GREEN)"[AS ] :" $(C_WHITE)"$(call width_s40, $1)"$(C_RED)$2$(C_RESET) >/dev/stderr
endef


$(BUILD_DIR)/$(DEPEND)/hw/%.d:$(SRC)/%.cpp
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(BUILD_DIR)/$(DEPEND)
	@mkdir -p $(BUILD_DIR)/$(OBJ_OUT)
	@mkdir -p $(BUILD_DIR)/$(DEPEND)/hw
	@mkdir -p $(BUILD_DIR)/$(OBJ_OUT)/hw
	$(CROSS_CPP) $(CPP_OPTS) $(DEP_OPTS) $(BUILD_DIR)/$(OBJ_OUT)/hw/$(patsubst %.d,%.o,$(@F))  $(INC) -o $(BUILD_DIR)/$(DEPEND)/hw/$(@F) -c $<
	@echo "	"$(CROSS_CPP) $(INC)   $(CPP_OPTS)  -o $(BUILD_DIR)/$(OBJ_OUT)/hw/$(patsubst %.d,%.o,$(@F)) $< >>$(BUILD_DIR)/$(DEPEND)/hw/$(@F)
	$(call add_cpp_msg_rules, $< ,$(BUILD_DIR)/$(OBJ_OUT)/$(patsubst %.d,%.o,$(@F)), $@)
	@echo>>$@






$(BUILD_DIR)/$(DEPEND)/hw/%.d:$(SRC)/%.c
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(BUILD_DIR)/$(DEPEND)
	@mkdir -p $(BUILD_DIR)/$(OBJ_OUT)
	@mkdir -p $(BUILD_DIR)/$(DEPEND)/hw
	@mkdir -p $(BUILD_DIR)/$(OBJ_OUT)/hw	
	$(CROSS_C) $(C_OPTS) $(DEP_OPTS) $(BUILD_DIR)/$(OBJ_OUT)/hw/$(patsubst %.d,%.o,$(@F))  $(INC) -o $(BUILD_DIR)/$(DEPEND)/hw/$(@F) -c $<
	@echo "	"$(CROSS_C) $(INC)   $(C_OPTS)  -o $(BUILD_DIR)/$(OBJ_OUT)/hw/$(patsubst %.d,%.o,$(@F)) $< >>$(BUILD_DIR)/$(DEPEND)/hw/$(@F)
	$(call add_c_msg_rules, $< ,$(BUILD_DIR)/$(OBJ_OUT)/$(patsubst %.d,%.o,$(@F)), $@)
	@echo>>$@

#double pass : first create dependences with compile command so build it

-include $(DEP_OBJ) 

$(BUILD_DIR)/$(GE_ASM)/hw/%.s:$(SRC)/%.cpp
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(BUILD_DIR)/$(GE_ASM)
	$(CROSS_CPP) $(CPP_OPTS)  $(INC) -S -fverbose-asm -g -O2 -o $(BUILD_DIR)/$(GE_ASM)/$(@F) $<
	$(call add_list_msg,$<,$(BUILD_DIR)/$(GE_ASM)/$(patsubst %.cpp,%.s,$(@F)) )



$(BUILD_DIR)/$(GE_ASM)/hw/%.s:$(SRC)/%.c
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(BUILD_DIR)/$(GE_ASM)
	$(CROSS_C) $(C_OPTS)  $(INC) -S -o $(BUILD_DIR)/$(GE_ASM)/$(@F) -c $<
	$(call add_list_msg,$<,$(BUILD_DIR)/$(GE_ASM)/$(patsubst %.c,%.s,$(@F)))

$(BUILD_DIR)/$(GE_ASM)/hw/%.s:$(SRC)/%.s
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(BUILD_DIR)/$(GE_ASM)
	cp $< $(BUILD_DIR)/$(GE_ASM)/$(@F) 
	$(call add_list_msg,$<,$(BUILD_DIR)/$(GE_ASM)/$(patsubst %.s,%.s,$(@F)))

$(BUILD_DIR)/$(OBJ_OUT)/hw/%.o:$(SRC)/%.s
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(BUILD_DIR)/$(OBJ_OUT)
	$(CROSS_AS) -o $(BUILD_DIR)/$(OBJ_OUT)/hw/$(@F)  $(ASM_OPTS)  $<
	$(call add_asm_msg,$<,$(BUILD_DIR)/$(OBJ_OUT)/$(patsubst %.s,%.o,$(@F)))




define add_msg
    @echo $(EH) $(C_GREEN)"[$1] :" $(C_WHITE)"$(call width_s40, $2)"$(C_RED)$2$(C_RESET) >/dev/stderr
endef
	
	
link_file:
	$(CROSS_LD)  $(OBJS)  $(LK_OPTS) -o $(BUILD_DIR)/caOS.elf > $(BUILD_DIR)/caOS.map
	$(call add_msg,"LINKER ",$(BUILD_DIR)/caOS.elf)


objdump_file:
	@$(CROSS_OBJDUMP) -D $(BUILD_DIR)/caOS.elf -S > $(BUILD_DIR)/caOS.list
	$(call add_msg,"OBJDUMP",$(BUILD_DIR)/caOS.list)
	@$(CROSS_OBJCOPY) $(BUILD_DIR)/caOS.elf -O ihex $(BUILD_DIR)/caOS.hex
	$(call add_msg,"OBJCOPY",$(BUILD_DIR)/caOS.hex)
	@$(CROSS_OBJCOPY) $(BUILD_DIR)/caOS.elf -O binary $(BUILD_DIR)/caOS.bin
	$(call add_msg,"OBJCOPY",$(BUILD_DIR)/caOS.bin)
	@cp $(BUILD_DIR)/caOS.bin kernel7.img
	$(call add_msg,"OBJCOPY",kernel7.img)
	@$(CROSS_OBJDUMP) $(OBJD_CPU) -b ihex -D $(BUILD_DIR)/caOS.hex > $(BUILD_DIR)/caOS.s
	$(call add_msg,"OBJDUMP",$(BUILD_DIR)/caOS.s)
	@$(CROSS_OBJCOPY) --only-keep-debug $(BUILD_DIR)/caOS.elf $(BUILD_DIR)/caOS.syms
	$(call add_msg,"OBJDUMP",$(BUILD_DIR)/caOS.syms)


kernel7.img: link_file objdump_file

.PHONY: doversion all clean info dwload qemu asm

all: depend all_asm_file all_c_file all_cpp_file


depend: $(DEP_OBJ)

all_asm_file:	$(OBJ_ASM)

all_c_file:  	$(OBJ_C)

all_cpp_file: 	$(OBJ_CPP)
	


define dump_var    
    @echo  $(EH) $(C_YELLOW)"$(shell printf "%-30s %s = " $1)"$(C_WHITE)$2$(C_RESET)
endef


info:	
	$(call dump_var,"Assembler source","$(SRC_ASM)")
	$(call dump_var,"C source","$(SRC_C)")
	$(call dump_var,"CPP source","$(SRC_CPP) ")
	$(call dump_var,"Assembler objs","$(OBJ_ASM)")
	$(call dump_var,"C objs","$(OBJ_C)")
	$(call dump_var,"CPP objs","$(OBJ_CPP) ")
	$(call dump_var,"All objs","$(OBJS)")
	$(call dump_var,"All dep","$(DEP_OBJ)")
	$(call dump_var,"Asm","$(ASM_OBJ)")
	

asm:    $(ASM_OBJ)


