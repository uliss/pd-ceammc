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

echo "Generating pddoc files for module: ${MOD_NAME} ..."
#rm -R "${BUILD_DIR}/doc"
mkdir -p "${BUILD_DIR}/doc"
env python3 generate_pddoc.py -m ${MOD_NAME} -d ${BUILD_DIR}

VERSION=$(cat ${BUILD_DIR}/library.json | grep version | cut -d: -f2 | tr -d ' "')
echo "version: $VERSION"

#rm -R "${BUILD_DIR}/help"
mkdir -p "${BUILD_DIR}/help"

cd "${BUILD_DIR}/help"
for file in ${BUILD_DIR}/doc/*.pddoc
do
    echo "    processing $(basename $file) ..."
    pd_doc2pd "$file" --force --xlet-db "${BUILD_DIR}/doc/${MOD_NAME}.db"
done

echo "Generating library help files ..."
cd "${BUILD_DIR}/doc"
pd_makelibrary --version "$VERSION" \
    --library ${MOD_NAME} \
    *.pddoc > "lib_${MOD_NAME}.xml"

pd_lib2pd "lib_${MOD_NAME}.xml"

echo "Generating library categories ..."
pd_cat2pd "lib_${MOD_NAME}.xml"
mv *-help.pd "${BUILD_DIR}/help"

## fix
#sed -i "" "2i\\
##X declare -lib $LIBRARY_NAME;\\
##X obj 300 15 declare -lib $LIBRARY_NAME;\\
#" $1/help/$LIBRARY_NAME-help.pd
