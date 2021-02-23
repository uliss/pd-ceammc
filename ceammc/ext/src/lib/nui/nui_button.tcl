namespace eval ::nui {
namespace eval button {
    proc tag_all  { id } { return "#btn${id}" }
    proc tag_box  { id } { return "#btn${id}_box" }
    proc tag_knob { id } { return "#btn${id}_kn" }

    proc create { cnv model id x y w h zoom state border_color fill_color active_color } {
        set c [::nui::widget_canvas $cnv $model]
        set ta [tag_all $id]
        set tb [tag_box $id]
        set tn [tag_knob $id]

        # draw box
        $c create rectangle $x $y [expr $x+$w] [expr $y+$h] \
            -fill $fill_color -outline $border_color -width $zoom -tags [list $ta $tb]

        # draw knob
        set pad [expr 3*$zoom]
        set x0 [expr $x + $pad ]
        set y0 [expr $y + $pad ]
        set x1 [expr $x + $w - $pad]
        set y1 [expr $y + $h - $pad]

        if { $state } {
            $c create rectangle $x0 $y0 $x1 $y1 \
                -fill $active_color -outline $active_color -width $zoom -tags [list $ta $tn]
        } else {
            $c create rectangle $x0 $y0 $x1 $y1 \
                -fill $fill_color -outline $fill_color -width $zoom -tags [list $ta $tn]
        }
    }

    proc update { cnv model id w h state border_color fill_color active_color } {
        set c [::nui::widget_canvas $cnv $model]
        set ta [tag_all $id]
        set tb [tag_box $id]
        set tn [tag_knob $id]

        # draw box
        $c itemconfigure $tb -fill $fill_color -outline $border_color

        if { $state } {
            $c itemconfigure $tn -fill $active_color -outline $active_color
        } else {
            $c itemconfigure $tn -fill $fill_color -outline $fill_color
        }
    }
}
}
