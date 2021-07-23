#!/bin/bash -x

PWD=$(pwd)
NAME=$(cat ./gcc-cross/conf.mk | grep TARGET:= | sed 's/:=/ /g'| awk '{print $2}')

CROSSGCC=$PWD/$NAME
echo "CURRENT CROSS PLATFORM : $NAME"
export PATH=$CROSSGCC/bin:$CROSSGCC/$NAME/bin:/usr/bin:/bin:/usr/local/bin
