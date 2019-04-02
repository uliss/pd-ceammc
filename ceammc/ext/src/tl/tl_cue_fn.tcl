namespace eval ::ceammc::tl:: {
    variable receive_symbol
}

# idea from #tcl for a Tcl unbind
proc ::ceammc::tl::unbind {tag event script} {
    set bind {}
    foreach x [split [bind $tag $event] \"\n\"] {
        if {$x != $script} {
            lappend bind $x
        }
    }
    bind $tag $event {}
    foreach x $bind {bind $tag $event $x}
}

proc ::ceammc::tl::setbounds {x y w h name} {
    variable receive_symbol
    pdsend "$receive_symbol setbounds $x $y $w $h $name"
}

proc ::ceammc::tl::startpolling {} {
    bind PatchWindow <Configure> {+::ceammc::tl::setbounds %x %y %w %h %W}
}

proc ::ceammc::tl::stoppolling {} {
    unbind PatchWindow <Configure> {::ceammc::tl::setbounds %x %y %w %h %W}
}

proc ::ceammc::tl::setup {symbol} {
    variable receive_symbol $symbol
}
