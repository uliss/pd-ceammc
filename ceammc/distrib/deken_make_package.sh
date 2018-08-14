#!/bin/bash

BINDIR="@PROJECT_BINARY_DIR@/dist"
VERSION="@CEAMMC_LIB_VERSION@"
DEKEN="@DEKEN@"

CEAMMC_LIB_DIR="$BINDIR/ceammc"

if [ ! -d "$CEAMMC_LIB_DIR" ]
then
    echo "ceammc external directory not found: $CEAMMC_LIB_DIR"
    echo "may be you shoudl call make ceammc_lib"
    exit 1
fi

$DEKEN package --version $VERSION "${CEAMMC_LIB_DIR}"
