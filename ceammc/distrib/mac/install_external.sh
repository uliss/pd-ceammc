#!/bin/sh

BIN_DIR="@PROJECT_BINARY_DIR@"
DIST_DIR="@PROJECT_BINARY_DIR@/dist"
OUTFILE="@CEAMMC_EXTERNAL_NAME@"
TAR_PATH="${BIN_DIR}/dist/${OUTFILE}"
DEST="${HOME}/Documents/Pd/externals"

# functions
function section() {
    tput setaf 2
    echo $1 "..."
    tput sgr0
}

# main
if [ ! -f "${TAR_PATH}" ]
then
    echo "file not found: ${TAR_PATH}"
    echo 'you have to run `make ceammc_lib` first'
    exit 1
fi

section "create directory '${DEST}'"
mkdir -p "${DEST}"

if [ -d "${DEST}/ceammc" ]
then
    section "rename existing ceammc directory to ceammc.old"
    rm -rf "${DEST}/ceammc.old"
    mv -f "${DEST}/ceammc" "${DEST}/ceammc.old"
fi

section "install '$(basename $TAR_PATH)'"
tar -C "${DEST}" -xzf "${TAR_PATH}"
section "done"
