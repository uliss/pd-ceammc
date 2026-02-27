#!/bin/bash

FILE="@PROJECT_SOURCE_DIR@/cmake/version.cmake"

cd "@PROJECT_SOURCE_DIR@"
let REV=$(git describe | cut -d- -f2)
if [ -z "$REV" ]; then
    echo "can't get git revision"
    exit 1
fi
echo "trying to set deb revision: r$REV ..."

gsed -E -i 's/LINUX_DEBIAN_PACKAGE_REVISION[[:space:]]+"\.r[0-9]+"/LINUX_DEBIAN_PACKAGE_REVISION ".r'$REV'"/' "$FILE"
echo "updated to: r$REV"
