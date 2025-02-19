package provide pd_colors 0.1

namespace eval ::pd_colors:: {
    variable window_background      [ttk::style lookup TFrame -background]
    variable window_transparent     "lightgray"
    variable text_background        "white"
    variable text                   "black"
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

switch -- [tk windowingsystem] {
    "aqua" {
        set ::pd_colors::window_background  systemWindowBackgroundColor
        set ::pd_colors::window_transparent systemTransparent
        set ::pd_colors::text               systemTextColor
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
    "win32" {
        set ::pd_colors::window_background  [ttk::style lookup TFrame -background]
        set ::pd_colors::window_transparent systemWindow
        set ::pd_colors::text               systemWindowText
        set ::pd_colors::text_background    systemWindow
        set ::pd_colors::log_print          systemWindowText
        set ::pd_colors::log_debug          systemDisabledText
        set ::pd_colors::xlet_control       system3dDarkShadow
        set ::pd_colors::obj_text           systemWindowText
        set ::pd_colors::obj_text_selected  systemHighlight
        set ::pd_colors::obj_fill           systemMenu
        set ::pd_colors::obj_border         system3dDarkShadow
        set ::pd_colors::msg_clicked        systemHighlight
    }
}

