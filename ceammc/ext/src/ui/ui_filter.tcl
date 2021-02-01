namespace eval ui {

namespace eval filter {
    variable db_min -24.0
    variable db_max 24.0
    variable db_range [expr $db_max - $db_min]
    variable db_labels_short  { -12 0 12 }
    variable db_labels_long   { -18 -12 -6 0 6 12 18 }
    variable freq_min 10.0
    variable freq_max 20000.0
    variable freq_nyq 22050.0
    variable freq_steps { 500 1000 2000 2500 5000 10000 }
    variable fmin_log10 [expr log10($freq_min)]
    variable fmax_log10 [expr log10($freq_max)]

    proc clip { v min max } {
        if {$v < $min} { return $min }
        if {$v > $max} { return $max }
        return $v
    }

    proc finite {x} {
        expr {[string is double -strict $x] && $x == $x && $x + 1 != $x}
    }
}

proc filter_font { zoom } { return "Helvetica [expr $zoom * 7] normal roman" }

proc filter_draw_hdb { c t w h zoom labels color } {
    set ft [filter_font $zoom]
    set lx [expr $w - (2*$zoom)]
    set t0 [lindex $labels 0]

    foreach l $labels {
        set y [expr ($h/$::ui::filter::db_range)*$l + $h*0.5]
        # accent zero level
        if { $l == 0 } { set lc [::tk::Darken $color 80] } { set lc $color }
        $c create line 0 $y $w $y -fill $lc -width 1 -tag $t

        set txt [expr -$l]
        if { $l == 0 } { set txt {0db} }
        set tt [$c create text $lx $y -text $txt -anchor se -justify right \
            -font $ft -fill $color -width 0 -tags $t]

        if { $l == $t0 } {
            set miny [expr 10*$zoom]
            lassign [$c bbox $tt] tx0 ty0 tx1 ty1
            if { $ty0 < $miny || $ty1 < $miny } { $c delete $tt }
        }
    }

    # top db value
    $c create text $lx 3 -text [expr int($::ui::filter::db_max)] -anchor ne -justify right \
        -font $ft -fill $color -width 0 -tags $t

    # bottom db value
    $c create text $lx $h -text [expr int($::ui::filter::db_min)] -anchor se -justify right \
        -font $ft -fill $color -width 0 -tags $t
}

proc complex_new {real imag} {
   return [list $real $imag]
}

proc complex_add {z1 z2} {
    set result {}
    foreach c $z1 d $z2 {
       lappend result [expr {$c+$d}]
    }
    return $result
 }

proc complex_mul {z1 z2} {
   set result {}
   foreach {c1 d1} $z1 {break}
   foreach {c2 d2} $z2 {break}

   return [list [expr {$c1*$c2-$d1*$d2}] [expr {$c1*$d2+$c2*$d1}]]
}

proc complex_div {z1 z2} {
    set result {}
    foreach {c1 d1} $z1 {break}
    foreach {c2 d2} $z2 {break}

    set denom [expr {$c2*$c2+$d2*$d2}]
    return [list [expr {($c1*$c2+$d1*$d2)/$denom}] \
                 [expr {(-$c1*$d2+$c2*$d1)/$denom}]]
}

proc complex_mod {z1} {
   foreach {c d} $z1 {break}
   return [expr {hypot($c,$d)}]
}

proc complex_exp {z1} {
   foreach {c d} $z1 {break}
   return [list [expr {exp($c)*cos($d)}] [expr {exp($c)*sin($d)}]]
}

proc filter_jw { w k } {
    set n [llength $k]
    if { $n == 0 } { return [complex_new 0.0 0.0] }
    set a [complex_new [lindex $k 0] 0.0]
    for { set j 1 } { $j < $n } { incr j } {
        set kn  [lindex $k $j]
        set jw  [complex_new 0.0 [expr -1.0*$j*$w]]
        set ejw [complex_mul [complex_new $kn 0.0] [complex_exp $jw]]
        set a   [complex_add $a $ejw]
    }

    return $a
}

proc filter_freq_amp {w b a} {
    set z [complex_new 0.0 0.0]
    set ajw [filter_jw $w $a]
    if { $ajw == $z } { return 0.0 }

    set bjw [filter_jw $w $b]

    return [complex_mod [complex_div $bjw $ajw]]
}

proc filter_x_to_omega { x w scale } {
    set f 0
    if { $scale == "lin" } {
        set k [expr $::ui::filter::freq_max/$::ui::filter::freq_nyq]
        set f [expr ($k*$x)/$w]
    } elseif { $scale == "log" } {
        set loga $filter::fmin_log10
        set logb $filter::fmax_log10
        set lrng [expr $logb-$loga]

        set px  [expr (($lrng*$x)/$w) + $loga]
        set f   [expr pow(10, $px)/$::ui::filter::freq_nyq]
    }

    expr 3.141592653589793 * $f
}

proc filter_x_to_herz { x w scale } {
    set f 0
    if { $scale == "lin" } {
        # lin range from 0 to $freq_max
        set x [filter::clip $x 0 $w]
        set f [expr $filter::freq_max * $x / $w]
    } elseif { $scale == "log" } {
        # log range from $freq_min to $freq_max
        set loga $filter::fmin_log10
        set logb $filter::fmax_log10
        set lrng [expr $logb-$loga]

        set x [filter::clip $x 0 $w]
        set px [expr (1.0*$x/$w * $lrng) + $loga]
        set f  [expr pow(10, $px)]
    }
    return $f
}

proc filter_draw_fresp { c t w h zoom color b0 b1 b2 a1 a2 scale } {
    set db_hstep [expr $h / $ui::filter::db_range]

    set pts {}

    for { set x 0 } { $x < $w } { incr x } {
        set omega [filter_x_to_omega $x $w $scale]
        set wamp  [filter_freq_amp $omega [list $b0 $b1 $b2] [list 1 $a1 $a2]]
        set dbamp [expr 20 * log10($wamp)]

        set y [expr $h - (($dbamp * $db_hstep) + ($h*0.5))]
        if { ![filter::finite $y] } { set y [expr $w+10] }

        lappend pts $x $y
    }

    $c create line $pts -fill $color -width $zoom -tags $t
}

proc filter_draw_hgrid { c t w h zoom gridcolor } {
    set min_line_space 20
    set n_long  [llength $::ui::filter::db_labels_long]
    set n_short [llength $::ui::filter::db_labels_short]
    set space_long  [expr $h/$n_long]
    set space_short [expr $h/$n_short]

    if { $space_long >= $min_line_space } {
        filter_draw_hdb $c $t $w $h $zoom $::ui::filter::db_labels_long $gridcolor
    } else {
        filter_draw_hdb $c $t $w $h $zoom $::ui::filter::db_labels_short $gridcolor
    }
}

proc filter_draw_vgrid { c t w h zoom gridcolor scale } {
    if { $scale == "lin" } {
        set min_hstep [expr 30 * $zoom]
        set fstep 5000
        set hstep [expr $fstep / $::ui::filter::freq_max * $w]
        # fine best freq step
        foreach f $::ui::filter::freq_steps {
            # hstep in pixels
            set hstep [expr ($f / $::ui::filter::freq_max) * $w]
            if { $hstep > $min_hstep } {
                set fstep $f
                break
            }
        }
        set nl [expr ($w/$hstep)]
        set ft [filter_font $zoom]
        for { set i 1 } { $i < $nl } { incr i } {
            set x [expr $i * $hstep]
            $c create line $x 0 $x $h -width 1 -fill $gridcolor -tags $t

            set f [expr $i*$fstep]
            if { $f < 1000 } { set txt "[expr int($f)]" } { set txt "[expr $f/1000.0]k" }
            set tx [expr $x-(2*$zoom+1)]
            $c create text $tx $h -text $txt -anchor se -justify right \
                -font $ft -fill $gridcolor -width 0 -tags $t

        }
    } elseif { $scale == "log" } {
        set loga [expr log10($::ui::filter::freq_min)]
        set logb [expr log10($::ui::filter::freq_max)]
        set lrng [expr $logb-$loga]
        set ft [filter_font $zoom]
        set txt_right -1
        set lcolor0 [::tk::Darken $gridcolor 80]
        set lcolor1 [::tk::Darken $gridcolor 120]

        for { set i [expr int($loga)] } { $i <= $logb } { incr i } {
            set f0 [expr pow(10, $i)]
            set draw_txt 1

            set line_prev_x -1

            for { set k 0 } { $k < 9 } { incr k } {
                set f [expr $f0*(1+$k)]
                set x [expr (log10($f)-$loga)/$lrng*$w]
                # too close lines, skipping
                set too_close [expr $x-$line_prev_x < (2*$zoom)]
                if { $too_close && $k != 0 } { continue }
                set line_prev_x $x

                # 10^x freq highlight
                if { $k == 0 } { set lc $lcolor0 } { set lc $lcolor1 }
                $c create line $x 0 $x $h -width 1 -fill $lc -tags $t

                # not enough space to freq label
                if { $x < $txt_right } { $c delete $tid }

                if { $f < 1000 } { set txt "[expr int($f)]" } { set txt "[expr int($f/1000.0)]k" }
                set tx [expr $x+(2*$zoom)]
                set tid [$c create text $tx $h -text $txt -anchor sw -justify left \
                    -font $ft -fill $gridcolor -width 0 -tags $t]

                lassign [$c bbox $tid] tx0 ty0 tx1 ty1
                set txt_right $tx1
            }
        }
    }
}

proc filter_info_txt { freq q bw type } {
    set freq [expr int($freq)]
    if { $type == "notch" || $type == "peak"} {
        return "f=${freq}Hz Q=$q bw=$bw"
    } else {
        return "f=${freq}Hz"
    }
}

proc filter_draw_handle { c t w h zoom color scale x y q bw type } {
    set cc [::tk::Darken $color 75]

    # draw Q and bandwidth label
    set pad [expr 2*$zoom]
    set tx  [expr $w/2]
    set ty  [expr $pad]
    set ft [filter_font $zoom]
    set info [filter_info_txt [filter_x_to_herz $x $w $scale] $q $bw $type]
    set tid [$c create text $tx $ty -text $info -anchor n -justify center \
        -font $ft -fill $cc -width 0 -tags $t]

    # calc bbox for bg rectangle
    lassign [$c bbox $tid] tx0 ty0 tx1 ty1
    set rx0 [expr $tx0-$pad]
    set rx1 [expr $tx1+$pad]
    set ry0 -1
    set ry1 [expr $ty1+$pad]

    # draw label bg rectangle
    set rbc [::tk::Darken $color 170]
    set rid [$c create rectangle $rx0 $ry0 $rx1 $ry1 \
        -outline $cc -fill $rbc -width 1 -tags $t]
    $c raise $tid $rid

    # draw handle
    set r  [expr 5*$zoom]
    set x [filter::clip $x 0 $w]
    set y [filter::clip $y 0 $h]
    set x0 [expr $x-$r]
    set y0 [expr $y-$r]
    set x1 [expr $x+$r]
    set y1 [expr $y+$r]

    $c create oval $x0 $y0 $x1 $y1 -fill $color -outline $cc -width 1 -tags $t
}

proc filter_update { cnv id w h zoom gridcolor plotcolor knobcolor b0 b1 b2 a1 a2 x y scale type q bw } {
    set c [::ceammc::ui::widget_canvas $cnv $id]
    set t [::ceammc::ui::widget_tag $id]
    $c delete $t

    filter_draw_hgrid $c $t $w $h $zoom $gridcolor
    filter_draw_vgrid $c $t $w $h $zoom $gridcolor $scale
    filter_draw_fresp $c $t $w $h $zoom $plotcolor $b0 $b1 $b2 $a1 $a2 $scale
    filter_draw_handle $c $t $w $h $zoom $knobcolor $scale $x $y $q $bw $type
}

}
