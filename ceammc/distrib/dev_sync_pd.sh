#!/bin/bash

PD_EXEC="$1"

DIST_DIR="@PROJECT_BINARY_DIR@/dist"
if [ `uname` == "Darwin" ]
then
    APP_DIR="${DIST_DIR}/@PD_MACOSX_APP@"
    PD_DIR="${APP_DIR}/Contents/Resources/bin"
    PD_CORE="${APP_DIR}/Contents/Resources/bin"
else
    PD_DIR="@CMAKE_INSTALL_PREFIX@/@PD_EXE_INSTALL_PATH@"
    PD_CORE="@CMAKE_INSTALL_PREFIX@/@PDCORE_LIB_INSTALL_PATH@"
fi

cp -v "$PD_EXEC" "${PD_DIR}/"
