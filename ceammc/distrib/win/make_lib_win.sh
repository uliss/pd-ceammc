#!/bin/bash

if [ $# -ne 1 ]
then
    echo "Usage: $0 OUTDIR"
    exit 1
fi

OUTDIR="$1/ceammc"
BASEOUTDIR="ceammc"
SRCDIR="@PROJECT_SOURCE_DIR@/ceammc"
BINDIR="@CMAKE_INSTALL_PREFIX@"
VERSION="@CEAMMC_LIB_VERSION@"
ARCH="@CEAMMC_DISTRIB_ARCH@"
PRECISION="f@PD_FLOAT_SIZE@"
PD_VERSION="@PD_TEXT_VERSION_FULL@"
OUTFILE="ceammc-${VERSION}-win-pd-${PD_VERSION}-${ARCH}-${PRECISION}.zip"
P7Z_EXE="@7Z_EXE@"

echo "    - source dir:  ${SRCDIR}"
echo "    - binary dir:  ${BINDIR}"
echo "    - output dir:  ${OUTDIR}"
echo "    - version:     ${VERSION}"
echo "    - output file: ${OUTFILE}"
echo "    - 7zip: ${P7Z_EXE}"


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

if [ ! -d "${BINDIR}" ]
then
    echo "ERROR: directory with installed binaries not exists: ${BINDIR} ..."
    exit 1
fi

echo "Making CEAMMC library from build directory: $BINDIR"
mkdir -p "${OUTDIR}"
rm -f "${OUTDIR}/*"

echo "Copying externals to ${OUTDIR} ..."
find "${BINDIR}/extra/ceammc" -name *.dll -print0 | while read -r -d '' file
do
    cp "$file" "${OUTDIR}"
    echo "+ Ext:  $(basename $file)"
done

echo "Copying 64-bit externals to ${OUTDIR} ..."
find "${BINDIR}/extra/ceammc" -name *.m_amd64 -print0 | while read -r -d '' file
do
    cp "$file" "${OUTDIR}"
    echo "+ Ext:  $(basename $file)"
done

echo "Copying 32-bit externals to ${OUTDIR} ..."
find "${BINDIR}/extra/ceammc" -name *.m_i386 -print0 | while read -r -d '' file
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

rm -f "${OUTDIR}/debug.gensym.dll"

echo "Copying help files to ${OUTDIR} ..."
find "${SRCDIR}/ext/doc" -name *\\.pd | while read file
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

echo "Copying HOA help files to ${OUTDIR} ..."
mkdir -p "${OUTDIR}/hoa"
find "@PROJECT_SOURCE_DIR@/ceammc/ext/doc/hoa" -type f | while read file
do
    help=$(basename $file)
    cp "$file" "${OUTDIR}/hoa"
    echo "+ HOA:  '$help'"
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
echo "+ MISC: sur_la_planche.glitch"
cp "${SRCDIR}/ext/doc/sur_la_planche.glitch" "${OUTDIR}"
echo "+ MISC: system.serial-help.pd"
cp "${SRCDIR}/extra/comport/system.serial-help.pd" "${OUTDIR}"

echo "Copying Soundtouch files to ${OUTDIR} ..."
cp "${SRCDIR}/extra/SoundTouch/pd/soundtouch-help.pd" "${OUTDIR}"
cp "${SRCDIR}/extra/SoundTouch/pd/soundtouch~-help.pd" "${OUTDIR}"
cat "${OUTDIR}/index-help.pd" | sed 's/ceammc\/soundtouch-help\.pd/soundtouch-help.pd/' > tmp
mv tmp "${OUTDIR}/index-help.pd"

if [ -x "${P7Z_EXE}" ]
then
    cd "${OUTDIR}/.."
    "${P7Z_EXE}" a "${OUTFILE}" "${BASEOUTDIR}"
    cp "${OUTFILE}" ..
else
    echo "7z is not found. Create zip archive manually..."
fi
