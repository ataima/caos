#!/bin/bash -x

PWD=$(pwd)
NAME=arm-none-eabi

CROSSGCC=$PWD/$NAME

export PATH=$CROSSGCC/bin:$CROSSGCC/arm-none-eabi/bin:/usr/bin:/bin
