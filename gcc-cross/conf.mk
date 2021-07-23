
# SETUP VERSION TO DOWNLOAD
VERSION:=8.3

ifeq ($(VERSION),8.3)
BINUTILS :=binutils-2.32.tar.gz
GCC:=gcc-8.3.0
GMP:=gmp-6.1.2.tar.bz2
MPFR:=mpfr-4.0.2.tar.xz
MPC:=mpc-1.1.0.tar.gz
GDB:=gdb-8.2.tar.gz
endif

ifeq ($(VERSION),6.3)
BINUTILS :=binutils-2.28.tar.gz
GCC:=gcc-6.3.0
GMP:=gmp-6.1.2.tar.bz2
MPFR:=mpfr-3.1.5.tar.xz
MPC:=mpc-1.0.3.tar.gz
GDB:=gdb-7.12.1.tar.gz
endif


#SET DESTINATION PATH
CROSS_GCC_NAME:=../cross-gcc-self-build

#SET TARGET TYPE
TARGET:=arm-none-eabi
#arm-none-eabi
#i686-elf

