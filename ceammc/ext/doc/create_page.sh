#!/bin/bash

[ $# -eq 1 ] || { echo "Usage: $(basename $0) NAME"; exit 1; }

pd_create --page $1 --library ceammc "@CMAKE_CURRENT_SOURCE_DIR@"
