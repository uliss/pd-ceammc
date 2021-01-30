namespace eval ui {

proc filter_font { zoom } { return "Helvetica [expr $zoom * 7] normal roman" }

proc filter_draw_hdb { c t w h zoom labels color } {
    set ft [filter_font $zoom]
    set lx [expr $w - (2*$zoom)]

    foreach l $labels {
        set y [expr ($h/60.0)*$l + $h*0.5]
        set wd 1
        # highlight zero level
        if { $l == 0 } {
            $c create line 0 $y $w $y -fill white -width 3 -tag $t
        }

        $c create line 0 $y $w $y -fill $color -width $wd -tag $t

        set txt [expr -$l]
        if { $l == 0 } { set txt {0db} }
        set tt "${t}_t${l}"
        $c create text $lx $y -text $txt -anchor se -justify right \
            -font $ft -fill $color -width 0 -tags [list $t $tt]

        if { [expr $l == -24] } {
            lassign [$c bbox $tt] tx0 ty0 tx1 ty1
            if { $ty0 < 0 || $ty1 < 0 } { $c delete $tt }
        }
    }
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
        set nyq 22050.0
        set k [expr 20000.0/$nyq]
        set f [expr ($k*$x)/$w]
    }

    expr 3.141592653589793 * $f
}

proc filter_draw_fresp { c t w h color b0 b1 b2 a1 a2 tag } {
    set nsteps [expr int($w)]
    set db_hstep [expr $h / 60.0]

    set pts {}

    for { set x 0 } { $x < $w } { incr x 2} {
        set omega [filter_x_to_omega $x $w "lin"]
        set wamp [filter_freq_amp $omega [list $b0 $b1 $b2] [list 1 $a1 $a2]]
        set dbamp [expr 20 * log10($wamp)]

        set y [expr $h - (($dbamp * $db_hstep) + ($h*0.5))]

        lappend pts $x $y
    }

    $c create line $pts -fill $color -width 1 -tags $tag
}

proc filter_draw_hgrid { c t w h zoom gridcolor } {
    set min_line_space 20
    set lb0 { -24 -18 -12 -6 0 6 12 18 24 }
    set lb1 { -24 -12 0 12 24 }
    set nl0 [llength $lb0]
    set nl1 [llength $lb1]
    set sp0 [expr $h/$nl0]
    set sp1 [expr $h/$nl1]

    if { $sp0 >= $min_line_space } {
        filter_draw_hdb $c $t $w $h $zoom $lb0 $gridcolor
    } else {
        filter_draw_hdb $c $t $w $h $zoom $lb1 $gridcolor
    }
}

proc filter_draw_vgrid { c t w h zoom gridcolor scale } {
    if { $scale == "lin" } {
        set min_hstep [expr 30 * $zoom]
        set a 0
        set b 20000.0
        set fsteps { 500 1000 2000 2500 5000 10000 }
        set fstep 5000
        set hstep [expr $fstep / ($b-$a) * $w]
        # fine best freq step
        foreach f $fsteps {
            # hstep in pixels
            set hstep [expr ($f / ($b-$a)) * $w]
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
            if { $f < 1000 } { set txt "$f" } { set txt "[expr $f/1000.0]k" }
            set tx [expr $x-(2*$zoom+1)]
            $c create text $tx $h -text $txt -anchor se -justify right \
                -font $ft -fill $gridcolor -width 0 -tags $t

        }
    }
}

proc filter_update { cnv id w h zoom bdcolor b0 b1 b2 a1 a2 } {
    set c [::ceammc::ui::widget_canvas $cnv $id]
    set t [::ceammc::ui::widget_tag $id]
    $c delete $t

    filter_draw_hgrid $c $t $w $h $zoom $bdcolor
    filter_draw_vgrid $c $t $w $h $zoom $bdcolor "lin"
    filter_draw_fresp $c $t $w $h black $b0 $b1 $b2 $a1 $a2 $t
}

}
