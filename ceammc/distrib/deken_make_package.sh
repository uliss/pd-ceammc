#!/bin/bash

BINDIR="@PROJECT_BINARY_DIR@/dist"
VERSION="@CEAMMC_LIB_VERSION@"
OBJLIST="@PROJECT_SOURCE_DIR@/ceammc/ext/doc/ceammc_deken_objlist.txt"
DEKEN="@DEKEN@"
FLOATSIZE="@DEKEN_FLOAT_SIZE@"

CEAMMC_LIB_DIR="$BINDIR/ceammc"

if [ ! -d "$CEAMMC_LIB_DIR" ]
then
    echo "ceammc external directory not found: $CEAMMC_LIB_DIR"
    echo "may be you should call make ceammc_lib"
    exit 1
fi

cd "@BINDIR@"
rm -f ceammc[v@CEAMMC_LIB_VERSION@*.dek.txt

$DEKEN package --version $VERSION \
    --objects "${OBJLIST}" \
    --default-floatsize ${FLOATSIZE} \
    "${CEAMMC_LIB_DIR}"
