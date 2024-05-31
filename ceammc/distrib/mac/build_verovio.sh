#!/bin/bash
#

PWD="@PROJECT_BINARY_DIR@"
CLANG=""
CLANGXX=""

# CLANG="-DCMAKE_C_COMPILER=clang-mp-11"
# CLANGXX="-DCMAKE_CXX_COMPILER=clang++-mp-11"

mkdir -p "$PWD/_deps"
cd "$PWD/_deps"
echo "cloning verovio info $(pwd)"

if [[ -d verovio ]]
then
    cd verovio
    git checkout
    git submodule update --init --recursive
else
    git clone https://github.com/rism-digital/verovio
    if [[ $? -ne 0 ]]
    then
        exit 1
    fi

    cd verovio
    git submodule update --init --recursive
fi

echo "building verovio ..."

#rm -rf build
mkdir -p build
cd build

cmake -GNinja \
    $CLANG $CLANGXX \
    -DCMAKE_INSTALL_PREFIX=~/.local/verovio \
    -DCMAKE_BUILD_TYPE=Release \
    -DNO_RUNTIME=OFF \
    -DBUILD_AS_LIBRARY=ON \
    ../cmake

cmake -L

rm -rf ~/.local/verovio
ninja clean
ninja && ninja install

