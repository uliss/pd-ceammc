#!/bin/bash

VER="@CEAMMC_LIB_VERSION@"
CVER="@CEAMMC_DISTRIB_VERSION@"
DVER="ubuntu18.10"
DISTRIB="cosmic"
IMAGE="altoviola/pd-${DVER}"
BUILD="/build/release"
LIB="ceammc-${VER}-${DVER}-pd-@PD_TEXT_VERSION_SHORT@.tar.gz"
PD="pd-ceammc-${CVER}-@PD_TEXT_VERSION_SHORT@_${DISTRIB}_x86_64.deb"

echo ""
echo "++++++++++++++++++++++++++++++++++++++++++++++"
echo "    DOCKER ${IMAGE}"
echo "++++++++++++++++++++++++++++++++++++++++++++++"
echo ""

docker run --mount type=bind,source=@CMAKE_SOURCE_DIR@,target=/pure-data $IMAGE /bin/bash -c "
mkdir -p ${BUILD}
cd ${BUILD}
cmake -DCMAKE_INSTALL_PREFIX=/usr \
    -DCMAKE_BUILD_TYPE=RELEASE \
    -DWITH_PORTAUDIO=OFF \
    -DWITH_ALSA=ON \
    -DWITH_PORTMIDI=ON \
    -DWITH_FLUIDSYNTH=ON \
    /pure-data
make -j2
make test
make package
ls
"

ID=$(docker ps -ql)

docker cp ${ID}:${BUILD}/${PD} .

docker stop $ID
docker rm $ID
