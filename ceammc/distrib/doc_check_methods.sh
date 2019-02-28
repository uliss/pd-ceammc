#!/bin/bash

LSDOC="@PROJECT_BINARY_DIR@/list_all_doc_externals.sh"
CHECKER="@PROJECT_BINARY_DIR@/check_doc_external.py"

trap "exit" INT

$LSDOC | while read f
do
    $CHECKER -m $f
done
