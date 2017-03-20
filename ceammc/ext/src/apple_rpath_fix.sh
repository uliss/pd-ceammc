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

rpath_fix() {
    DEP_LIB=$1
    _TARGET=$2

    echo "FIX dependency: \"$DEP_LIB\" in \"${_TARGET}\""

    if [ ! -f ${DEP_LIB} ]
    then
        echo "    file not found: ${DEP_LIB}. skipping..."
        return
    fi

    LIB_IN_BUNDLE=${BINDIR}/$(basename $DEP_LIB)
    if [ ! -f ${LIB_IN_BUNDLE} ]
    then
        echo "    copy \"${DEP_LIB}\" to \"${LIB_IN_BUNDLE}\""
        cp "${DEP_LIB}" ${LIB_IN_BUNDLE}
        chmod +w ${LIB_IN_BUNDLE}
    fi

    LIB_OLD_ID=$(otool_dx $DEP_LIB)
    LIB_NEW_ID=@loader_path/$(basename $DEP_LIB)

    if [ -z ${LIB_OLD_ID} ]
    then
        echo "    already done. skipping..."
        return
    fi

    echo "    change \"${LIB_OLD_ID}\" to \"${LIB_NEW_ID}\" in \"${_TARGET}\""

    install_name_tool -id $LIB_NEW_ID ${LIB_IN_BUNDLE}
    install_name_tool -change $LIB_OLD_ID $LIB_NEW_ID ${_TARGET}
}

otool_lx "$TARGET" | grep glib | while read shlib
do
    GLIB=${BINDIR}/$(basename $shlib)
    rpath_fix ${shlib} ${TARGET}

    otool_lx ${GLIB} | grep pcre | while read dep
    do
        rpath_fix ${dep} ${GLIB}
    done

    otool_lx ${GLIB} | grep libintl | while read dep
    do
        rpath_fix ${dep} ${GLIB}
    done
done

otool_lx "$TARGET" | grep libsndfile | while read shlib
do
    LIBSNDFILE=${BINDIR}/$(basename $shlib)
    rpath_fix ${shlib} ${TARGET}

    otool_lx ${LIBSNDFILE} | grep FLAC | while read dep
    do
        rpath_fix ${dep} ${LIBSNDFILE}
    done

    otool_lx ${LIBSNDFILE} | grep ogg | while read dep
    do
        rpath_fix ${dep} ${LIBSNDFILE}
    done

    otool_lx ${LIBSNDFILE} | grep libvorbis\\. | while read dep
    do
        rpath_fix ${dep} ${LIBSNDFILE}

        if [ -f ${dep} ]
        then
            otool_lx ${dep} | grep ogg | while read dep2
            do
                rpath_fix ${dep2} "${BINDIR}/$(basename $dep)"
            done
        fi
    done

    otool_lx ${LIBSNDFILE} | grep libvorbisenc | while read dep
    do
        rpath_fix ${dep} ${LIBSNDFILE}

        if [ -f ${dep} ]
        then
            otool_lx ${dep} | grep ogg | while read dep2
            do
                rpath_fix ${dep2} "${BINDIR}/$(basename $dep)"
            done
        fi
    done
done
