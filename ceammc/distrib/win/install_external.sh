#!/bin/sh

BIN_DIR="@PROJECT_BINARY_DIR@"
VERSION="@CEAMMC_LIB_VERSION@"
ARCH="@CEAMMC_DISTRIB_ARCH@"
PRECISION="f@PD_FLOAT_SIZE@"
PD_VERSION="@PD_TEXT_VERSION_FULL@"
OUTFILE="ceammc-${VERSION}-win-pd-${PD_VERSION}-${ARCH}-${PRECISION}.zip"
DEST="${USERPROFILE}/Documents/Pd/externals/${ARCH%-bit}"
P7Z_EXE="@7Z_EXE@"

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

cd "${BIN_DIR}"
"${P7Z_EXE}" x "${OUTFILE}" -o"${DEST}"
echo "Done"
