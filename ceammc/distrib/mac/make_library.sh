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
DYLIBFIX="@DYLIBFIX@"
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

function find_dlls() {
    find "$1" -name "$2\\.d_fat" \
        -o -name "$2\\.d_amd64" \
        -o -name "$2\\.pd_darwin" \
        -o -name "$2\\.d_i386" \
        -o -name "$2\\.dylib"
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
        --ignore /usr/local/opt/llvm/lib &>"${BINDIR}/ceammc_lib.log"
}

# main

section "installing"
$CMAKE --install ${BINDIR} --prefix ${INSTALL_DIR}

section "copy installed files"

rm -rf "${OUTFILE}"
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
$DYLIBFIX --dir ${CEAMMC_DIR} \
    --files  $(find_dlls ${CEAMMC_DIR} "*" | tr '\n' ' ')  \
    --rpaths $(otool -l ${BINDIR}/ceammc/ext/src/ceammc.pd_darwin | grep RPATH -A2 | grep path | awk '{print $2}' | tr '\n' ' ')

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

section "check for external dylib references"
num_errors=0
for external in $(find_dlls ${CEAMMC_DIR} "*")
do
    short_name=$(basename $external)
    dep=$(otool -L $external | grep -v -e '/System/Library' \
            -e '@loader_path' \
            -e '@rpath' \
            -e $short_name \
            -e 'libc++' \
            -e '/usr/lib' \
            -e 'libSystem' \
            -e 'libgcc')

    if [ "$dep" ]; then
        echo "${red}WARNING:${rst} external ${blu}${short_name}${rst} has this dependencies:"
        echo "         $dep"
        let num_errors=num_errors+1
    fi
done

if [[ $num_errors -eq 0 ]]
then
    echo "Check bundle: ${green}OK${rst}"
else
    echo "Check bundle: ${red}FAILED${rst}"
    exit 2
fi

cd "$OUTDIR"
section "create ZIP archive"
tar cfvz "${OUTFILE}" $(basename $CEAMMC_DIR)

section "ZIP archive: "
ls -l "${OUTFILE}"
