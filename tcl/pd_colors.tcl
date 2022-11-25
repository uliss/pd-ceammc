package provide pd_colors 0.1

namespace eval ::pd_colors:: {
    variable window_background      "lightgray"
    variable text_background        "white"
    variable xlet_signal            "#110088"
    variable xlet_control           "#555555"
    variable obj_fill               "#F5F8F8"
    variable obj_text               "#444444"
    variable obj_text_selected      "#0022EE"
    variable obj_border             "#BFBFBF"
    variable obj_border_selected    "#1441E6"
    variable log_print              "black"
    variable log_debug              "#484848"
    variable cord_normal            "#555555"
    variable cord_selected          "#1441E6"
    variable comment_border         "#BFBFBF"
    variable msg_clicked            "#00AAFF"
}

if {[tk windowingsystem] eq "aqua"} {
    set ::pd_colors::window_background  systemWindowBackgroundColor
    set ::pd_colors::text_background    systemTextBackgroundColor
    set ::pd_colors::log_print          systemTextColor
    set ::pd_colors::log_debug          systemControlAccentColor
    set ::pd_colors::xlet_control       systemWindowBackgroundColor7
    set ::pd_colors::obj_text           systemTextColor
    set ::pd_colors::obj_text_selected  systemControlAccentColor
    set ::pd_colors::obj_fill           systemWindowBackgroundColor
    set ::pd_colors::obj_border         systemWindowBackgroundColor6
    set ::pd_colors::msg_clicked        systemControlAccentColor
    set ::pd_colors::comment_border     systemWindowBackgroundColor6
}
