#!/bin/bash

DIST_DIR="@PROJECT_BINARY_DIR@/dist"
if [ `uname` == "Darwin" ]
then
    APP_DIR="${DIST_DIR}/@PD_MACOSX_APP@"
    TCL_DIR="${APP_DIR}/Contents/Resources/tcl"
else
    TCL_DIR="@CMAKE_INSTALL_DIR@/@PD_GUI_PLUGINS_PATH@"
fi

cp -v @PROJECT_SOURCE_DIR@/tcl/*.tcl "${TCL_DIR}/"
