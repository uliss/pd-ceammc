#!/bin/bash

COV=$1
DIR=$2

echo "extrating gcov dat with $COV in $DIR"

find ${DIR} -name *.gcda | while read f
do
    echo "    coverage: processing \"$(basename $f)\""
    ${COV} gcov $f 1>/dev/null
done
