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

find_all_dll_deps() {
    # PureData vanilla binaries
    list_dll @PROJECT_BINARY_DIR@/src/pdsend.exe
    list_dll @PROJECT_BINARY_DIR@/src/pdreceive.exe
    list_dll @PROJECT_BINARY_DIR@/src/pd.exe
    list_dll @PROJECT_BINARY_DIR@/src/pd.dll

    # ceammc dlls
    find "@PROJECT_BINARY_DIR@/ceammc/ext" -name *.dll | grep -v tests | while read dll
    do
        list_dll "$dll"
    done

    # ceammc externals x86_64
    find "@PROJECT_BINARY_DIR@/ceammc/ext" -name *.m_amd64 | grep -v tests | while read dll
    do
        list_dll "$dll"
    done

    # ceammc externals x86
    find "@PROJECT_BINARY_DIR@/ceammc/ext" -name *.m_i386 | grep -v tests | while read dll
    do
        list_dll "$dll"
    done
}

find_all_dll_deps | sort | uniq | while read dll
do
    cp -v "$dll" "@CMAKE_INSTALL_PREFIX@/@PD_EXE_INSTALL_PATH@"
done
