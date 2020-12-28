#!/bin/bash

BINDIR="@PROJECT_BINARY_DIR@/dist"
VERSION="v@CEAMMC_DISTRIB_VERSION@"

if [ $# -lt 1 ]
then
    echo "Usage: $(basename $0) FILE [SUFFIX]"
    exit 1
fi

if [[ -z ${BINTRAY_API_KEY} ]]
then
    tput setaf 9
    echo "ERROR: "
    tput setaf 15
    echo "  \$BINTRAY_API_KEY is not set. Are you using 'gpgenv exec'?"
    exit 1
fi

if [ $# -eq 2 ]
then
    VERSION="${VERSION}.$2"
fi

URL="https://api.bintray.com/content/uliss/ceammc/PureData/${VERSION}/$1?override=1"

curl -T $1 -uuliss:$BINTRAY_API_KEY $URL
