#!/bin/bash -x

PWD=$(pwd)
NAME=$(cat gcc-cross/conf.mk | grep "CROSS_GCC_NAME:=" | sed -e 's/CROSS_GCC_NAME:=//g')

CROSSGCC=$PWD/$NAME

echo "GCC PATH    = $CROSSGCC"
CROSSPLATFORM=$(ls $CROSSGCC)
ALL_BIN_PATH=""
for i in $CROSSPLATFORM ; do ALL_BIN_PATH+=$CROSSGCC/$i/bin:;done
export PATH=$ALL_BIN_PATH/usr/bin:/bin
