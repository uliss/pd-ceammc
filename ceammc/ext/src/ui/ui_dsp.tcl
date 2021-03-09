namespace eval ui {

proc dsp_update { cnv id w h zoom color } {
    set c [::ceammc::ui::widget_canvas $cnv $id]
    set t [::ceammc::ui::widget_tag $id]
    $c delete $t

    # draw speaker
    set x0 [expr $w * 0.25]
    set y0 [expr $h / 3]
    set x1 [expr $w * 0.5]
    set y1 [expr $h - $y0]
    set x2 [expr $w * 0.65]
    set x3 $x2
    set y2 [expr $y0/2]
    set y3 [expr $h-$y2]

    set bcolor [::tk::Darken $color 80]
    $c create polygon $x0 $y0 $x1 $y0 $x2 $y2 $x3 $y3 $x1 $y1 $x0 $y1 -fill $color -outline $bcolor \
        -width $zoom -tags $t

    # draw air
    set x0 [expr $w * 0.75]
    set y0 [expr $h * 0.25]
    set y1 [expr $h - $y0]
    set wd [expr 2*$zoom]
    $c create line $x0 $y0 $x0 $y1 -fill $bcolor -capstyle round -width $wd -tags $t

    set x0 [expr $w * 0.85]
    set y0 [expr $h * 0.35]
    set y1 [expr $h - $y0]
    $c create line $x0 $y0 $x0 $y1 -fill $bcolor -capstyle round -width $wd -tags $t
}

}
