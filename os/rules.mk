
#SELECT CURRENT HARDWARE RASPI2  686   SIMULA
# ===========================================	
HARDWARE=RASPI2
#        ^^^^^^^^^^^^^^^ 
# ===========================================	
#COLOURED TERMINAL....
C_WHITE="\033[1;37m"
C_GRAY_LIGHT="\033[0;37m"
C_GRAY_DARK="\033[1;30m"
C_BLUE="\033[0;34m"
C_BLUE_LIGHT="\033[1;34m"
C_GREEN="\033[0;32m"
C_GREEN_LIGHT="\033[1;32m"
C_CYAN="\033[0;36m"
C_CYAN_LIGHT="\033[1;36m"
C_RED="\033[0;31m"
C_RED_LIGHT="\033[1;31m"
C_PURPLE="\033[0;35m"
C_PURPLE_LIGHT="\033[1;35m"
C_BROWN="\033[0;33m"
C_YELLOW="\033[1;33m"
C_BLACK="\033[0;30m"
C_RESET="\033[0m"
I_CRED= "\"\033[0;31m\""
I_RED= "\"\033[0;31m\""
I_GREEN= "\"\033[0;32m\""
I_RESET="\"\033[0m\""
I_CPURPLE= "\"\033[1;35m\""
I_TAB="\"\t\""
#MAKE VERSION x.xx.xxx
OS=$(shell uname -s)
CAOS_VERSION_1:= 1
CAOS_VERSION_2:= 00
CAOS_VERSION_3:= 023
BUILD_NUMBER:=

#CUSTOMIZE ENVIROMENT FROM SELECTED HARDWARE
#CASE 1: RASPBERRY-2
ifeq ($(HARDWARE),RASPI2)	
	#CROSS TOOLS 
	ARMGNU:=arm-none-eabi
	# CURRENT DEBUG LEVEL
	DBG:=  -O0 -g
	# OBJDUMP CPU SPEC
	OBJD_CPU=-m arm
	# C LANGUAGE OPTIONS
	CPU_SPEC_C:=-mfpu=neon-vfpv4 -mfloat-abi=hard -march=armv7ve -mtune=cortex-a7 -DHW_RASPI2
	# CPP LANGUAGE OPTIONS
	CPU_SPEC_CPP:=-mfpu=neon-vfpv4 -mfloat-abi=hard -march=armv7ve -mtune=cortex-a7 -DHW_RASPI2
	# ASM LANGUAGE OPTIONS
	CPU_SPEC_ASM:=-mfpu=neon-vfpv4 -mfloat-abi=hard -march=armv7ve  
	# LINKER OPTIONS
	LK_OPTS:= -M -T ld_conf/BCM2836.ld
	# ABSOLUTE PATH TO CROSS TOOLCHAIN
	ARMPATH:=$(HOME)/baremetal/gcc-arm-none-eabi-5_4-2016q3/bin
	# CROSS TOOOL PROGRAMS
	CROSS_CC:=$(ARMPATH)/$(ARMGNU)-gcc 
	CROSS_CPP:=$(ARMPATH)/$(ARMGNU)-g++ 
	CROSS_AS:=$(ARMPATH)/$(ARMGNU)-as 
	CROSS_LD:=$(ARMPATH)/$(ARMGNU)-ld 
	CROSS_OBJDUMP:=$(ARMPATH)/$(ARMGNU)-objdump 
	CROSS_OBJCOPY:=$(ARMPATH)/$(ARMGNU)-objcopy 
	CROSS_GDB:=$(ARMPATH)/$(ARMGNU)-gdb 
endif
#CASE2 686 TODO
ifeq ($(HARDWARE),686)	
	# CURRENT DEBUG LEVEL
	DBG:=-O2	
	# OBJDUMP CPU SPEC
	OBJD_CPU=
	# C LANGUAGE OPTIONS
	CPU_SPEC_C:=-DHW_686 
	# CPP LANGUAGE OPTIONS
	CPU_SPEC_CPP:=-DHW_686 
	# ASM LANGUAGE OPTIONS
	CPU_SPEC_ASM:=-DHW_686 
	# LINKER OPTIONS
	LK_OPTS:=	
	# CROSS TOOOL PROGRAMS
	CROSS_CC:=gcc 
	CROSS_CPP:=g++ 
	CROSS_AS:=as 
	CROSS_LD:=ld 
	CROSS_OBJDUMP:=objdump 
	CROSS_OBJCOPY:=objcopy 
	CROSS_GDB:=gdb 
endif
#CASE 3 SIMULA
ifeq ($(HARDWARE),SIMULA)	
	# CURRENT DEBUG LEVEL
	#DBG:=  -O2
	DBG:=  -O0 -g
	# OBJDUMP CPU SPEC
	OBJD_CPU=-m i386
	C_MODE:=-m32
	# C LANGUAGE OPTIONS
	CPU_SPEC_C:=$(C_MODE)
	# CPP LANGUAGE OPTIONS
	CPU_SPEC_CPP:=$(C_MODE)
	# ASM LANGUAGE OPTIONS
	CPU_SPEC_ASM:=  
	# LINKER OPTIONS
	ifeq ($(C_MODE),-m32)
	LK_OPTS= -lpthread -lstdc++ 
	else
	LK_OPTS= -lpthread -lstdc++
	endif	
	# CROSS TOOOL PROGRAMS
	CROSS_CC:=gcc 
	CROSS_CPP:=g++ 
	CROSS_AS:=as 
	CROSS_LD:=g++ $(C_MODE)
	CROSS_OBJDUMP:=objdump 
	CROSS_OBJCOPY:=objcopy 
	CROSS_GDB:=gdb 
endif
#SILENT --silent no gcc cmd printed
SILENT:=--silent
# C LANGUAGE OPTIONS
C_OPTS:=-Wfatal-errors -Wextra -Wpedantic -Wconversion -Wshadow  -Wall $(DBG) -std=c99 -nostdlib -nostartfiles -ffreestanding $(CPU_SPEC_C) -c 
# CPP LANGUAGE OPTIONS
CPP_OPTS:=-Wfatal-errors -Wextra -Wpedantic -Wconversion -Wshadow  -Wall $(DBG) -std=c++11 -nostdlib -nostartfiles -fno-rtti  -fno-exceptions  -ffreestanding  -fverbose-asm $(CPU_SPEC_CPP) -c 
# ASM LANGUAGE OPTIONS
ASM_OPTS:= -g $(CPU_SPEC_ASM)  
# OPTIONAL PROGTAM TO DOWNLOAD NEW FIRMWARE
DWLOAD:=./download.sh
# OPTIONAL TERMINAL PROGRAM TO CONNECT TO BOARD
ifeq ($(OS),Linux)  
    PUTTY:= putty -load "pi-tty"     
    EH:= -e
else	    
    PUTTY:= putty.exe -load "pi-tty"      
    EH:=-e	
endif
#OUTPUT BUILD DIR
BUILDIR:=build
DEPEND:=depend
GE_ASM:=r_asm
OBJ_OUT:=obj
DEP_OPTS:=-MM -MT
