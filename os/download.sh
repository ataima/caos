#! /bin/sh

OS=$(uname | grep Linux )
echo "O.System=$OS"
if [ "$OS" = "Linux" ]; then
killall putty 
putty  -load ttyUSB0 &
sleep 0.1
echo "000000000000000000000000000000000g">/dev/ttyUSB0
sleep 0.1
while read line; do  
    echo "$line" >  /dev/ttyUSB0
done < build/caOS.hex 
echo "g">/dev/ttyUSB0
else
./DWLOAD.exe $1
fi
