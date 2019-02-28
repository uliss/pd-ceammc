#!/bin/sh

echo "[generator script]"

if [ "$#" -ne 2 ]
then
    echo "Usage: $(basename $0) MODULE_DIR CMAKE_BINARY_DIR"
    exit 1
fi

BUILD_DIR="$1"
# needed for xlets d
CMAKE_BINARY_DIR="$2"
CEAMMC_XLET_DB="ceammc/ext/doc/ceammc.db"
MOD_NAME="$(basename $1)"

echo "Generating pddoc files for module: ${MOD_NAME} ..."
#rm -R "${BUILD_DIR}/doc"
#mkdir -p "${BUILD_DIR}/doc"
update_list=$(env python3 generate_pddoc.py -m ${MOD_NAME} -d ${BUILD_DIR})

VERSION=$(cat ${BUILD_DIR}/library.json | grep version | cut -d: -f2 | tr -d ' "')
echo "version: $VERSION"

cd "${BUILD_DIR}/help"

# check obsolete pd files
for file in *help.pd
do
    name=$(echo $file | sed 's/\-help.pd//')
    pddoc_name="${BUILD_DIR}/doc/${name}.pddoc"

    # skip library help file
    if [ "${name}" == "${MOD_NAME}" ]
    then
        continue
    fi

    if [ ! -f "$pddoc_name" ]
    then
        echo "removing obsolete file: $name ..."
        rm -f "$file"
    fi
done

echo "updates: $update_list"

for file in $update_list
do
    echo "    processing $(basename $file) ..."
    pd_doc2pd "$file" --force \
        --xlet-db "${BUILD_DIR}/doc/${MOD_NAME}.db" \
        --xlet-db "${CMAKE_BINARY_DIR}/${CEAMMC_XLET_DB}"
done

echo "Generating library help files ..."
cd "${BUILD_DIR}/doc"
pd_makelibrary --version "$VERSION" \
    --library ${MOD_NAME} \
    --version ${VERSION} \
    --output "lib_${MOD_NAME}.xml" \
    *.pddoc

pd_lib2pd "lib_${MOD_NAME}.xml"

echo "Generating library categories ..."
pd_cat2pd "lib_${MOD_NAME}.xml"
mv *-help.pd "${BUILD_DIR}/help"

## fix
#sed -i "" "2i\\
##X declare -lib $LIBRARY_NAME;\\
##X obj 300 15 declare -lib $LIBRARY_NAME;\\
#" $1/help/$LIBRARY_NAME-help.pd
