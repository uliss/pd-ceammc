namespace eval ::nui {
namespace eval slider {
    proc tag_all  { id } { return "#sl${id}" }
    proc tag_box  { id } { return "#sl${id}_box" }
    proc tag_fill { id } { return "#sl${id}_fill" }
    proc tag_knob { id } { return "#sl${id}_kn" }
    proc tag_min { id } { return "#sl${id}_min" }
    proc tag_max { id } { return "#sl${id}_max" }
    proc tag_value { id } { return "#sl${id}_val" }

    proc minmax_font { zoom } { return "Helvetica [expr $zoom*8]" }

    proc knob_xpos { x w pos zoom } {
        # keep in sync with knob width!
        if { $zoom > 1 } {
            return [expr (($w-$zoom)*$pos)+$x+($zoom/2)]
        } {
            return [expr (($w-1)*$pos)+$x+1]
        }
    }

    proc hcreate { cnv model id x y w h zoom pos value min max out_color fill_color knob_color } {
        set c [::nui::widget_canvas $cnv $model]
        set ta [tag_all $id]
        set tb [tag_box $id]
        set tf [tag_fill $id]
        set tkn [tag_knob $id]
        set tmin [tag_min $id]
        set tmax [tag_max $id]
        set tval [tag_value $id]

        # draw fill
        $c create rectangle $x $y [expr $x+$w] [expr $y+$h] \
            -fill $fill_color -outline {} -width $zoom -tags [list $ta $tf]

        # draw min/max
        $c create text [expr $x+2] [expr $y+($h*0.5)] -text "$min" \
            -fill $out_color -font [minmax_font $zoom] -justify left\
            -anchor w -tags [list $ta $tmin]

        $c create text [expr $x+$w-2] [expr $y+($h*0.5)] -text "$max" \
            -fill $out_color -font [minmax_font $zoom] -justify right \
            -anchor e -tags [list $ta $tmax]

        # draw value
        $c create text [expr $x+($w*0.5)] [expr $y+($h*0.5)] -text "$value" \
            -fill $out_color -font [minmax_font $zoom] -justify center \
            -anchor center -tags [list $ta $tval]

        # draw knob
        set kx [knob_xpos $x $w $pos $zoom]
        set ky0 [expr $y+1]
        set ky1 [expr $y+$h-0.5]
        $c create line $kx $ky0 $kx $ky1 \
            -fill $knob_color -width [expr 2*$zoom] -tags [list $ta $tkn]

        # draw box
        $c create rectangle $x $y [expr $x+$w] [expr $y+$h] \
            -fill {} -outline $out_color -width $zoom -tags [list $ta $tb]
    }

    proc hupdate { cnv model id zoom pos value min max out_color fill_color knob_color } {
        set c [::nui::widget_canvas $cnv $model]
        set tb [tag_box $id]
        set tf [tag_fill $id]
        set tmin [tag_min $id]
        set tmax [tag_max $id]
        set tval [tag_value $id]
        set tkn [tag_knob $id]

        # update box
        $c itemconfigure $tb -outline $out_color
        $c itemconfigure $tf -fill $fill_color

        # update labels
        $c itemconfigure $tmin -fill $out_color -text "$min"
        $c itemconfigure $tmax -fill $out_color -text "$max"
        # update value
        $c itemconfigure $tval -fill $out_color -text "$value"

        lassign [$c coords $tb] x0 y0 x1 y1
        set w [expr $x1-$x0]
        set x [knob_xpos $x0 $w $pos $zoom]

        # update knob
        lassign [$c coords $tkn] x0 y0 x1 y1
        $c coords $tkn $x $y0 $x $y1
        $c itemconfigure $tkn -fill $knob_color
    }

    proc erase { cnv model id } {
        set ta [tag_all $id]
        ::nui::item_erase $cnv $model $ta
    }

    proc move  { cnv model id x y } {
        set c [::nui::widget_canvas $cnv $model]
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
