#!/bin/bash

BINDIR="@PROJECT_BINARY_DIR@/dist"
VERSION="@CEAMMC_LIB_VERSION@"
OBJLIST="@PROJECT_SOURCE_DIR@/ceammc/ext/doc/ceammc_deken_objlist.txt"
DEKEN="@DEKEN@"
EXPECT="@EXPECT@"
EXPECT_LOGIN="@PROJECT_BINARY_DIR@/dist/deken_expect_login"

CEAMMC_LIB_DIR="$BINDIR/ceammc"

if [ ! -d "$CEAMMC_LIB_DIR" ]
then
    echo "ceammc external directory not found: $CEAMMC_LIB_DIR"
    echo "may be you should call make ceammc_lib"
    exit 1
fi

cd "$BINDIR"

FOUND=$(ls ceammc"["v@CEAMMC_LIB_VERSION@*.dek | wc -l)

if [[ "$FOUND" -ne 1 ]]
then
    echo "deken file not found: $CEAMMC_LIB_DIR"
    echo "may be you should call make deken_package"
    exit 1
fi

DEKEN_FILE=$(find . -name 'ceammc\[v@CEAMMC_LIB_VERSION@*.dek')

echo "found deken file: $DEKEN_FILE"

echo "loading gpgenv"
cd "@PROJECT_BINARY_DIR@"
eval `gpgshell`
echo "username: $DEKEN_USERNAME"
cd "$BINDIR"
$EXPECT_LOGIN "$DEKEN_FILE" "$DEKEN_PASS"
