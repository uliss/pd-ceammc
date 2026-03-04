namespace eval ::ceammc::colorpanel:: {}

proc ::ceammc::colorpanel::parse_zenity {str} {
    set raw [string trim $str]

    # parse rgb(r,g,b) or rgba(r,g,b,a)
    if {[regexp {rgb[a]?\((\d+),(\d+),(\d+)} $raw -> r g b]} {
        return [format "#%02x%02x%02x" $r $g $b]
    } else {
        return $raw
    }
}

proc ::ceammc::colorpanel::do_open {objectid initialcolor} {
    set color {}

    switch $::windowingsystem {
        "x11" {
            set cmd "exec zenity --color-selection --title=Color --color=$initialcolor"
            # ::pdwindow::debug "$cmd\n"
            if {[catch $cmd result]} {
                return
            }

            set color [::ceammc::colorpanel::parse_zenity $result]
        }
        default {
            set color [tk_chooseColor -initialcolor $initialcolor]
        }
    }

    if {$color ne ""} {
        pdsend "$objectid .tcl_callback $color"
    }
}

proc ::ceammc::colorpanel::open {objectid initialcolor} {
    after idle [list after 100 ::ceammc::colorpanel::do_open $objectid $initialcolor]
}
