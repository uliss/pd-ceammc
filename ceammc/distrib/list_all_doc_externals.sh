#!/bin/bash

IFS=' '
all=$(xmllint --xpath '//entry[@name]/@name'  @PROJECT_SOURCE_DIR@/ceammc/ext/doc/ceammc_lib.xml)

for x in $all
do
    echo $x | cut -d= -f2 | tr -d '"' | sed '/^[[:space:]]*$/d'
done

