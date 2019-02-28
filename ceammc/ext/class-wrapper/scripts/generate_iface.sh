#!/bin/sh

echo "[generator script]"

if [ "$#" -ne 1 ]
then
    echo "Usage: $(basename $0) MODULE_DIR"
    exit 1
fi

BUILD_DIR="$1"
MOD_NAME="$(basename $1)"
CFG_NAME="$(basename $1).ini"

echo "Generating wrapper for module: ${MOD_NAME} ..."
env python3 generate_wrapper.py -m ${MOD_NAME} -d ${BUILD_DIR}
