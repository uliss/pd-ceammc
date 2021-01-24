#!/bin/bash

usage() {
    echo "Usage: $(basename $0) (-jpg|-png|-pdf)"
}

if [ $# -ne 1 ]
then
    usage
    exit 1
fi

FMT=$1
MAKE_POSTSCRIPT="@PROJECT_BINARY_DIR@/scripts/save_postscript.sh"
MAKE_IMG="@PROJECT_BINARY_DIR@/scripts/ps_to_png.sh"
DEST="@PROJECT_BINARY_DIR@/doc/examples"

case $1 in
-jpg) FMT=jpg;;
-png) FMT=png;;
-pdf) FMT=pdf;;
*)    echo "invalid format option: \"$1\", expected: -jpg, -png or -pdf"
      usage
      exit 1;;
esac

mkdir -p "$DEST"

for ex in @PROJECT_BINARY_DIR@/ceammc/ext/doc/examples/*.pd
do
    fname=$(basename $ex)
    echo "processing \"$fname\" ..."
    ps="$DEST/${fname%.pd}.ps"
    $MAKE_POSTSCRIPT $ex $ps
    $MAKE_IMG -$FMT $ps
done

