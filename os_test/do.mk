# set environment variables
include rules.mk
#define includes 
INC=-I inc   -I ../os/inc

#dependences and objs 

#SOURCE FILES
SRC_CPP:=$(wildcard $(SRC)/*.cpp)

SRC_C:=$(wildcard $(SRC)/*.c)


#OBJ FILES
OBJ_CPP:= $(patsubst $(SRC)/%.cpp,$(BUILDIR)/$(OBJ_OUT)/%.o,$(SRC_CPP))

OBJ_C:=$(patsubst $(SRC)/%.c,$(BUILDIR)/$(OBJ_OUT)/%.o,$(SRC_C))



#DEPEND FILES
DEP_CPP:= $(patsubst $(SRC)/%.cpp,$(BUILDIR)/$(DEPEND)/%.d,$(SRC_CPP))

DEP_C:=$(patsubst $(SRC)/%.c,$(BUILDIR)/$(DEPEND)/%.d,$(SRC_C))


#GENERATED ASM FILES
ASM_CPP:= $(patsubst $(SRC)/%.cpp,$(BUILDIR)/$(GE_ASM)/%.s,$(SRC_CPP))

ASM_C:=$(patsubst $(SRC)/%.c,$(BUILDIR)/$(GE_ASM)/%.s,$(SRC_C))

#ALL OBJS
OBJS:= $(OBJ_C) $(OBJ_CPP) 

#ALL DEPEND
DEP_OBJ:= $(DEP_CPP)  $(DEP_C) 

$(BUILDIR)/$(DEPEND)/%.d:$(SRC)/%.cpp
	@mkdir -p $(BUILDIR)
	@mkdir -p $(BUILDIR)/$(DEPEND)
	@mkdir -p $(BUILDIR)/$(OBJ_OUT)
	$(CPP) $(CPP_OPTS) $(DEP_OPTS) $(BUILDIR)/$(OBJ_OUT)/$(patsubst %.d,%.o,$(@F))  $(INC) -o $(BUILDIR)/$(DEPEND)/$(@F) -c $<
	@echo "	"$(CPP) $(INC)   $(CPP_OPTS)  -o $(BUILDIR)/$(OBJ_OUT)/$(patsubst %.d,%.o,$(@F)) $< >>$(BUILDIR)/$(DEPEND)/$(@F)
	@echo "	@echo "$(EH) $(I_CRED)"[CPP]"$(I_RESET)" $<"$(I_RED)$(I_TAB)": "$(I_RESET) $(I_GREEN)"$(BUILDIR)/$(OBJ_OUT)/$(patsubst %.d,%.o,$(@F))"$(I_RESET)">/dev/stderr">>$@
	@echo>>$@


$(BUILDIR)/$(DEPEND)/%.d:$(SRC)/%.c
	@mkdir -p $(BUILDIR)
	@mkdir -p $(BUILDIR)/$(DEPEND)
	@mkdir -p $(BUILDIR)/$(OBJ_OUT)
	$(C) $(C_OPTS) $(DEP_OPTS) $(BUILDIR)/$(OBJ_OUT)/$(patsubst %.d,%.o,$(@F))  $(INC) -o $(BUILDIR)/$(DEPEND)/$(@F) -c $<
	@echo "	"$(C) $(INC)   $(C_OPTS)  -o $(BUILDIR)/$(OBJ_OUT)/$(patsubst %.d,%.o,$(@F)) $< >>$(BUILDIR)/$(DEPEND)/$(@F)
	@echo "	@echo "$(EH) $(I_CRED)"[ C ]"$(I_RESET)" $<"$(I_RED)$(I_TAB)": "$(I_RESET) $(I_GREEN)"$(BUILDIR)/$(OBJ_OUT)/$(patsubst %.d,%.o,$(@F))"$(I_RESET)">/dev/stderr">>$@
	@echo>>$@

#double pass : first create dependences with compile command so build it

-include $(DEP_OBJ) 

$(BUILDIR)/$(GE_ASM)/%.s:$(SRC)/%.cpp
	@mkdir -p $(BUILDIR)
	@mkdir -p $(BUILDIR)/$(GE_ASM)
	$(CPP) $(CPP_OPTS)  $(INC) -S -o $(BUILDIR)/$(GE_ASM)/$(@F) -c $<
	@echo $(EH) $(C_CYAN)"[LST]"$(C_RESET)" $<"$(C_CYAN)"\t: "$(C_GREEN)"$(BUILDIR)/$(OBJ_OUT)/$(patsubst %.cpp,%.s,$(@F))" $(C_RESET) >/dev/stderr


$(BUILDIR)/$(GE_ASM)/%.s:$(SRC)/%.c
	@mkdir -p $(BUILDIR)
	@mkdir -p $(BUILDIR)/$(GE_ASM)
	$(C) $(C_OPTS)  $(INC) -S -o $(BUILDIR)/$(GE_ASM)/$(@F) -c $<
	@echo $(EH) $(C_CYAN)"[LST]"$(C_RESET)" $<"$(C_CYAN)"\t: "$(C_GREEN)"$(BUILDIR)/$(OBJ_OUT)/$(patsubst %.c,%.s,$(@F))" $(C_RESET) >/dev/stderr



.PHONY:  all clean info dwload qemu asm distclean prepare unprepare

prepare:
	@ln ../os/inc/caos_c_types.h inc/caos_c_types.h
	@ln ../os/inc/hal.h inc/hal.h
	@ln ../os/inc/memaux.h inc/memaux.h	
	@ln ../os/inc/idevice.h inc/idevice.h
	@ln ../os/inc/syslog.h inc/syslog.h
	@ln ../os/inc/memory.h inc/memory.h
	@ln ../os/src/memaux.cpp src/memaux.cpp
	@ln ../os/src/idevice.cpp src/idevice.cpp
	@ln ../os/src/halcomdevice.cpp src/halcomdevice.cpp
	@ln ../os/src/syslog.cpp src/syslog.cpp
	@ln ../os/src/memory.cpp src/memory.cpp


unprepare:
	@rm -f inc/caos_c_types.h
	@rm -f inc/hal.h
	@rm -f inc/memaux.h
	@rm -f inc/idevice.h
	@rm -f inc/syslog.h
	@rm -f inc/memory.h
	@rm -f src/memaux.cpp
	@rm -f src/idevice.cpp
	@rm -f src/halcomdevice.cpp
	@rm -f src/syslog.cpp
	@rm -f src/memory.cpp

	
	


distclean: clean unprepare prepare 	
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
	@rm -f *.html
	@rm -f *.xml
	@rm -f *.txt




depend: $(DEP_OBJ)


all_c_file:  	$(OBJ_C)

all_cpp_file: 	$(OBJ_CPP)
	
link_file:
	$(LD) $(OBJ_CPP)  $(OBJ_C) $(OBJ_ASM)  $(LK_OPT) -o $(BUILDIR)/test.exe
	@echo $(EH) $(C_PURPLE)"[LINKER ]"$(C_RESET)" $(BUILDIR)/test.exe">/dev/stderr
	

all :   depend  all_c_file all_cpp_file  link_file 

info:	
	@echo $(EH) $(C_YELLOW)"Current Debug level = " $(C_RESET)$(DBG)
	@echo $(EH) $(C_YELLOW)"Current Options for depend  files = " $(C_RESET)$(DEP_OPTS)
	@echo $(EH) $(C_YELLOW)"Current Options for C(*.c) files = " $(C_RESET)$(C_OPTS)
	@echo $(EH) $(C_YELLOW)"Current Options for CPP(*.cpp) files = " $(C_RESET)$(CPP_OPTS)
	@echo $(EH) $(C_YELLOW)"Current Options for ASM(*.s) files = " $(C_RESET)$(ASM_OPTS)
	@echo $(EH) $(C_YELLOW)"Current Options for LINKER files = " $(C_RESET)$(LK_OPT)
	@echo $(EH) $(C_YELLOW)"Current Download Program = " $(C_RESET)$(DWLOAD)
	@echo $(EH) $(C_YELLOW)"Current Terminal Program = " $(C_RESET)$(PUTTY)
	@echo $(EH) $(C_YELLOW)"Assembler source = "$(C_RESET)$(SRC_ASM)
	@echo $(EH) $(C_YELLOW)"C source = "$(C_RESET)$(SRC_C)
	@echo $(EH) $(C_YELLOW)"CPP source = "$(C_RESET)$(SRC_CPP) 
	@echo $(EH) $(C_YELLOW)"C objs = "$(C_RESET)$(OBJ_C)
	@echo $(EH) $(C_YELLOW)"CPP objs = "$(C_RESET)$(OBJ_CPP) 
	@echo $(EH) $(C_YELLOW)"All objs = "$(C_RESET)$(OBJS)
	@echo $(EH) $(C_YELLOW)"All dep = "$(C_RESET)$(DEP_OBJ)
	@echo $(EH) $(C_YELLOW)"MAIN ROOT = "$(C_RESET)$(ROOT)
	@echo $(EH) $(C_YELLOW)"OUTPUT BUILD DIR = " $(C_RESET)$(BUILDIR)
	

test:
	@./build/test.exe -f 15