# set environment variables
include rules.mk
#define includes 
INC=-I inc -I inc/hal -I inc/hw/simula 
 
#dependences and objs 

SRC:=src/hw/simula


#SOURCE FILES
SRC_CPP:=$(wildcard $(SRC)/*.cpp) 

SRC_C:=$(wildcard $(SRC)/*.c)

SRC_ASM:=$(wildcard $(SRC)/*.s)


#test


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

$(BUILDIR)/$(DEPEND)/hw/%.d:$(SRC)/%.cpp
	@mkdir -p $(BUILDIR)
	@mkdir -p $(BUILDIR)/$(DEPEND)
	@mkdir -p $(BUILDIR)/$(OBJ_OUT)
	@mkdir -p $(BUILDIR)/$(DEPEND)/hw
	@mkdir -p $(BUILDIR)/$(OBJ_OUT)/hw	
	$(CROSS_CPP) $(CPP_OPTS) $(DEP_OPTS) $(BUILDIR)/$(OBJ_OUT)/hw/$(patsubst %.d,%.o,$(@F))  $(INC) -o $(BUILDIR)/$(DEPEND)/hw/$(@F) -c $<
	@echo "	"$(CROSS_CPP) $(INC)   $(CPP_OPTS)  -o $(BUILDIR)/$(OBJ_OUT)/hw/$(patsubst %.d,%.o,$(@F)) $< >>$(BUILDIR)/$(DEPEND)/hw/$(@F)
	@echo "	@echo "$(EH)  $(I_CRED)"[CPP]"$(I_RESET)" $<"$(I_RED)$(I_TAB)": "$(I_RESET) $(I_GREEN)"$(BUILDIR)/$(OBJ_OUT)/$(patsubst %.d,%.o,$(@F))"$(I_RESET)">/dev/stderr">>$@
	@echo>>$@






$(BUILDIR)/$(DEPEND)/hw/%.d:$(SRC)/%.c
	@mkdir -p $(BUILDIR)
	@mkdir -p $(BUILDIR)/$(DEPEND)
	@mkdir -p $(BUILDIR)/$(OBJ_OUT)
	@mkdir -p $(BUILDIR)/$(DEPEND)/hw
	@mkdir -p $(BUILDIR)/$(OBJ_OUT)/hw
	$(CROSS_C) $(C_OPTS) $(DEP_OPTS) $(BUILDIR)/$(OBJ_OUT)/hw/$(patsubst %.d,%.o,$(@F))  $(INC) -o $(BUILDIR)/$(DEPEND)/hw/$(@F) -c $<
	@echo "	"$(CROSS_C) $(INC)   $(C_OPTS)  -o $(BUILDIR)/$(OBJ_OUT)/hw/$(patsubst %.d,%.o,$(@F)) $< >>$(BUILDIR)/$(DEPEND)/hw/$(@F)
	@echo "	@echo "$(EH)  $(I_CRED)"[ C ]"$(I_RESET)" $<"$(I_RED)$(I_TAB)": "$(I_RESET) $(I_GREEN)"$(BUILDIR)/$(OBJ_OUT)/$(patsubst %.d,%.o,$(@F))"$(I_RESET)">/dev/stderr">>$@
	@echo>>$@

#double pass : first create dependences with compile command so build it

-include $(DEP_OBJ) 

$(BUILDIR)/$(GE_ASM)/hw/%.s:$(SRC)/%.cpp
	@mkdir -p $(BUILDIR)
	@mkdir -p $(BUILDIR)/$(GE_ASM)
	$(CROSS_CPP) $(CPP_OPTS)  $(INC) -S -fverbose-asm -g -O2 -o $(BUILDIR)/$(GE_ASM)/$(@F) $<
	@echo $(EH) $(C_CYAN)"[LST]"$(C_RESET)" $<"$(C_CYAN)"\t: "$(C_GREEN)"$(BUILDIR)/$(OBJ_OUT)/$(patsubst %.cpp,%.s,$(@F))" $(C_RESET) >/dev/stderr



$(BUILDIR)/$(GE_ASM)/hw/%.s:$(SRC)/%.c
	@mkdir -p $(BUILDIR)
	@mkdir -p $(BUILDIR)/$(GE_ASM)
	$(CROSS_C) $(C_OPTS)  $(INC) -S -o $(BUILDIR)/$(GE_ASM)/$(@F) -c $<
	@echo  $(EH) $(C_CYAN)"[LST]"$(C_RESET)" $<"$(C_CYAN)"\t: "$(C_GREEN)"$(BUILDIR)/$(OBJ_OUT)/$(patsubst %.c,%.s,$(@F))" $(C_RESET) >/dev/stderr

$(BUILDIR)/$(GE_ASM)/hw/%.s:$(SRC)/%.s
	@mkdir -p $(BUILDIR)
	@mkdir -p $(BUILDIR)/$(GE_ASM)
	cp $< $(BUILDIR)/$(GE_ASM)/$(@F) 
	@echo  $(EH) $(C_CYAN)"[LST]"$(C_RESET)" $<"$(C_CYAN)"\t: "$(C_GREEN)"$(BUILDIR)/$(OBJ_OUT)/$(patsubst %.s,%.s,$(@F))" $(C_RESET) >/dev/stderr

$(BUILDIR)/$(OBJ_OUT)/hw/%.o:$(SRC)/%.s
	@mkdir -p $(BUILDIR)
	@mkdir -p $(BUILDIR)/$(OBJ_OUT)
	$(CROSS_AS) -o $(BUILDIR)/$(OBJ_OUT)/$(@F)  $(ASM_OPTS)  $<
	@echo  $(EH) $(C_BLUE)"[AS ]"$(C_RESET)" $<"$(C_BLUE)"\t: "$(C_GREEN)"$(BUILDIR)/$(OBJ_OUT)/$(patsubst %.s,%.o,$(@F))" $(C_RESET) >/dev/stderr


.PHONY: doversion all clean info dwload qemu asm

all: depend all_asm_file all_c_file all_cpp_file


depend: $(DEP_OBJ)

all_asm_file:	$(OBJ_ASM)

all_c_file:  	$(OBJ_C)

all_cpp_file: 	$(OBJ_CPP)
	



info:	
	@echo  $(EH) $(C_YELLOW)"Current HARDWARE = "$(C_RESET)$(HARDWARE)
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
	

asm:    $(ASM_OBJ)


