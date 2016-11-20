#!/bin/bash

OTOOL="/usr/bin/otool"
TARGET="$1"
BINDIR="$2"

${OTOOL} -LX "$TARGET" | cut -f2 | cut -d ' ' -f1 | grep glib | while read shlib
do
    TARGETID=$(otool -DX $shlib)
    GLIB=${BINDIR}/$(basename $shlib)
    cp "$shlib" ${GLIB}
    chmod +w ${GLIB}
    NEWTARGETID=@loader_path/$(basename $shlib)
    install_name_tool -id $NEWTARGETID $GLIB
    install_name_tool -change $TARGETID $NEWTARGETID $TARGET

    ${OTOOL} -LX ${GLIB} | cut -f2 | cut -d ' ' -f1 | grep pcre | while read dep
    do
        PCRE=${BINDIR}/$(basename $dep)
        cp "$dep" $PCRE
        chmod +w ${PCRE}

        TARGETID=$(otool -DX $dep)
        NEWTARGETID=@loader_path/$(basename $dep)
        install_name_tool -id $NEWTARGETID $PCRE
        install_name_tool -change $TARGETID $NEWTARGETID ${GLIB}
    done

    ${OTOOL} -LX ${GLIB} | cut -f2 | cut -d ' ' -f1 | grep libintl | while read dep
    do
        INTL=${BINDIR}/$(basename $dep)
        cp "$dep" $INTL
        chmod +w ${INTL}

        TARGETID=$(otool -DX $dep)
        NEWTARGETID=@loader_path/$(basename $dep)
        install_name_tool -id $NEWTARGETID $INTL
        install_name_tool -change $TARGETID $NEWTARGETID ${GLIB}
    done
done

