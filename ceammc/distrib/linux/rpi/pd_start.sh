#!/bin/bash

PD=@CMAKE_INSTALL_PREFIX@/bin/pd-ceammc
FILE="${HOME}/Documents/Pd/main.pd"

if [ -f $FILE ]; then
    echo "Start file exists"
    $PD "$FILE"
else
    echo "Start file not found ($FILE) ..."
    $PD
fi
