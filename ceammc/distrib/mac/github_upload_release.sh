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

if [ ! -f "$DMG" ]
then
    tput setaf 9
    echo "ERROR: "
    tput setaf 15
    echo "  file is not found: $DMG. Exiting..."
    exit 1
fi

TAG="v$2"
USER=uliss
REPO="pure-data"
NAME=$(basename $DMG)

github-release upload --user $USER \
	--repo $REPO \
	--tag $TAG \
	--name $NAME \
	--file "$DMG" \
	--replace


