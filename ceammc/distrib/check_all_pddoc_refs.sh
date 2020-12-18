#!/bin/bash

PDDOC_LS="pd_doc2ls"
OBJ_CHECK="pd_objcheck"
DB="@PROJECT_SOURCE_DIR@/ceammc/ext/doc/ceammc.db"

for pddoc in @PROJECT_SOURCE_DIR@/ceammc/ext/doc/$1*.pddoc
do
    echo "checking $(basename $pddoc) ..."
    ${PDDOC_LS} "$pddoc" | sort | uniq | while read line
    do
        echo "    - $line"
        ${OBJ_CHECK} --xlet-db "${DB}" $line 1>/dev/null
    done
done
