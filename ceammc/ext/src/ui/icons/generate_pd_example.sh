#!/bin/bash

ICONS=$(ls ic_*24dp.png | cut -d_ -f2- | sed 's/_black_24dp\.png//' | sort)
LEN=1

for i in $ICONS
do
    len=$(echo $i | wc -c)
    if [[ $len -gt $LEN ]];
    then
        LEN=$len;
    fi
done

NLEN=$((LEN+9))
CLEN=$((LEN+3))

for i in $ICONS
do
    printf "%s\n%s\n\n" "/*$i*/" "[ui.icon $i]"
done
