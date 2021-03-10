namespace eval ::ui {

proc radio_draw_single {c t w h zoom orient states color} {
    set min_size [expr 2 * $zoom]
    if { $orient == "v" } {
        set cell_size $w
        set pad [expr round($cell_size/5.0)]
        if { $pad < $min_size } { set pad $min_size }
        set i 0
        foreach x [split $states {}] {
            if { $x == "1" } {
                set y0 [expr $i*$cell_size + $pad]
                set y1 [expr ($i+1)*$cell_size - $pad]
                set x0 $pad
                set x1 [expr $w - $pad]
                $c create rectangle $x0 $y0 $x1 $y1 -fill $color -outline $color -width $zoom -tags $t
                break
            }

            incr i
        }
    } else {
        set cell_size $h
        set pad [expr round($cell_size/5.0)]
        if { $pad < $min_size } { set pad $min_size }
        set i 0
        foreach x [split $states {}] {
            if { $x == "1" } {
                set x0 [expr $i*$cell_size + $pad]
                set x1 [expr ($i+1)*$cell_size - $pad]
                set y0 $pad
                set y1 [expr $h - $pad]
                $c create rectangle $x0 $y0 $x1 $y1 -fill $color -outline $color -width $zoom -tags $t
                break
            }

            incr i
        }
    }
}

proc radio_draw_cross {c t x y w h zoom color} {
    set xpad [expr floor($w * 0.1)]
    set ypad [expr floor($h * 0.1)]
    if {$xpad > 16} { set xpad 16 }
    if {$ypad > 16} { set ypad 16 }

    if {$zoom == 1} {set corr 1} {set corr 0}

    set x0 [expr $x + $xpad + $corr]
    set y0 [expr $y + $ypad + $corr]
    set x1 [expr $x + $w - $xpad]
    set y1 [expr $y + $h - $ypad]
    set wd [expr 1 + $zoom]

    if {$w>30} {set wd [expr $wd + $xpad*0.5]}

    $c create line $x0 $y0 $x1 $y1 -fill $color -width $wd -capstyle butt -tags $t
    $c create line $x0 $y1 $x1 $y0 -fill $color -width $wd -capstyle butt -tags $t
}

proc radio_draw_multi {c t w h zoom orient states color} {
    if { $orient == "v" } {
        set cell_size $w
        set i 0
        foreach x [split $states {}] {
            if { $x == "1" } {
                set y0 [expr $i*$cell_size]
                radio_draw_cross $c $t 0 $y0 $cell_size $cell_size $zoom $color
            }

            incr i
        }
    } else {
        set cell_size $h
        set i 0
        foreach x [split $states {}] {
            if { $x == "1" } {
                set x0 [expr $i*$cell_size]
                radio_draw_cross $c $t $x0 0 $cell_size $cell_size $zoom $color
            }

            incr i
        }
    }
}

proc radio_update {cnv id w h zoom orient mode states act_color out_color} {
    set c [::ceammc::ui::widget_canvas $cnv $id]
    set t [::ceammc::ui::widget_tag $id]
    $c delete $t

    set num_cells [string length $states]
    if { $orient == "v" } {
        set cell_size $w
        for { set i 1 } { $i < $num_cells } { incr i } {
            set y [expr $cell_size * $i]
            $c create line 0 $y $w $y -fill $out_color -width $zoom -capstyle butt -tags $t
        }
    } else {
        set cell_size $h
        for { set i 1 } { $i < $num_cells } { incr i } {
            set x [expr $cell_size * $i]
            $c create line $x 0 $x $h -fill $out_color -width $zoom -capstyle butt -tags $t
        }
    }

    if { $mode == "." } {
        radio_draw_single $c $t $w $h $zoom $orient $states $act_color
    } else {
        radio_draw_multi  $c $t $w $h $zoom $orient $states $act_color
    }
}

}
