#!/bin/sh  -x
VERSION=$(ls  $HOME/baremetal  | grep gcc-arm-none-eabi- | sed -e 's/gcc-arm-none-eabi-*//g')
CROSSGCC=$HOME/baremetal/gcc-arm-none-eabi-$VERSION

echo "GCC VERSION = $VERSION"
echo "GCC PATH    = $CROSSGCC"
export PATH=$CROSSGCC/bin:$CROSSGCC/arm-none-eabi/bin:/usr/bin:/bin
