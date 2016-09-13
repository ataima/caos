#MAKE VERSION x.xx.xxx
CAOS_VERSION_1:= 1
CAOS_VERSION_2:= 00
CAOS_VERSION_3:= 023
BUILD_NUMBER:=
#CROS TOOLS 
ARMGNU:= arm-none-eabi
# CURRENT DEBUG LEVEL
DBG:= -O2 
# C LANGUAGE OPTIONS
C_OPTS:= -Wfatal-errors -Wextra -Wpedantic -Wconversion -Wshadow  -Wall $(DBG) -std=c99 -nostdlib -nostartfiles -ffreestanding -mfpu=neon-vfpv4 -mfloat-abi=hard -march=armv7ve -mtune=cortex-a7 -c 
# CPP LANGUAGE OPTIONS
CPP_OPTS := -Wfatal-errors -Wextra -Wpedantic -Wconversion -Wshadow  -Wall $(DBG) -std=c++11 -nostdlib -nostartfiles -fno-rtti  -fno-exceptions  -ffreestanding  -fverbose-asm -mfpu=neon-vfpv4 -mfloat-abi=hard -march=armv7ve -mtune=cortex-a7 -c 
# ASM LANGUAGE OPTIONS
ASM_OPTS:= -g -mfpu=neon-vfpv4 -mfloat-abi=hard -march=armv7ve  
#LIBRARY TO LOAD FROM CROSS TOOL CHAIN FOR BASIC FUNCTION
# OR IF NOT IN CLUDFED HAVE TO SUPPLI TO MISSING FUNCTION
# TRY TO SEE ...
LK_OPT:= ../../tools/GNU-CROSS-GCC/5.3-2016q1/lib/gcc/arm-none-eabi/5.3.1/libgcc.a 
# OPTIONAL PROGTAM TO DOWNLOAD NEW FIRMWARE
DWLOAD:= ../DWLOAD.EXE  
# OPTIONAL TERMINAL PROGRAM TO CONNECT TO BOARD
PUTTY:= putty.exe -load "pi-tty" 
# CROSS TOOOL PROGRAMS
CROSS_CC:= $(ARMGNU)-gcc 
CROSS_CPP:= $(ARMGNU)-g++ 
CROSS_AS:= $(ARMGNU)-as 
CROSS_LD:= $(ARMGNU)-ld 
CROSS_OBJDUMP:= $(ARMGNU)-objdump 
CROSS_OBJCOPY:= $(ARMGNU)-objcopy 
CROSS_GDB:= $(ARMGNU)-gdb 
#OUTPUT BUILD DIR
BUILDIR:=build
DEPEND:=depend
GE_ASM:=r_asm
OBJ_OUT:=obj
SRC:=src
TEST:=test
DEP_OPTS:=-MM -MT
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
I_CRED=-e "\"\033[0;31m\""
I_RED="\"\033[0;31m\""
I_GREEN= "\"\033[0;32m\""
I_RESET="\"\033[0m\""
I_TAB="\"\t\""
I_CPURPLE=-e "\"\033[1;35m\""
