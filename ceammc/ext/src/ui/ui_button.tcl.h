// this is autogenerated file, do not edit manually!
#include "m_pd.h"
#ifndef __ui_button_tcl_h
#define __ui_button_tcl_h
namespace {
void ui_button_tcl_output() {
	sys_vgui("%s", "namespace eval ::ui {\nproc button_update {cnv id w h zoom c0 c1 state} {\nset c [::ceammc::ui::widget_canvas $cnv $id]\nset t [::ceammc::ui::widget_tag $id]\n$c delete $t\nif { $state == 1 } { set color $c1 } { set color $c0 }\nset xpad [expr {round($w/10)} + 1 + $zoom]\nset ypad [expr {round($h/10)} + 1 + $zoom]\nset x0 [expr $xpad]\nset x1 [expr $w - $xpad]\nset y0 [expr $ypad]\nset y1 [expr $h - $ypad]\n$c create rectangle $x0 $y0 $x1 $y1 -fill $color -outline $color -width $zoom -tags $t\n}\n}\n");
}
}
#endif // __ui_button_tcl_h
