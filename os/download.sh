#! /bin/sh -x

OS=$(uname | grep Linux )
echo "O.System=$OS"
if [ "$OS" = "Linux" ]; then
killall putty 
putty  -load ttyUSB0 &
sleep 1
echo "000000000000000000000000000000000g">/dev/ttyUSB0
sleep 1
dd if=$1 of=/dev/ttyUSB0 bs=1
else
./DWLOAD.exe $1
fi
