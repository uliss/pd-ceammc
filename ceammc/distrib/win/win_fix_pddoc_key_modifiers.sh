#!/bin/bash
SRCDIR="@PROJECT_SOURCE_DIR@/ceammc"
BINDIR="@CMAKE_INSTALL_PREFIX@"

echo "Fixing help files in ${SRCDIR} ..."
find "${BINDIR}/extra/ceammc" -name *-help\\.pd | while read file
do
    help=$(basename $file)
    sed -i -e 's/⌘/⌃/g' "$file"
    echo "+ Done: '$help'"
done
