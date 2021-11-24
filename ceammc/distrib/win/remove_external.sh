#!/bin/sh

ARCH="@CEAMMC_DISTRIB_ARCH@"
DEST="${USERPROFILE}/Documents/Pd/externals/${ARCH}"

cd "${DEST}"
rm -rf "ceammc"
echo "external removed: ${DEST}/ceammc"
