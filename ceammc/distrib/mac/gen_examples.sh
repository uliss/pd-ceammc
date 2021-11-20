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

echo `pwd`

for ex in @PROJECT_SOURCE_DIR@/ceammc/ext/doc/*.pddoc
do
    fname=$(basename $ex)
    echo "processing \"$fname\" ..."  
    xmllint --nocdata --xpath '//pdascii[not(@id) or @id=main]/text()' $ex > tmp.ascii
    if [[ $? -ne 0 ]]
    then
        echo "no example found in pddoc: $fname"
        continue
    fi

    cat tmp.ascii | recode html..ascii > tmp.ascii2
    mv tmp.ascii2 tmp.ascii
    pd_ascii2pd --auto --xlet-db "@PROJECT_SOURCE_DIR@/ceammc/ext/doc/ceammc.db" tmp.ascii
    ps="$DEST/${fname%.pd}.ps"
    $MAKE_POSTSCRIPT tmp.pd $ps
    $MAKE_IMG -$FMT $ps
done

