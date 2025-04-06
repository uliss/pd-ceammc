#!/bin/bash

ZOOM=1.5
BG='#fff'
BOTTOM=150
RIGHT=150
LEFT=150
TOP=500
IN=$(basename $1)
OUT=${IN%.svg}.png

BOTTOM=550
RIGHT=150
LEFT=150
TOP=130

if [ $# -ne 1 ]
then
  echo "Usage $0 SVG"
  exit 1
fi

resvg --background $BG -z $ZOOM $1 $OUT
convert $OUT -crop +$LEFT+$TOP -crop -$RIGHT-$BOTTOM tmp
mv tmp $OUT
