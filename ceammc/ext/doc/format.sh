#!/bin/bash

dir=$(dirname $0)

for f in $dir/*.pddoc
do
	perl -pi -e 's/\s+(<!\[CDATA\[)/$1/' $f
	perl -pi -e 's/^(<\/pdascii>)/            $1/' $f
done


