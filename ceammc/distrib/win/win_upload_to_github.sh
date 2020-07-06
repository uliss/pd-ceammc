#!/bin/bash

if [ $# -ne 1 ]
then
    echo "Usage: $(basename $0) FILE"
    exit 1
fi

TAG="v@CEAMMC_DISTRIB_VERSION@"
NAME=$(basename $1)
USER="uliss"
REPO="pure-data"

echo "Uploading $NAME to github release $TAG ..."

gpgenv exec \
	github-release upload -u $USER -r $REPO -t ${TAG} --name $NAME -f $1 -R

