namespace eval ui {

proc meter_db2color { db cold tepid warm hot over } {
    if {$db < -30} { return $cold  }
    if {$db < -21} { return $tepid }
    if {$db < -12} { return $warm  }
    if {$db < -3}  { return $hot   } { return $over }
}

proc meter_create_hrms { id cnv w h rms cold tepid warm hot over } {
    # optimization for for silence
    if {$rms < -49} { return }

    set t [::ceammc::ui::widget_tag $id]
    set led_wstep [expr ($w+1) / 13.0]
    set led_dbstep 3
    set led_pad 3

    for {set i 0} {$i < 13} {incr i} {
        # left to right -49..0
        set led_db [expr -$led_dbstep * (13 - $i)]
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

proc meter_create_vrms { id cnv w h rms cold tepid warm hot over } {
    # optimization for for silence
    if {$rms < -49} { return }

    set t [::ceammc::ui::widget_tag $id]
    set led_hstep [expr ($h+1) / 13.0]
    set led_dbstep 3
    set led_pad 3

    for {set i 0} {$i < 13} {incr i} {
        # left to right -49..0
        set led_db [expr -$led_dbstep * (13 - $i)]
        if {$rms > $led_db} {
            set delta [expr $rms - $led_db]
            # last led
            set last [expr $delta < $led_dbstep]
            set c [meter_db2color $led_db $cold $tepid $warm $hot $over]
            set led_ratio [expr $last ? ($delta / $led_dbstep) : 1]
            # vertical
            set x0 $led_pad
            set x1 [expr $w-$led_pad]
            set y0 [expr $h - {round($i*$led_hstep)} + 1]
            set y1 [expr $h - {round(($i+$led_ratio)*$led_hstep)}]

            $cnv create rectangle $x0 $y0 $x1 $y1 -fill $c -outline $c -width 1 -tags $t
        }
    }
}

proc meter_create_hpeak { id cnv w h peak cold tepid warm hot over } {
    # optimization for for silence
    if {$peak < -49} { return }

    set t [::ceammc::ui::widget_tag $id]
    set c [meter_db2color $peak $cold $tepid $warm $hot $over]

    set x0 [expr (($peak+49)/49) * $w]
    set y0 -1
    set y1 [expr $h + 1]
    if { $x0 > $w } { set x0 $w }
    set x1 $x0

    $cnv create line $x0 $y0 $x1 $y1 -fill $c -width 2 -tags $t
}

proc meter_create_vpeak { id cnv w h peak cold tepid warm hot over } {
    # optimization for for silence
    if {$peak < -49} { return }

    set t [::ceammc::ui::widget_tag $id]
    set c [meter_db2color $peak $cold $tepid $warm $hot $over]

    set x0 -1
    set x1 [expr $w + 1]
    set y0 [expr $h * (1 - (($peak+49)/49))  ]
    if { $y0 < 0 } { set y0 0 }
    set y1 $y0

    $cnv create line $x0 $y0 $x1 $y1 -fill $c -width 2 -tags $t
}

proc meter_create_hoverload { id cnv w h over } {
    set t [::ceammc::ui::widget_tag $id]
    set led_pad 3

    set step [expr ($w+1) / 13.0]
    set x0 [expr round(12 * $step) + 1]
    set x1 [expr round(13 * $step)]
    set y0 $led_pad
    set y1 [expr ($h-$led_pad)+2]

    $cnv create rectangle $x0 $y0 $x1 $y1 -fill $over -width 0 -tags $t
}

proc meter_create_voverload { id cnv w h over } {
    set t [::ceammc::ui::widget_tag $id]
    set led_pad 3

    set step [expr ($h+1) / 13.0]
    set x0 $led_pad
    set x1 [expr ($w-$led_pad)+2]
    set y0 1
    set y1 $step

    $cnv create rectangle $x0 $y0 $x1 $y1 -fill $over -width 0 -tags $t
}

proc meter_create_hlines { id cnv w h bdcolor } {
    set t [::ceammc::ui::widget_tag $id]
    set step [expr ($w+1) / 13.0]
    set lh [expr $h + 1]
    for {set i 1} {$i < 13} {incr i} {
        set x [expr {round($i*$step)}]
        $cnv create line $x -1 $x $lh -fill $bdcolor -tags $t
    }
}

proc meter_create_vlines { id cnv w h bdcolor } {
    set t [::ceammc::ui::widget_tag $id]
    set step [expr ($h+1) / 13.0]
    set lw [expr $w + 1]
    for {set i 1} {$i < 13} {incr i} {
        set y [expr {round($i*$step)}]
        $cnv create line -1 $y $lw $y -fill $bdcolor -tags $t
    }
}

proc meter_delete {id cnv} {
    set t [::ceammc::ui::widget_tag $id]
    $cnv delete $t
}

proc meter_create {id cnv w h bdcolor cold tepid warm hot over rms peak overload} {
    if {$w > $h} {
        meter_create_hlines $id $cnv $w $h $bdcolor
        meter_create_hrms $id $cnv $w $h $rms $cold $tepid $warm $hot $over
        meter_create_hpeak $id $cnv $w $h $peak $cold $tepid $warm $hot $over
        if { $overload } { meter_create_hoverload $id $cnv $w $h $over }
    } else {
        meter_create_vlines $id $cnv $w $h $bdcolor
        meter_create_vrms $id $cnv $w $h $rms $cold $tepid $warm $hot $over
        meter_create_vpeak $id $cnv $w $h $peak $cold $tepid $warm $hot $over
        if { $overload } { meter_create_voverload $id $cnv $w $h $over }
    }
}

}
