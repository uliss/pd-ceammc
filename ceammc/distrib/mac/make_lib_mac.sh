#!/bin/sh

if [ $# -ne 1 ]
then
    echo "Usage: $0 OUTDIR"
fi

SRCDIR="@PROJECT_SOURCE_DIR@/ceammc"
BINDIR="@PROJECT_BINARY_DIR@"
OUTDIR="$1"
OUTFILE="@CEAMMC_EXTERNAL_NAME@"
DYLIBBUNDLER="@DYLIBBUNDLER@"
CMAKE="@CMAKE_COMMAND@"
INSTALL_DIR="@PROJECT_BINARY_DIR@/dist/pd_ceammc"
CEAMMC_DIR="${OUTDIR}/ceammc"

# functions

function section() {
    echo
    tput setaf 2
    echo $1 "..."
    tput sgr0
}

function find_ext() {
    find "$1" -name "$2\\.d_fat" \
        -o -name "$2\\.d_amd64" \
        -o -name "$2\\.pd_darwin" \
        -o -name "$2\\.d_i386"
}

function skip_ext {
    #skip experimental extensions
    exp=$(echo $1 | grep -v '/exp' | grep -v '/tl' | grep -v '/test')
    if [ -z $exp ]
    then
        return 1
    else
        return 0
    fi
}

function dylib_external_fix() {
    exec=$1
    dir=$2

    echo "\t- fixing @loader_path for $(basename $exec) ..."
    $DYLIBBUNDLER \
        --fix-file "${exec}" \
        --bundle-deps \
        --create-dir \
        --dest-dir "${dir}" \
        --install-path @loader_path/ \
        --overwrite-files \
        --ignore /usr/local/opt/llvm/lib
}

# main

section "installing"
$CMAKE --install ${BINDIR} --prefix ${INSTALL_DIR}

section "copy installed files"

rm -rf "${CEAMMC_DIR}"
mkdir -p "${CEAMMC_DIR}"

cp "${INSTALL_DIR}/lib/"*.dylib "${CEAMMC_DIR}"
cp -R "${INSTALL_DIR}/lib/pd_ceammc/extra/ceammc" "${OUTDIR}"

# modules
cp "${INSTALL_DIR}/lib/pd_ceammc/extra/numeric"/* "${CEAMMC_DIR}"
cp "${INSTALL_DIR}/lib/pd_ceammc/extra/matrix"/* "${CEAMMC_DIR}"
cp "${INSTALL_DIR}/lib/pd_ceammc/extra/soundtouch~"/* "${CEAMMC_DIR}"
cp "${INSTALL_DIR}/lib/pd_ceammc/extra/index-help.pd" "${CEAMMC_DIR}"

section "fix dlls"
find_ext ${CEAMMC_DIR} "*" | while read file
do
    ext_name=$(basename $file)
    echo "+ Fix DLL: '$ext_name'"
    ${DYLIBBUNDLER} -x ${CEAMMC_DIR}/$ext_name -b -d ${CEAMMC_DIR} -p @loader_path/ -of
done

section "fix help files index"
find "${CEAMMC_DIR}" -name *\\.pd -maxdepth 1 | while read file
do
    help=$(basename $file)
    gsed -i \
        -e 's/ceammc\/ceammc-help\.pd/ceammc-help.pd/' \
        -e 's/\.\.\/index-help\.pd/index-help.pd/' \
        -e 's/ceammc\/soundtouch-help\.pd/soundtouch-help.pd/' \
        $file
    echo "+ Help: '$help'"
done

cd "$OUTDIR"
section "create ZIP archive"
tar cfvz "${OUTFILE}" $(basename $CEAMMC_DIR)
