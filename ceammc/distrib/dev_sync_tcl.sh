#!/bin/bash

DIST_DIR="@PROJECT_BINARY_DIR@/dist"
if [ `uname` == "Darwin" ]
then
    APP_DIR="${DIST_DIR}/@PD_MACOSX_APP@"
    TCL_DIR="${APP_DIR}/Contents/Resources/tcl"
else
    TCL_DIR="@CMAKE_INSTALL_PREFIX@/@PD_GUI_PLUGINS_PATH@"
fi

cp -v @PROJECT_SOURCE_DIR@/tcl/*.tcl "${TCL_DIR}/"

cp -v @PROJECT_SOURCE_DIR@/ceammc/gui/plugins/*.tcl "${TCL_DIR}/ceammc"

cp -v @PROJECT_SOURCE_DIR@/ceammc/gui/plugins/completion-plugin/*.tcl "${TCL_DIR}/ceammc"

cp -v @PROJECT_SOURCE_DIR@/ceammc/gui/plugins/completion-plugin/*.pd "${TCL_DIR}/ceammc"

