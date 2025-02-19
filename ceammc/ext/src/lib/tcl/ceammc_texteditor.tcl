# Copyright (c) 2002-2021 ceammc, krzYszcz and others.
# For information on usage and redistribution, and for a DISCLAIMER OF ALL
# WARRANTIES, see the file, "LICENSE.txt," in this distribution.  */

# pdtk_textwindow - a window containing scrollable text for "qlist" and
# "textfile" objects - later the latter might get renamed just "text"

# this is adapted from krzYszcz's code for coll in cyclone

package provide ceammc_texteditor 0.1

if {[catch {package require ctext}]} {
    ::pdwindow::error "\[TCL\] package error: can't find ctext package\n"
} else {

namespace eval ::ceammc {
namespace eval texteditor {
    array set colors {
        bg      #282a36
        fg      #f8f8f2
        pink    #ff79c6
        cyan    #8be9fd
        green   #50fa7b
        orange  #ffb86c
        purple  #bd93f9
        red     #ff5555
        yellow  #f1fa8c
        select  #44475a
        comment #6272a4
    }

    array set escape_map {}

    proc open {name geometry title font showlinenum syntax} {
        if {[winfo exists $name]} {
            $name.f.text fastdelete 1.0 end
        } else {
            variable colors

            toplevel $name
            wm title $name $title
            wm geometry $name $geometry
            wm protocol $name WM_DELETE_WINDOW "ceammc::texteditor::close $name 1"
            bind $name <<Modified>> "ceammc::texteditor::dodirty $name"

            frame $name.f -bg red
            scrollbar $name.f.scroll -command "$name.f.text yview"
            ctext $name.f.text -linemap $showlinenum -linemapfg $colors(comment) -linemapbg $colors(bg) \
                -linemap_markable 0 \
                -bg $colors(bg) \
                -fg $colors(fg) \
                -insertbackground $colors(cyan) \
                -font [get_font_for_size $font] \
                -relief flat \
                -yscrollcommand "$name.f.scroll set"

            # colors
            $name.f.text.t configure -selectbackground $colors(select)
            $name.f.text.t configure -highlightcolor $colors(bg)
            $name.f.text.t configure -highlightbackground $colors(bg)
            $name.f.text.l configure -highlightbackground $colors(comment)
            $name.f.text.t configure -highlightthickness 1
            $name.f.text.l configure -highlightthickness 1

            # tabs
            $name.f.text.t configure -tabs "[expr {3 * [font measure $font 0]}] left" -tabstyle tabular

            # syntax
            if { $syntax != "none" } { ceammc::texteditor::set_syntax $name.f.text $syntax }

            # layout
            pack $name.f -fill both -expand 1
            pack $name.f.scroll -side right -fill y
            pack $name.f.text -fill both -expand 1

            bind $name.f.text <$::modifier-Key-s> "ceammc::texteditor::send $name"
            bind $name.f.text <$::modifier-Key-w> "ceammc::texteditor::close $name 1"

            bind $name.f.text.t <Tab> "ceammc::texteditor::indent $name.f.text.t"
            bind $name.f.text.t <Shift-Tab> "ceammc::texteditor::unindent $name.f.text.t"
            bind $name.f.text.t <$::modifier-Key-z> "catch {$name.f.text.t edit undo}; break"
            bind $name.f.text.t <$::modifier-Shift-Key-Z> "catch {$name.f.text.t edit redo}; break"
        }
    }

    proc indent { txt } {
        set sel [$txt tag ranges sel]
        if { $sel != "" } {
            foreach {b e} $sel {
                for {set i [$txt index "$b linestart"]} {[$txt compare $i < $e]} {set i [$txt index "$i +1 lines linestart"]} {
                    $txt insert $i "\t"
                }
            }
            $txt highlight "$b linestart" "$e lineend"
            $txt tag add sel "$b linestart" "$e lineend"
            return -code break
        } else {
            return -code continue
        }
    }

    proc unindent { txt } {
        set sel [$txt tag ranges sel]
        if { $sel != "" } {
            foreach {b e} $sel {
                for {set i [$txt index "$b linestart"]} {[$txt compare $i < $e]} {set i [$txt index "$i +1 lines linestart"]} {
                    set fc [$txt get $i]
                    if {$fc == "\t" || $fc == " "} { $txt delete $i }
                }
            }
            $txt highlight "$b linestart" $e
            $txt tag add sel "$b linestart" "$e lineend"
        } else {
            set i [$txt index "insert linestart"]
            set fc [$txt get $i]
            if {$fc == "\t" || $fc == " "} { $txt delete $i }
        }
        return -code break
    }

    proc set_syntax { w name } {
        variable colors

        switch $name {
            "none"     {}
            "selector" {
                ctext::addHighlightClass $w float $colors(purple) { float }
                ctext::addHighlightClass $w symbol $colors(green) { symbol }
                ctext::addHighlightClass $w "list" $colors(cyan)  { list }
                ctext::addHighlightClassForRegexp $w numbers $colors(pink) {[-]?[0-9]+(?:\.[0-9]+)?(?:[eE][-+]?[0-9]+)?}
            }
            "lua" {
                ctext::addHighlightClass $w cond $colors(yellow) { and not or }
                ctext::addHighlightClass $w blok $colors(cyan)   { function }
                ctext::addHighlightClass $w vars $colors(red)    { true false nil }
                ctext::addHighlightClass $w flow $colors(orange) { break do else elseif end goto for if in local repeat return then until while }
                ctext::addHighlightClass $w func $colors(purple) {
                    pd_error pd_post
                    bang_to float_to symbol_to list_to any_to
                    send_bang send_float send_symbol send_list
                    on_bang on_float on_symbol on_list
                }
                ctext::addHighlightClassForRegexp $w numbers $colors(pink) {[-]?[0-9]+(?:\.[0-9]+)?(?:[eE][-+]?[0-9]+)?}
                ctext::addHighlightClassForRegexp $w strings $colors(green) {[\"][^\n\"]+[\"]}
                ctext::addHighlightClassForRegexp $w comment $colors(comment) {--.*}
            }
            "faust" {
                ctext::addHighlightClass $w keywords $colors(purple) { process with declare case letrec where environment waveform mem }
                ctext::addHighlightClass $w gui      $colors(cyan)   { hslider vslider hgroup vgroup tgroup button checkbox nentry soundfile hbargraph vbargraph }
                ctext::addHighlightClass $w func     $colors(red)    { import library component ffunction fvariable inputs outputs par seq sum prod route acos asin \
                                                                       atan atan2 cos sin tan exp log log10 pow sqrt abs min max fmod remainder floor ceil rint round \
                                                                       rdtable rwtable select2 select3 attach
                                                                     }
                ctext::addHighlightClass $w vars     $colors(yellow) { SR BS }
                ctext::addHighlightClassForRegexp $w numbers $colors(pink) {[-]?[0-9]+(?:\.[0-9]+)?(?:[eE][-+]?[0-9]+)?}
                ctext::addHighlightClassForRegexp $w strings $colors(green) {[\"][^\n\"]+[\"]}
                ctext::addHighlightClassForRegexp $w comment $colors(comment) {//.*}
            }
            default {
                ctext::addHighlightClassWithOnlyCharStart $w props $colors(cyan) "@"
                ctext::addHighlightClassWithOnlyCharStart $w props $colors(cyan) "\$"
                ctext::addHighlightClassForSpecialChars $w brackets $colors(yellow) {[]()}
                ctext::addHighlightClassForRegexp $w dicts $colors(cyan) {[a-zA-Z]+:}
                ctext::addHighlightClassForRegexp $w numbers $colors(pink) {[-]?[0-9]+(?:\.[0-9]+)?(?:[eE][-+]?[0-9]+)?}
                ctext::addHighlightClassForRegexp $w strings $colors(green) {[\"]([^\n`\"]||`\"|``)+[\"]}
                ctext::addHighlightClass $w esc_ticks $colors(red)  {``}
                ctext::addHighlightClass $w esc_dblq  $colors(red)  {`\"}
            }
        }
    }

    proc show {name} {
        wm deiconify $name
        raise $name
        focus $name.f.text.t
    }

    proc dodirty {name} {
        if {[catch {$name.f.text edit modified} dirty]} {set dirty 1}
        set title [wm title $name]
        set dt [string equal -length 1 $title "*"]
        if {$dirty} {
            if {$dt == 0} {wm title $name *$title}
        } else {
            if {$dt} {wm title $name [string range $title 1 end]}
        }
    }

    proc setdirty {name flag} {
        if {[winfo exists $name]} {
            catch {$name.f.text edit modified $flag}
            dodirty $name
        }
    }

    proc setundo {name flag} {
        if {[winfo exists $name]} {
            $name.f.text.t configure -undo $flag
        }
    }

    proc set_escape {name mode} {
        variable escape_map
        set escape_map("esc$name") "$mode"
    }

    proc doclose {name} {
        destroy $name
        pdsend [concat $name signoff]
    }

    proc append {name contents} {
        variable escape_map
        if {[winfo exists $name]} {
            switch $escape_map("esc$name") {
                lua {
                    $name.f.text fastinsert end [string map {
                        {\x7b} "{"
                        {\x7d} "}"
                        {\x2c} ","
                        {\x3b} ";"
                        {\x5c} "\\"
                        {\x09} "\t"
                        {\x24} "\$"
                    } $contents]
                }
                data    {
                    $name.f.text fastinsert end [string map {
                        {\$} {$}
                        {\,} ","
                        {\;} ";"
                    } $contents]
                }
                default {
                    $name.f.text fastinsert end $contents
                }
            }
        }
    }

    proc clear {name} {
        if {[winfo exists $name]} {
            $name.f.text fastdelete 1.0 end
        }
    }

    proc highlight {name} {
        if {[winfo exists $name]} {
            $name.f.text highlight 1.0 end
            focus $name.f.text.t
        }
    }

    proc send {name} {
        variable escape_map
        if {[winfo exists $name]} {
            pdsend [concat $name .clear]
            for {set i 1} {[$name.f.text compare $i.end < end]} {incr i 1} {
                set lin [$name.f.text get $i.0 $i.end]
                if {$lin != "" && $lin != "\t"} {
                    switch $escape_map("esc$name") {
                        lua     {
                            set lin [string map {
                                "{"  {\\x7b}
                                "}"  {\\x7d}
                                ","  {\\x2c}
                                ";"  {\\x3b}
                                "\\" {\\x5c}
                                "\t" {\\x09}
                                "\$" {\\x24}
                                "\ \ " { \  }
                            } $lin]
                            pdsend [concat $name .addline $lin]
                        }
                        data    {
                            set lin [string map {
                                "," {\,}
                                ";" {\;}
                                {$} {\$}
                                "\ \ " { \  }
                                "{" {`(}
                                "}" {`)}
                            } $lin]
                            pdsend [concat $name .addline $lin]
                        }
                        default {
                            set lin [string map {
                                "{" {} "}" {}
                            } $lin]
                            pdsend [concat $name .addline $lin]
                        }
                    }
                }
            }
            pdsend [concat $name .sync]
        }
        setdirty $name 0
    }

    proc close {name ask} {
        if {[winfo exists $name]} {
            if {[catch {$name.f.text edit modified} dirty]} {set dirty 1}
            if {$ask && $dirty} {
                set title [wm title $name]
                if {[string equal -length 1 $title "*"]} {
                    set title [string range $title 1 end]
                }
                set answer [tk_messageBox -type yesnocancel -icon question -message [_ "Save changes to \"$title\"?"]]
                if {$answer == "yes"}    {ceammc::texteditor::send $name}
                if {$answer != "cancel"} {pdsend [concat $name .close]}
            } else {
                pdsend [concat $name .close]
            }
        }
    }

}
}

}
