#!/bin/sh

currentDir=`pwd`

export LD_LIBRARY_PATH=$currentDir:$LD_LIBRARY_PATH

exec ./raine -fullscreen 1 -nb -nc
