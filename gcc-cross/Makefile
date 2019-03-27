include  ./conf.mk

 
CROSS_GCC:= $(GCC).tar.gz
FULL_CROSS_GCC := $(GCC)/$(CROSS_GCC)


V_BINUTILS:= $(subst .tar.gz,,$(BINUTILS))
V_CROSS_GCC:=$(subst .tar.gz,,$(CROSS_GCC))
V_GMP:=$(subst .tar.bz2,,$(GMP))
V_MPFR:=$(subst .tar.xz,,$(MPFR))
V_MPC:=$(subst .tar.gz,,$(MPC))
V_GDB:=$(subst .tar.gz,,$(GDB))

B_BINUTILS:=$(V_BINUTILS)-build
B_CROSS_GCC:=$(V_CROSS_GCC)-build
B_GDB:=$(V_GDB)-build

ROOT:=$(shell pwd)
DEST_PATH:=$(ROOT)/../$(CROSS_GCC_NAME)/$(TARGET)

test:
	@echo "TEST MAKE VAR"
	@echo "binutils pack:"$(BINUTILS)
	@echo "gcc pack:"$(FULL_CROSS_GCC)
	@echo "gcc pack:"$(CROSS_GCC)
	@echo "gmp pack:"$(GMP)
	@echo "mpfr pack:"$(MPFR)
	@echo "mpc pack:"$(MPC)
	@echo "gdb pack:"$(GDB)
	@echo "binutils version:"$(V_BINUTILS)
	@echo "gcc version: "$(V_CROSS_GCC)
	@echo "gmp version:"$(V_GMP)
	@echo "mpfr version:"$(V_MPFR)
	@echo "mpc version:"$(V_MPC)
	@echo "gdb version:"$(V_GDB)
	@echo "build binutils:"$(B_BINUTILS)
	@echo "build gcc:"$(B_CROSS_GCC)
	@echo "build gcc:"$(B_GDB)

clean_all : download_clean expand_clean binutils_clean gcc_clean gdb_clean

download_clean:
	@rm -f  $(BINUTILS)
	@rm -f  $(BINUTILS).*
	@rm -f  $(CROSS_GCC)
	@rm -f  $(CROSS_GCC).*
	@rm -f  $(GMP)
	@rm -f  $(GMP).*
	@rm -f  $(MPFR)
	@rm -f  $(MPFR).*
	@rm -f  $(MPC)
	@rm -f  $(MPC).*
	@rm -f  $(GDB)
	@rm -f  $(GDB).*
	
	
download_binutils:
	@echo "get binutils ("$(BINUTILS)")"
	@wget ftp://ftp.gnu.org/gnu/binutils/$(BINUTILS)
	@wget ftp://ftp.gnu.org/gnu/binutils/$(BINUTILS).sig

download_gcc:
	@echo "get gcc ("$(FULL_CROSS_GCC)")"
	@wget ftp://ftp.gnu.org/gnu/gcc/$(FULL_CROSS_GCC)
	@wget ftp://ftp.gnu.org/gnu/binutils/$(BINUTILS).sig

download_gmp:
	@echo "get gmp ("$(GMP)")"
	@wget ftp://ftp.gnu.org/gnu/gmp/$(GMP)
	@wget ftp://ftp.gnu.org/gnu/gmp/$(GMP).sig
	
download_mpfr:
	@echo "get mpfr ("$(MPFR)")"
	@wget http://mpfr.loria.fr/mpfr-current/$(MPFR)
	@wget http://mpfr.loria.fr/mpfr-current/$(MPFR).asc

download_mpc:
	@echo "get mpc ("$(MPC)")"
	@wget ftp://ftp.gnu.org/gnu/mpc/$(MPC)	
	@wget ftp://ftp.gnu.org/gnu/mpc/$(MPC).sig	

download_gdb:
	@echo "get gdb ("$(GDB)")"
	@wget http://ftp.gnu.org/gnu/gdb/$(GDB)	
	@wget http://ftp.gnu.org/gnu/gdb/$(GDB).sig	


download:download_binutils  download_gcc download_gmp download_mpfr download_mpc download_gdb
	@echo "done..."
	
expand_clean:
	@rm -f  -rf $(V_BINUTILS)
	@rm -f  -rf $(V_CROSS_GCC)
	@rm -f  -rf $(V_GMP)
	@rm -f  -rf $(V_MPFR)
	@rm -f  -rf $(V_MPC)
	@rm -f  -rf $(V_GDB)
	
	
expand_binutils:
	@tar -xvf $(BINUTILS)	
	@echo "binutils:"$(V_BINUTILS)
	
expand_gcc:
	@tar -xvf $(CROSS_GCC)
	@echo "gcc:"$(V_CROSS_GCC)

expand_gmp:
	@tar -xvf $(GMP)	
	@echo "gmp:"$(V_GMP)
	
expand_mpfr:	
	@tar -xvf $(MPFR)
	@echo "mpfr:"$(V_MPFR)

