package provide pd_colors 0.1

namespace eval ::pd_colors:: {
    variable window_background lightgray
}

if {$::windowingsystem eq "aqua"} {
    set ::pd_colors::window_background systemWindowBackgroundColor
}
