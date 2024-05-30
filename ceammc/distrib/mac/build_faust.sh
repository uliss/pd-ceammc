#!/bin/bash
#


PWD="@PROJECT_BINARY_DIR@"

mkdir -p "$PWD/_deps"
cd "$PWD/_deps"
echo "cloning faust info $(pwd)"

if [[ -d faust ]]
then
    cd faust
    git checkout
    cd build
else
    git clone https://github.com/grame-cncm/faust.git
    if [[ $? -ne 0 ]]
    then
        exit 1
    fi

    cd faust/build
fi

pwd
ls

echo "building faust ..."

cmake -GNinja \
    -DCMAKE_INSTALL_PREFIX=~/.local/faust \
    -DCMAKE_BUILD_TYPE=Release \
    -DLLVM_BACKEND='STATIC DYNAMIC' \
    -DC_BACKEND='STATIC DYNAMIC COMPILER' \
    -DCPP_BACKEND='STATIC DYNAMIC COMPILER' \
    -DINCLUDE_EMCC=OFF \
    -DINCLUDE_OSC=OFF \
    -DINCLUDE_HTTP=OFF \
    -DINCLUDE_WASM_GLUE=OFF \
    -DINCLUDE_STATIC=OFF \
    -DINCLUDE_DYNAMIC=ON \
    -DINCLUDE_EXECUTABLE=ON \
    -DSELF_CONTAINED_LIBRARY=OFF \
    .

cmake -L

rm -rf ~/.local/faust
ninja clean
ninja && ninja install

