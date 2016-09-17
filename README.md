# caos
a Baremetal Operating System for raspberry PI 2 

setup to build.

Download or build Cross Compiler  ( You can use my script at https://github.com/ataima/quick_cross_compile.git )

Git Clone caos project.

Build with Visula Studio Community the project DWLOAD to  create the downloader for window PC.

On Cygwin :

cd on caos directory.

set script path.sh and 5.3setenv to right path for GNU CROSS COMPILER

source 5.3setenv

make distclean dwload

after the dwload of image You have to press 'g' Key to start os image.

THE RASPBERRY PI2 MUST HAVE AN SD WITH  BOOTLOADER07 IMAGE
( see https://github.com/dwelch67/raspberrypi.git )


