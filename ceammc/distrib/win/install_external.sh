#!/bin/sh

BIN_DIR="@PROJECT_BINARY_DIR@"
ARCH="@CEAMMC_DISTRIB_ARCH@"
OUTFILE="@CEAMMC_EXTERNAL_NAME@"
DEST="${USERPROFILE}/Documents/Pd/externals/${ARCH}"
P7Z_EXE="@7Z_EXE@"
ZIP_FULL_PATH="${BIN_DIR}/dist/${OUTFILE}"

if [ ! -f "${ZIP_FULL_PATH}" ]
then
    echo "file not found: ${ZIP_FULL_PATH}"
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

cd "${BIN_DIR}/dist"
"${P7Z_EXE}" x "${OUTFILE}" -o"${DEST}"
echo "external installed to '${DEST}'"
