# set environment variables
include rules.mk
#define includes 
INC=-I inc   -I ../os/inc -I ../os/inc/hal 

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

default: all

.PHONY:  all clean info dwload qemu asm distclean add_ext_src remove_ext_src

define link_ext_file
    $(shell if [ ! -e src/$1 ]; then ln ../os/src/hal/$1 src/$1 ; fi)
endef

define ulink_ext_file
    $(shell if [ -e src/$1 ]; then rm -f src/$1 ; fi)
endef

add_ext_src:
	$(call link_ext_file,memaux.cpp)
	$(call link_ext_file,idevice.cpp)
	$(call link_ext_file,halcomdevice.cpp)
	$(call link_ext_file,halsystimerdevice.cpp)
	$(call link_ext_file,haljobdevice.cpp)
	$(call link_ext_file,syslog.cpp)
	$(call link_ext_file,memory.cpp)
	$(call link_ext_file,scheduler.cpp)
	$(call link_ext_file,kdebug.cpp)


remove_ext_src:
	$(call ulink_ext_file,memaux.cpp)
	$(call ulink_ext_file,idevice.cpp)
	$(call ulink_ext_file,halcomdevice.cpp)
	$(call ulink_ext_file,halsystimerdevice.cpp)
	$(call ulink_ext_file,haljobdevice.cpp)
	$(call ulink_ext_file,syslog.cpp)
	$(call ulink_ext_file,memory.cpp)
	$(call ulink_ext_file,scheduler.cpp)
	$(call ulink_ext_file,kdebug.cpp)

	
	


distclean: clean  remove_ext_src add_ext_src
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
	@mkdir $(BUILDIR)/$(OBJ_OUT)




depend: $(DEP_OBJ)


all_c_file:  	$(OBJ_C)

all_cpp_file: 	$(OBJ_CPP)
	
link_file:
	$(LD) $(OBJ_CPP)  $(OBJ_C) $(OBJ_ASM)  $(LK_OPT) -o $(BUILDIR)/test.exe
	@echo $(EH) $(C_PURPLE)"[LINKER ]"$(C_RESET)" $(BUILDIR)/test.exe">/dev/stderr
	

all :  add_ext_src depend  all_c_file all_cpp_file  link_file 

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
	

test: all
	@./build/test.exe -f 15
