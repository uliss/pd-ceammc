#!/bin/bash
BUNDLE=$1

if [[ $# -lt 1 ]]; then
    echo "Usage: $(basename $0) PATH_TO_PD.app"
    exit 1
fi

echo $BUNDLE
