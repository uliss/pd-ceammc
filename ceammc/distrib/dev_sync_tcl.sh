#!/bin/bash

DIST_DIR="@PROJECT_BINARY_DIR@/dist"
APP_DIR="${DIST_DIR}/@PD_MACOSX_APP@"
TCL_DIR="${APP_DIR}/Contents/Resources/tcl"

cp @PROJECT_SOURCE_DIR@/tcl/*.tcl "${TCL_DIR}/"
