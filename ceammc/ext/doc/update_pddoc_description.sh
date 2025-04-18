#!/bin/bash

echo "- processing $1 ..."

DESCR=$(xmlstarlet sel -t -v //description $1)
echo $DESCR

xmlstarlet ed -P -u //description -v "" $1 | 
	xmlstarlet ed -P -s //description -t elem -n tr -v "$DESCR" |
	xmlstarlet ed -P -s //description/tr -t attr -n lang -v en > tmp

if [ $? -eq 0 ]
then
	mv tmp $1
fi


