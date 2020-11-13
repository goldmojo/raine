#!/bin/sh

currentDir=`pwd`

export LD_LIBRARY_PATH=$currentDir:$LD_LIBRARY_PATH
export LD_PRELOAD=$currentDir/libpng.so

exec ./raine -screenx 320 -screeny 240 -fullscreen 1 -nb -nc
