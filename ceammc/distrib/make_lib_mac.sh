#!/bin/sh

if [ $# -ne 3 ]
then
    echo "Usage: $0 INDIR OUTDIR VERSION"
fi

DIR="$1"
VERSION="$3"
OUTDIR="$2/pd_ceammclib-$VERSION"


function skip_ext {
    #skip experimental extensions
    exp=$(echo $1 | grep -v '/exp' | grep -v '/tl' | grep -v '/test')
    if [ -z $exp ]
    then
        return 1
    else
        return 0
    fi
}

echo "Making CEAMMC library from build directory: $DIR"
mkdir -p "${OUTDIR}"
rm "${OUTDIR}/*"

echo "Copying libraries to ${OUTDIR} ..."
find "${DIR}" -name *.dylib -print0 | while read -r -d '' file
do
    cp "$file" "${OUTDIR}"
    echo "+ Lib:  $(basename $file)"
done


echo "Copying extension files to ${OUTDIR} ..."
find "${DIR}" -name *.d_fat -print0 | while read -r -d '' file
do
    ext_name=$(basename $file)
    cp_ext_name="${ext_name%.d_fat}.pd_darwin"
    skip_ext $file
    if [ $? -eq 1 ]
    then
        echo "- Skip: '$ext_name'"
        continue
    fi

    cp "$file" "${OUTDIR}/${ext_name%.d_fat}.pd_darwin"
    echo "+ Copy: '$ext_name' as '$cp_ext_name'"
done

echo "Copying help files to ${OUTDIR} ..."
find "${DIR}/ext/doc" -name *-help\\.pd | while read file
do
    help=$(basename $file)
    cp "$file" "${OUTDIR}"
    echo "+ Copy: '$help'"
done

