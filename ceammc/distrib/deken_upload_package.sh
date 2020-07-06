#!/bin/bash

BINDIR="@PROJECT_BINARY_DIR@/dist"
VERSION="@CEAMMC_LIB_VERSION@"
OBJLIST="@PROJECT_SOURCE_DIR@/ceammc/ext/doc/ceammc_deken_objlist.txt"
DEKEN="@DEKEN@"

CEAMMC_LIB_DIR="$BINDIR/ceammc"

if [ ! -d "$CEAMMC_LIB_DIR" ]
then
    echo "ceammc external directory not found: $CEAMMC_LIB_DIR"
    echo "may be you should call make ceammc_lib"
    exit 1
fi

cd "$BINDIR"

if [ ! -f ceammc[v@CEAMMC_LIB_VERSION@*.dek ]
then
    echo "deken file not found: $CEAMMC_LIB_DIR"
    echo "may be you should call make deken_package"
    exit 1
fi

$DEKEN upload --version $VERSION \
    --objects "${OBJLIST}" \
    --no-source-error \
    ceammc[v@CEAMMC_LIB_VERSION@*.dek
