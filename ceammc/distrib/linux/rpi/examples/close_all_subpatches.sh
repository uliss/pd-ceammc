#!/bin/bash

if [ -z "$1" ]; then
    echo "Usage: $0 <pd-file>"
    exit 1
fi

FILE=$1

sed '/^#N .*1;$/ {
    s/1;$/0;/
}' "$FILE" > "$FILE.bak"

mv "$FILE.bak" "$FILE"
