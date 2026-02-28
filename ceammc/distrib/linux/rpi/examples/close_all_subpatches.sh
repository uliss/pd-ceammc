#!/bin/bash

if [ $# -eq 0 ]; then
    echo "Usage: $0 file1 [file2 ...]"
    exit 1
fi

for FILE in "$@"; do
    if [ -f "$FILE" ]; then
        echo "=== processing: $FILE ==="
        sed '/^#N .*1;$/ {
            s/1;$/0;/
        }' "$FILE" > "$FILE.bak"
        mv "$FILE.bak" "$FILE"
    else
        echo "Warning: file '$FILE' not found, skipping ..."
    fi
done

