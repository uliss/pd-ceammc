namespace eval ::ui {

proc lcd_update {cnv id w h cols rows sz args} {
    set c [::ceammc::ui::widget_canvas $cnv $id]
    set t [::ceammc::ui::widget_tag $id]
    $c delete $t
    set nargs [llength $args]

    for {set j 0} {$j < $rows} {incr j} {
        for {set i 0} {$i < $cols} {incr i} {
            set x0 [expr $i*$sz]
            set y0 [expr $j*$sz]
            set x1 [expr $x0+$sz]
            set y1 [expr $y0+$sz]
            set bitidx [expr $i+($j*$cols)]
            set bitpos [expr $bitidx%32]
            set argidx [expr int($bitidx/32)]
            if {$argidx >= $nargs} { break }
            set intarg [lindex $args $argidx]
            set bitval [expr $intarg&(1<<$bitpos)]

            if { $bitval } {
                $c create rectangle $x0 $y0 $x1 $y1 \
                    -outline black -fill black -width 1 -tags [list $t]
            }
        }
    }
}

}
