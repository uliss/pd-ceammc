#!/bin/bash
#

PWD="@PROJECT_BINARY_DIR@"
RHVOICE_SRC_DIR="$PWD/_deps/RHVoice"
RHVOICE_BUILD_DIR="${RHVOICE_SRC_DIR}/build"
RHVOICE_INSTALL_DIR=~/.local/rhvoice
TAGS="1.8.0"

mkdir -p "$PWD/_deps"
cd "$PWD/_deps"
echo "cloning rhvoice info $(pwd)"

if [[ ! -d RHVoice ]]
then
    git clone  --recurse-submodules --shallow-submodules --depth 1 https://github.com/RHVoice/RHVoice
    if [[ $? -ne 0 ]]
    then
        exit 1
    fi
fi

cd "${RHVOICE_SRC_DIR}"
git fetch --tags
git checkout tags/$TAGS
git submodule update --init --recursive --depth 1

echo "building RHVoice ..."

rm -rf ${RHVOICE_INSTALL_DIR}
cd RHVoice
scons install languages=belarusian,russian,english prefix=$RHVOICE_INSTALL_DIR
