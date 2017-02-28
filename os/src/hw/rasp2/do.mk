# set environment variables
include rules.mk
#define includes 
INC=-I inc -I inc/hal -I inc/hw/rasp2 
 
#dependences and objs 

SRC:=src/hw/rasp2

#SOURCE FILES
SRC_CPP:=$(wildcard $(SRC)/*.cpp) 

SRC_C:=$(wildcard $(SRC)/*.c)

SRC_ASM:=$(wildcard $(SRC)/*.s)


#OBJ FILES
OBJ_CPP:= $(patsubst $(SRC)/%.cpp,$(BUILDIR)/$(OBJ_OUT)/hw/%.o,$(SRC_CPP))

OBJ_C:=$(patsubst $(SRC)/%.c,$(BUILDIR)/$(OBJ_OUT)/hw/%.o,$(SRC_C))

OBJ_ASM:=$(patsubst $(SRC)/%.s,$(BUILDIR)/$(OBJ_OUT)/hw/%.o,$(SRC_ASM))



#DEPEND FILES
DEP_CPP:= $(patsubst $(SRC)/%.cpp,$(BUILDIR)/$(DEPEND)/hw/%.d,$(SRC_CPP))

DEP_C:=$(patsubst $(SRC)/%.c,$(BUILDIR)/$(DEPEND)/hw/%.d,$(SRC_C))


#GENERATED ASM FILES
ASM_CPP:= $(patsubst $(SRC)/%.cpp,$(BUILDIR)/$(GE_ASM)/hw/%.s,$(SRC_CPP))

ASM_C:=$(patsubst $(SRC)/%.c,$(BUILDIR)/$(GE_ASM)/hw/%.s,$(SRC_C))

ASM_S:=$(patsubst $(SRC)/%.s,$(BUILDIR)/$(GE_ASM)/hw/%.s,$(SRC_ASM))


ASM_OBJ:= $(ASM_CPP)  $(ASM_C) $(ASM_S) $(TEST_ASM_CPP)

#ALL OBJS
OBJS:= $(OBJ_C) $(OBJ_CPP)  $(OBJ_ASM) 

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


$(BUILDIR)/$(DEPEND)/hw/%.d:$(SRC)/%.cpp
	@mkdir -p $(BUILDIR)
	@mkdir -p $(BUILDIR)/$(DEPEND)
	@mkdir -p $(BUILDIR)/$(OBJ_OUT)
	@mkdir -p $(BUILDIR)/$(DEPEND)/hw
	@mkdir -p $(BUILDIR)/$(OBJ_OUT)/hw
	$(CROSS_CPP) $(CPP_OPTS) $(DEP_OPTS) $(BUILDIR)/$(OBJ_OUT)/hw/$(patsubst %.d,%.o,$(@F))  $(INC) -o $(BUILDIR)/$(DEPEND)/hw/$(@F) -c $<
	@echo "	"$(CROSS_CPP) $(INC)   $(CPP_OPTS)  -o $(BUILDIR)/$(OBJ_OUT)/hw/$(patsubst %.d,%.o,$(@F)) $< >>$(BUILDIR)/$(DEPEND)/hw/$(@F)
	$(call add_cpp_msg_rules, $< ,$(BUILDIR)/$(OBJ_OUT)/$(patsubst %.d,%.o,$(@F)), $@)
	@echo>>$@






$(BUILDIR)/$(DEPEND)/hw/%.d:$(SRC)/%.c
	@mkdir -p $(BUILDIR)
	@mkdir -p $(BUILDIR)/$(DEPEND)
	@mkdir -p $(BUILDIR)/$(OBJ_OUT)
	@mkdir -p $(BUILDIR)/$(DEPEND)/hw
	@mkdir -p $(BUILDIR)/$(OBJ_OUT)/hw	
	$(CROSS_C) $(C_OPTS) $(DEP_OPTS) $(BUILDIR)/$(OBJ_OUT)/hw/$(patsubst %.d,%.o,$(@F))  $(INC) -o $(BUILDIR)/$(DEPEND)/hw/$(@F) -c $<
	@echo "	"$(CROSS_C) $(INC)   $(C_OPTS)  -o $(BUILDIR)/$(OBJ_OUT)/hw/$(patsubst %.d,%.o,$(@F)) $< >>$(BUILDIR)/$(DEPEND)/hw/$(@F)
	$(call add_c_msg_rules, $< ,$(BUILDIR)/$(OBJ_OUT)/$(patsubst %.d,%.o,$(@F)), $@)
	@echo>>$@

#double pass : first create dependences with compile command so build it

-include $(DEP_OBJ) 

$(BUILDIR)/$(GE_ASM)/hw/%.s:$(SRC)/%.cpp
	@mkdir -p $(BUILDIR)
	@mkdir -p $(BUILDIR)/$(GE_ASM)
	$(CROSS_CPP) $(CPP_OPTS)  $(INC) -S -fverbose-asm -g -O2 -o $(BUILDIR)/$(GE_ASM)/$(@F) $<
	$(call add_list_msg,$<,$(BUILDIR)/$(GE_ASM)/$(patsubst %.cpp,%.s,$(@F)) )



$(BUILDIR)/$(GE_ASM)/hw/%.s:$(SRC)/%.c
	@mkdir -p $(BUILDIR)
	@mkdir -p $(BUILDIR)/$(GE_ASM)
	$(CROSS_C) $(C_OPTS)  $(INC) -S -o $(BUILDIR)/$(GE_ASM)/$(@F) -c $<
	$(call add_list_msg,$<,$(BUILDIR)/$(GE_ASM)/$(patsubst %.c,%.s,$(@F)))

$(BUILDIR)/$(GE_ASM)/hw/%.s:$(SRC)/%.s
	@mkdir -p $(BUILDIR)
	@mkdir -p $(BUILDIR)/$(GE_ASM)
	cp $< $(BUILDIR)/$(GE_ASM)/$(@F) 
	$(call add_list_msg,$<,$(BUILDIR)/$(GE_ASM)/$(patsubst %.s,%.s,$(@F)))

$(BUILDIR)/$(OBJ_OUT)/hw/%.o:$(SRC)/%.s
	@mkdir -p $(BUILDIR)
	@mkdir -p $(BUILDIR)/$(OBJ_OUT)
	$(CROSS_AS) -o $(BUILDIR)/$(OBJ_OUT)/$(@F)  $(ASM_OPTS)  $<
	$(call add_asm_msg,$<,$(BUILDIR)/$(OBJ_OUT)/$(patsubst %.s,%.o,$(@F)))


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
	$(call dump_var,"Current HARDWARE = ", "$(HARDWARE)")
	$(call dump_var,"Current TOOLS CHAIN = ", "$(ARMGNU) ")
	$(call dump_var,"Current TOOLS CHAIN PATH= ", "$(ARMPATH)")
	$(call dump_var,"Current TOOLS CHAIN CC= ", "$(CROSS_CC)")
	$(call dump_var,"Current TOOLS CHAIN CPP= ", "$(CROSS_CPP) ")
	$(call dump_var,"Current TOOLS CHAIN AS= ", "$(CROSS_AS) ")
	$(call dump_var,"Current TOOLS CHAIN LD= ", "$(CROSS_LD)")
	$(call dump_var,"Current TOOLS CHAIN OBJDUMP= ", "$(CROSS_OBJDUMP) ")
	$(call dump_var,"Current TOOLS CHAIN OBJCOPY= ", "$(CROSS_OBJCOPY)")
	$(call dump_var,"Current TOOLS CHAIN LD= ", "$(CROSS_GDB)")
	$(call dump_var,"Current Debug level = " , "$(DBG)")
	$(call dump_var,"Current Options for depend  files = " , "$(DEP_OPTS)")
	$(call dump_var,"Current Options for C(*.c) files = " , "$(C_OPTS)")
	$(call dump_var,"Current Options for CPP(*.cpp) files = " , "$(CPP_OPTS)")
	$(call dump_var,"Current Options for ASM(*.s) files = " , "$(ASM_OPTS)")
	$(call dump_var,"Current Options for LINKER files = " , "$(LK_OPT)")
	$(call dump_var,"Current Download Program = " , "$(DWLOAD)")
	$(call dump_var,"Current Terminal Program = " , "$(PUTTY)")
	$(call dump_var,"Assembler source = ", "$(SRC_ASM)")
	$(call dump_var,"C source = ", "$(SRC_C)")
	$(call dump_var,"CPP source = ", "$(SRC_CPP) $(TEST_CPP)")
	$(call dump_var,"Assembler objs = ", "$(OBJ_ASM)")
	$(call dump_var,"C objs = ", "$(OBJ_C)")
	$(call dump_var,"CPP objs = ", "$(OBJ_CPP) $(TEST_OBJ_CPP)")
	$(call dump_var,"All objs = ", "$(OBJS)")
	$(call dump_var,"All dep = ", "$(DEP_OBJ)")
	$(call dump_var,"Asm = ", "$(ASM_OBJ)")
	$(call dump_var,"MAIN ROOT = ", "$(ROOT)")
	$(call dump_var,"OUTPUT BUILD DIR = " , "$(BUILDIR)")
	

asm:    $(ASM_OBJ)


