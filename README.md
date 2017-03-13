# caos
experimental  Baremetal Operating System 
(current version for raspberry PI 2 )




BUILD PROCESS...

to build caos is needed some step before to do  make caos
FOR RASPERRY2 HAVE TO DO A SD with serial dowloader for IHex 


for OS  : cd in OS path

STEP 1: download source from github
STEP 2: build gcc cross from sources ( only one time , can copy the cross path at the next times)
STEP 3: source ./setenv.sh
STEP 4: cd os && make distclean all dwload 
STEP 5: from putty press g key 

for Gcc : cd in gcc_croos

make phase1 : download sources + make binuitls from scratch
make phase2 : build gcc
make phase3 : install and clean 

for test : cd in os_test

make test.

