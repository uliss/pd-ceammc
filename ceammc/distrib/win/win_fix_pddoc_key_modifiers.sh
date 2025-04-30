#!/bin/bash

BINDIR="@CMAKE_INSTALL_PREFIX@"

echo "Fixing help files in ${BINDIR} ..."

fix_pdhelp() {
    sed -i -e 's/⌘/⌃/g' "$1"
    echo "+ Done: '$(basename $1)'"
}

export -f fix_pdhelp

find "${BINDIR}/extra/ceammc" -name '*-help\.pd' | while read file
do
    fix_pdhelp $file
done
