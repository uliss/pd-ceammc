// this is autogenerated file, do not edit manually!
#include "m_pd.h"
#ifndef __ui_circular_meter_tcl_h
#define __ui_circular_meter_tcl_h
namespace {
void ui_circular_meter_tcl_output() {
	sys_vgui("%s", "namespace eval ui {\nnamespace eval cm {\nvariable FS 39.0\nvariable LED_PAD 3.0\nvariable PEAK_PAD 1.0\nvariable CHAN_CIRCLE 8.0\nvariable FONT\nswitch -- [tk::windowingsystem] {\n\"aqua\" { set FONT {Monaco 8} }\n\"win32\" { set FONT {DejaVuSansMono 8} }\ndefault { set FONT {Monaco 8} }\n}\nproc mmeter_db2color { db cold tepid warm hot over } {\nif {$db < -30} { return $cold }\nif {$db < -21} { return $tepid }\nif {$db < -12} { return $warm }\nif {$db < -3 } { return $hot } { return $over }\n}\nproc draw_pie {cnv w h tag n db led_size a0 a1 color} {\nif { $db > $n } {\nset pie [expr $led_size*min($n+1, $db)]\nset x0 [expr ($w-$pie)/2]\nset y0 [expr ($h-$pie)/2]\nset x1 [expr $x0 + $pie]\nset y1 [expr $y0 + $pie]\n$cnv create arc $x0 $y0 $x1 $y1 -fill $color -outline $color -width 0 -start $a0 -extent $a1 -tags $tag\n}\n}\nproc create_crms { id cnv w h cold tepid warm hot over nch offset cw angles data } {\nvariable FS\nvariable LED_PAD\nvariable CHAN_CIRCLE\nvariable FONT\nset t [::ceammc::ui::widget_tag $id]\nset area_ht ");
	sys_vgui("%s", "[expr $w-(4*$LED_PAD)]\nset led_len [expr $area_ht/5.0]\nset x0 $LED_PAD\nset y0 $LED_PAD\nset x1 [expr $w-2*$LED_PAD]\nset y1 [expr $h-2*$LED_PAD]\nset radius [expr $area_ht/2]\nset segm [expr 360.0/$nch]\nfor {set ch 0} {$ch < $nch} {incr ch} {\nset rms [lindex $data [expr $ch*3]]\nif {$rms <= -$FS} { continue }\nif {$cw} { set k [expr $nch-($ch+0.5)-$offset] } { set k [expr $ch-(0.5+$offset)] }\nset angle [expr $segm*$k+90]\nset led_db [expr ($FS+$rms)/9.0]\ndraw_pie $cnv $w $h $t 4 $led_db $led_len $angle $segm $over\ndraw_pie $cnv $w $h $t 3 $led_db $led_len $angle $segm $hot\ndraw_pie $cnv $w $h $t 2 $led_db $led_len $angle $segm $warm\ndraw_pie $cnv $w $h $t 1 $led_db $led_len $angle $segm $tepid\ndraw_pie $cnv $w $h $t 0 $led_db $led_len $angle $segm $cold\n}\nfor {set i 1} {$i <= 5} {incr i} {\nset led [expr $led_len*$i]\nset x0 [expr ($w-$led)/2]\nset y0 [expr ($h-$led)/2]\nset x1 [expr $x0 + $led]\nset y1 [expr $y0 + $led]\n$cnv create oval $x0 $y0 $x1 $y1 -outline gray -width 1 -tags $t\n}\nset pi2 [exp");
	sys_vgui("%s", "r 3.1415926*2]\nset half_pi [expr 3.1415926/2]\nset cx [expr $w/2]\nset cy [expr $h/2]\nfor {set ch 0} {$ch < $nch} {incr ch} { \nset angle [expr $pi2*($ch+0.5)/$nch - $half_pi]\nset x1 [expr $cx+$radius*cos($angle)]\nset y1 [expr $cy+$radius*sin($angle)]\n$cnv create line $cx $cy $x1 $y1 -fill gray -width 3 -tags $t\nset angle [expr $pi2*$ch/$nch - $half_pi]\nset x1 [expr $cx+$radius*cos($angle)]\nset y1 [expr $cy+$radius*sin($angle)]\n$cnv create oval [expr $x1-$CHAN_CIRCLE] [expr $y1-$CHAN_CIRCLE] [expr $x1+$CHAN_CIRCLE] [expr $y1+$CHAN_CIRCLE] \\\n-fill darkgray -width 0 -tags $t\nif {$cw} { set txt [expr ($ch-$offset)%$nch+1] } { set txt [expr (($nch-$ch)+$offset)%$nch+1] }\n$cnv create text $x1 $y1 -text $txt -width 20 -justify center -anchor center -fill white -font $FONT -tags $t\n}\n}\nproc create_cpeak { id cnv w h cold tepid warm hot over nch offset cw angles data } {\nvariable FS\nvariable PEAK_PAD\nvariable LED_PAD\nset t [::ceammc::ui::widget_tag $id]\nset area_ht [expr $w-(4*$LED_PAD)]\nset led_le");
	sys_vgui("%s", "n [expr $area_ht/5.0]\nset segm [expr 360.0/$nch]\nfor {set ch 0} {$ch < $nch} {incr ch} {\nset peak [lindex $data [expr $ch*3+1]]\nif {$peak < -$FS} { continue }\nif {$cw} { set k [expr $nch-($ch+0.5)-$offset] } { set k [expr $ch-(0.5+$offset)] }\nset angle [expr $segm*$k+90]\nset c [mmeter_db2color $peak $cold $tepid $warm $hot $over]\nset led_db [expr ($FS+$peak)/9.0]\nset pie [expr $led_len*$led_db]\nset x0 [expr ($w-$pie)/2]\nset y0 [expr ($h-$pie)/2]\nset x1 [expr $x0 + $pie]\nset y1 [expr $y0 + $pie]\n$cnv create arc $x0 $y0 $x1 $y1 -style arc -outline $c -width 3 -start $angle -extent $segm -tags $t\n}\n}\nproc delete {id cnv} {\nset t [::ceammc::ui::widget_tag $id]\n$cnv delete $t\n}\nproc create {id cnv w h bdcolor cold tepid warm hot over nch offset cw angles data} {\ncreate_crms $id $cnv $w $h $cold $tepid $warm $hot $over $nch $offset $cw $angles $data\ncreate_cpeak $id $cnv $w $h $cold $tepid $warm $hot $over $nch $offset $cw $angles $data\n}\n}\n}\nif {[info exists ::env(TEST_PD_TCL)]} {\nnamespace e");
	sys_vgui("%s", "val ceammc {\nnamespace eval ui {\nproc widget_tag {id} { return \"tag_$id\" }\n}\n}\nwm title . \"ui.cm~ test\"\nwm geometry . 400x400\nset c [tk::canvas .canvas -width 500 -height 400 -background white]\npack $c\n$c create rectangle 0 0 200 200 -outline blue -width 1\nui::cm::create id $c 200 200 grey green yellow orange red black 5 1 1 [list 0 72 144 180 270] [list -3 0 0 -12 0 0 -21 0 0 -31 0 0 0 0 0]\n}\n");
}
}
#endif // __ui_circular_meter_tcl_h
