#!/bin/bash
#

PWD="@PROJECT_BINARY_DIR@"
VEROVIO_SRC_DIR="$PWD/_deps/verovio"
VEROVIO_BUILD_DIR="${VEROVIO_SRC_DIR}/build"
VEROVIO_INSTALL_DIR=~/.local/verovio
TAGS="version-5.2.0"
COPY_INCLUDES=0

case "$(sw_vers -productVersion | cut -d. -f1,2)" in
    "10.15")
        CLANG="-DCMAKE_C_COMPILER=clang-mp-11"
        CLANGXX="-DCMAKE_CXX_COMPILER=clang++-mp-11"
    ;;
    "10.14")
        CLANG="-DCMAKE_C_COMPILER=clang-mp-11"
        CLANGXX="-DCMAKE_CXX_COMPILER=clang++-mp-11"
        TAGS="version-4.1.0"
        COPY_INCLUDES=1
    ;;
    *)
        CLANG=""
        CLANGXX=""
    ;;
esac

mkdir -p "$PWD/_deps"
cd "$PWD/_deps"
echo "cloning verovio info $(pwd)"

if [[ ! -d verovio ]]
then
    git clone https://github.com/rism-digital/verovio
    if [[ $? -ne 0 ]]
    then
        exit 1
    fi
fi

cd "${VEROVIO_SRC_DIR}"
git fetch --tags
git checkout tags/$TAGS
git submodule update --init --recursive

echo "building verovio ..."

#rm -rf build
mkdir -p build
cd "${VEROVIO_BUILD_DIR}"
rm -f CMakeCache.txt

cmake -GNinja \
    $CLANG $CLANGXX \
    -DCMAKE_INSTALL_PREFIX=~/.local/verovio \
    -DCMAKE_BUILD_TYPE=Release \
    -DNO_RUNTIME=OFF \
    -DBUILD_AS_LIBRARY=ON \
    ../cmake

cmake -L

rm -rf ${VEROVIO_INSTALL_DIR}
# ninja clean
ninja && ninja install

# fin missing install for version-4.1.0
if [[ "$COPY_INCLUDES" -ne 0 ]]
then
    echo "copy include files ..."
    rm -rf ${VEROVIO_INSTALL_DIR}/lib
    mv ${VEROVIO_INSTALL_DIR}/bin ${VEROVIO_INSTALL_DIR}/lib
    mkdir -p ${VEROVIO_INSTALL_DIR}/include/verovio
    cp -v ${VEROVIO_SRC_DIR}/include/vrv/*.h ${VEROVIO_INSTALL_DIR}/include/verovio
    cp -v ${VEROVIO_SRC_DIR}/libmei/addons/*.h ${VEROVIO_INSTALL_DIR}/include/verovio
    cp -v ${VEROVIO_SRC_DIR}/libmei/dist/*.h ${VEROVIO_INSTALL_DIR}/include/verovio
    cp -v ${VEROVIO_SRC_DIR}/tools/*.h ${VEROVIO_INSTALL_DIR}/include/verovio
fi

