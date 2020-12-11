#!/bin/bash

CEAMMC_PATH="@PROJECT_SOURCE_DIR@/ceammc/ext/src"
DOC_PATH="@PROJECT_SOURCE_DIR@/ceammc/ext/doc"
PD_DOC2CXX="pd_doc2cxx"

if [ $# -ne 1 ]
then
   echo "Usage: $(basename $0) CPP_FILE"
   exit 1
fi

F=$(basename $1)
CXXF=$(find $CEAMMC_PATH -name $F)

if [ ! -f "$CXXF" ]
then
    echo "cpp file not found: $F ..."
    exit 2
fi

obj=`cat "$CXXF" | grep 'Factory<' | cut -d'"' -f 2`

if [ -z $obj ]
then
    echo "object name not found in cpp file: $CXXF ..."
    exit 3
fi

DOCF="$DOC_PATH/$obj.pddoc"

if [ ! -f "$DOCF" ]
then
    echo "pddoc file not found: $DOCF ..."
    exit 4
fi

$PD_DOC2CXX "$DOCF"
