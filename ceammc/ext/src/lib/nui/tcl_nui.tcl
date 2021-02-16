namespace eval ::nui {

proc pd_canvas { cnv } { return ".x${cnv}.c" }

proc widget_canvas { cnv id } { return ".x${cnv}.c.cnv${id}" }
proc widget_window { cnv id } { return ".x${cnv}.c.win${id}" }
proc widget_w   { w zoom } { expr $w * $zoom + 1 }
proc widget_h   { h zoom } { expr $h * $zoom + 1 }
proc widget_tag { id } { return "#${id}" }

proc widget_create { cnv id x y w h zoom } {
    set win [widget_window $cnv $id]
    set c [widget_canvas $cnv $id]
    set w [widget_w $w $zoom]
    set h [widget_h $h $zoom]

    namespace eval "nui$id" {}
    destroy $c
    canvas $c -width $w -height $h -bd 0 \
        -highlightthickness 0 -insertborderwidth 0 \
        -state normal -takefocus 1 -insertwidth 0 -confine 0

    [pd_canvas $cnv] create window $x $y -anchor nw -window $c \
        -tags $win -width [expr $w+1] -height [expr $h+1]
}

proc widget_erase { cnv id } {
    destroy [widget_window $cnv $id]
}

proc widget_move { cnv id x y } {
    set c [pd_canvas $cnv]
    set win [widget_window $cnv $id]
    $c coords $win $x $y
}

namespace eval frame {
    proc tag { id } { return "#f${id}" }

    proc create { cnv id x y w h fill_color out_color width } {
        set c [::nui::widget_canvas $cnv $id]
        set t [tag $id]
        $c create rectangle $x $y [expr $x+$w] [expr $y+$h] \
            -fill $fill_color -outline $out_color -width $width -tags $t
    }

    proc update_outline { cnv id color } {
        set c [::nui::widget_canvas $cnv $id]
        set t [tag $id]
        $c itemconfigure $t -outline $color
    }

    proc erase { cnv id } {
        set c [::nui::widget_canvas $cnv $id]
        set t [tag $id]
        $c delete $t
    }
}

namespace eval label {
    proc create { cnv id x y width anchor font color txt } {
        set c [::nui::widget_canvas $cnv $id]
        set t [::nui::widget_tag $id]

        $c create text $x $y -fill $color -text $txt -font $font -anchor $anchor -width $width -tags $t
    }
}

namespace eval slider {
    proc tag { id } { return "#sl${id}" }
    proc knob_tag { id } { return "#sl${id}_kn" }
    proc knob_xpos { x w pos } { return [expr ($w*$pos)+$x] }

    proc hcreate { cnv id x y w h pos value out_color fill_color knob_color } {
        set c [::nui::widget_canvas $cnv $id]
        set t [tag $id]

        $c create rectangle $x $y [expr $x+$w] [expr $y+$h] \
            -fill $fill_color -outline $out_color -width 1 -tags $t

        # draw knob
        set knob_x [knob_xpos $x $w $pos]
        set kt [knob_tag $id]
        $c create line $knob_x $y $knob_x [expr $y+$h] \
            -fill $knob_color -width 2 -tags $kt
    }

    proc hupdate { cnv id pos value out_color fill_color knob_color } {
        set c [::nui::widget_canvas $cnv $id]
        set t [tag $id]

        $c itemconfigure $t -fill $fill_color -outline $out_color

        lassign [$c coords $t] x0 y0 x1 y1
        set w [expr $x1-$x0]
        set x [knob_xpos $x0 $w $pos]

        # update knob
        set kt [knob_tag $id]
        lassign [$c coords $kt] x0 y0 x1 y1
        $c coords $kt $x $y0 $x $y1
        $c itemconfigure $kt -fill $knob_color
    }
}

}
