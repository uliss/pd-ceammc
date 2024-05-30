#!/bin/bash

##############
# VARS
##############

# from CMake
SRC_DIR="@PROJECT_SOURCE_DIR@"
BUILD_DIR="@PROJECT_BINARY_DIR@"
DIST_DIR="@PROJECT_BINARY_DIR@/dist"
DYLIBBUNDLER="@DYLIBBUNDLER@"
DYLIBFIX="@DYLIBFIX@"
BUNDLE="@BUNDLE@"
WISH_APP="@WISH_APP@"
TK_VERSION="@TK_VERSION@"
LIB_LEAPMOTION="@LEAPMOTION_LIBRARY@"
CEAMMC_LIB_VERSION="@CEAMMC_LIB_VERSION@"
INSTALL_DIR="@PROJECT_BINARY_DIR@/dist/pd_ceammc"
CMAKE="@CMAKE_COMMAND@"

# relative dir names
PD_APP="$(basename $BUNDLE)"
PD_CONTENT=$PD_APP/Contents
PD_RESOURCES=$PD_CONTENT/Resources
PD_FRAMEWORKS=$PD_CONTENT/Frameworks
PD_EXTRA=$PD_RESOURCES/extra
PD_INCLUDE=$PD_RESOURCES/include
PD_BIN=$PD_RESOURCES/bin
PD_TCL=$PD_RESOURCES/tcl
PD_PO=$PD_RESOURCES/po
PD_CEAMMC=$PD_EXTRA/ceammc

PD_FONTS=$PD_CEAMMC/fonts
PD_IMG=$PD_CEAMMC/img
PD_IR=$PD_CEAMMC/ir
PD_LUA=$PD_CEAMMC/lua
PD_MIDI=$PD_CEAMMC/midi
PD_MUSIC=$PD_CEAMMC/music
PD_SAMPLES=$PD_CEAMMC/sound
PD_SF2=$PD_CEAMMC/sf2
PD_SFZ=$PD_CEAMMC/sfz

# absolute dir names
BUNDLE_APP="${DIST_DIR}/${PD_APP}"
BUNDLE_CONTENT="${DIST_DIR}/${PD_CONTENT}"
BUNDLE_INFO_PLIST="${BUNDLE_CONTENT}/Info.plist"
BUNDLE_RESOURCES="${DIST_DIR}/${PD_RESOURCES}"
BUNDLE_FRAMEWORKS="${DIST_DIR}/${PD_FRAMEWORKS}"
BUNDLE_PO="${DIST_DIR}/${PD_PO}"
BUNDLE_EXTRA="${DIST_DIR}/${PD_EXTRA}"
BUNDLE_BIN="${DIST_DIR}/${PD_BIN}"
BUNDLE_TCL="${DIST_DIR}/${PD_TCL}"
BUNDLE_CEAMMC="${DIST_DIR}/${PD_CEAMMC}"
BUNDLE_SF2="${DIST_DIR}/${PD_SF2}"
BUNDLE_SFZ="${DIST_DIR}/${PD_SFZ}"
BUNDLE_MIDI="${DIST_DIR}/${PD_MIDI}"
BUNDLE_LUA="${DIST_DIR}/${PD_LUA}"
BUNDLE_SAMPLES="${DIST_DIR}/${PD_SAMPLES}"
BUNDLE_INCLUDE="${DIST_DIR}/${PD_INCLUDE}"
BUNDLE_COMPLETIONS="${BUNDLE_TCL}/ceammc/custom_completions"
BUNDLE_FONTS="${DIST_DIR}/${PD_FONTS}"
BUNDLE_IMAGES="${DIST_DIR}/${PD_IMG}"
BUNDLE_MUSIC="${DIST_DIR}/${PD_MUSIC}"
BUNDLE_IR="${DIST_DIR}/${PD_IR}"

