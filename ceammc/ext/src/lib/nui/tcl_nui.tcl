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

proc item_erase { cnv tags } {
    set c [widget_canvas $cnv $id]
    $c delete $tags
}

proc item_move { cnv tag x y } {
    set c [widget_canvas $cnv $id]

    lassign [$c coords $tag] x0 y0 x1 y1
    set w [expr $x1-$x0]
    set h [expr $y1-$y0]
    $c coords $tag $x $y [expr $x+$w] [expr $y+$h]
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
        ::nui::item_erase $cnv [tag $id]
    }

    proc move  { cnv id x y } {
        ::nui::item_move $cnv [tag $id] $x $y
    }
}

namespace eval label {
    proc tag { id } { return "#l${id}" }

    proc create { cnv id x y width anchor font color txt } {
        set c [::nui::widget_canvas $cnv $id]
        set t [tag $id]

        $c create text $x $y -fill $color -text $txt -font $font -anchor $anchor -width $width -tags $t
    }

    proc erase { cnv id } {
        ::nui::item_erase $cnv [tag $id]
    }

    proc move  { cnv id x y } {
        ::nui::item_move $cnv [tag $id] $x $y
    }
}

namespace eval slider {
    proc tag_all  { id } { return "#sl${id}" }
    proc tag_box  { id } { return "#sl${id}_box" }
    proc tag_knob { id } { return "#sl${id}_kn" }

    proc knob_xpos { x w pos } { return [expr ($w*$pos)+$x] }

    proc hcreate { cnv id x y w h pos value out_color fill_color knob_color } {
        set c [::nui::widget_canvas $cnv $id]
        set ta [tag_all $id]
        set tb [tag_box $id]
        set tkn [tag_knob $id]

        # draw box
        $c create rectangle $x $y [expr $x+$w] [expr $y+$h] \
            -fill $fill_color -outline $out_color -width 1 -tags [list $ta $tb]

        # draw knob
        set knob_x [knob_xpos $x $w $pos]
        $c create line $knob_x $y $knob_x [expr $y+$h] \
            -fill $knob_color -width 2 -tags [list $ta $tkn]
    }

    proc hupdate { cnv id pos value out_color fill_color knob_color } {
        set c [::nui::widget_canvas $cnv $id]
        set tb [tag_box $id]
        set tkn [tag_knob $id]

        # update box
        $c itemconfigure $tb -fill $fill_color -outline $out_color

        lassign [$c coords $tb] x0 y0 x1 y1
        set w [expr $x1-$x0]
        set x [knob_xpos $x0 $w $pos]

        # update knob
        lassign [$c coords $tkn] x0 y0 x1 y1
        $c coords $tkn $x $y0 $x $y1
        $c itemconfigure $tkn -fill $knob_color
    }

    proc erase { cnv id } {
        set ta [tag_all $id]
        ::nui::item_erase $cnv $ta
    }

    proc move  { cnv id x y } {
        set c [::nui::widget_canvas $cnv $id]
        set tb [tag_box $id]
        set tkn [tag_knob $id]


        lassign [$c coords $tb] x0 y0 x1 y1
        set w [expr $x1-$x0]
        set h [expr $y1-$y0]

        $c coords $tb $x $y [expr $x+$w] [expr $y+$h]

        set dx [expr $x0-$x]
        set dy [expr $y0-$y]
        lassign [$c coords $tkn] x0 y0 x1 y1
        $c coords $tkn [expr $x+$dx] [expr $y+$dy]
    }
}

}
