namespace eval ::ui {

proc button_tag {id} { return  ${id}_#all }

proc button_update {cnv id w h zoom c0 c1 state} {
    set c [::ceammc::ui::widget_canvas $cnv $id]
    set t [button_tag $id]
    $c delete $t

    if { $state == 1 } { set color $c1 } { set color $c0 }
    set xpad [expr {round($w/10)} + 1 + $zoom]
    set ypad [expr {round($h/10)} + 1 + $zoom]

    set x0 [expr $xpad]
    set x1 [expr $w - $xpad]
    set y0 [expr $ypad]
    set y1 [expr $h - $ypad]
    $c create rectangle $x0 $y0 $x1 $y1 -fill $color -outline $color -width $zoom -tags $t
}

}
