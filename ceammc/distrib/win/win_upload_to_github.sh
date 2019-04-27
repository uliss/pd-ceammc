#!/bin/bash

if [ $# -ne 1 ]
then
    echo "Usage: $(basename $0) FILE"
    exit 1
fi

if [ -z "$GITHUB_TOKEN" ]
then
    echo "env variable GITHUB_TOKEN is not set..."
    exit 2
fi

TAG="v@CEAMMC_DISTRIB_VERSION@"
NAME=$(basename $1)
USER="uliss"
REPO="pure-data"

echo "Uploading $NAME to github release $TAG ..."

github-release upload -u $USER -r $REPO -t ${TAG} --name $NAME -f $1 -R

