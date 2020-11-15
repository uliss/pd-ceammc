#!/bin/sh

if [ $# -ne 1 ]
then
    echo "Usage: $0 OUTDIR"
fi

SRCDIR="@PROJECT_SOURCE_DIR@/ceammc"
BINDIR="@PROJECT_BINARY_DIR@"
OUTDIR="$1/ceammc"
VERSION="@CEAMMC_LIB_VERSION@"
SYSVER=$(sw_vers | grep ProductVersion | cut -f2 | cut -f1,2 -d.)
OUTFILE="ceammc-${VERSION}-macosx-${SYSVER}-pd-@PD_TEXT_VERSION_SHORT@.tar.gz"
DYLIBBUNDLER="@DYLIBBUNDLER@"

CURRENT_DATE=$(LANG=C date -u '+%d %h %Y %Z %H:%M:%S')
GIT_BRANCH=$(git --git-dir '@PROJECT_SOURCE_DIR@/.git' symbolic-ref --short HEAD)
GIT_COMMIT=$(git --git-dir '@PROJECT_SOURCE_DIR@/.git' describe --tags)


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
find "${BINDIR}/ceammc/ext" -name *.dylib -print0 | while read -r -d '' file
do
    cp "$file" "${OUTDIR}"
    echo "+ Lib:  $(basename $file)"
done

find_ext() {
    find "$1" -name "$2\\.d_fat" \
        -o -name "$2\\.d_amd64" \
        -o -name "$2\\.pd_darwin" \
        -o -name "$2\\.d_i386"
}


echo "Copying extension files to ${OUTDIR} ..."
find_ext ${BINDIR}/ceammc/ext "*" | while read file
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

echo "Copying [system.serial] extension files to ${OUTDIR} ..."
find_ext "${BINDIR}/ceammc/extra/comport" "*" | while read file
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

ceammc_lib=$(find_ext "${BINDIR}/ceammc/ext" ceammc)
cp $ceammc_lib "${OUTDIR}"
${DYLIBBUNDLER} -x ${OUTDIR}/$(basename $ceammc_lib) -b -d ${OUTDIR} -p @loader_path/ -of

echo "Copying help files to ${OUTDIR} ..."
find "@PROJECT_SOURCE_DIR@/ceammc/ext/doc" -name *-help\\.pd -maxdepth 1 | while read file
do
    help=$(basename $file)
    cat "$file" | sed -e 's/ceammc\/ceammc-help\.pd/ceammc-help.pd/' \
        -e 's/\.\.\/index-help\.pd/index-help.pd/' > "${OUTDIR}/${help}"
    echo "+ Help: '$help'"
done

echo "Copying wrapper help files to ${OUTDIR} ..."
find "${SRCDIR}/ext/class-wrapper/modules" -name *-help\\.pd | while read file
do
    help=$(basename $file)
    cat "$file" | sed -e 's/ceammc\/ceammc-help\.pd/ceammc-help.pd/' \
        -e 's/\.\.\/index-help\.pd/index-help.pd/' > "${OUTDIR}/${help}"
    echo "+ Help: '$help'"
done

echo "Copying HOA help files to ${OUTDIR} ..."
mkdir -p "${OUTDIR}/hoa"
find "@PROJECT_SOURCE_DIR@/ceammc/ext/doc/hoa" -type f | while read file
do
    help=$(basename $file)
    cp "$file" "${OUTDIR}/hoa"
    echo "+ HOA:  '$help'"
done

echo "Copying about.pd to ${OUTDIR} ..."
cat "@PROJECT_BINARY_DIR@/ceammc/ext/doc/about.pd" | sed  -e "s/%GIT_BRANCH%/$GIT_BRANCH/g" \
    -e "s/%GIT_COMMIT%/$GIT_COMMIT/g" \
    -e "s/%BUILD_DATE%/$CURRENT_DATE/g" > ${OUTDIR}/about.pd
echo "+ Help: 'about.pd'"

echo "Copying STK rawwaves files to ${OUTDIR}/stk ..."
mkdir -p "${OUTDIR}/stk"
find "${SRCDIR}/extra/stk/stk/rawwaves" -name *\\.raw | while read file
do
    help=$(basename $file)
    cp "$file" "${OUTDIR}/stk"
    echo "+ RAW: '$help'"
done

echo "Copying CEAMMC wav examples to ${OUTDIR} ..."
find "${SRCDIR}/ext/doc" -name *\\.wav | while read file
do
    help=$(basename $file)
    cp "$file" ${OUTDIR}
    echo "+ WAV: '$help'"
done

echo "Copying sound samples to ${OUTDIR}/sound ..."
cp -R "${SRCDIR}/ext/doc/sound" ${OUTDIR}

echo "Copying SF2 fonts to ${OUTDIR}/sf2 ..."
cp -R "${SRCDIR}/extra/fluidsynth/fluidsynth/sf2" ${OUTDIR}

echo "+ Copying abstractions:"
for abs in ${SRCDIR}/ext/abstractions/*.pd
do
    echo "    $(basename $abs)"
    cp "${abs}" "${OUTDIR}"
done

echo "+ Copying misc files:"
echo "    stargazing.mod"
cp "${SRCDIR}/ext/doc/stargazing.mod" "${OUTDIR}"
echo "    prs.txt"
cp "${SRCDIR}/ext/doc/prs.txt" "${OUTDIR}"
echo "    soundtouch~"
soundtouch_ext=$(find_ext "${BINDIR}/ceammc/extra/SoundTouch/pd" "soundtouch~")
cp "$soundtouch_ext" "${OUTDIR}"
${DYLIBBUNDLER} -x ${OUTDIR}/$(basename $soundtouch_ext) -b -d ${OUTDIR} -p @loader_path/ -of
echo "    soundtouch~-help.pd"
cp "@PROJECT_SOURCE_DIR@/ceammc/extra/SoundTouch/pd/soundtouch~-help.pd" "${OUTDIR}"
echo "    soundtouch-help.pd"
cp "@PROJECT_SOURCE_DIR@/ceammc/extra/SoundTouch/pd/soundtouch-help.pd" "${OUTDIR}"

echo "+ Fix soundtouch link in index-help.pd..."
sed -i "" 's/ceammc\/soundtouch-help\.pd/soundtouch-help.pd/' "${OUTDIR}/index-help.pd"
sed -i -e 's|\.\./index-help\.pd|index-help.pd|' "${OUTDIR}/soundtouch-help.pd"
sed -i -e 's|\.\./index-help\.pd|index-help.pd|' "${OUTDIR}/soundtouch~-help.pd"

cd "$1"
tar cfvz "${OUTFILE}" $(basename $OUTDIR)
mv "${OUTFILE}" "@PROJECT_BINARY_DIR@"
