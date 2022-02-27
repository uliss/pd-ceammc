// this is autogenerated file, do not edit manually!
#include "m_pd.h"
#ifndef __ceammc_texteditor_tcl_h
#define __ceammc_texteditor_tcl_h
namespace {
void ceammc_texteditor_tcl_output() {
	sys_vgui("%s", "package provide ceammc_texteditor 0.1\nif {[catch {package require ctext}]} {\n::pdwindow::error \"[TCL] package error: can't find ctext package\\n\"\n} else {\nnamespace eval ::ceammc {\nnamespace eval texteditor {\narray set colors {\nbg #282a36\nfg #f8f8f2\npink #ff79c6\ncyan #8be9fd\ngreen #50fa7b\norange #ffb86c\npurple #bd93f9\nred #ff5555\nyellow #f1fa8c\nselect #44475a\ncomment #6272a4\n}\nproc open {name geometry title font showlinenum highlight} {\nif {[winfo exists $name]} {\n$name.f.text fastdelete 1.0 end\n} else {\nupvar ::ceammc::texteditor::colors colors\ntoplevel $name\nwm title $name $title\nwm geometry $name $geometry\nwm protocol $name WM_DELETE_WINDOW \"ceammc::texteditor::close $name 1\"\nbind $name <<Modified>> \"ceammc::texteditor::dodirty $name\"\nframe $name.f -bg red\nscrollbar $name.f.scroll -command \"$name.f.text yview\"\nctext $name.f.text -linemap $showlinenum -linemapfg $colors(comment) -linemapbg $colors(bg) \\\n-linemap_markable 0 \\\n-bg $colors(bg) \\\n-fg $colors(fg) \\\n-insertbackground $colors");
	sys_vgui("%s", "(cyan) \\\n-font [get_font_for_size $font] \\\n-relief flat \\\n-yscrollcommand \"$name.f.scroll set\"\n$name.f.text.t configure -selectbackground $colors(select)\n$name.f.text.t configure -highlightcolor $colors(bg)\n$name.f.text.t configure -highlightbackground $colors(bg)\n$name.f.text.l configure -highlightbackground $colors(comment)\n$name.f.text.t configure -highlightthickness 1\n$name.f.text.l configure -highlightthickness 1\nif { $highlight } {\nctext::addHighlightClassWithOnlyCharStart $name.f.text props $colors(cyan) \"@\"\nctext::addHighlightClassForSpecialChars $name.f.text brackets $colors(yellow) {[]()}\nctext::addHighlightClassForRegexp $name.f.text numbers $colors(pink) {[-+]?[0-9]+(?:\\.[0-9]+)?(?:[eE][-+]?[0-9]+)?}\nctext::addHighlightClassForRegexp $name.f.text strings $colors(green) {[\\\"][^\\n\\\"]+[\\\"]}\nctext::addHighlightClassForRegexp $name.f.text dicts $colors(cyan) {[a-zA-Z]+:}\n}\npack $name.f -fill both -expand 1\npack $name.f.scroll -side right -fill y\npack $name.f.text -fill both -expan");
	sys_vgui("%s", "d 1\nbind $name.f.text <$::modifier-Key-s> \"ceammc::texteditor::send $name\"\nbind $name.f.text <$::modifier-Key-w> \"ceammc::texteditor::close $name 1\"\nfocus $name.f.text\n}\n}\nproc show {name} {\nwm deiconify $name\nraise $name\nfocus $name.f.text\n}\nproc dodirty {name} {\nif {[catch {$name.f.text edit modified} dirty]} {set dirty 1}\nset title [wm title $name]\nset dt [string equal -length 1 $title \"*\"]\nif {$dirty} {\nif {$dt == 0} {wm title $name *$title}\n} else {\nif {$dt} {wm title $name [string range $title 1 end]}\n}\n}\nproc setdirty {name flag} {\nif {[winfo exists $name]} {\ncatch {$name.f.text edit modified $flag}\ndodirty $name\n}\n}\nproc doclose {name} {\ndestroy $name\npdsend [concat $name signoff]\n}\nproc append {name contents} {\nif {[winfo exists $name]} {\n$name.f.text fastinsert end $contents\n}\n}\nproc clear {name} {\nif {[winfo exists $name]} {\n$name.f.text fastdelete 1.0 end\n}\n}\nproc highlight {name} {\nif {[winfo exists $name]} {\n$name.f.text highlight 1.0 end\n}\n}\nproc send {name} {\nif {[winfo e");
	sys_vgui("%s", "xists $name]} {\npdsend [concat $name .clear]\nfor {set i 1} \\\n{[$name.f.text compare $i.end < end]} \\\n{incr i 1} {\nset lin [$name.f.text get $i.0 $i.end]\nif {$lin != \"\"} {\nset lin [string map {\",\" \" \\\\, \" \";\" \" \\\\; \" \"$\" \"\\\\$\"} $lin]\npdsend [concat $name .addline $lin]\n}\n}\npdsend [concat $name .sync]\n}\nsetdirty $name 0\n}\nproc close {name ask} {\nif {[winfo exists $name]} {\nif {[catch {$name.f.text edit modified} dirty]} {set dirty 1}\nif {$ask && $dirty} {\nset title [wm title $name]\nif {[string equal -length 1 $title \"*\"]} {\nset title [string range $title 1 end]\n}\nset answer [tk_messageBox -type yesnocancel -icon question -message [_ \"Save changes to \\\"$title\\\"?\"]]\nif {$answer == \"yes\"} {ceammc::texteditor::send $name}\nif {$answer != \"cancel\"} {pdsend [concat $name .close]}\n} else {\npdsend [concat $name .close]\n}\n}\n}\n}\n}\n}\n");
}
}
#endif // __ceammc_texteditor_tcl_h
