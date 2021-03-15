#!/bin/bash

if [ $# -lt 1 ];
then
    echo "Usage: $(basename $0) FILE [TAG]"
    exit 1
fi

cd ~
eval `gpgshell`
cd -

if [[ -z ${GITHUB_TOKEN} ]]
then
    tput setaf 9
    echo "ERROR: "
    tput setaf 15
    echo "  \$GITHUB_TOKEN is not set. Exiting..."
    exit 1
fi

FILE="$1"
VERSION="@CEAMMC_DISTRIB_VERSION@"
CEAMMC_VERSION="@CEAMMC_LIB_VERSION@"

if [ ! -f "$FILE" ]
then
    tput setaf 9
    echo "ERROR: "
    tput setaf 15
    echo "  file is not found: $FILE. Exiting..."
    exit 1
fi

if [ $# -ne 2 ]
then
    TAG="v$VERSION"
else
    TAG=$2
fi

USER=uliss
REPO="pure-data"
NAME=$(basename $FILE)
DIR=$(dirname $FILE)

gpgenv exec github-asset "$USER/$REPO" list $TAG | grep "$NAME" > /dev/null

if [ $? == 0 ]
then
    echo "remove old '$NAME' from github..."
    gpgenv exec github-asset "$USER/$REPO" delete $TAG "$NAME"
fi

echo "uploading release '$NAME' ($TAG) to github..."

gpgenv exec github-asset "$USER/$REPO" upload $TAG "$FILE"
