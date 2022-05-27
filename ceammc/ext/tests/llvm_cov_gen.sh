#!/bin/bash

COV=$1
DIR=$2

echo "pwd:           \"`pwd`\""
echo "llvm-cov:      \"$COV\""
echo "src directory: \"$DIR\""

find ${DIR} -name *.gcda | grep -v tab.c.gcda | grep -v lex\. | while read f
do
    echo "    coverage: processing \"$(basename $f)\""
    ${COV} gcov -f $f 1>/dev/null
done