# resources paths
PD_INFO_PLIST="${BUILD_DIR}/dist/Info.plist"
PD_ICON="${SRC_DIR}/ceammc/gui/icons/pd_ceammc.icns"
PD_ICON_FILE="${SRC_DIR}/ceammc/gui/icons/pd_doc.icns"

# git vars
CURRENT_DATE=$(LANG=C date -u '+%d %h %Y %Z %H:%M:%S')
GIT_BRANCH=$(git --git-dir $SRC_DIR/.git symbolic-ref --short HEAD)
GIT_COMMIT=$(git --git-dir $SRC_DIR/.git describe --tags)

# source paths
SRC_CEAMMC="${SRC_DIR}/ceammc"
SRC_TCL="${SRC_DIR}/tcl"
CEAMMC_EXT_SRC_DIR="${SRC_DIR}/ceammc/ext/src"
CEAMMC_EXT_BIN_DIR="${BUILD_DIR}/ceammc/ext/src"

#############
# FUNCTIONS
#############

# find all Pd externals in specified directory
# usage: external_files DIR
function external_files() {
    find "$1" -type f 2>/dev/null | grep -e '\.d_fat' -e '\.d_amd64' -e '\.d_i386' -e '\.pd_darwin'
}

function dylib_external_fix() {
    exec=$1
    dir=$2

    echo "\t- fixing @loader_path for $(basename $exec) ..."
    $DYLIBBUNDLER \
        --fix-file "${exec}" \
        --bundle-deps \
        --create-dir \
        --dest-dir "${dir}" \
        --install-path @loader_path/ \
        --overwrite-files &>"${BUILD_DIR}/bundle.log"
}

