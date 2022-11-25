package provide pd_colors 0.1

namespace eval ::pd_colors:: {
    variable window_background "lightgray"
    variable xlet_signal "#2200AA"
    variable xlet_control "#555555"
    variable obj_fill "#F5F8F8"
    variable obj_text "#444444"
    variable log_print "black"
    variable log_debug "#484848"
}

if {[tk windowingsystem] eq "aqua"} {
    set ::pd_colors::window_background systemWindowBackgroundColor
    set ::pd_colors::log_print systemTextColor
    set ::pd_colors::log_debug systemControlAccentColor
    set ::pd_colors::xlet_control systemWindowBackgroundColor7
    set ::pd_colors::obj_text systemWindowBackgroundColor7
    set ::pd_colors::obj_fill systemWindowBackgroundColor3
}
