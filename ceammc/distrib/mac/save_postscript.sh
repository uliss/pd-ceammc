#!/bin/bash

APP="@PD_BUNDLE_FULL_PATH@"

if [ $# -ne 2 ]
then
    echo "Usage: $(basename $0) IN_PD OUT_PS"
    exit 1
fi

IN=$1
OUT=$2
CMD0="tcl after 4000 exit"
CMD1="tcl after 3000 [tkcanvas_name [array names ::parentwindows]] postscript -file $OUT"
CMD2="tcl after 2500 [tkcanvas_name [array names ::parentwindows]] lower cord"
nohup $APP -send "ceammc $CMD0, $CMD1, $CMD2" "$IN" &

sleep 2

osascript <<EOD
   tell application "Wish"
       activate
   end tell
EOD


