# this script changes background color in edit/run modes

namespace eval ::editmode_look {
    set run_mode_color  "#FAFAFA"
    set edit_mode_color "#FFFFFF"
}

if {[tk windowingsystem] eq "aqua"} {
    set ::editmode_look::run_mode_color systemWindowBackgroundColor
    set ::editmode_look::edit_mode_color systemTextBackgroundColor
}

proc ::editmode_look::set_edit_style {mytoplevel} {
    set tkcanvas [tkcanvas_name $mytoplevel]
    $tkcanvas configure -background $::editmode_look::edit_mode_color
}

proc ::editmode_look::set_run_style {mytoplevel} {
    set tkcanvas [tkcanvas_name $mytoplevel]
    $tkcanvas configure -background $::editmode_look::run_mode_color
}

proc ::editmode_look::set_style {mytoplevel} {
    if {$mytoplevel eq ".pdwindow"} {
        return
    }

    if {$::editmode($mytoplevel) == 1} {
        ::editmode_look::set_edit_style $mytoplevel
    } else {
        ::editmode_look::set_run_style $mytoplevel
    }
}

bind PatchWindow <<EditMode>> {+::editmode_look::set_style %W}
bind PatchWindow <<Loaded>>   {+::editmode_look::set_edit_style %W}

::pdwindow::debug "\[ceammc\]: ceammc_editmode-plugin loaded\n"
