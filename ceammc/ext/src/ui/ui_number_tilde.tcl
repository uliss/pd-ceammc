namespace eval ui {

namespace eval number_tilde {
    variable font_size 11
    variable font_family "Helvetica"
}

switch -- $::windowingsystem {
    "aqua" { }
    "x11"  { }
    "win32" {
        set number_tilde::font_size 9
    }
}

proc number_font { zoom } {
    return "$number_tilde::font_family [expr $zoom * $number_tilde::font_size] normal roman"
}

proc number_draw_box {c tag w h zoom bdcolor acolor tcolor value} {
    set x [expr $h * 0.7]
    set ft [number_font $zoom]
    $c create line $x 0 $x $h -width 1 -fill $bdcolor -tags $tag

    set tx [expr $x * 0.5]
    set ty [expr $h * 0.5]
    $c create text $tx $ty -text "~" -anchor center -justify center \
        -font $ft -fill $acolor -width 0 -tags $tag

    set tx [expr $h * 0.9]
    set ty [expr $h * 0.55]

    $c create text $tx $ty -text $value -anchor w -justify left \
        -font $ft -fill $tcolor -width 0 -tags $tag
}

proc number_update {cnv id w h zoom bdcolor acolor tcolor value} {
    set c [::ceammc::ui::widget_canvas $cnv $id]
    set t [::ceammc::ui::widget_tag $id]
    $c delete $t
    number_draw_box $c $t $w $h $zoom $bdcolor $acolor $tcolor $value
}
}
