namespace eval ::ceammc::colorpanel:: { }

proc ::ceammc::colorpanel::do_open {objectid initialcolor} {
    set color [tk_chooseColor -initialcolor $initialcolor]
    pdsend "$objectid .tcl_callback $color"
}

proc ::ceammc::colorpanel::open {objectid initialcolor} {
    after idle [list after 100 ::ceammc::colorpanel::do_open $objectid $initialcolor]
}
