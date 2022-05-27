#!/bin/sh

build_target() {
    echo "build target: '$1'"
    @CMAKE_COMMAND@ --build "@PROJECT_BINARY_DIR@" --target $1
}

rm -rf "@CMAKE_INSTALL_PREFIX@"

build_target dev_touch_version
build_target install
build_target dev_fixdll
build_target dev_fixdoc
build_target ceammc_lib
build_target inno
