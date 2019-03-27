A Makefile to build binutils and gcc

How work ?

set on conf.mk the  version of  gcc source  to download

and then :
 

do: make phase 1
----> donwload source package, expand package, build binutils, check binutils
----> make phase1_2 for skip download  
do: sudo make binutils_install
 ---> install binutils 
 
 change PATH with new binutils (as explain fromrun script....    ) export PATH=.....
 
 do make phase2
 ---> build gcc with specific target,  check gcc 
 
 sudo make gcc_install
 
 do make phase3 ( optional , remove all !!)
 ---> clean all subdir
 



 
 
 
 
