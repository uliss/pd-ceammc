// this is autogenerated file: do not edit
// clang-format off
#ifndef ui_lcd_tcl_h_
#define ui_lcd_tcl_h_
constexpr const char* ui_lcd_tcl = 
"namespace eval ::ui {\n"
"proc lcd_update {cnv id w h cols rows sz args} {\n"
"    set c [::ceammc::ui::widget_canvas $cnv $id]\n"
"    set t [::ceammc::ui::widget_tag $id]\n"
"    $c delete $t\n"
"    set nargs [llength $args]\n"
"    for {set j 0} {$j < $rows} {incr j} {\n"
"        for {set i 0} {$i < $cols} {incr i} {\n"
"            set x0 [expr $i*$sz]\n"
"            set y0 [expr $j*$sz]\n"
"            set x1 [expr $x0+$sz]\n"
"            set y1 [expr $y0+$sz]\n"
"            set bitidx [expr $i+($j*$cols)]\n"
"            set bitpos [expr $bitidx%32]\n"
"            set argidx [expr int($bitidx/32)]\n"
"            if {$argidx >= $nargs} { break }\n"
"            set intarg [lindex $args $argidx]\n"
"            set bitval [expr $intarg&(1<<$bitpos)]\n"
"            if { $bitval } {\n"
"                $c create rectangle $x0 $y0 $x1 $y1 \\\n"
"                    -outline black -fill black -width 1 -tags [list $t]\n"
"            }\n"
"        }\n"
"    }\n"
"}\n"
"}\n"
;
#endif
// clang-format on
