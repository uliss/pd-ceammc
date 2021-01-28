namespace eval ::ui {

proc bang_tag {id} { return  ${id}_#all }

proc bang_update {cnv id w h zoom color state} {
    set c [::ceammc::ui::widget_canvas $cnv $id]
    set t [bang_tag $id]
    $c delete $t

    set xpad [expr round($w * 0.1)]
    set ypad [expr round($h * 0.1)]
    if {$w < 20 } { set xpad 1 }
    if {$h < 20 } { set ypad 1 }

    if {$zoom == 1} {set corr 0} {set corr 1}

    set x0 [expr $xpad + $corr]
    set y0 [expr $ypad + $corr]
    set x1 [expr $w - $xpad]
    set y1 [expr $h - $ypad]

    if { $state == 1 } {
        set fill_color $color
        set bd_color $color
    } else {
        set fill_color {}
        set bd_color $color
    }

    $c create oval $x0 $y0 $x1 $y1 -fill $fill_color -outline $bd_color -width $zoom -tags $t
}

}
