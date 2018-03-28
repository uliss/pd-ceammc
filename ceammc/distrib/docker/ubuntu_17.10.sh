#!/bin/bash

VER="@CEAMMC_LIB_VERSION@"
CVER="@CEAMMC_DISTRIB_VERSION@"
DVER="ubuntu17.10"
DISTRIB="artful"
IMAGE="altoviola/pd-${DVER}"
BUILD="/pure-data/build/release"
LIB="ceammc-${VER}-${DVER}-pd-@PD_TEXT_VERSION_SHORT@.tar.gz"
PD="pd-ceammc-${CVER}-@PD_TEXT_VERSION_SHORT@_${DISTRIB}_x86_64.deb"

docker run $IMAGE /bin/bash -c "
cd pure-data
git checkout ceammc
git pull
git submodule init
git submodule update
cd build/release
../config_release.sh
make -j2
make test
make package
make ceammc_lib
ls
"

ID=$(docker ps -ql)

docker cp ${ID}:${BUILD}/${LIB} .
docker cp ${ID}:${BUILD}/${PD} .

docker stop $ID
docker rm $ID
