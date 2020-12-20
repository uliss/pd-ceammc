#!/bin/bash

if [ $# -ne 4 ]
then
    echo "Usage: $0 SRCDIR BINDIR OUTDIR VERSION"
fi

if [ -f /etc/os-release ]
then
    source /etc/os-release
fi

PATCH_ELF=`which patchelf`

if [ -z $PATCH_ELF ];
then
    echo "patchelf is not found..."
    exit 1
fi

SYSVER="linux"

if [ ! -z $ID ] && [ ! -z $VERSION_ID ]
then
    SYSVER="${ID}${VERSION_ID}"
fi

SRCDIR="$1"
BINDIR="$2"
VERSION="$4"
OUTDIR="$3/ceammc"
OUTFILE="ceammc-${VERSION}-${SYSVER}-pd-@PD_TEXT_VERSION_SHORT@.tar.gz"
INSTALL_DIR="@CEAMMC_INSTALL_PREFIX@"

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
rm -f "${OUTDIR}/*"

echo "Copying libraries to ${OUTDIR} ..."
find "${BINDIR}" -name *.so -print0 | while read -r -d '' file
do
    cp "$file" "${OUTDIR}"
    # objdump -x "$file" | grep RPATH
    echo "+ Lib:  $(basename $file)"
done


echo "Copying extension files to ${OUTDIR} ..."
find "${BINDIR}" -name *.pd_linux -print0 | while read -r -d '' file
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
    # objdump -x "$file" | grep RPATH
done

echo "Copying [system.serial] extension files to ${OUTDIR} ..."
find "${BINDIR}/../extra/comport" -name *.pd_linux -print0 | while read -r -d '' file
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
done

ceammc_lib=$(find "${BINDIR}" -name ceammc\\.pd_linux)
cp $ceammc_lib "${OUTDIR}"

patchelf --set-rpath '$ORIGIN' "${OUTDIR}/ceammc.pd_linux"

echo "Copying help files to ${OUTDIR} ..."
find "${SRCDIR}/ext/doc" -name *-help\\.pd | while read file
do
    help=$(basename $file)
    cat "$file" |
        sed 's/ceammc\/ceammc-help\.pd/ceammc-help.pd/' |
        sed 's/\.\.\/index-help\.pd/index-help.pd/' > "${OUTDIR}/${help}"
    echo "+ Copy: '$help'"
done


echo "+ Copying misc files:"
echo "    stargazing.mod"
cp "${SRCDIR}/ext/doc/stargazing.mod" "${OUTDIR}"
echo "    prs.txt"
cp "${SRCDIR}/ext/doc/prs.txt" "${OUTDIR}"
echo "    sur_la_planche.glitch"
cp "${SRCDIR}/ext/doc/sur_la_planche.glitch" "${OUTDIR}"
echo "    soundtouch~.pd_linux"
cp "${BINDIR}/../extra/SoundTouch/pd/soundtouch~.pd_linux" "${OUTDIR}"
echo "    soundtouch~-help.pd"
cp "${BINDIR}/../extra/SoundTouch/pd/soundtouch~-help.pd" "${OUTDIR}"
echo "    soundtouch-help.pd"
cp "${BINDIR}/../extra/SoundTouch/pd/soundtouch-help.pd" "${OUTDIR}"

echo "+ Fix soundtouch link in index-help.pd..."
sed -i 's/ceammc\/soundtouch-help\.pd/soundtouch-help.pd/' "${OUTDIR}/index-help.pd"

cd "$3"
tar cfvz "${OUTFILE}" $(basename $OUTDIR)
mv "${OUTFILE}" ..


