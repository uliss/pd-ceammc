namespace eval ::ceammc::colorpanel:: { }

proc ::ceammc::colorpanel::open {objectid initialcolor} {
    set color [tk_chooseColor -initialcolor $initialcolor]
    pdsend "$objectid .callback $color"
}
