#!/bin/sh

romPath=`dirname "$1"`
romName=`basename "$1"`

cd `dirname $0`

export LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH

exec ./raine -screenx 320 -screeny 240 -nb -nogui -rp "$romPath" -nb ${romName%%.*} 2>&1 >/var/tmp/raine.log
