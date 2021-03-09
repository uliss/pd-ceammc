namespace eval ::ui {

namespace eval sliders {
    variable lbl_font_size 7
    variable lbl_font_family "Helvetica"
}

proc sliders_label_font {zoom} {
    return "$sliders::lbl_font_family [expr $sliders::lbl_font_size*$zoom] normal roman"
}

proc sliders_delete {cnv id} {
    set c [::ceammc::ui::widget_canvas $cnv $id]
    set t [::ceammc::ui::widget_tag $id]
    $c delete $t
}

proc sliders_draw_labels {cnv id w h zoom color lmin lmax} {
    set c [::ceammc::ui::widget_canvas $cnv $id]
    set f [sliders_label_font $zoom]
    set t [::ceammc::ui::widget_tag $id]
    set off [expr 3 * $zoom]
    set x0 $off
    set y0 $off
    set x1 $off
    set y1 [expr $h-$off]

    $c create text $x1 $y1 -text $lmin -anchor sw -justify left -font $f -fill $color -width 0 -tags $t
    $c create text $x0 $y0 -text $lmax -anchor nw -justify left -font $f -fill $color -width 0 -tags $t
}

proc sliders_draw_vbar {c t w h zoom color i bar} {
    set v [expr $bar / 999.0]
    if { $v < 0 } { set v 0 }
    if { $v > 1 } { set v 1 }
    set x0 [expr $i*$w]
    set y0 [expr (1-$v)*$h]
    set x1 [expr ($i+1)*$w]
    set y1 $h

    if {$w > 1 } {
        set oc [::tk::Darken $color 110]
    } else {
        set oc {}
    }

    $c create rectangle $x0 $y0 $x1 $y1 -fill $color -outline $oc -width 1 -tags $t
    set handle_color [::tk::Darken $color 70]
    $c create line $x0 $y0 $x1 $y0 -fill $handle_color -width [expr 2*$zoom] -capstyle butt -tags $t
}

proc sliders_draw_hbar {c t w h zoom color i bar} {
    set v [expr $bar / 999.0]
    if { $v < 0 } { set v 0 }
    if { $v > 1 } { set v 1 }
    set x0 0
    set y0 [expr $i*$h]
    set x1 [expr $v*$w]
    set y1 [expr ($i+1)*$h]

    if {$w > 1 } {
        set oc [::tk::Darken $color 110]
    } else {
        set oc {}
    }

    $c create rectangle $x0 $y0 $x1 $y1 -fill $color -outline $oc -width 1 -tags $t
    set handle_color [::tk::Darken $color 70]
    $c create line $x1 $y0 $x1 $y1 -fill $handle_color -width [expr 2*$zoom] -capstyle butt -tags $t
}

proc sliders_draw_bars {cnv id w h zoom bar_color active_color vert active args} {
    set c [::ceammc::ui::widget_canvas $cnv $id]
    set t [::ceammc::ui::widget_tag $id]
    set n [llength $args]

    if { $n == 0 } { return }
    if { $vert == 0 } {
        set bar_w [expr $w / ($n+0.0)]
        for { set i 0 } { $i < $n } { incr i } {
            # draw later
            if { $active == $i } { continue }

            set bar [lindex $args $i]
            sliders_draw_vbar $c $t $bar_w $h $zoom $bar_color $i $bar
        }

        if { $active >= 0 && $active < [llength $args] } {
            set bar [lindex $args $active]
            sliders_draw_vbar $c $t $bar_w $h $zoom $active_color $active $bar
        }
    } else {
        set bar_h [expr $h / ($n+0.0)]
        for { set i 0 } { $i < $n } { incr i } {
            # draw later
            if { $active == $i } { continue }

            set bar [lindex $args $i]
            sliders_draw_hbar $c $t $w $bar_h $zoom $bar_color $i $bar
        }

        if { $active >= 0 && $active < [llength $args] } {
            set bar [lindex $args $active]
            sliders_draw_hbar $c $t $w $bar_h $zoom $active_color $active $bar
        }
    }
}

}
