
SRC:=.

#SOURCE FILES
SRC_CPP:=$(wildcard $(SRC)/*.cpp) 


SRC_C:=$(wildcard $(SRC)/*.c)


SRC_ASM:=$(wildcard $(SRC)/*.s)

#OBJ FILES
OBJ_CPP:= $(patsubst $(SRC)/%.cpp,$(BUILD_DIR)/$(OBJ_OUT)/%.o,$(SRC_CPP))

OBJ_C:=$(patsubst $(SRC)/%.c,$(BUILD_DIR)/$(OBJ_OUT)/%.o,$(SRC_C))

OBJ_ASM:=$(patsubst $(SRC)/%.s,$(BUILD_DIR)/$(OBJ_OUT)/%.o,$(SRC_ASM))



#DEPEND FILES
DEP_CPP:= $(patsubst $(SRC)/%.cpp,$(BUILD_DIR)/$(DEPEND)/%.d,$(SRC_CPP))

DEP_C:=$(patsubst $(SRC)/%.c,$(BUILD_DIR)/$(DEPEND)/%.d,$(SRC_C))


#GENERATED ASM FILES
ASM_CPP:= $(patsubst $(SRC)/%.cpp,$(BUILD_DIR)/$(GE_ASM)/%.s,$(SRC_CPP))

ASM_C:=$(patsubst $(SRC)/%.c,$(BUILD_DIR)/$(GE_ASM)/%.s,$(SRC_C))

ASM_S:=$(patsubst $(SRC)/%.s,$(BUILD_DIR)/$(GE_ASM)/%.s,$(SRC_ASM))


ASM_OBJ:= $(ASM_CPP)  $(ASM_C) $(ASM_S) 

#ALL OBJS
OBJS+= $(OBJ_C) $(OBJ_CPP)  $(OBJ_ASM) 

#ALL DEPEND
DEP_OBJ:= $(DEP_CPP)  $(DEP_C) 

export define width_s40
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

$(BUILD_DIR)/$(DEPEND)/%.d:$(SRC)/%.cpp
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(BUILD_DIR)/$(DEPEND)
	@mkdir -p $(BUILD_DIR)/$(OBJ_OUT)
	$(CROSS_CPP) $(CPP_OPTS) $(DEP_OPTS) $(BUILD_DIR)/$(OBJ_OUT)/$(patsubst %.d,%.o,$(@F))  $(INC) -o $(BUILD_DIR)/$(DEPEND)/$(@F) -c $<
	@echo "	"$(CROSS_CPP) $(INC)   $(CPP_OPTS)  -o $(BUILD_DIR)/$(OBJ_OUT)/$(patsubst %.d,%.o,$(@F)) $< >>$(BUILD_DIR)/$(DEPEND)/$(@F)
	$(call add_cpp_msg_rules, $< ,$(BUILD_DIR)/$(OBJ_OUT)/$(patsubst %.d,%.o,$(@F)), $@)
	@echo>>$@


$(BUILD_DIR)/$(DEPEND)/%.d:$(SRC)/%.c
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(BUILD_DIR)/$(DEPEND)
	@mkdir -p $(BUILD_DIR)/$(OBJ_OUT)	
	$(CROSS_C) $(C_OPTS) $(DEP_OPTS) $(BUILD_DIR)/$(OBJ_OUT)/$(patsubst %.d,%.o,$(@F))  $(INC) -o $(BUILD_DIR)/$(DEPEND)/$(@F) -c $<
	@echo "	"$(CROSS_C) $(INC)   $(C_OPTS)  -o $(BUILD_DIR)/$(OBJ_OUT)/$(patsubst %.d,%.o,$(@F)) $< >>$(BUILD_DIR)/$(DEPEND)/$(@F)
	$(call add_c_msg_rules, $< ,$(BUILD_DIR)/$(OBJ_OUT)/$(patsubst %.d,%.o,$(@F)), $@)
	@echo>>$@

#double pass : first create dependences with compile command so build it

-include $(DEP_OBJ) 

$(BUILD_DIR)/$(GE_ASM)/%.s:$(SRC)/%.cpp
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(BUILD_DIR)/$(GE_ASM)
	$(CROSS_CPP) $(CPP_OPTS)  $(INC) -S -fverbose-asm -g -O2 -o $(BUILD_DIR)/$(GE_ASM)/$(@F) $<
	$(call add_list_msg,$<,$(BUILD_DIR)/$(GE_ASM)/$(patsubst %.cpp,%.s,$(@F)) )

$(BUILD_DIR)/$(GE_ASM)/%.s:$(SRC)/%.c
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(BUILD_DIR)/$(GE_ASM)
	$(CROSS_C) $(C_OPTS)  $(INC) -S -o $(BUILD_DIR)/$(GE_ASM)/$(@F) -c $<
	$(call add_list_msg,$<,$(BUILD_DIR)/$(GE_ASM)/$(patsubst %.c,%.s,$(@F)))

$(BUILD_DIR)/$(GE_ASM)/%.s:$(SRC)/%.s
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(BUILD_DIR)/$(GE_ASM)
	cp $< $(BUILD_DIR)/$(GE_ASM)/$(@F) 
	$(call add_list_msg,$<,$(BUILD_DIR)/$(GE_ASM)/$(patsubst %.s,%.s,$(@F)))

$(BUILD_DIR)/$(OBJ_OUT)/%.o:$(SRC)/%.s
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(BUILD_DIR)/$(OBJ_OUT)
	$(CROSS_AS) -o $(BUILD_DIR)/$(OBJ_OUT)/$(@F)  $(ASM_OPTS)  $<
	$(call add_asm_msg,$<,$(BUILD_DIR)/$(OBJ_OUT)/$(patsubst %.s,%.o,$(@F)))


.PHONY: doversion all clean info dwload qemu asm

all: depend all_asm_file all_c_file all_cpp_file
	@echo $(SEP)
	@$(MAKE) $(V) -C $(HW) all

depend: $(DEP_OBJ)
	@echo $(SEP)
	@$(MAKE) $(V) -C $(HW) depend

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
	@echo $(SEP)
	@$(MAKE) $(V) -C $(HW) info

asm:    $(ASM_OBJ)
	@echo $(SEP)
	@$(MAKE) $(V) -C $(HW) asm

link_file:
	@$(MAKE) $(V) -C $(HW) link_file


kernel7.img: 	
	@$(MAKE) $(V) -C $(HW) kernel7.img