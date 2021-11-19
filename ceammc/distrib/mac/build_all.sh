#!/bin/sh

build_target() {
    tput setaf 2
    echo "build target: '$1'"
    tput sgr0
    @CMAKE_COMMAND@ --build "@PROJECT_BINARY_DIR@" --target $1
}

rm -f "@PROJECT_BINARY_DIR@/dist/"*.app
rm -f "@PROJECT_BINARY_DIR@/dist/"*.zip
rm -f "@PROJECT_BINARY_DIR@/dist/"*.tar.gz
rm -f "@PROJECT_BINARY_DIR@/dist/"*.dmg

build_target dev_touch_version
build_target all
build_target dmg
build_target app-zip
build_target ceammc_lib
