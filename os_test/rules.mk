#COLOURED TERMINAL....
C_WHITE= "\033[1;37m"
C_GRAY_LIGHT= "\033[0;37m"
C_GRAY_DARK= "\033[1;30m"
C_BLUE= "\033[0;34m"
C_BLUE_LIGHT= "\033[1;34m"
C_GREEN= "\033[0;32m"
C_GREEN_LIGHT= "\033[1;32m"
C_CYAN= "\033[0;36m"
C_CYAN_LIGHT= "\033[1;36m"
C_RED= "\033[0;31m"
C_RED_LIGHT= "\033[1;31m"
C_PURPLE= "\033[0;35m"
C_PURPLE_LIGHT= "\033[1;35m"
C_BROWN= "\033[0;33m"
C_YELLOW= "\033[1;33m"
C_BLACK= "\033[0;30m"
C_RESET="\033[0m"
I_CRED= "\"\033[0;31m\""
I_RED="\"\033[0;31m\""
I_GREEN= "\"\033[0;32m\""
I_RESET="\"\033[0m\""
I_TAB="\"\t\""
I_CPURPLE=-e "\"\033[1;35m\""
#MAKE VERSION x.xx.xxx
OS=$(shell uname -s)
# CURRENT DEBUG LEVEL
DBG:= -O0 -g 
# 32 or 64 bit compiler mode
C_MODE:=-m32
# C LANGUAGE OPTIONS
C_OPTS:= $(C_MODE) -Wfatal-errors -Wextra -Wpedantic -Wconversion -Wshadow  -Wall $(DBG) -std=c99  -ffreestanding  -c
# CPP LANGUAGE OPTIONS
CPP_OPTS := $(C_MODE) -Wfatal-errors -Wextra -Wpedantic -Wconversion -Wshadow  -Wall $(DBG) -std=c++11 -fno-rtti  -fexceptions  -ffreestanding  -fverbose-asm   -c
# ASM LANGUAGE OPTIONS
ASM_OPTS:= $(C_MODE) -g
#LINK OPTIONS
ifeq ($(C_MODE),-m32)
LK_OPT= -m32 -lc -lstdc++  -lgcc_s  -L /usr/lib/gcc/x86_64-linux-gnu/6/32
else
LK_OPT= -m32 -lc -lstdc++  -lgcc_s  -L /usr/lib/gcc/x86_64-linux-gnu/6/
endif
# CROSS TOOOL PROGRAMS
CC:= gcc 
CPP:= g++ 
LD:= g++
OBJDUMP:= objdump 
OBJCOPY:= objcopy 
GDB:= gdb 
#OUTPUT BUILD DIR
BUILDIR:=build
DEPEND:=depend
GE_ASM:=r_asm
OBJ_OUT:=obj
SRC:=src
DEP_OPTS:=-MM -MT
ifeq ($(OS),Linux)  
    EH:= -e   
else	    
    EH:= -e	
endif
LNK:=@ln  
ULNK:=@rm -f  


