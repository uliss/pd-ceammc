#!/bin/bash

BINDIR="@CMAKE_INSTALL_PREFIX@"

echo "Fixing help files in ${BINDIR} ..."

fix_pdhelp() {
    sed -i -e 's/⌘/⌃/g' "$1"
    echo "+ Done: '$(basename $1)'"
}

export -f fix_pdhelp

if [ -x ${PARALLEL} ]
then
    find "${BINDIR}/extra/ceammc" -name '*-help\.pd' | parallel -j4 fix_pdhelp {}
else
    find "${BINDIR}/extra/ceammc" -name '*-help\.pd' | while read file
    do
        fix_pdhelp $file
    done
fi
