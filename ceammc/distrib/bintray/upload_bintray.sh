#!/bin/bash

BINDIR="@PROJECT_BINARY_DIR@/dist"
VERSION="v@CEAMMC_DISTRIB_VERSION@"
OVERRIDE=0
PUBLISH=0

usage() {
    echo "Usage: $(basename $0) [OPTIONS] FILE"
    echo " OPTIONS: -h show this help"
    echo "          -p publish uploaded file"
    echo "          -o override existing file"
    echo "          -v <version> specify version suffix"
}

if [ $# -lt 1 ]
then
    usage
    exit 1
fi

while getopts "hpov:" opt
do
case $opt in
h) usage && exit 0;;
v) VERSION="${VERSION}.${OPTARG}";;
o) OVERRIDE=1;;
p) PUBLISH=1;;
?) usage && exit 1;;
esac
done

if [[ -z ${BINTRAY_API_KEY} ]]
then
    tput setaf 9
    echo "ERROR: "
    tput setaf 15
    echo "  \$BINTRAY_API_KEY is not set. Are you using 'gpgenv exec'?"
    exit 1
fi

FILE=${@:$OPTIND:1}
NAME=$(basename $FILE)
URL="https://api.bintray.com/content/uliss/ceammc/PureData/${VERSION}/${NAME}?override=${OVERRIDE}&publish=${PUBLISH}"

curl -T $FILE -uuliss:$BINTRAY_API_KEY $URL
