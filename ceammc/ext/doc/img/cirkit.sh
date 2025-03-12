#!/bin/bash

ZOOM=1.5
BG='#fff'
CROP=150
IN=$(basename $1)
OUT=${IN%.svg}.png

if [ $# -ne 1 ]
then
  echo "Usage $0 SVG"
  exit 1
fi

resvg --background $BG -z $ZOOM $1 $OUT
convert $OUT -crop +$CROP+$CROP -crop -$CROP-$CROP tmp
mv tmp $OUT
