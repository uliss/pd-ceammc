// this is autogenerated file, do not edit manually!
#include "m_pd.h"
#ifndef __ui_display_tcl_h
#define __ui_display_tcl_h
namespace {
void ui_display_tcl_output() {
	sys_vgui("%s", "namespace eval ::ui {\nnamespace eval display {\nvariable font_size 11\nvariable font_family \"Helvetica\"\n}\nswitch -- $::windowingsystem {\n\"aqua\" { }\n\"x11\" { }\n\"win32\" {\nset display::font_size 7\n}\n}\nproc display_type_font { zoom } { return \"$display::font_family [expr $zoom * $display::font_size] normal roman\" }\nproc display_single_line { type } {\nswitch $type {\nbang { return 1 }\nfloat { return 1 }\nsymbol { return 1 }\ndefault { return 0 }\n}\n}\nproc display_txt_width { c tag } {\nlassign [$c bbox $tag] tx0 ty0 tx1 ty1\nreturn [expr $tx1 - $tx0]\n}\nproc display_txt_height { c tag } {\nlassign [$c bbox $tag] tx0 ty0 tx1 ty1\nreturn [expr abs($ty1 - $ty0)]\n}\nproc display_update {cnv id rid w h zoom hauto bdcolor bgcolor txtcolor typecolor showtype type txt} {\nset c [::ceammc::ui::widget_canvas $cnv $id]\nset t [::ceammc::ui::widget_tag $id]\n$c delete $t\n::ceammc::ui::widget_bg $cnv $id $bgcolor\nset txpad [expr $zoom*2+1]\nset typad [expr $zoom*2+1]\nset tx $txpad\nset ty $typad\nset ft [display_type_font ");
	sys_vgui("%s", "$zoom]\nset x 0\nset typeh 0\nif { $showtype == 1 } {\nset ttag \"${t}_t\"\n$c create text $tx $ty -text $type -anchor nw -justify left \\\n-font $ft -fill $txtcolor -width 0 -tags [list $t $ttag]\nset typew [display_txt_width $c $ttag]\nset typeh [display_txt_height $c $ttag]\nset x [expr (2*$txpad) + $typew]\nset ttagr \"${t}_tr\"\n$c create rectangle 0 0 $x $h -fill $typecolor -outline $typecolor -width $zoom -tags [list $t $ttagr]\n$c raise $ttag $ttagr\n$c create line $x 0 $x $h -fill $bdcolor -width $zoom -tag $t\n}\nset tx [expr $x + $txpad]\nset ty $typad\nif {$hauto == 1} {\nset tw 0\nif { ![display_single_line $type] } {\nset minw [expr 150*$zoom]\nset tw $minw\n}\n} {\nset tw [expr $w - ($tx + $txpad)]\n}\nset tt \"${t}_m\"\n$c create text $tx $ty -text $txt -anchor nw -justify left \\\n-font $ft -fill $txtcolor -width $tw -tags [list $t $tt]\nif {$hauto == 1} {\nswitch $type {\nbang {\npdsend \"$rid .resize [expr $tx+15] 10\"\n}\nfloat {\nset tw [display_txt_width $c $tt]\nset dispw [expr $tx + $tw + (2*$txpad)]\nif { $di");
	sys_vgui("%s", "spw != $w } {\npdsend \"$rid .resize $dispw 10\"\n}\n}\nsymbol {\nset tw [display_txt_width $c $tt]\nset dispw [expr $tx + $tw + (2*$txpad)]\nif { $dispw != $w } {\npdsend \"$rid .resize $dispw 10\"\n}\n}\ndefault {\nset nw $w\nset tw [display_txt_width $c $tt]\nset dispw [expr $tx + $tw + (2*$txpad)]\nif { $dispw != $w } { set nw $dispw }\nset nh 5\nset th [display_txt_height $c $tt]\nset disph [expr $th + $typad]\nif { $typeh > $nh } { set nh $typeh }\nif { $disph > $nh } { set nh $disph }\nif { $nh != $h || $nw != $w } {\npdsend \"$rid .resize $nw $nh\"\n}\n}\n}\n}\n}\n}\n");
}
}
#endif // __ui_display_tcl_h
