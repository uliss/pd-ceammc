#!/bin/bash

UI_DIR="@FAUST_UI_DIR@"
OUT_DIR="@ABSTRACTIONS_DIR@"
CMD=pd_faust2ui

cd "$OUT_DIR"

ls -1 $UI_DIR/*.json | while read json
do
    echo "Generating UI for \"$(basename $json)\" ..."
    $CMD "$json"
done

rm -f abstraction_objects.txt
for abs in *.pd
do
    echo ${abs%.pd} >> abstraction_objects.txt
done
