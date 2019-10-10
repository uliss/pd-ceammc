proc ceammc_fix_macos_state {n} { if { $n > 256 } { return [expr $n & 0xFF ] } { return $n } }
proc ceammc_fix_win32_alt {n} {
    if { $n & 131072 } {
        incr n -131072
        set n [expr $n | 16]
    }
    return $n
}
proc ceammc_fix_win32_state {n} {
    if { $n & 32 } { incr n -32 }
    return [expr 0xFF & [ceammc_fix_win32_alt $n]]
}

proc ceammc_fix_x11_state {n} {}

proc ceammc_send_motion {obj x y mod} {
    switch -- $::windowingsystem {
        "aqua" {
            # disable mouse dragging for buttons other then first
            if { $mod < 512 } { pdsend "$obj mousemove $x $y [ceammc_fix_macos_state $mod]" }
        } "win32" {
            set mod [ceammc_fix_win32_alt $mod]
            puts "motions: $obj $x $y $mod \n"
            if { $mod < 512 } { pdsend "$obj mousemove $x $y [ceammc_fix_win32_state $mod]" }
        } "default" {
            pdsend "$obj mousemove $x $y $mod"
        }
    }
}

proc ceammc_bind_mouse_down {id obj} {
    switch -- $::windowingsystem {
        "win32" {
            bind $id <ButtonPress-1> [subst -nocommands {+pdsend "$obj mousedown %x %y %X %Y [ceammc_fix_win32_state %s]"}]
        } "default" {
            bind $id <ButtonPress-1> [subst {+pdsend "$obj mousedown %x %y %X %Y %s"}]
        }
    }
}

# sends #obj mouseup X Y MOD
proc ceammc_bind_mouse_up {id obj} {
    switch -- $::windowingsystem {
        "aqua" {
            bind $id <ButtonRelease-1> [subst -nocommands {+pdsend "$obj mouseup %x %y [ceammc_fix_macos_state %s]"}]
        } "win32" {
            bind $id <ButtonRelease-1> [subst -nocommands {+pdsend "$obj mouseup %x %y [ceammc_fix_win32_state %s]"}]
        } "default" {
            bind $id <ButtonRelease-1> [subst {+pdsend "$obj mouseup %x %y %s"}]
        }
    }
}

# sends #obj mousemove X Y MOD
proc ceammc_bind_mouse_move {id obj} { bind $id <Motion> [subst {+ceammc_send_motion $obj %x %y %s}] }
proc ceammc_bind_mouse_enter {id obj} { bind $id <Enter> [subst {+pdsend "$obj mouseenter"}] }
proc ceammc_bind_mouse_leave {id obj} { bind $id <Leave> [subst {+pdsend "$obj mouseleave"}] }
proc ceammc_bind_mouse_double_click {id obj} { bind $id <Double-Button-1> [subst {+pdsend "$obj dblclick %x %y %s"}] }

# sends #obj mouseup X Y ABSX ABSY MOD
proc ceammc_bind_mouse_right_click {id obj} {
    switch -- $::windowingsystem {
        "aqua" {
            bind $id <ButtonPress-2> [subst {+pdsend "$obj rightclick %x %y %X %Y %s"}]
            bind $id <Control-ButtonPress-1> [subst {+pdsend "$obj rightclick %x %y %X %Y %s"}]
        } "win32" {
            bind $id <ButtonPress-3> [subst -nocommands {+pdsend "$obj rightclick %x %y %X %Y [ceammc_fix_win32_state %s]"}]
        } "default" {
            bind $id <ButtonPress-3> [subst {+pdsend "$obj rightclick %x %y %X %Y %s"}]
        }
    }
}

# sends #obj mouseup X Y DELTA MOD
proc ceammc_bind_mouse_wheel {id obj} {
    switch -- $::windowingsystem {
        "win32" {
            bind $id <MouseWheel> [subst -nocommands {+pdsend "$obj mousewheel %x %y [expr %D / 120.0] [ceammc_fix_win32_state %s]"}]
        } "default" {
            bind $id <MouseWheel> [subst {+pdsend "$obj mousewheel %x %y %D %s"}]
        }
    }
}
