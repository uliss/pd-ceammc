#!/bin/bash

OTOOL="/usr/bin/otool"
TARGET="$1"
BINDIR="$2"

otool_dx() {
    ${OTOOL} -DX "$1" | tail -1
}

otool_lx() {
    ${OTOOL} -LX "$1" | sed -n /^$'\t'/p | cut -f2 | cut -d ' ' -f1
}

glib_fix() {
    DEP_LIB=$1
    GLIB=$2

    echo "FIX dependency: \"$DEP_LIB\" in \"$GLIB\""

    LIB_IN_BUNDLE=${BINDIR}/$(basename $DEP_LIB)
    if [ ! -f ${LIB_IN_BUNDLE} ]
    then
        echo "    copy \"${DEP_LIB}\" to \"${LIB_IN_BUNDLE}\""
        cp "${DEP_LIB}" ${LIB_IN_BUNDLE}
        chmod +w ${LIB_IN_BUNDLE}
    fi

    LIB_OLD_ID=$(otool_dx $DEP_LIB)
    LIB_NEW_ID=@loader_path/$(basename $DEP_LIB)

    echo "    change \"${LIB_OLD_ID}\" to \"${LIB_NEW_ID}\" in \"${GLIB}\""

    install_name_tool -id $LIB_NEW_ID ${LIB_IN_BUNDLE}
    install_name_tool -change $LIB_OLD_ID $LIB_NEW_ID ${GLIB}
}

otool_lx "$TARGET" | grep glib | while read shlib
do
    GLIB=${BINDIR}/$(basename $shlib)
    glib_fix ${shlib} ${TARGET}

    otool_lx ${GLIB} | grep pcre | while read dep
    do
        glib_fix ${dep} ${GLIB}
    done

    otool_lx ${GLIB} | grep libintl | while read dep
    do
        glib_fix ${dep} ${GLIB}
    done
done
