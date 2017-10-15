#!/bin/bash

COV=$1
DIR=$2

echo "llvm-cov:      \"$COV\""
echo "src directory: \"$DIR\""

find ${DIR} -name *.gcda | while read f
do
    echo "    coverage: processing \"$(basename $f)\""
    ${COV} gcov -f $f 1>/dev/null
done
