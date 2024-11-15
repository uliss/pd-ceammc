#!/bin/bash

cat "@CMAKE_CURRENT_SOURCE_DIR@/ceammc_popup_menu_vanilla.tcl" \
    "@CMAKE_CURRENT_SOURCE_DIR@/ceammc_popup_menu_ceammc.tcl" \
    "@CMAKE_CURRENT_BINARY_DIR@/ceammc_popup_menu_extra.tcl" \
    > "@CMAKE_CURRENT_BINARY_DIR@/ceammc_popup_menu_tree.tcl"
