# set environment variables
include rules.mk
#define includes 
INC=-I inc -I inc/hal  -I inc/hw/rasp2

#dependences and objs 

SRC:=test




#test
TEST_CPP:=$(wildcard $(SRC)/*.cpp)

#OBJ FILES


TEST_OBJ_CPP:= $(patsubst $(SRC)/%.cpp,$(BUILDIR)/$(OBJ_OUT)/%.o,$(TEST_CPP))


#DEPEND FILES


TEST_DEP_CPP:= $(patsubst $(SRC)/%.cpp,$(BUILDIR)/$(DEPEND)/%.d,$(TEST_CPP))


TEST_ASM_CPP:= $(patsubst $(SRC)/%.cpp,$(BUILDIR)/$(GE_ASM)/%.s,$(TEST_CPP))

ASM_OBJ:=  $(TEST_ASM_CPP)

#ALL OBJS
OBJS:= $(TEST_OBJ_CPP) 

#ALL DEPEND
DEP_OBJ:= $(TEST_DEP_CPP)


$(BUILDIR)/$(DEPEND)/%.d:$(SRC)/%.cpp
	@mkdir -p $(BUILDIR)
	@mkdir -p $(BUILDIR)/$(DEPEND)
	@mkdir -p $(BUILDIR)/$(OBJ_OUT)
	$(CROSS_CPP) $(CPP_OPTS) $(DEP_OPTS) $(BUILDIR)/$(OBJ_OUT)/$(patsubst %.d,%.o,$(@F))  $(INC) -o $(BUILDIR)/$(DEPEND)/$(@F) -c $<
	@echo "	"$(CROSS_CPP) $(INC)   $(CPP_OPTS)  -o $(BUILDIR)/$(OBJ_OUT)/$(patsubst %.d,%.o,$(@F)) $< >>$(BUILDIR)/$(DEPEND)/$(@F)
	@echo "	@echo "$(EH)  $(I_CPURPLE)"[TEST]"$(I_RESET)" $<"$(I_RED)$(I_TAB)": "$(I_RESET) $(I_GREEN)"$(BUILDIR)/$(OBJ_OUT)/$(patsubst %.d,%.o,$(@F))"$(I_RESET)">/dev/stderr">>$@
	@echo>>$@






#double pass : first create dependences with compile command so build it

-include $(DEP_OBJ) 


$(BUILDIR)/$(GE_ASM)/%.s:$(SRC)/%.cpp
	@mkdir -p $(BUILDIR)
	@mkdir -p $(BUILDIR)/$(GE_ASM)
	$(CROSS_CPP) $(CPP_OPTS)  $(INC) -S -fverbose-asm -g -O2 -o $(BUILDIR)/$(GE_ASM)/$(@F)  $<
	@echo  $(EH) $(C_CYAN)"[LST]"$(C_RESET)" $<"$(C_CYAN)"\t: "$(C_GREEN)"$(BUILDIR)/$(OBJ_OUT)/$(patsubst %.cpp,%.s,$(@F))" $(C_RESET) >/dev/stderr




.PHONY: doversion all clean info dwload qemu asm



depend: $(DEP_OBJ)

	
all_test_file: 	$(TEST_OBJ_CPP)	

all: all_test_file

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
	
