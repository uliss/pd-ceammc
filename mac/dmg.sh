#!/bin/bash

VERSION_NUMBER="1.0 beta"

VERSION="$0 version ${VERSION_NUMBER}
Original name: make_dmg.sh
Author: Valentine Silvansky <v.silvansky@gmail.com>"

LICENSE="This software is distributed under GNU GPLv3 license.
Visit http://www.gnu.org/licenses/gpl-3.0.txt for more information."

HELP="Usage:
  $0 [options...] bundle_path"

GIT="/usr/local/bin/git"

ARG_DIR=`pwd`
ARG_ICON=
ARG_BACKGROUND=
ARG_COORDS=
ARG_SIZE="640:480"
ARG_VOL_NAME="PureData_$2"
ARG_TMP_DIR="./tmp"
ARG_ADD_VERSION=
ARG_CODESIGN_ID=
ARG_ICON_SIZE=72

APP_BUNDLE_PATH=$1
APP_BUNDLE_NAME=$(basename ${APP_BUNDLE_PATH})
ARG_DMG_NAME="Pd_$2"
SRC_PATH="$3"

if [ $# -eq 3 ]; then
        echo "Bundle path set to ${APP_BUNDLE_PATH}";
        echo "DMG name set to ${ARG_DMG_NAME}";
        echo "Source path set to ${SRC_PATH}";
else
        echo "Error! Bundle path is not specified."
        exit 1;
fi

if [ ! -e "${APP_BUNDLE_PATH}" ]; then
        echo "Error! Bundle \"${APP_BUNDLE_PATH}\" does not exist!"
        exit 1
fi

TARGET_DIR=${ARG_DIR}

if [ ! "${TARGET_DIR}" ]; then
        TARGET_DIR=`pwd`
fi

if [ ! -e "${TARGET_DIR}" ]; then
        echo "Error! Directory \"${TARGET_DIR}\" does not exist."
        exit 1
fi

cd "${TARGET_DIR}"

TMP_DIR=${ARG_TMP_DIR}

VOL_NAME=${ARG_VOL_NAME}

if [ ! "${VOL_NAME}" ]; then
        VOL_NAME=${APP_BUNDLE_NAME%.*}
        echo "Defaulting dmg volume name to ${VOL_NAME}"
fi


BG_IMG_PATH=${ARG_BACKGROUND}
BG_IMG_NAME=
if [ ! -n $BG_IMG_PATH ]; then
    BG_IMG_NAME=$(basename ${BG_IMG_PATH})
fi

VOL_ICON_NAME=${ARG_ICON}

INFO_PLIST="${APP_BUNDLE_PATH}/Contents/Info.plist"
if [ "${ARG_ADD_VERSION}" ]; then
        APP_VERSION=`/usr/libexec/PlistBuddy -c "Print :CFBundleShortVersionString" "${INFO_PLIST}"`
        APP_BUILD_VERSION=`/usr/libexec/PlistBuddy -c "Print :CFBundleVersion" "${INFO_PLIST}t"`
        DMG_NAME_SUFFIX=" ${APP_VERSION}.${APP_BUILD_VERSION}"
else
        APP_VERSION=
        APP_BUILD_VERSION=
        DMG_NAME_SUFFIX=
fi

DMG_NAME_TMP="${APP_BUNDLE_NAME%.*}_tmp.dmg"

if [ "${ARG_DMG_NAME}" ]; then
        DMG_NAME_BASE=${ARG_DMG_NAME}
else
        DMG_NAME_BASE=${APP_BUNDLE_NAME%.*}
fi

DMG_NAME="${DMG_NAME_BASE}${DMG_NAME_SUFFIX}.dmg"

CODESIGN_IDENTITY=${ARG_CODESIGN_ID}

if [ "${CODESIGN_IDENTITY}" ]; then
        echo "*** Signing ${APP_BUNDLE_NAME} with identity '${CODESIGN_IDENTITY}'... "
        codesign -fs "${CODESIGN_IDENTITY}" "${APP_BUNDLE_PATH}"
        echo "done!"
fi

echo -n "*** Copying ${APP_BUNDLE_PATH} to the temporary dir... "
mkdir "$TMP_DIR"
cp -R "${APP_BUNDLE_PATH}" ${TMP_DIR}/
echo "done!"

TMP_INFO_PLIST="${TMP_DIR}/${APP_BUNDLE_NAME}/Contents/Info.plist"
BUNDLE_VERSION=`/usr/libexec/PlistBuddy -c "Print :CFBundleDisplayName" "${TMP_INFO_PLIST}"`

echo -n "*** Creating temporary dmg disk image..."
rm -f "${DMG_NAME_TMP}"
hdiutil create -ov -srcfolder $TMP_DIR -format UDRW -volname "${VOL_NAME}" "${DMG_NAME_TMP}"

echo -n "*** Mounting temporary image... "
device=$(hdiutil attach -readwrite -noverify -noautoopen "${DMG_NAME_TMP}" | egrep '^/dev/' | sed 1q | awk '{print $1}')
echo "done! (device ${device})"

echo -n "*** Sleeping for 5 seconds..."
sleep 5
echo " done!"

echo "*** Setting style for temporary dmg image..."


if [ "${ARG_BACKGROUND}" ]; then
        echo -n "    * Copying background image... "

        BG_FOLDER="/Volumes/${VOL_NAME}/.background"
        mkdir "${BG_FOLDER}"
        cp "${BG_IMG_PATH}" "${BG_FOLDER}/"

        echo "done!"
        NO_BG=
else
        NO_BG="-- "
fi

if [ "${ARG_ICON}" ]; then
        echo -n "    * Copying volume icon... "

        ICON_FOLDER="/Volumes/${VOL_NAME}"
        cp "${VOL_ICON_NAME}" "${ICON_FOLDER}/.VolumeIcon.icns"

        echo "done!"

        echo -n "    * Setting volume icon... "

        SetFile -c icnC "${ICON_FOLDER}/.VolumeIcon.icns"
        SetFile -a C "${ICON_FOLDER}"

        echo "done!"
fi

if [ "${ARG_COORDS}" ]; then
        ALL_COORDS=(`echo "${ARG_COORDS}" | tr ":" "\n"`)
        APPS_X=${ALL_COORDS[0]}
        APPS_Y=${ALL_COORDS[1]}
        BUNDLE_X=${ALL_COORDS[2]}
        BUNDLE_Y=${ALL_COORDS[3]}
        NO_COORDS=""
else
        NO_COORDS="-- "
fi

if [ "${ARG_SIZE}" ]; then
        WINDOW_SIZE=(`echo "${ARG_SIZE}" | tr ":" "\n"`)
        WINDOW_WIDTH=${WINDOW_SIZE[0]}
        WINDOW_HEIGHT=${WINDOW_SIZE[1]}
else
        WINDOW_WIDTH=640
        WINDOW_HEIGHT=480
fi

WINDOW_LEFT=400
WINDOW_TOP=100

WINDOW_RIGHT=$((${WINDOW_LEFT} + ${WINDOW_WIDTH}))
WINDOW_BOTTOM=$((${WINDOW_TOP} + ${WINDOW_HEIGHT}))

echo -n "    * Executing applescript for further customization... "
APPLESCRIPT="
tell application \"Finder\"
        tell disk \"${VOL_NAME}\"
                open
                -- Setting view options
                set current view of container window to icon view
                set toolbar visible of container window to false
                set statusbar visible of container window to false
                set the bounds of container window to {${WINDOW_LEFT}, ${WINDOW_TOP}, ${WINDOW_RIGHT}, ${WINDOW_BOTTOM}}
                set theViewOptions to the icon view options of container window
                set arrangement of theViewOptions to not arranged
                set icon size of theViewOptions to ${ARG_ICON_SIZE}
                -- Settings background
                ${NO_BG}set background picture of theViewOptions to file \".background:${BG_IMG_NAME}\"
                -- Adding symlink to /Applications
                make new alias file at container window to POSIX file \"/Applications\" with properties {name:\"Applications\"}
                -- Reopening
                close
                open
                -- Rearranging
                ${NO_COORDS}set the position of item \"Applications\" to {${APPS_X}, ${APPS_Y}}
                ${NO_COORDS}set the position of item \"${APP_BUNDLE_NAME}\" to {${BUNDLE_X}, ${BUNDLE_Y}}
                -- Updating and sleeping for 5 secs
                update without registering applications
                delay 5
        end tell
end tell
"

echo "$APPLESCRIPT" | osascript

echo "done!"

echo "*** Converting tempoprary dmg image in compressed readonly final image... "
echo "    * Changing mode and syncing..."
chmod -Rf go-w /Volumes/"${VOL_NAME}"
sync
sync
echo "    * Detaching ${device}..."
hdiutil detach -force ${device}
rm -f ${DMG_NAME}
echo "    * Converting..."
hdiutil convert "${DMG_NAME_TMP}" -format UDZO -imagekey zlib-level=9 -o "${DMG_NAME}"
echo "done!"

echo -n "*** Removing temporary image... "
rm -f "${DMG_NAME_TMP}"
echo "done!"

echo -n "*** Cleaning up temp folder... "
rm -rf $TMP_DIR
echo "done!"

echo "
*** Everything done. DMG disk image is ready for distribution.
"
