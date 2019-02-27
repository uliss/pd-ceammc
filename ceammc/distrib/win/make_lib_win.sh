#!/bin/bash

if [ $# -ne 4 ]
then
    echo "Usage: $0 SRCDIR BINDIR OUTDIR VERSION"
    exit 1
fi

SRCDIR="$1"
BINDIR="$2"
VERSION="$4"
OUTDIR="$3/ceammc"
OUTFILE="ceammc-${VERSION}-win-pd-0.47.zip"

echo "    - source dir:  ${SRCDIR}"
echo "    - binary dir:  ${BINDIR}"
echo "    - output dir:  ${OUTDIR}"
echo "    - version:     ${VERSION}"
echo "    - output file: ${OUTFILE}"

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

echo "Copying externals to ${OUTDIR} ..."
find "${BINDIR}/extra/ceammc" -name *.dll -print0 | while read -r -d '' file
do
    cp "$file" "${OUTDIR}"
    echo "+ Ext:  $(basename $file)"
done

echo "Copying Dll's to ${OUTDIR} ..."
find "${BINDIR}/bin" -name lib*.dll -print0 | while read -r -d '' file
do
    cp "$file" "${OUTDIR}"
    echo "+ Dll:  $(basename $file)"
done

echo "Copying TCL files to ${OUTDIR} ..."
find "${SRCDIR}/extra/hcs" -name *\\.tcl | while read file
do
    cp "$file" "${OUTDIR}"
    echo "+ Tcl:  $(basename $file)"
done

echo "Copying help files to ${OUTDIR} ..."
find "${SRCDIR}/ext/doc" -name *-help\\.pd | while read file
do
    help=$(basename $file)
    cat "$file" |
        sed 's/ceammc\/ceammc-help\.pd/ceammc-help.pd/' |
        sed 's/\.\.\/index-help\.pd/index-help.pd/' > "${OUTDIR}/${help}"
    echo "+ Copy: '$help'"
done

echo "Copying wav files to ${OUTDIR} ..."
find "${SRCDIR}/ext/doc" -name *\\.wav | while read file
do
    cp "$file" "${OUTDIR}"
    echo "+ WAV:  $(basename $file)"
done

echo "Copying STK raw files to ${OUTDIR}/stk ..."
mkdir -p "${OUTDIR}/stk"
find "${SRCDIR}/extra/stk/stk/rawwaves" -name *\\.raw | while read file
do
    cp "$file" "${OUTDIR}/stk/"
    echo "+ STK:  $(basename $file)"
done

echo "Copying SF2 files to ${OUTDIR}/sf2 ..."
mkdir -p "${OUTDIR}/sf2"
find "${SRCDIR}/extra/sf2" -name *\\.sf2 | while read file
do
    cp "$file" "${OUTDIR}/sf2"
    echo "+ SF2:  $(basename $file)"
done

echo "Copying UI abstractions files to ${OUTDIR} ..."
find "${SRCDIR}/ext/abstractions" -name *\\.pd | while read file
do
    cp "$file" "${OUTDIR}"
    echo "+ ABS:  $(basename $file)"
done

echo "Copying wrapper DLL files to ${OUTDIR} ..."
find ${SRCDIR}/ext/class-wrapper/modules/* -maxdepth 0 -type d | while read mod
do
    m=$(basename $mod)
    echo "    module: $m"
    find "${BINDIR}/extra/$m" -type f | while read f
    do
        echo "+ MOD: $(basename $f)"
        cp "$f" "${OUTDIR}"
    done
done

echo "Copying misc files to ${OUTDIR} ..."
echo "+ MISC: stargazing.mod"
cp "${SRCDIR}/ext/doc/stargazing.mod" "${OUTDIR}"
echo "+ MISC: prs.txt"
cp "${SRCDIR}/ext/doc/prs.txt" "${OUTDIR}"

echo "Copying Soundtouch files to ${OUTDIR} ..."
cp "${SRCDIR}/extra/SoundTouch/pd/soundtouch-help.pd" "${OUTDIR}"
cp "${SRCDIR}/extra/SoundTouch/pd/soundtouch~-help.pd" "${OUTDIR}"
cat "${OUTDIR}/index-help.pd" | sed 's/ceammc\/soundtouch-help\.pd/soundtouch-help.pd/' > tmp
mv tmp "${OUTDIR}/index-help.pd"

cd "$3"
7z a "${OUTFILE}" $(basename $OUTDIR)
mv "${OUTFILE}" ..
