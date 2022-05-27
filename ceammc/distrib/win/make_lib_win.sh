#!/bin/bash

if [ $# -ne 1 ]
then
    echo "Usage: $0 OUTDIR"
    exit 1
fi

OUTDIR="$1"
INSTALL_EXTRA_DIR="@CMAKE_INSTALL_PREFIX@/extra"
TMP_CEAMMC_DIR="${OUTDIR}/ceammc"
OUTFILE="@CEAMMC_EXTERNAL_NAME@"
P7Z_EXE="@7Z_EXE@"
OBJDUMP=$(which objdump.exe)

if [ -z $OBJDUMP ]
then
    echo "objdump.exe is not found"
    exit 1
fi

section() {
    echo
    echo -e "\033[32m" $1 "...\033[0m"
}

list_dll () {
    $OBJDUMP -x $1 | grep 'DLL Name' | awk '{print $3}' | while read dll
    do
        fname="$MINGW_PREFIX/bin/$dll"
        if [ -e "$fname" ]
        then
            echo "$fname"
            list_dll "$fname"
        fi
    done
}

find_all_dll_deps() {
    find "${TMP_CEAMMC_DIR}" \
        -name '*.dll' \
        -o -name '*.m_amd64' \
        -o -name '*.m_i386' | while read dll
    do
        list_dll "$dll"
    done
}


echo "    - install dir:  @CMAKE_INSTALL_PREFIX@"
echo "    - output dir:   ${OUTDIR}"
echo "    - tmp dir:      ${TMP_CEAMMC_DIR}"
echo "    - output file:  ${OUTFILE}"
echo "    - 7zip: ${P7Z_EXE}"

if [ ! -d "@CMAKE_INSTALL_PREFIX@" ]
then
    echo "ERROR: directory with installed binaries not found: @CMAKE_INSTALL_PREFIX@ ..."
    echo "probably your should invoke 'make install'"
    exit 1
fi

section "remove temp directory '${TMP_CEAMMC_DIR}'"
rm -rf "${TMP_CEAMMC_DIR}"

section "copy ceammc"
cp -R "${INSTALL_EXTRA_DIR}/ceammc" "${TMP_CEAMMC_DIR}"

# modules
section "copy numeric"
cp "${INSTALL_EXTRA_DIR}/numeric"/*     "${TMP_CEAMMC_DIR}"
section "copy matrix"
cp "${INSTALL_EXTRA_DIR}/matrix"/*      "${TMP_CEAMMC_DIR}"
section "copy soundtouch"
cp "${INSTALL_EXTRA_DIR}/soundtouch~"/* "${TMP_CEAMMC_DIR}"
cp "${INSTALL_EXTRA_DIR}/index-help.pd" "${TMP_CEAMMC_DIR}"

section "fix dlls"
find_all_dll_deps | sort | uniq | while read dll
do
    echo "DLL: $dll"
    cp -v "$dll" "${TMP_CEAMMC_DIR}"
done

section "fix help files links"
find "${TMP_CEAMMC_DIR}" -name "*\\.pd" -maxdepth 1 | while read file
do
    help=$(basename $file)
    sed -i \
        -e 's|ceammc/ceammc-help\.pd|ceammc-help.pd|' \
        -e 's|\.\./index-help\.pd|index-help.pd|' \
        -e 's|ceammc/soundtouch-help\.pd|soundtouch-help.pd|' \
        $file
    echo "+ Help: '$help'"
done

section "make ZIP archive"
if [ -x "${P7Z_EXE}" ]
then
    cd "${OUTDIR}"
    "${P7Z_EXE}" a "${OUTFILE}" ceammc
else
    echo "7z is not found. Create zip archive manually..."
fi
