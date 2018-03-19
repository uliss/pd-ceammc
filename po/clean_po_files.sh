#!/bin/bash

if [ $# -ne 1 ];
then
	echo "Usage: $(basename $0) PO_FILE"
	exit 1
fi

PO_FILE=$1

if [ ! -f "$PO_FILE" ]
then
	echo "File not exists: $PO_FILE"
	exit 2
fi

cat $PO_FILE | grep '#:' | cut -d: -f2 | sort | uniq | while read f
do
	if [ ! -f "$f" ]
	then
		echo "Warning: source file is not found \"$f\""
		gsed -i "/$f/d" $PO_FILE
	fi
done


