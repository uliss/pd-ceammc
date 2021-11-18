#!/bin/bash

OBJDUMP=$(which objdump.exe)
if [ -z $OBJDUMP ]
then
    echo "objdump.exe is not found"
    exit 1
fi

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

find_by_dll_ext() {
    find "@PROJECT_BINARY_DIR@/ceammc/ext" -path '*/tests' -prune -o -name "*.$1" -print | while read dll
    do
        list_dll "$dll"
    done
}

find_all_dll_deps() {
    # PureData vanilla binaries
    list_dll "@PROJECT_BINARY_DIR@/src/pdsend.exe"
    list_dll "@PROJECT_BINARY_DIR@/src/pdreceive.exe"
    list_dll "@PROJECT_BINARY_DIR@/src/pd.exe"
    list_dll "@PROJECT_BINARY_DIR@/src/pd.dll"

    find_by_dll_ext dll
    find_by_dll_ext m_i386
    find_by_dll_ext m_amd64
}

find_all_dll_deps | sort | uniq | while read dll
do
    cp -v "$dll" "@CMAKE_INSTALL_PREFIX@/@PD_EXE_INSTALL_PATH@"
done
