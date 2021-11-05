namespace eval ::ceammc::cursor:: {
    variable continue_pollmotion 0
    variable last_x 0
    variable last_y 0
    variable receive_symbol
}

# idea from #tcl for a Tcl unbind
proc ::ceammc::cursor::unbind {tag event script} {
    set bind {}
    foreach x [split [bind $tag $event] \"\n\"] {
        if {$x != $script} { lappend bind $x }
    }
    bind $tag $event {}
    foreach x $bind {bind $tag $event $x}
}

proc ::ceammc::cursor::button {button state} {
    variable receive_symbol
    pdsend "$receive_symbol .button $button $state"
}

proc ::ceammc::cursor::mousewheel {delta} {
    variable receive_symbol
    pdsend "$receive_symbol .mousewheel $delta"
} 

proc ::ceammc::cursor::motion {x y w h} {
    variable last_x
    variable last_y
    variable receive_symbol
    if { $x != $last_x || $y != $last_y } {
        pdsend "$receive_symbol .motion $x $y $w $h"
        set last_x $x
        set last_y $y
    }
}

proc ::ceammc::cursor::pollmotion {} {
    variable continue_pollmotion
    motion [winfo pointerx .] [winfo pointery .] [winfo screenwidth .] [winfo screenheight .]
    if {$continue_pollmotion != 0} { after 10 ::ceammc::cursor::pollmotion }
}

proc ::ceammc::cursor::startpolling {} {
    variable continue_pollmotion 1
    pollmotion 
    bind all <ButtonPress> {+::ceammc::cursor::button %b 1}
    bind all <ButtonRelease> {+::ceammc::cursor::button %b 0}
    bind all <MouseWheel> {+::ceammc::cursor::mousewheel %D}
}

proc ::ceammc::cursor::stoppolling {} {
    variable continue_pollmotion 0 
    ::ceammc::cursor::unbind all <ButtonPress> {::ceammc::cursor::button %b 1}
    ::ceammc::cursor::unbind all <ButtonRelease> {::ceammc::cursor::button %b 0}
    ::ceammc::cursor::unbind all <MouseWheel> {::ceammc::cursor::mousewheel %D}
}

# in Pd 0.43, the internal proc changed from 'pd' to 'pdsend'
proc ::ceammc::cursor::setup {symbol} {
    variable receive_symbol $symbol
}
