#!/bin/sh
buildnumber=$(awk '{ print ++$11 }' ./inc/caos_version.h )
echo "static const char *caos_version = ""\"Version : "$1"."$2"."$3" - Build "$buildnumber" \";">./inc/caos_version.h;
echo $buildnumber