expand_mpc:
	@tar -xvf $(MPC)
	@echo "mpc:"$(V_MPC)

expand_gdb:
	@tar -xvf $(GDB)
	@echo "gdb:"$(V_GDB)
	
expand:	expand_binutils expand_gcc expand_gmp expand_mpfr expand_mpc expand_gdb
	@echo "done ...."
	
	
binutils_clean:
	@echo "REMOVE OLD BINUTILS BUILD"
	@rm -f  -rfv $(ROOT)/$(B_BINUTILS)
	@echo "done.."	
  
binutils_configure: binutils_clean
	@mkdir $(ROOT)/$(B_BINUTILS)
	@echo "configure binutils to target --prefix=$(DEST_PATH) --target=$(TARGET)"	
	@cd $(ROOT)/$(B_BINUTILS) && \
	../$(V_BINUTILS)/configure --prefix=$(DEST_PATH) --target=$(TARGET)  --disable-nls 

binutils_build:
	nice make -C $(B_BINUTILS) -j8 all

binutils_install:
	@cd $(ROOT)/$(B_BINUTILS) && make install
	@echo "have to do: export PATH=$(DEST_PATH):/usr/bin:/bin"
	@echo " and make with command phase2"


binutils_test:
	make -C $(B_BINUTILS) check


binutils_rebuild: binutils_clean binutils_configure binutils_build binutils_test binutils_install

gcc_clean:
	@echo "REMOVE OLD GCC BUILD"
	rm -rfv $(ROOT)/$(B_CROSS_GCC)
	
gcc_configure:
	@if [ -a $(ROOT)/$(V_CROSS_GCC)/gmp ] ; then rm -rf $(ROOT)/$(V_CROSS_GCC)/gmp ; fi;
	@if [ -a $(ROOT)/$(V_CROSS_GCC)/mpc ] ; then rm -rf $(ROOT)/$(V_CROSS_GCC)/mpc ; fi;
	@if [ -a $(ROOT)/$(V_CROSS_GCC)/mpfr ] ; then rm -rf $(ROOT)/$(V_CROSS_GCC)/mpfr ; fi;
	@cd $(ROOT)/$(V_CROSS_GCC) &&\
	ln -s $(ROOT)/$(V_GMP) gmp &&\
	ls -al gmp &&\
	ln -s $(ROOT)/$(V_MPC) mpc &&\
	ls -al mpc &&\
	ln -s $(ROOT)/$(V_MPFR) mpfr &&\
	ls -al mpfr 
	@mkdir $(ROOT)/$(B_CROSS_GCC)
	@cd $(ROOT)/$(B_CROSS_GCC) && \
	../$(V_CROSS_GCC)/configure --prefix=$(DEST_PATH) --target=$(TARGET) --enable-languages=c,c++ --disable-nls --without-headers 

gcc_build: 
	nice make -C $(B_CROSS_GCC) -j8 all-gcc 
	nice make -C $(B_CROSS_GCC) -j8 all-target-libgcc

gcc_check: 
	make -C $(B_CROSS_GCC) check 


gcc_rebuild: gcc_clean gcc_configure gcc_build 
	
	
gcc_install:
	@cd $(ROOT)/$(B_CROSS_GCC) && make install-gcc && make install-target-libgcc


gdb_clean:
	@echo "REMOVE OLD GDB BUILD"
	rm -rfv $(ROOT)/$(B_GDB)


gdb_configure:
	@mkdir $(ROOT)/$(B_GDB)
	@cd $(ROOT)/$(B_GDB) && \
	../$(V_GDB)/configure --prefix=$(DEST_PATH) --target=$(TARGET) --enable-languages=c,c++ --disable-nls --without-headers 


gdb_build:
	nice make -C $(B_GDB) -j8 all
	
	
gdb_install:
	@cd $(ROOT)/$(B_GDB) && make install
	
	
gdb_rebuild: gdb_clean gdb_configure gdb_build 
	
	
phase1: 	download expand binutils_rebuild
	@echo "END PHASE 1"
	@echo "have to do: ( opt sudo ) make binutils_install"
	@echo " and export PATH:=$(DEST_PATH): <char dollar>PATH"
	@echo " and redo make with command phase2"


phase1_2:  expand binutils_rebuild
	@echo "END PHASE 1"
	@echo "have to do: ( opt sudo ) make binutils_install"
	@echo " and export PATH:=$(DEST_PATH): <char dollar>PATH"
	@echo " and set on conf.mk file the new CROSS target , ex arm-none-eabihf"	
	@echo " and redo make with command phase2"


phase2: gcc_rebuild gdb_rebuild
	@echo "END PHASE 2"
	@ echo "have to do: ( opt sudo ) make gcc_install gdb_install"
	@ echo " and redo make with command phase3"
	
phase3: clean_all
	@echo "done... cross compiler pack !"

	
PHONY :  phase1 phase2  phase3 download
