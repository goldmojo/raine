#!/bin/sh

currentDir=`pwd`

romPath=`dirname "$1"`
romName=`basename "$1"`

cd `dirname $0`

export LD_LIBRARY_PATH=$currentDir:$LD_LIBRARY_PATH

exec ./raine -fullscreen 1 -nb -nc -nogui -rp "$romPath" -nb ${romName%%.*}
