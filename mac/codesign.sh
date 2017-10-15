#!/bin/sh
DEST=$1
CERT=gdb-cert
CODESIGN="codesign --force --sign $CERT --verbose"
cp entitlements.plist $DEST/Contents/
$CODESIGN $DEST/Contents/entitlements.plist
ls -1 $DEST/Contents/Frameworks | while read lib
do
    $CODESIGN $DEST/Contents/Frameworks/$lib
done

$CODESIGN $DEST/Contents/Resources/bin/pd
$CODESIGN $DEST/Contents/MacOS/Pd

codesign -dv $DEST
