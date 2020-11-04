#!/bin/sh

romPath=`dirname "$1"`
romName=`basename "$1"`

cd `dirname $0`

export LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH

exec ./raine -screenx 320 -screeny 240 -fullscreen 1 -nb -nc -nogui -rp "$romPath" -nb ${romName%%.*}
