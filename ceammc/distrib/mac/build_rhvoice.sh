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
cd ${RHVOICE_SRC_DIR}
scons install languages=belarusian,russian,english prefix=$RHVOICE_INSTALL_DIR

RHVOICE_LIB=~/.local/rhvoice/lib/libRHVoice.5.1.0.dylib
RHVOICE_CORE_LIB=~/.local/rhvoice/lib/libRHVoice_core.7.0.0.dylib

install_name_tool -id @rpath/libRHVoice.5.1.0.dylib ${RHVOICE_LIB}
install_name_tool -change build/darwin/core/libRHVoice_core.7.0.0.dylib @rpath/libRHVoice_core.7.0.0.dylib ${RHVOICE_LIB}
install_name_tool -id @rpath/libRHVoice_core.7.0.0.dylib ${RHVOICE_CORE_LIB}
