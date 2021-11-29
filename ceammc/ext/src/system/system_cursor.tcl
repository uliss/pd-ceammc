namespace eval ::ceammc::cursor:: {
    variable continue_pollmotion 0
    variable last_x 0
    variable last_y 0
    variable receive_symbol
    variable polltime 10
    variable pollmin 10
    variable pollmax 1000
}

proc ::ceammc::cursor::clip { v min max } {
    if { $v < $min } {
        return $min
    } elseif { $v > $max } {
        return $max
    } else {
        return $v
    }
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

proc ::ceammc::cursor::motion {dest {force 1}} {
    variable last_x
    variable last_y

    set px [winfo pointerx .]
    set py [winfo pointery .]
    set sw [winfo screenwidth .]
    set sh [winfo screenheight .]

    if { $force || $px != $last_x || $py != $last_y } {
        pdsend "$dest .motion $px $py $sw $sh"
        set last_x $px
        set last_y $py
    }
}

proc ::ceammc::cursor::pollmotion {} {
    variable continue_pollmotion
    variable polltime
    variable receive_symbol

    motion $receive_symbol 0
    if {$continue_pollmotion != 0} { after $polltime ::ceammc::cursor::pollmotion }
}

proc ::ceammc::cursor::startpolling {{pollms 0}} {
    variable continue_pollmotion 1
    variable pollmin
    variable pollmax
    variable polltime
    set polltime [clip $pollms $pollmin $pollmax]
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

proc ::ceammc::cursor::setup {symbol} {
    variable receive_symbol $symbol
}
