#!/bin/bash

VERSION="1.12"

for x in "$@" 
do
	echo $x
	xmlstarlet ed -P -L -u '//pddoc/@version' -v $VERSION "$x"
done
