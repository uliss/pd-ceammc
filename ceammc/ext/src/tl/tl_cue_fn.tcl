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

proc ::ceammc::tl::update_line {win x y w h} {
    pdsend "tl_cue_canvas_resize .update_line $win.c $x $y $w $h"
}

proc ::ceammc::tl::startpolling {} {
    ::pdwindow::verbose 0 "\[ceammc\] start tl.cue canvas resize polling...\n"
    bind PatchWindow <Configure> {+::ceammc::tl::update_line %W %x %y %w %h }
}

proc ::ceammc::tl::stoppolling {} {
    ::pdwindow::verbose 0 "\[ceammc\] stop tl.cue canvas resize polling...\n"
    unbind PatchWindow <Configure> {::ceammc::tl::update_line %W %x %y %w %h }
}

