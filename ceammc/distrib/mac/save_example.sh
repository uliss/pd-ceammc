#!/bin/bash

usage() {
    echo "Usage: $(basename $0) (-jpg|-png|-pdf) NAME"
}

if [ $# -ne 2 ]
then
    usage
    exit 1
fi

MAKE_POSTSCRIPT="@PROJECT_BINARY_DIR@/scripts/save_postscript.sh"
MAKE_IMG="@PROJECT_BINARY_DIR@/scripts/ps_to_png.sh"
IN="@PROJECT_BINARY_DIR@/ceammc/ext/doc/examples/$2-example.pd"

if [ ! -f $IN ]
then
    echo "file not found: $IN"
    exit 2
fi

case $1 in
-jpg) FMT=jpg;;
-png) FMT=png;;
-pdf) FMT=pdf;;
*)    echo "invalid format option: \"$1\", expected: -jpg, -png or -pdf"
      usage
      exit 1;;
esac

f=$(basename $IN)
fname=${f%.pd}.ps
$MAKE_POSTSCRIPT $IN $fname
$MAKE_IMG -$FMT $fname
rm $fname
