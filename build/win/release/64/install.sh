#!/bin/bash

ninja 
ninja install

./fix_dll_deps.sh

ninja inno

