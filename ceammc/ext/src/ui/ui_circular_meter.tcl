namespace eval ui {
namespace eval cm {

# full scale is 50db
variable FS 39.0
variable LED_PAD 3.0
variable PEAK_PAD 1.0
variable CHAN_CIRCLE 8.0
variable FONT

switch -- [tk::windowingsystem] {
    "aqua"  { set FONT {Monaco 8} }
    "win32" { set FONT {DejaVuSansMono 8} }
    default { set FONT {Monaco 8} }
}

proc mmeter_db2color { db cold tepid warm hot over } {
    if {$db < -30} { return $cold  }
    if {$db < -21} { return $tepid }
    if {$db < -12} { return $warm  }
    if {$db < -3 } { return $hot   } { return $over }
}

proc draw_pie {cnv w h tag n db led_size a0 a1 color} {
    if { $db > $n } {
        set pie [expr $led_size*min($n+1, $db)]
        set x0 [expr ($w-$pie)/2]
        set y0 [expr ($h-$pie)/2]
        set x1 [expr $x0 + $pie]
        set y1 [expr $y0 + $pie]
        $cnv create arc $x0 $y0 $x1 $y1 -fill $color -outline $color -width 0 -start $a0 -extent $a1 -tags $tag
    }
}

# offset - channel offset
# cw - clockwise
proc create_crms { id cnv w h cold tepid warm hot over nch offset cw angles data } {
    variable FS
    variable LED_PAD
    variable CHAN_CIRCLE
    variable FONT

    set pi2     6.283185307179586
    set half_pi 1.5707963267948966
    set rad     0.017453292519943295

    set t        [::ceammc::ui::widget_tag $id]
    set area_ht  [expr $w-(4*$LED_PAD)]
    set led_len  [expr $area_ht/5.0]

    set x0 $LED_PAD
    set y0 $LED_PAD
    set x1 [expr $w-2*$LED_PAD]
    set y1 [expr $h-2*$LED_PAD]
    set radius [expr $area_ht/2]
    set seg_off_deg [expr abs(([lindex $angles 1]-[lindex $angles 0])/2)]
    set seg_off_rad [expr $seg_off_deg*$rad]

    # box center x/y
    set cx [expr $w/2]
    set cy [expr $h/2]

    # draw charts
    for {set ch 0} {$ch < $nch} {incr ch} {
        # RMS
        set rms [lindex $data [expr $ch*3]]
        if {$rms <= -$FS} { continue }

        set idx   [expr ($ch-$offset)%$nch]
        set angle [expr [lindex $angles $idx]]
        if {[expr $idx+1]<$nch} { set a1 [lindex $angles [expr $idx+1]] } { set a1 360 }
        set segm [expr $a1-$angle]

        set led_db [expr ($FS+$rms)/9.0]
        set a [expr $angle-$seg_off_deg+90]
        draw_pie $cnv $w $h $t 4 $led_db $led_len $a $segm $over
        draw_pie $cnv $w $h $t 3 $led_db $led_len $a $segm $hot
        draw_pie $cnv $w $h $t 2 $led_db $led_len $a $segm $warm
        draw_pie $cnv $w $h $t 1 $led_db $led_len $a $segm $tepid
        draw_pie $cnv $w $h $t 0 $led_db $led_len $a $segm $cold

        # peak
        set peak [lindex $data [expr $ch*3+1]]
        if {$peak < -$FS} { continue }

        set c  [mmeter_db2color $peak $cold $tepid $warm $hot $over]
        set led_db [expr ($FS+$peak)/9.0]
        set pie [expr $led_len*$led_db]
        set x0 [expr ($w-$pie)/2]
        set y0 [expr ($h-$pie)/2]
        set x1 [expr $x0 + $pie]
        set y1 [expr $y0 + $pie]
        $cnv create arc $x0 $y0 $x1 $y1 -style arc -outline $c -width 3 -start $a -extent $segm -tags $t
    }

    # draw circles
    for {set i 1} {$i <= 5} {incr i} {
        set led [expr $led_len*$i]
        set x0  [expr ($w-$led)/2]
        set y0  [expr ($h-$led)/2]
        set x1  [expr $x0 + $led]
        set y1  [expr $y0 + $led]
        $cnv create oval $x0 $y0 $x1 $y1 -outline gray -width 1 -tags $t
    }

    # draw texts
    for {set ch 0} {$ch < $nch} {incr ch} {    
        set idx [expr ($ch-$offset)%$nch]
        set angle [expr [lindex $angles $idx]*$rad]
        if {[expr $idx+1]<$nch} { set a1 [expr [lindex $angles [expr $idx+1]]*$rad] } { set a1 $pi2 }
        set segm [expr $a1-$angle]
        set a [expr $angle-$seg_off_rad+$half_pi]

        set x1 [expr $cx+$radius*cos($a)]
        set y1 [expr $cy-$radius*sin($a)]
        # draw radials beams
        $cnv create line $cx $cy $x1 $y1 -fill gray -width 3 -tags $t

        set x1 [expr $cx+$radius*cos($a+$segm/2)]
        set y1 [expr $cy-$radius*sin($a+$segm/2)]
        # text circle
        $cnv create oval [expr $x1-$CHAN_CIRCLE] [expr $y1-$CHAN_CIRCLE] [expr $x1+$CHAN_CIRCLE] [expr $y1+$CHAN_CIRCLE] \
            -fill darkgray -width 0 -tags $t

        if {$cw} { set txt [expr ($nch-$ch)%$nch+1] } { set txt [expr ($ch)%$nch+1] }
        $cnv create text $x1 $y1 -text $txt -width 20 -justify center -anchor center -fill white -font $FONT -tags $t
    }
}

proc delete {id cnv} {
    set t [::ceammc::ui::widget_tag $id]
    $cnv delete $t
}

# id
# cnv
# w - width in pixels
# h - height in pixels
# bdcolor - background color (#hex)
# nch - number of channels (int)
# offset - channel offset (int)
# cw - clockwise or counter clockwise
# args - data
proc create {id cnv w h bdcolor cold tepid warm hot over nch offset cw angles data} {
    create_crms  $id $cnv $w $h $cold $tepid $warm $hot $over $nch $offset $cw $angles $data
}

}
}

