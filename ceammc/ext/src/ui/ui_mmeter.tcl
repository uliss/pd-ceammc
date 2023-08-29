namespace eval ui {
namespace eval mm {

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

proc calc_coord { n value size step } {
    variable LED_PAD
    set y0 [expr $size-($n*$step)+$LED_PAD]
    set y1 [expr $y0-((min($value,$n+1)-$n)*$step)]
    return [list $y0 $y1]
}

proc create_vrms { id cnv w h cold tepid warm hot over nch data } {
    variable FS
    variable LED_NUM
    variable LED_DB
    variable LED_PAD

    set t        [::ceammc::ui::widget_tag $id]
    set area_ht  [expr $h-(2*$LED_PAD)]
    set led_wd   [expr ($w-(2*$nch*$LED_PAD))/$nch]
    set led_grid [expr $led_wd+(2*$LED_PAD)]
    set led_ht   [expr $area_ht/$FS*9.0]

    for {set ch 0} {$ch < $nch} {incr ch} {
        set rms [lindex $data [expr $ch*3]]
        if {$rms <= -$FS} { continue }

        set x0 [expr round(($led_grid*$ch)+$LED_PAD)]
        set x1 [expr round($x0 + $led_wd)]
        set led_db [expr ($FS+$rms)/9.0]

        if { $led_db > 0 } {
            lassign [calc_coord 0 $led_db $area_ht $led_ht] y0 y1
            $cnv create rectangle $x0 $y0 $x1 $y1 -fill $cold -outline $cold -width 1 -tags $t
        }

        if { $led_db > 1 } {
            lassign [calc_coord 1 $led_db $area_ht $led_ht] y0 y1
            $cnv create rectangle $x0 $y0 $x1 $y1 -fill $tepid -outline $tepid -width 1 -tags $t
        }

        if { $led_db > 2 } {
            lassign [calc_coord 2 $led_db $area_ht $led_ht] y0 y1
            $cnv create rectangle $x0 $y0 $x1 $y1 -fill $warm -outline $warm -width 1 -tags $t
        }

        if { $led_db > 3 } {
            lassign [calc_coord 3 $led_db $area_ht $led_ht] y0 y1
            $cnv create rectangle $x0 $y0 $x1 $y1 -fill $hot -outline $hot -width 1 -tags $t
        }

        if { $led_db > 4 } {
            lassign [calc_coord 4 $led_db $area_ht $led_ht] y0 y1
            $cnv create rectangle $x0 $y0 $x1 $y1 -fill $over -outline $over -width 1 -tags $t
        }
    }
}

proc create_vpeak { id cnv w h cold tepid warm hot over nch data } {
    variable FS
    variable PEAK_PAD
    variable LED_PAD

    set t [::ceammc::ui::widget_tag $id]
    set peak_wd   [expr ($w-(2*$nch*$PEAK_PAD))/$nch]
    set peak_grid [expr $peak_wd+(2*$PEAK_PAD)]
    set area_ht   [expr $h-(2*$LED_PAD)]

    for {set ch 0} {$ch < $nch} {incr ch} {
        set peak [lindex $data [expr $ch*3+1]]
        if {$peak < -$FS} { continue }

        set c  [mmeter_db2color $peak $cold $tepid $warm $hot $over]
        set x0 [expr round(($peak_grid*$ch)+$PEAK_PAD)]
        set x1 [expr round($x0 + $peak_wd)]
        set y0 [expr ($area_ht * (1 - (($peak+$FS)/$FS)))+$LED_PAD]
        if { $y0 < 0 } { set y0 0 }
        set y1 $y0

        $cnv create line $x0 $y0 $x1 $y1 -fill $c -width 2 -tags $t
    }
}

proc create_vover { id cnv w h over nch data } {
    variable LED_PAD

    set t [::ceammc::ui::widget_tag $id]
    set led_wd   [expr ($w-(2*$nch*$LED_PAD))/$nch]
    set led_grid [expr $led_wd+(2*$LED_PAD)]

    for {set ch 0} {$ch < $nch} {incr ch} {
        set ovld [lindex $data [expr $ch*3+2]]
        if {$ovld <= 0} { continue }

        set x0 [expr round(($led_grid*$ch)+$LED_PAD)]
        set x1 [expr round($x0 + $led_wd)]
        set y0 $LED_PAD
        set y1 [expr 2*$LED_PAD]

        $cnv create rectangle $x0 $y0 $x1 $y1 -fill $over -outline $over -width 0 -tags $t
    }
}

proc create_hrms { id cnv w h cold tepid warm hot over nch args } {
    variable FS
    # optimization for for silence
    if {$rms < -$FS} { return }

    set t [::ceammc::ui::widget_tag $id]
    set led_wstep [expr ($w+1) / 13.0]
    set led_dbstep 3
    set led_pad 3

    for {set i 0} {$i < 13} {incr i} {
        # left to right -49..0
        set led_db [expr -$led_dbstep * (13 - $i)]
        set rms [lindex $args [expr $i*3]]
        if {$rms < -49} { continue }
        if {$rms > $led_db} {
            set delta [expr $rms - $led_db]
            # last led
            set last [expr $delta < $led_dbstep]
            set c [meter_db2color $led_db $cold $tepid $warm $hot $over]
            set led_ratio [expr $last ? ($delta / $led_dbstep) : 1]
            # horizontal
            set x0 [expr {round($i*$led_wstep)} + 1]
            set x1 [expr {round(($i+$led_ratio)*$led_wstep)}]
            set y0 $led_pad
            set y1 [expr $h-$led_pad]

            $cnv create rectangle $x0 $y0 $x1 $y1 -fill $c -outline $c -width 1 -tags $t
        }
    }
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

proc create_crms { id cnv w h cold tepid warm hot over nch data } {
    variable FS
    variable LED_PAD
    variable CHAN_CIRCLE
    variable FONT

    set t        [::ceammc::ui::widget_tag $id]
    set area_ht  [expr $w-(4*$LED_PAD)]
    set led_len  [expr $area_ht/5.0]
    set led_ht   [expr $area_ht/2/$FS*9.0]

    set x0 $LED_PAD
    set y0 $LED_PAD
    set x1 [expr $w-2*$LED_PAD]
    set y1 [expr $h-2*$LED_PAD]

    # draw charts
    for {set ch 0} {$ch < $nch} {incr ch} {
        set rms [lindex $data [expr $ch*3]]
        if {$rms <= -$FS} { continue }

        set a0 [expr 360.0/$nch*($ch)+90]
        set a1 [expr 360.0/$nch]

        set led_db [expr ($FS+$rms)/9.0]
        draw_pie $cnv $w $h $t 4 $led_db $led_len $a0 $a1 $over
        draw_pie $cnv $w $h $t 3 $led_db $led_len $a0 $a1 $hot
        draw_pie $cnv $w $h $t 2 $led_db $led_len $a0 $a1 $warm
        draw_pie $cnv $w $h $t 1 $led_db $led_len $a0 $a1 $tepid
        draw_pie $cnv $w $h $t 0 $led_db $led_len $a0 $a1 $cold
    }

    # draw lines
    for {set i 1} {$i <= 5} {incr i} {
        set ww [expr $led_len*$i]
        set x0 [expr ($w-$ww)/2]
        set y0 [expr ($h-$ww)/2]
        set x1 [expr $x0 + $ww]
        set y1 [expr $y0 + $ww]
        $cnv create oval $x0 $y0 $x1 $y1 -outline gray -width 1 -tags $t
    }

    set pi2 [expr 3.1415926*2]
    set half_pi [expr 3.1415926/2]

    # draw texts
    for {set ch 0} {$ch < $nch} {incr ch} {
        set x0 [expr $w/2]
        set y0 [expr $h/2]
        set x1 [expr $x0+$area_ht/2*cos($pi2*$ch/$nch - $half_pi)]
        set y1 [expr $y0+$area_ht/2*sin($pi2*$ch/$nch - $half_pi)]
        $cnv create line $x0 $y0 $x1 $y1 -fill gray -width 3 -tags $t
        $cnv create oval [expr $x1-$CHAN_CIRCLE] [expr $y1-$CHAN_CIRCLE] [expr $x1+$CHAN_CIRCLE] [expr $y1+$CHAN_CIRCLE] -fill darkgray -width 0 -tags $t
        $cnv create text $x1 $y1 -text [expr ($nch-$ch)%$nch+1] -width 20 -justify center -anchor center -fill white -font $FONT -tags $t
    }
}

proc delete {id cnv} {
    set t [::ceammc::ui::widget_tag $id]
    $cnv delete $t
}

proc create_h {id cnv w h bdcolor cold tepid warm hot over nch data} {
    create_hrms $id $cnv $w $h $cold $tepid $warm $hot $over $nch $data
}

proc create_v {id cnv w h bdcolor cold tepid warm hot over nch data} {
    create_vrms $id $cnv $w $h $cold $tepid $warm $hot $over $nch $data
    create_vpeak $id $cnv $w $h $cold $tepid $warm $hot $over $nch $data
    create_vover $id $cnv $w $h $over $nch $data
}

proc create_c {id cnv w h bdcolor cold tepid warm hot over nch data} {
    create_crms  $id $cnv $w $h $cold $tepid $warm $hot $over $nch $data
#    create_cpeak $id $cnv $w $h $cold $tepid $warm $hot $over $nch $data
}

proc create {id cnv w h bdcolor cold tepid warm hot over mode nch args} {
    switch $mode {
        h { create_h $id $cnv $w $h $bdcolor $cold $tepid $warm $hot $over $nch $args }
        v { create_v $id $cnv $w $h $bdcolor $cold $tepid $warm $hot $over $nch $args }
        c { create_c $id $cnv $w $h $bdcolor $cold $tepid $warm $hot $over $nch $args }
    }
}

}
}

if {[info exists ::env(TEST_PD_TCL)]} {
namespace eval ceammc {
namespace eval ui {
proc widget_tag {id} { return "tag_$id" }
}
}

    wm title . "ui.mm~ test"
    wm geometry . 400x400
    set c [tk::canvas .canvas -width 500 -height 400 -background white]
    pack $c
    $c create rectangle 0 0 200 200 -outline blue -width 1
    ui::mm::create id $c 200 200 grey green yellow orange red black c 5 -3 -3 1 -12 -12 0 -22 -22 0 -33 -33 0 0 0 0
}
