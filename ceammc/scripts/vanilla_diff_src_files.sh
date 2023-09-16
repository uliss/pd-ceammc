#!/bin/bash

if [ $# -ne 1 ]
then
    echo "Usage: `basename $0` VANILLA_SRC_DIR GREP_PATTERN?"
    exit 1
fi

VANILLA_SRC_DIR="$1/src"
CEAMMC_SRC_DIR="@PROJECT_SOURCE_DIR@/src"

TMPDIR=$(mktemp -d)
CEAMMC_FILES=$TMPDIR/ceammc_files
VANILLA_FILES=$TMPDIR/vanilla_files

CEAMMC_FILES=(g_style.h g_style.c CMakeLists.txt pd-ceammc.rc.in s_midi_coremidi.c x_ceammc.c x_ceammc.h g_ceammc_draw.h g_ceammc_draw.c)

for item in ${CEAMMC_FILES[*]}
do
    CEAMMC_FILES_SED="$CEAMMC_FILES_SED/$item/d;"
done

if [ -z "$2" ]
then
    ls "${CEAMMC_SRC_DIR}"  | sed $CEAMMC_FILES_SED > $CEAMMC_FILES
    ls "${VANILLA_SRC_DIR}" | grep "$2" > $VANILLA_FILES
else
    ls "${CEAMMC_SRC_DIR}"  | sed $CEAMMC_FILES_SED | grep "$2" > $CEAMMC_FILES
    ls "${VANILLA_SRC_DIR}" | grep "$2" > $VANILLA_FILES
fi

colordiff $CEAMMC_FILES $VANILLA_FILES

rm -rf $TMPDIR
