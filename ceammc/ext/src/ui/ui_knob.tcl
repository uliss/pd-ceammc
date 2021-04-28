namespace eval ::ui {
namespace eval knob {

namespace eval font {
    variable size 7
    variable family "Helvetica"

    switch -- $::windowingsystem {
        "aqua" { }
        "x11"  { }
        "win32" {
            set size 6
        }
    }
}

proc label_font {zoom} {
    return "$font::family [expr $font::size*$zoom] normal roman"
}

proc update {cnv id w h zoom vmin vmax value show_range show_value active_scale border_color scale_color knob_color txt_color} {
    set c [::ceammc::ui::widget_canvas $cnv $id]
    set t [::ceammc::ui::widget_tag $id]
    $c delete $t

    set wd [expr round($h / 20) + 1]

    switch -- $::windowingsystem {
        "win32" { set wd [expr $wd*0.5] }
    }

    # do not show range on small sizes
    if {$show_range && $w < 30} { set show_range 0 }

    if {$show_range} {
        set x0 [expr 2*$zoom]
        set y0 [expr $h - 1]

        $c create text $x0 $y0 -text [format "%g" $vmin] -anchor sw -justify left \
            -font [label_font $zoom] -fill $txt_color -width 0 -tags $t

        $c create text [expr $w-$x0] $y0 -text [format "%g" $vmax] -anchor se -justify right \
            -font [label_font $zoom] -fill $txt_color -width 0 -tags $t

        set yoff 0
    } else {
        if { $h < 30 } {
            set yoff [expr round($wd*0.5)]
        } else {
            set yoff [expr round($wd/3.0)]
        }
    }

    set k 0.1
    set x0 [expr round($k*$w)+$zoom]
    set y0 [expr round($k*$h)+$zoom + $yoff]
    set x1 [expr round($w-($k*$w))]
    set y1 [expr round($h-($k*$h)) + $yoff]

    set phase_norm [expr ($vmax-$value)/($vmax-$vmin)]

    if {$active_scale} {
        set a -45
        set b [expr 270 * $phase_norm]

        $c create arc $x0 $y0 $x1 $y1 -start $a -extent $b -width $wd \
            -outline $scale_color -style arc -tags $t

        set a [expr $a+$b]
        set b [expr 270 * (1-$phase_norm)]
        $c create arc $x0 $y0 $x1 $y1 -start $a -extent $b -width $wd \
            -outline $knob_color -style arc -tags $t

    } else {
        $c create arc $x0 $y0 $x1 $y1 -start -45 -extent 270 -width $wd \
            -outline $scale_color -style arc -tags $t
    }

    set phase_rad [expr ($phase_norm*1.5+0.25) * 3.1415926]
    set x0 [expr ($x1-$x0)*0.5+$x0]
    set y0 [expr ($y1-$y0)*0.5+$y0]

    set r  [expr round(0.5*($w*(1-$k) - ($wd*2)))]
    set x1 [expr sin($phase_rad)*$r+$x0]
    set y1 [expr cos($phase_rad)*$r+$y0]
    $c create line $x0 $y0 $x1 $y1 -width $wd -fill $knob_color \
        -capstyle round -tags $t

    if {$show_value} {
        set y0 [expr round(0.707*$r + $y0 + (0.5*$wd))]
        $c create text $x0 $y0 -text [format "%g" $value] -anchor s -justify center \
            -font [label_font $zoom] -fill $txt_color -width 0 -tags $t
    }
}

}
}
