#!/bin/sh

BIN_DIR="@PROJECT_BINARY_DIR@"
DIST_DIR="@PROJECT_BINARY_DIR@/dist"
VERSION="@CEAMMC_LIB_VERSION@"
FLOATSIZE="f@PD_FLOAT_SIZE@"
SYSVER=$(sw_vers | grep ProductVersion | cut -f2 | cut -f1,2 -d.)
OUTFILE="ceammc-${VERSION}-macosx-${SYSVER}-pd-@PD_TEXT_VERSION_SHORT@-${FLOATSIZE}.tar.gz"
DEST="${HOME}/Documents/Pd/externals"

if [ ! -f "${BIN_DIR}/${OUTFILE}" ]
then
    echo "file not found: ${OUTFILE}"
    echo 'you have to run `make ceammc_lib` first'
    exit 1
fi

mkdir -p "${DEST}"

if [ -d "${DEST}/ceammc" ]
then
    echo "renaming ceammc directory to ceammc.old ..."
    rm -rf "${DEST}/ceammc.old"
    mv -f "${DEST}/ceammc" "${DEST}/ceammc.old"
fi

tar -C "${DEST}" -xzf "${BIN_DIR}/${OUTFILE}"
echo "Done"