if {[info exists ::env(TEST_PD_TCL)]} {
namespace eval ceammc {
namespace eval ui {
proc widget_tag {id} { return "tag_$id" }
}
}

    wm title . "ui.cm~ test"
    wm geometry . 840x450
    set c0 [tk::canvas .canvas0 -width 200 -height 200 -background white]
    ui::cm::create id $c0 200 200 grey green yellow orange red black 5 0 0 [list 0 72 144 180 270] [list -3 0 0 -12 0 0 -21 0 0 -31 0 0 0 0 0]
    set c1 [tk::canvas .canvas1 -width 200 -height 200 -background white]
    ui::cm::create id $c1 200 200 grey green yellow orange red black 5 -1 0 [list 0 72 144 180 270] [list -3 0 0 -12 0 0 -21 0 0 -31 0 0 0 0 0]

    set c2 [tk::canvas .canvas2 -width 200 -height 200 -background white]
    ui::cm::create id $c2 200 200 grey green yellow orange red black 5 0 1 [list 0 72 144 180 270] [list -3 1 0 -12 -10 0 -21 -15 0 -31 -25 0 0 3 0]
    set c3 [tk::canvas .canvas3 -width 200 -height 200 -background white]
    ui::cm::create id $c3 200 200 grey green yellow orange red black 5 1 1 [list 0 72 144 180 270] [list -3 1 0 -12 -10 0 -21 -15 0 -31 -25 0 0 3 0]

    pack $c0 $c1 -side left
    pack $c2 $c3 -side left
}