function copy()
{
    file=$1
    dest=$2
    rel_dest=${dest#$BUNDLE_APP}
    rel_dest=${rel_dest:1}

    cp -f "${file}" "${dest}"
    if [ $? -eq 0 ];
    then
        echo "\t- copying $(basename $1) to \"${rel_dest}\""
    fi
}

function copy_and_fix_exe()
{
    file=$1
    dir=$2

    copy "${file}" "${dir}"
    dylib_external_fix "${dir}/$(basename $file)"  "${dir}"
}

function section() {
    echo
    tput setaf 2
    echo $1 "..."
    tput sgr0
}

function rsync_output_decorate() {
    tail -n +2 | sed '/\/$/d' | sed -E "s/^(.)/\t- copy \1/" | ghead -n -2
}

function rsync_copy() {
    src=$1
    dest=$2
    if [[ -n $3 ]]
    then
        exclude="--exclude='$3'"
    else
        exclude=''
    fi

    rsync --archive \
        --recursive \
        --whole-file \
        --prune-empty-dirs \
        $include \
        --exclude='CMake*' \
        --exclude='Make*' \
        --exclude='*.pddoc' \
        --exclude='*.sh' \
        --exclude='.*' \
        --exclude='*.cmake' \
        --exclude='*.yml' \
        $exclude \
        "${src}/" "$dest" \
        --no-motd \
        --verbose | rsync_output_decorate
}

##############
# MAIN
##############

# set globbing
shopt -s extglob
mkdir -p "${DIST_DIR}"
cd "${DIST_DIR}"

##############
# TCL/TK/WISH
##############

section "installing"
rm -rf "${INSTALL_DIR}"
$CMAKE --install ${BUILD_DIR} --prefix ${INSTALL_DIR}

section "Copy Wish Shell to ${PD_APP}"
cp -R "${WISH_APP}" "${BUNDLE_APP}"

# tcllib: base64
rsync -a -m "${SRC_DIR}/ceammc/extra/tcltk/base64" ${BUNDLE_FRAMEWORKS}
# tklib: tooltip
rsync -a -m "${SRC_DIR}/ceammc/extra/tcltk/tooltip" ${BUNDLE_FRAMEWORKS}
# tklib: ctext
rsync -a -m "${SRC_DIR}/ceammc/extra/tcltk/ctext" ${BUNDLE_FRAMEWORKS}
# tklib: tablelist
rsync -a -m "${SRC_DIR}/ceammc/extra/tcltk/tablelist" ${BUNDLE_FRAMEWORKS}

#
# Info.plist
#
copy "${PD_INFO_PLIST}" "${BUNDLE_CONTENT}"
# add git hash to version
BUNDLE_VERSION=`/usr/libexec/PlistBuddy -c "Print :CFBundleDisplayName" "${BUNDLE_INFO_PLIST}"`
BUNDLE_VERSION="${BUNDLE_VERSION} [tcl:${TK_VERSION} git:${GIT_COMMIT}]"
/usr/libexec/PlistBuddy -c "Set :CFBundleDisplayName '${BUNDLE_VERSION}'" "${BUNDLE_INFO_PLIST}"

# format plist XML
tidy -errors -quiet -xml -m -i -wrap 128 "${BUNDLE_INFO_PLIST}"

section "Update Wish icon"
cd "${BUNDLE_RESOURCES}"
rm -f Wish.icns
cp -p "${PD_ICON}" "${PD_ICON_FILE}" .
mkdir -p "${BUNDLE_BIN}"

############
# Pd files
############

section "Copying vanilla binaries"
copy_and_fix_exe "$BUILD_DIR/src/pd" "${BUNDLE_BIN}"
copy_and_fix_exe "$BUILD_DIR/src/pdsend" "${BUNDLE_BIN}"
copy_and_fix_exe "$BUILD_DIR/src/pdreceive" "${BUNDLE_BIN}"

section "Copying vanilla tcl files"
mkdir -p "${BUNDLE_TCL}/ceammc"
cd "${BUNDLE_TCL}/.."
rsync -a --exclude="CMake*" --exclude="Make*" -m "${SRC_TCL}" .
# link
ln -s tcl Scripts

section "Copying vanilla docs"
cd "${BUNDLE_RESOURCES}"
rsync -a \
    --recursive \
    --exclude=.DS_Store \
    --exclude=CMake* \
    --verbose --no-motd \
     --prune-empty-dirs \
    "${SRC_DIR}/doc" . | rsync_output_decorate

section "Copying compiled PO translations"
mkdir -p "${BUNDLE_PO}"
cd "${BUNDLE_PO}"
for msg in ${BUILD_DIR}/po/*.msg
do
    copy "${msg}" "${BUNDLE_PO}"
done

section "Copying source PO translations"
cd "${BUNDLE_PO}"
for po in ${SRC_DIR}/po/*.po
do
    copy "${po}" "${BUNDLE_PO}"
done

section "Copying vanilla abstractions and help"
mkdir -p "${BUNDLE_EXTRA}"
for pd in ${SRC_DIR}/extra/*.pd
do
    copy "${pd}" "${BUNDLE_EXTRA}"
done

section "Copying vanilla externals"
find $BUILD_DIR/extra -maxdepth 1 -type d | sed 1d | grep -v -i cmake | while read ext_dir
do
    EXT_NAME=$(basename $ext_dir)
    mkdir -p "${BUNDLE_EXTRA}/${EXT_NAME}"
    find "${ext_dir}" -type f | grep -v -i cmake | grep -v Makefile | while read target
    do
        copy "${target}" "${BUNDLE_EXTRA}/${EXT_NAME}"
    done
done

section "Copying vanilla headers"
mkdir -p "${BUNDLE_INCLUDE}"
copy "${SRC_DIR}/src/m_pd.h" "${BUNDLE_INCLUDE}"

############
# CEAMMC
############

section "Copying CEAMMC tcl plugins"
mkdir -p "${BUNDLE_COMPLETIONS}"
for tcl in ${SRC_DIR}/ceammc/gui/plugins/*.tcl
do
    copy ${tcl} "${BUNDLE_TCL}/ceammc"
done

section "Copying Tk Completion plugin"
mkdir -p "${BUNDLE_TCL}/completion-plugin"
for f in ${SRC_DIR}/ceammc/gui/plugins/completion-plugin/*.@(tcl|cfg|pd)
do
    copy ${f} "${BUNDLE_TCL}/ceammc"
done

section "Copying Tk Drag-and-Drop plugin"
mkdir -p "${BUNDLE_TCL}/tkdnd"
for tcl in ${SRC_DIR}/ceammc/distrib/tcl/tkdnd/library/*.tcl
do
    copy ${tcl} "${BUNDLE_TCL}/tkdnd"
done
copy ${BUILD_DIR}/ceammc/distrib/tcl/tkdnd/*.dylib "${BUNDLE_TCL}/tkdnd"
copy ${BUILD_DIR}/ceammc/distrib/tcl/tkdnd/library/*.tcl "${BUNDLE_TCL}/tkdnd"

section "Copying CEAMMC tcl completion"
copy $SRC_DIR/ceammc/ext/ceammc_objects.txt "${BUNDLE_COMPLETIONS}"
copy $SRC_DIR/ceammc/ext/extra_objects.txt "${BUNDLE_COMPLETIONS}"

section "Copying CEAMMC"
mkdir -p "${BUNDLE_CEAMMC}"
rsync_copy "${BUILD_DIR}/dist/pd_ceammc/lib/pd_ceammc/extra/ceammc/"  "${BUNDLE_CEAMMC}"
rsync_copy "${BUILD_DIR}/dist/pd_ceammc/lib/pd_ceammc/extra/numeric/" "${BUNDLE_CEAMMC}"
rsync_copy "${BUILD_DIR}/dist/pd_ceammc/lib/pd_ceammc/extra/matrix/"  "${BUNDLE_CEAMMC}"
if [[ -d "${BUILD_DIR}/dist/pd_ceammc/share/verovio" ]]
then
    rsync_copy "${BUILD_DIR}/dist/pd_ceammc/share/verovio/"           "${BUNDLE_CEAMMC}/music/verovio"
fi

section "Copying CEAMMC dll and externals"
$DYLIBFIX --dir "${BUNDLE_CEAMMC}" \
    --rpaths=${BUILD_DIR}/ceammc/ext/src/lib \
    --files $(find ${BUILD_DIR}/ceammc -name '*.dylib' -o -name '*.d_fat' -o -name '*.d_amd64' -o -name '*.d_i386' -o -name '*.pd_darwin' | grep -v tcl | tr '\n' ' ')

if [[ $? -ne 0 ]]
then
    exit 1
fi

section "Copying CEAMMC cmake files"
mkdir -p "${BUNDLE_INCLUDE}"
copy "${SRC_DIR}/cmake/PdExternal.cmake" "${BUNDLE_INCLUDE}"

section "Copying CEAMMC class wrappers completions"
mkdir -p "${BUNDLE_COMPLETIONS}"
find ${SRC_CEAMMC}/ext/class-wrapper/modules -name 'completion_*.txt' | while read txt
do
    copy $txt "${BUNDLE_COMPLETIONS}"
done

section "Change Pd help file"
rm -f "${BUNDLE_CEAMMC}/index-help.pd"
rm -f "${BUNDLE_EXTRA}/index-help.pd"
copy $SRC_DIR/ceammc/ext/doc/index-help.pd "${BUNDLE_EXTRA}"
chmod 0444 "${BUNDLE_EXTRA}/index-help.pd"
for r in  $SRC_DIR/ceammc/distrib/release_*.@(pd|lua)
do
    copy $r "${BUNDLE_CEAMMC}"
done

section "Copying CEAMMC about file"
cat $BUILD_DIR/ceammc/ext/doc/about.pd | sed "s/%GIT_BRANCH%/$GIT_BRANCH/g" | \
   sed "s/%GIT_COMMIT%/$GIT_COMMIT/g" | \
   sed "s/%BUILD_DATE%/$CURRENT_DATE/g" > about.pd
copy about.pd ${BUNDLE_CEAMMC}
chmod 0444 "${BUNDLE_CEAMMC}/about.pd"
rm about.pd

section "Copying license"
copy $SRC_DIR/LICENSE.txt "${BUNDLE_RESOURCES}/Scripts"

##############
# 3RD PARTY
##############

# args
# $1: name - relative project path to the external
# $2: output_dir - relative output directory name
copy_external() {
    name=$(basename $1)
    src_dir="${SRC_DIR}/$1"
    bin_dir="${BUILD_DIR}/$1"
    out_dir="${BUNDLE_EXTRA}/$2"

    FILES=$(external_files $bin_dir)
    if [[ -n $FILES ]]
    then
        section "Copying $name to $2"
        mkdir -p "$out_dir"
        for ext in ${FILES}
        do
            copy $ext "$out_dir"
        done

        for pdhelp in $(find ${src_dir} -name '*\.pd')
        do
            copy ${pdhelp} "$out_dir"
        done

        for txt in $(find ${src_dir} -name '*\.txt' | grep -v CMake)
        do
            copy ${txt} "$out_dir"
        done
    fi
}

copy_external ceammc/extra/SoundTouch ceammc
$DYLIBFIX --dir "${BUNDLE_EXTRA}/ceammc" --files $(external_files ${BUILD_DIR}/ceammc/extra/SoundTouch)

copy_external ceammc/extra/autotune autotune~
copy_external ceammc/extra/import import
copy_external ceammc/extra/libdir libdir
copy_external ceammc/extra/flext/disis_munger disis_munger~
copy_external ceammc/extra/flext/leapmotion leapmotion
copy_external ceammc/extra/flext/vasp vasp
copy_external ceammc/extra/flext/zconf zconf

XSAMPLE_FILES=$(external_files $BUILD_DIR/ceammc/extra/flext/xsample)
if [[ -n $XSAMPLE_FILES ]]
then
    section "Copying xsample"
    mkdir -p "${BUNDLE_EXTRA}/xsample"
    copy $BUILD_DIR/ceammc/extra/flext/xsample/xsample.@(d_fat|d_amd64|d_i386|pd_darwin) "${BUNDLE_EXTRA}/xsample"
    cp $SRC_DIR/ceammc/extra/flext/xsample/xsample/pd/* "${BUNDLE_EXTRA}/xsample"
    cp $SRC_DIR/ceammc/extra/flext/xsample/xsample/pd-ex/* "${BUNDLE_EXTRA}/xsample"
    copy $SRC_DIR/ceammc/extra/flext/xsample/xsample/gpl.txt "${BUNDLE_EXTRA}/xsample"
    copy $SRC_DIR/ceammc/extra/flext/xsample/xsample/license.txt "${BUNDLE_EXTRA}/xsample"
fi

section "Copying rust apps"
mkdir -p "${BUNDLE_CEAMMC}"
copy $BUILD_DIR/ceammc/extra/rust/rhvoice_download "${BUNDLE_CEAMMC}"

# "code signing" which also sets entitlements
# note: "-" identity results in "ad-hoc signing" aka no signing is performed
# for one, this allows loading un-validated external libraries on macOS 10.15+:
# https://cutecoder.org/programming/shared-framework-hardened-runtime
ENTITLEMENTS="${SRC_DIR}/mac/stuff/pd.entitlements"
codesign -f --deep -s "-" --entitlements ${ENTITLEMENTS} \
    "${BUNDLE_FRAMEWORKS}/Tcl.framework/Versions/Current"
codesign -f --deep -s "-" --entitlements ${ENTITLEMENTS} \
    ${BUNDLE_FRAMEWORKS}/Tk.framework/Versions/Current
codesign --deep -s "-" --entitlements ${ENTITLEMENTS} ${BUNDLE_APP}

cd "${BUILD_DIR}"
