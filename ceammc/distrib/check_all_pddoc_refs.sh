#!/bin/bash

PD_DOCLS="@PD_DOCLS@"
OBJ_CHECK="@PD_OBJCHECK@"
DB="@PROJECT_SOURCE_DIR@/ceammc/ext/doc/ceammc.db"
DOCDIR="@PROJECT_SOURCE_DIR@/ceammc/ext/doc"
ABSDIR="@PROJECT_SOURCE_DIR@/ceammc/ext/abstractions"

for pddoc in @PROJECT_SOURCE_DIR@/ceammc/ext/doc/$1*.pddoc
do
    echo "checking $(basename $pddoc) ..."
    ${PD_DOCLS} --objects "$pddoc" | while read line
    do
        echo "    - $line"
        ${OBJ_CHECK} \
            --search-paths "${DOCDIR}" "${ABSDIR}" \
            --xlet-db "${DB}" \
            "$line"

        sleep 0.1
    done
done
