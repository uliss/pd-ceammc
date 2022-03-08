// this is autogenerated file, do not edit manually!
#include "m_pd.h"
#ifndef __ceammc_texteditor_tcl_h
#define __ceammc_texteditor_tcl_h
namespace {
void ceammc_texteditor_tcl_output() {
	sys_vgui("%s", "package provide ceammc_texteditor 0.1\nif {[catch {package require ctext}]} {\n::pdwindow::error \"[TCL] package error: can't find ctext package\\n\"\n} else {\nnamespace eval ::ceammc {\nnamespace eval texteditor {\narray set colors {\nbg #282a36\nfg #f8f8f2\npink #ff79c6\ncyan #8be9fd\ngreen #50fa7b\norange #ffb86c\npurple #bd93f9\nred #ff5555\nyellow #f1fa8c\nselect #44475a\ncomment #6272a4\n}\narray set escape_map {}\nproc open {name geometry title font showlinenum syntax} {\nif {[winfo exists $name]} {\n$name.f.text fastdelete 1.0 end\n} else {\nvariable colors\ntoplevel $name\nwm title $name $title\nwm geometry $name $geometry\nwm protocol $name WM_DELETE_WINDOW \"ceammc::texteditor::close $name 1\"\nbind $name <<Modified>> \"ceammc::texteditor::dodirty $name\"\nframe $name.f -bg red\nscrollbar $name.f.scroll -command \"$name.f.text yview\"\nctext $name.f.text -linemap $showlinenum -linemapfg $colors(comment) -linemapbg $colors(bg) \\\n-linemap_markable 0 \\\n-bg $colors(bg) \\\n-fg $colors(fg) \\\n-insertbackground $colors(cyan");
	sys_vgui("%s", ") \\\n-font [get_font_for_size $font] \\\n-relief flat \\\n-yscrollcommand \"$name.f.scroll set\"\n$name.f.text.t configure -selectbackground $colors(select)\n$name.f.text.t configure -highlightcolor $colors(bg)\n$name.f.text.t configure -highlightbackground $colors(bg)\n$name.f.text.l configure -highlightbackground $colors(comment)\n$name.f.text.t configure -highlightthickness 1\n$name.f.text.l configure -highlightthickness 1\n$name.f.text.t configure -tabs \"[expr {3 * [font measure $font 0]}] left\" -tabstyle tabular\nif { $syntax != \"none\" } { ceammc::texteditor::set_syntax $name.f.text $syntax }\npack $name.f -fill both -expand 1\npack $name.f.scroll -side right -fill y\npack $name.f.text -fill both -expand 1\nbind $name.f.text <$::modifier-Key-s> \"ceammc::texteditor::send $name\"\nbind $name.f.text <$::modifier-Key-w> \"ceammc::texteditor::close $name 1\"\nbind $name.f.text.t <Tab> \"ceammc::texteditor::indent $name.f.text.t\"\nbind $name.f.text.t <Shift-Tab> \"ceammc::texteditor::unindent $name.f.text.t\"\n}\n}\npr");
	sys_vgui("%s", "oc indent { txt } {\nset sel [$txt tag ranges sel]\nif { $sel != \"\" } {\nforeach {b e} $sel {\nfor {set i [$txt index \"$b linestart\"]} {[$txt compare $i < $e]} {set i [$txt index \"$i +1 lines linestart\"]} {\n$txt insert $i \"\\t\"\n}\n}\n$txt highlight \"$b linestart\" $e\nreturn -code break\n} else {\nreturn -code continue\n}\n}\nproc unindent { txt } {\nset sel [$txt tag ranges sel]\nif { $sel != \"\" } {\nforeach {b e} $sel {\nfor {set i [$txt index \"$b linestart\"]} {[$txt compare $i < $e]} {set i [$txt index \"$i +1 lines linestart\"]} {\nset fc [$txt get $i]\nif {$fc == \"\\t\" || $fc == \" \"} { $txt delete $i }\n}\n}\n$txt highlight \"$b linestart\" $e\n} else {\nset i [$txt index \"insert linestart\"]\nset fc [$txt get $i]\nif {$fc == \"\\t\" || $fc == \" \"} { $txt delete $i }\n}\nreturn -code break\n}\nproc set_syntax { w name } {\nvariable colors\nswitch $name {\n\"none\" {}\n\"selector\" {\nctext::addHighlightClass $w float $colors(purple) { float }\nctext::addHighlightClass $w symbol $colors(green) { symbol }\nctext::addHighlightClass $w \"");
	sys_vgui("%s", "list\" $colors(cyan) { list }\nctext::addHighlightClassForRegexp $w numbers $colors(pink) {[-]?[0-9]+(?:\\.[0-9]+)?(?:[eE][-+]?[0-9]+)?}\n}\n\"lua\" {\nctext::addHighlightClass $w cond $colors(yellow) { and not or }\nctext::addHighlightClass $w blok $colors(cyan) { function end }\nctext::addHighlightClass $w vars $colors(red) { true false nil }\nctext::addHighlightClass $w flow $colors(orange) { break do else elseif goto for if in local repeat return then until while }\nctext::addHighlightClass $w func $colors(purple) { bang_to float_to symbol_to list_to on_bang on_float on_symbol on_list }\nctext::addHighlightClassForRegexp $w numbers $colors(pink) {[-]?[0-9]+(?:\\.[0-9]+)?(?:[eE][-+]?[0-9]+)?}\nctext::addHighlightClassForRegexp $w strings $colors(green) {[\\\"][^\\n\\\"]+[\\\"]}\nctext::addHighlightClassForRegexp $w comment $colors(comment) {--.*}\n}\ndefault {\nctext::addHighlightClassWithOnlyCharStart $w props $colors(cyan) \"@\"\nctext::addHighlightClassForSpecialChars $w brackets $colors(yellow) {[]()}\nctext::");
	sys_vgui("%s", "addHighlightClassForRegexp $w numbers $colors(pink) {[-]?[0-9]+(?:\\.[0-9]+)?(?:[eE][-+]?[0-9]+)?}\nctext::addHighlightClassForRegexp $w strings $colors(green) {[\\\"][^\\n\\\"]+[\\\"]}\nctext::addHighlightClassForRegexp $w dicts $colors(cyan) {[a-zA-Z]+:}\n}\n}\n}\nproc show {name} {\nwm deiconify $name\nraise $name\nfocus $name.f.text.t\n}\nproc dodirty {name} {\nif {[catch {$name.f.text edit modified} dirty]} {set dirty 1}\nset title [wm title $name]\nset dt [string equal -length 1 $title \"*\"]\nif {$dirty} {\nif {$dt == 0} {wm title $name *$title}\n} else {\nif {$dt} {wm title $name [string range $title 1 end]}\n}\n}\nproc setdirty {name flag} {\nif {[winfo exists $name]} {\ncatch {$name.f.text edit modified $flag}\ndodirty $name\n}\n}\nproc setundo {name flag} {\nif {[winfo exists $name]} {\n$name.f.text.t configure -undo $flag\n}\n}\nproc set_escape {name {flag 1}} {\nvariable escape_map\nset escape_map(\"esc$name\") $flag\n}\nproc doclose {name} {\ndestroy $name\npdsend [concat $name signoff]\n}\nproc append {name contents} {\nif {");
	sys_vgui("%s", "[winfo exists $name]} {\n$name.f.text fastinsert end [string map {{\\x7b} \"{\" {\\x7d} \"}\" {\\x2c} \",\" {\\x3b} \";\" {\\x5c} \"\\\\\" {\\x09 } \"\\t\"} $contents]\n}\n}\nproc clear {name} {\nif {[winfo exists $name]} {\n$name.f.text fastdelete 1.0 end\n}\n}\nproc highlight {name} {\nif {[winfo exists $name]} {\n$name.f.text highlight 1.0 end\nfocus $name.f.text.t\n}\n}\nproc send {name} {\nvariable escape_map\nif {[winfo exists $name]} {\npdsend [concat $name .clear]\nfor {set i 1} \\\n{[$name.f.text compare $i.end < end]} \\\n{incr i 1} {\nset lin [$name.f.text get $i.0 $i.end]\nif {$lin != \"\" && $lin != \"\\t\"} {\nif { $escape_map(\"esc$name\") } {\nset lin [string map {\"{\" {\\\\x7b} \"}\" {\\\\x7d} \",\" {\\\\x2c} \";\" {\\\\x3b} \"\\\\\" {\\\\x5c} \"\\t\" {\\\\x09 }} $lin]\npdsend [concat $name .addline $lin]\n} else {\nset lin [string map {\"{\" {} \"}\" {}} $lin]\npdsend [concat $name .addline $lin]\n}\n}\n}\npdsend [concat $name .sync]\n}\nsetdirty $name 0\n}\nproc close {name ask} {\nif {[winfo exists $name]} {\nif {[catch {$name.f.text edit modified} dirty]} {set dir");
	sys_vgui("%s", "ty 1}\nif {$ask && $dirty} {\nset title [wm title $name]\nif {[string equal -length 1 $title \"*\"]} {\nset title [string range $title 1 end]\n}\nset answer [tk_messageBox -type yesnocancel -icon question -message [_ \"Save changes to \\\"$title\\\"?\"]]\nif {$answer == \"yes\"} {ceammc::texteditor::send $name}\nif {$answer != \"cancel\"} {pdsend [concat $name .close]}\n} else {\npdsend [concat $name .close]\n}\n}\n}\n}\n}\n}\n");
}
}
#endif // __ceammc_texteditor_tcl_h
