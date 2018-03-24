#!/bin/bash

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

DMG="$1"
VERSION="@CEAMMC_DISTRIB_VERSION@"
CEAMMC_VERSION="@CEAMMC_LIB_VERSION@"

if [ ! -f "$DMG" ]
then
    tput setaf 9
    echo "ERROR: "
    tput setaf 15
    echo "  file is not found: $DMG. Exiting..."
    exit 1
fi

TAG="v$VERSION"
USER=uliss
REPO="pure-data"
NAME=$(basename $DMG)
DIR=$(dirname $DMG)

SRC_TAR="${DIR}/pd-ceammc-${VERSION}-src.tar.gz"

if [ -f "$SRC_TAR" ]
then
    echo "uploading $(basename $SRC_TAR) to github..."
    github-release upload --user $USER \
        --repo $REPO \
        --tag $TAG \
        --name $(basename $SRC_TAR) \
        --file "$SRC_TAR" \
        --replace
fi

SRC_ZIP="${DIR}/pd-ceammc-${VERSION}-src.zip"

if [ -f "$SRC_ZIP" ]
then
    echo "uploading $(basename $SRC_ZIP) to github..."
    github-release upload --user $USER \
        --repo $REPO \
        --tag $TAG \
        --name $(basename $SRC_ZIP) \
        --file "$SRC_ZIP" \
        --replace
fi

CEAMMC_TAR_GZ="${DIR}/ceammc-${CEAMMC_VERSION}-macosx-*.tar.gz"

if [ -f ${CEAMMC_TAR_GZ} ]
then
    fname=$(basename ${CEAMMC_TAR_GZ})
    echo "uploading $fname to github..."
    github-release upload --user $USER \
        --repo $REPO \
        --tag $TAG \
        --name $fname \
        --file ${CEAMMC_TAR_GZ} \
        --replace
fi

echo "uploading release DMG ($TAG) to github..."

github-release upload --user $USER \
    --repo $REPO \
    --tag $TAG \
    --name $NAME \
    --file "$DMG" \
    --replace


