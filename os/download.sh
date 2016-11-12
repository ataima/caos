#! /bin/dash

set -x

OS=$(uname -a | grep Linux | awk '{print $1}')
echo "O.System=$OS"


if [ "$OS" = "Linux" ]; then
killall putty 
putty  -load ttyUSB0 &
dd if=$1 of=/dev/ttyUSB0 bs=1
else
../DOWNLOAD.exe $1
fi
