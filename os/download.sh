#! /bin/sh

OS=$(uname -a | grep Linux)
if [[ $OS == "Linux" ]]; then
killall putty 
putty  -load ttyUSB0
dd if=$1 of=/dev/ttyUSB0 bs=1
else
#cygwin
../DOWNLOAD.exe $1
fi