namespace eval ::ui {

proc button_delete {id cnv} {
    $cnv delete ${id}_#all
}

proc button_create {id cnv w h c0 c1 state} {
    if { $state == 1 } { set c $c1 } { set c $c0 }
    set xpad [expr {round($w/10)} + 1]
    set ypad [expr {round($h/10)} + 1]

    set x0 [expr $xpad + 1]
    set x1 [expr ($w - $xpad) + 1]
    set y0 [expr $ypad + 1]
    set y1 [expr ($h - $ypad) + 1]
    $cnv create rectangle $x0 $y0 $x1 $y1 -fill $c -width 0 -tags ${id}_#all
}

}
