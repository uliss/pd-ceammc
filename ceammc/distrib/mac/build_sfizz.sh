#!/bin/bash

PWD="@PROJECT_BINARY_DIR@"
SFIZZ_SRC_DIR="$PWD/_deps/sfizz"
SFIZZ_BUILD_DIR="${SFIZZ_SRC_DIR}/build"
SFIZZ_INSTALL_DIR=~/.local/sfizz

mkdir -p "$PWD/_deps"
cd "$PWD/_deps"
echo "cloning sfizz info $(pwd)"

if [[ ! -d sfizz ]]
then
    git clone https://github.com/sfztools/sfizz --depth 10
    if [[ $? -ne 0 ]]
    then
        exit 1
    fi
fi

cd "${SFIZZ_SRC_DIR}"
git checkout f5c6e29f23b8057867c08e88f5f6ac6738baa30b
git submodule update --init --recursive --depth 10

echo "building sfizz ..."

#rm -rf build
mkdir -p build
cd "${SFIZZ_BUILD_DIR}"
rm -f CMakeCache.txt

cmake -GNinja \
    -DCMAKE_BUILD_TYPE=Release \
    -DSFIZZ_GIT_SUBMODULE_CHECK=OFF \
    -DCMAKE_INSTALL_PREFIX=${SFIZZ_INSTALL_DIR}\
    -DSFIZZ_SHARED=ON \
    -DSFIZZ_RENDER=OFF \
    ..

cmake -L

rm -rf ${SFIZZ_INSTALL_DIR}
# ninja clean
ninja && ninja install

