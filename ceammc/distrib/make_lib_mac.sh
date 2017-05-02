#!/bin/sh

if [ $# -ne 4 ]
then
    echo "Usage: $0 SRCDIR BINDIR OUTDIR VERSION"
fi

SRCDIR="$1"
BINDIR="$2"
VERSION="$4"
OUTDIR="$3/ceammclib"
SYSVER=$(sw_vers | grep ProductVersion | cut -f2 | cut -f1,2 -d.)
OUTFILE="ceammclib-${VERSION}-macosx-${SYSVER}-vanilla-0.47.tar.gz"
DYLIBBUNDLER="@DYLIBBUNDLER@"


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

echo "Making CEAMMC library from build directory: $BINDIR"
mkdir -p "${OUTDIR}"
rm "${OUTDIR}/*"

echo "Copying libraries to ${OUTDIR} ..."
find "${BINDIR}" -name *.dylib -print0 | while read -r -d '' file
do
    cp "$file" "${OUTDIR}"
    echo "+ Lib:  $(basename $file)"
done


echo "Copying extension files to ${OUTDIR} ..."
find "${BINDIR}" -name *.d_fat -print0 | while read -r -d '' file
do
    ext_name=$(basename $file)
    skip_ext $file
    if [ $? -eq 1 ]
    then
        echo "- Skip: '$ext_name'"
        continue
    fi

    cp "$file" "${OUTDIR}/${ext_name}"
    echo "+ Copy: '$ext_name'"
    ${DYLIBBUNDLER} -x ${OUTDIR}/$ext_name -b -d ${OUTDIR} -p @loader_path/ -of
done

ceammc_lib=$(find "${BINDIR}" -name ceammc\\.d_fat)
cp $ceammc_lib "${OUTDIR}"
${DYLIBBUNDLER} -x ${OUTDIR}/ceammc.d_fat -b -d ${OUTDIR} -p @loader_path/ -of

echo "Copying help files to ${OUTDIR} ..."
find "${SRCDIR}/ext/doc" -name *-help\\.pd | while read file
do
    help=$(basename $file)
    cat "$file" |
        sed 's/ceammc\/ceammc-help\.pd/ceammc-help.pd/' |
        sed 's/\.\.\/index-help\.pd/index-help.pd/' > "${OUTDIR}/${help}"
    echo "+ Copy: '$help'"
done

cd "$3"
tar cfvz "${OUTFILE}" $(basename $OUTDIR)
mv "${OUTFILE}" ..



