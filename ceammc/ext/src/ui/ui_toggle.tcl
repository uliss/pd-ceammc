namespace eval ::ui {

proc toggle_tag {id} { return  ${id}_#all }

proc toggle_update {cnv id w h zoom color state} {
    set c [::ceammc::ui::widget_canvas $cnv $id]
    set t [toggle_tag $id]
    $c delete $t

    if { $state == 0 } { return }

    set xpad [expr floor($w * 0.1)]
    set ypad [expr floor($h * 0.1)]
    if {$xpad > 16} { set xpad 16 }
    if {$ypad > 16} { set ypad 16 }

    if {$zoom == 1} {set corr 1} {set corr 0}

    set x0 [expr $xpad + $corr]
    set y0 [expr $ypad + $corr]
    set x1 [expr $w - $xpad]
    set y1 [expr $h - $ypad]
    set wd [expr 1 + $zoom]

    if {$w>30} {set wd [expr $wd + $xpad*0.5]}

    $c create line $x0 $y0 $x1 $y1 -fill $color -width $wd -capstyle butt -tags $t
    $c create line $x0 $y1 $x1 $y0 -fill $color -width $wd -capstyle butt -tags $t
}

}
