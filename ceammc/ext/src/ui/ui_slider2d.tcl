namespace eval ::ui {

namespace eval slider2d {
    variable lbl_font_size 7
    variable lbl_font_family "Helvetica"
}

proc slider2d_label_font {zoom} {
    return "$sliders::lbl_font_family [expr $sliders::lbl_font_size*$zoom] normal roman"
}

proc slider2d_draw_grid {cnv id w h zoom color_bg} {
    set c [::ceammc::ui::widget_canvas $cnv $id]
    set t [::ceammc::ui::widget_tag $id]

    set grid_xstep [expr $w / 10.0]
    set grid_ystep [expr $h / 10.0]

    # draw center crossing
    set lx [expr round(5*$grid_xstep)]
    set ly [expr round(5*$grid_ystep)]
    set xcolor [::tk::Darken $color_bg 150]
    $c create line 0 $ly $w $ly -width 3 -fill $xcolor -tag $t
    $c create line $lx 0 $lx $h -width 3 -fill $xcolor -tag $t

    set grid_color [::tk::Darken $color_bg 80]
    # draw lines
    for { set i 1 } { $i < 10 } { incr i } {
        set x [expr round($i * $grid_xstep)]
        set y [expr round($i * $grid_ystep)]

        $c create line 0 $y $w $y -width 1 -fill $grid_color -tag $t
        $c create line $x 0 $x $h -width 1 -fill $grid_color -tag $t
    }
}

proc slider2d_draw_range {cnv id w h zoom show_grid color_txt color_bg xl xr yt yb vx vy} {
    set c [::ceammc::ui::widget_canvas $cnv $id]
    set t [::ceammc::ui::widget_tag $id]
    set ft [slider2d_label_font $zoom]

    set padx [expr 2*$zoom]
    set pady [expr 3*$zoom]

    if { $show_grid == 1 } {
        set xc [expr $w/2+$padx]
        set yc [expr $h/2]
        $c create text $xc $pady -text $yt -anchor nw -justify left -font $ft -fill $color_txt -tag $t
        $c create text $xc $h    -text $yb -anchor sw -justify left -font $ft -fill $color_txt -tag $t
        $c create text $padx $yc -text $xl -anchor sw -justify left -font $ft -fill $color_txt -tag $t
        $c create text [expr $w-$padx] $yc -text $xr -anchor se -justify right -font $ft -fill $color_txt -tag $t
    } else {
        set xc [expr $w/2]
        set yc [expr $h/2]
        $c create text $xc $pady -text $yt -anchor n -justify center -font $ft -fill $color_txt -tag $t
        $c create text $xc $h    -text $yb -anchor s -justify center -font $ft -fill $color_txt -tag $t
        $c create text $padx $yc -text $xl -anchor w -justify left -font $ft -fill $color_txt -tag $t
        $c create text [expr $w-$padx] $yc -text $xr -anchor e -justify right -font $ft -fill $color_txt -tag $t
    }

    set tid [$c create text [expr $w-$padx] $pady -text "$vx $vy" -justify right -anchor ne -font $ft -fill $color_txt -tag $t]

    # calc bbox for bg rectangle
    lassign [$c bbox $tid] tx0 ty0 tx1 ty1
    set rx0 [expr $tx0-$padx]
    set rx1 [expr $tx1+$padx]
    set ry0 -1
    set ry1 [expr $ty1+$padx-1]

    # draw label bg rectangle
    set rid [$c create rectangle $rx0 $ry0 $rx1 $ry1 \
        -outline $color_bg -fill $color_bg -width 1 -tags $t]
    $c raise $tid $rid
}

proc slider2d_draw_knob {cnv id x y vx vy w h zoom show_grid show_range active} {
    set c [::ceammc::ui::widget_canvas $cnv $id]
    set t [::ceammc::ui::widget_tag $id]

    # draw guide lines
    if {$show_grid == 0} {
        set gcolor "#00C0F0"
        $c create line $x 0 $x $h -width 1 -fill $gcolor -tag $t
        $c create line 0 $y $w $y -width 1 -fill $gcolor -tag $t
    }

    # knob
    variable sz
    variable color_bd
    variable color_bg

    if { $h < $w } { set sz [expr $h/20.0] } { set sz [expr $w/20.0] }
    if { $sz < 2 } { set sz 2 }
    if { $sz > 10 } { set sz 10 }


    if { $active == 1 } {
        set color_bg "#003070"
        set color_bd "#00C0FF"
    } else {
        set color_bg "#C0C0C0"
        set color_bd "#707070"
    }

    set knid [$c create rect [expr $x-$sz] [expr $y-$sz] [expr $x+$sz] [expr $y+$sz] \
        -width 1 -outline $color_bd -fill $color_bg -tag $t]

    if {$show_range == 0 } { ceammc_cnv_tooltip $c $knid "$vx $vy" }
}

proc slider2d_update {cnv id x y vx vy w h zoom show_grid show_range mouse_down color_bg color_bd xl xr yt yb} {
    set c [::ceammc::ui::widget_canvas $cnv $id]
    set t [::ceammc::ui::widget_tag $id]
    $c delete $t

    if { $show_grid == 1 } { slider2d_draw_grid $cnv $id $w $h $zoom $color_bg }
    if { $show_range == 1 } { slider2d_draw_range $cnv $id $w $h $zoom $show_grid $color_bd $color_bg $xl $xr $yt $yb $vx $vy }

    slider2d_draw_knob $cnv $id $x $y $vx $vy $w $h $zoom $show_grid $show_range $mouse_down
}

proc slider2d_draw_labels {cnv id w h zoom color lmin lmax} {
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

}
