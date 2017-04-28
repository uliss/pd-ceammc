# Copyright (c) 2011 yvan volochine <yvan.volochine@gmail.com>
#
# This file is part of completion-plugin.
#
# completion-plugin is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

# META NAME completion plugin
# META DESCRIPTION enables completion for objects
# META AUTHOR <Yvan Volochine> yvan.volochine@gmail.com
# META VERSION 0.42

# TODO
# - add user arguments (tabread $1 ...)


namespace eval ::completion:: {
    variable ::completion::config
    variable external_filetype ""
}

###########################################################
# overwritten
rename pdtk_text_editing pdtk_text_editing_old
rename ::dialog_font::ok ::dialog_font::ok_old

############################################################
# GLOBALS

# default
set ::completion::config(save_mode) 1 ;# save keywords (s/r/array/table/...)
set ::completion::config(lines) 8
set ::completion::config(font) "DejaVu Sans Mono"
set ::completion::config(font_size) 10 ;# FIXME ???
set ::completion::config(bg) "#418bd4"
set ::completion::config(fg) white
set ::completion::config(offset) 0

set ::completion::config(border_width) 1
switch -- $::windowingsystem {
    "aqua"  {
        set ::completion::config(border_width) 0
        set ::completion::config(font) "Monaco"
        set ::completion::config(font_size) 12
    }
    "win32" {
        set ::completion::config(font_size) 10
    }
    "x11"   {  }
}

# private
set ::toplevel ""
set ::current_canvas ""
set ::current_tag ""
set ::current_text ""
set ::erase_text ""
set ::completions {"(none)"}
set ::new_object false
set ::editx 0
set ::edity 0
set ::focus ""
set ::completion_text_updated 0

# all pd internals (hopefully)
set ::all_externals {abs abs~ adc~ append array {array get} {array max} {array min} \
{array quantile} {array random} {array set} {array size} {array sum} \
atan atan2 bag bang~ bendin bendout biquad~ block~ bng bob~ bonk~ \
bp~ catch~ change choice clip clip~ clone cnv cos cos~ cpole~ cputime \
ctlin ctlout czero_rev~ czero~ dac~ dbtopow dbtopow~ dbtorms dbtorms~ \
declare del delay delread4~ delread~ delwrite~ div drawcurve drawnumber \
drawpolygon drawsymbol drawtext element env~ exp expr expr~ exp~ fexpr~ \
fft~ fiddle~ filledcurve filledpolygon framp~ fswap ftom ftom~ get getsize \
hdl hip~ hradio hsl hslider ifft~ inlet inlet~ int key keyname keyup \
line line~ {list append} {list fromsymbol} {list length} {list prepend} \
{list split} {list tosymbol} {list trim} loadbang log log~ loop~ lop~ \
lrshift~ makefilename makenote max max~ metro midiclkin midiin midiout \
midirealtimein min min~ mod moses mtof mtof~ \
namecanvas nbx netreceive netsend noise~ notein noteout openpanel \
oscformat oscparse osc~ outlet outlet~ pack page pd~ pgmin pgmout phasor~ \
pipe pique plot pointer poly polytouchin polytouchout pow powtodb powtodb~ \
pow~ print print~ q8_rsqrt~ q8_sqrt~ qlist radiobut radiobutton random \
rdb readsf~ realtime receive receive~ rfft~ rifft~ rmstodb rmstodb~ \
route rpole~ rsqrt~ rzero_rev~ rzero~ samphold~ samplerate~ savepanel \
scalar sel select send send~ set setsize sigmund~ sig~ sin snapshot~ \
soundfiler spigot sqrt sqrt~ stdout stripnote struct swap switch~ \
sysexin table tabosc4~ tabplay~ tabread tabread4 tabread4~ tabread~ \
tabreceive~ tabsend~ tabwrite tabwrite~ tan template text {text define} \
{text delete} {text fromlist} {text get} {text search} {text sequence} \
{text set} {text size} {text tolist} textfile tgl threshold~ throw~ \
timer toggle touchin touchout trigger unpack until value vcf~ vdl vd~ \
vline~ vradio vsl vslider vsnapshot~ wrap wrap~ writesf~}


proc ::completion::init {} {
    variable external_filetype
    ::completion::read_config
    switch -- $::windowingsystem {
        "aqua"  { set external_filetype *.d_fat }
        "win32" { set external_filetype *.dll }
        "x11"   { set external_filetype *.pd_linux }
    }

    ::completion::add_user_externals
    ::completion::add_libraries_externals
    ::completion::add_user_objectlist
    ::completion::add_ceammc_externals
    set ::all_externals [lsort $::all_externals]
}

proc ::completion::loaded {mytoplevel} {
    bind ${mytoplevel} <Tab> {+::completion::trigger }
}

# taken from kiosk-plugin.tcl by Iohannes
proc ::completion::read_config {{filename completion.cfg}} {
    if {[file exists $filename]} {
        set fp [open $filename r]
    } else {
        set filename [file join $::current_plugin_loadpath $filename]
        if {[file exists $filename]} {
            set fp [open $filename r]
        } else {
            puts "completion.cfg not found"
            return False
        }
    }
    while {![eof $fp]} {
        set data [gets $fp]
        if { [string is list $data ] } {
            if { [llength $data ] > 1 } {
                set ::completion::config([lindex $data 0]) [lindex $data 1]
            }
        }
    }
    close $fp
    return True
}

proc ::completion::add_user_externals {} {
    variable external_filetype
    foreach pathdir [concat $::sys_searchpath $::sys_staticpath] {
        set dir [file normalize $pathdir]
        if { ! [file isdirectory $dir]} {continue}
        foreach filename [glob -directory $dir -nocomplain -types {f} -- $external_filetype] {
            set basename [file tail $filename]
            set name [file rootname $basename]
            lappend ::all_externals $name
        }
    }
}

proc ::completion::add_libraries_externals {} {
    foreach lib $::startup_libraries {
        set filename [file join $::current_plugin_loadpath "extra_objects" $lib]
        ::completion::read_objectlist_file [format "%s.txt" $filename]
    }
}

proc ::completion::add_ceammc_externals {} {
    set filename [file join $::current_plugin_loadpath "ceammc_objects"]
    ::completion::read_objectlist_file [format "%s.txt" $filename]

    set filename [file join $::current_plugin_loadpath "cream_objects"]
    ::completion::read_objectlist_file [format "%s.txt" $filename]
}

proc ::completion::add_user_objectlist {} {
    set userdir [file join $::current_plugin_loadpath "user_objects"]
    foreach filename [glob -directory $userdir -nocomplain -types {f} -- \
                         *.txt] {
        ::completion::read_objectlist_file $filename
    }
}

proc ::completion::read_objectlist_file {afile} {
    if {[file exists $afile]
        && [file readable $afile]
    } {
        set fl [open $afile r]
        while {[gets $fl line] >= 0} {
            if {[string index $line 0] ne ";"
                && [string index $line 0] ne " "
                && [lsearch -exact $::all_externals $line] == -1} {
                lappend ::all_externals $line
            }
        }
        close $fl
    }
}

proc ::completion::trigger {} {
    if {$::current_canvas ne ""
        && $::current_text eq ""
        && ! $::completion_text_updated
    } {
        set ::current_text \
            [$::current_canvas itemcget $::current_tag -text]
        ::completion::trimspaces
    }


    if {$::new_object && $::current_text ne ""} {
        bind $::current_canvas <KeyRelease> {::completion::text_keys %K}

        if {![winfo exists .pop]} {
            ::completion::popup_draw
            ::completion::search
            ::completion::try_common_prefix
            ::completion::update_gui
            set first [lindex $::completions 0]
            if {[::completion::unique] } {
                ::completion::replace_text ${first}
                ::completion::popup_destroy 1
                ::completion::disable
            }
        } {
            if {[::completion::unique]} {
                ::completion::choose_selected
            } elseif { [llength $::completions] > 1 } {
                if {![::completion::try_common_prefix]} {
                    ::completion::increment
                }
            }
        }
    }
}

proc ::completion::search {{text ""}} {
    # without the arg there are some bugs when keys come from listbox
    set ::erase_text $::current_text
    if {$text ne ""} {
        set ::current_text $text
        set ::erase_text $text
    } elseif { !$::completion_text_updated } {
        set ::current_text \
            [$::current_canvas itemcget $::current_tag -text]
    }
    ::completion::trimspaces
    if {$::current_text ne ""} {
        # prevent wildcards ? ;# FIXME
        set ::completions [lsearch -all -inline -glob \
                               $::all_externals $::current_text*]
    } { set ::completions {} }
    ::completion::update_gui
}

proc ::completion::update_gui {} {
    if {[winfo exists .pop.f.lb]} {
        ::completion::scrollbar_check
        if {$::completions == {}} { ::completion::disable }
        if {[llength $::completions] > 1} {
            .pop.f.lb configure -state normal
            .pop.f.lb select clear 0 end
            .pop.f.lb select set 0 0
            .pop.f.lb yview scroll -100 page
        }
    }
}

proc ::completion::unique {} {
    return [expr {[llength $::completions] == 1
                  && [::completion::valid]}]
}

proc ::completion::valid {} {
    return [expr {[lindex $::completions 0] ne "(none)"}]
}

proc ::completion::disable {} {
    if {[winfo exists .pop.f.lb]} {
        ::completion::scrollbar_check
        .pop.f.lb configure -state disabled
    }
    set ::completions {"(none)"}
}

proc ::completion::increment {} {
    if {$::focus != "pop"} {
        focus .pop.f.lb
        set ::focus "pop"
    } {
        set selected [.pop.f.lb curselection]
        set updated [expr {($selected + 1) % [llength $::completions]}]
        .pop.f.lb selection clear 0 end
        .pop.f.lb selection set $updated
        # wrap
        if {$updated >= $::completion::config(lines)} {
            .pop.f.lb yview scroll 1 units
        } elseif {$updated == 0} {
            .pop.f.lb yview scroll -100 page
        }
    }
}

# store keywords (send/receive or array)
proc ::completion_store {tag} {
    set name 0
    set kind(sr) {s r}
    set kind(sra) {send~ receive~}
    set kind(tc) {throw~ catch~}
    set kind(arr) {tabosc4~ tabplay~ tabread tabread4 \
                         tabread4~ tabread~ tabwrite tabwrite~}
    set kind(del) {delread~ delwrite~}

    if {[regexp {^(s|r|send|receive)\s(\S+)$} $tag -> any name]} {
        set which sr
    }
    if {[regexp {^(send\~|receive\~)\s(\S+)$} $tag -> any name]} {
        set which sra
    }
    if {[regexp {^(throw\~|catch\~)\s(\S+)$} $tag -> any name]} {
        set which tc
    }
    if {[regexp {^table\s(\S+)$} $tag -> name]} {
        set which arr
    }
    if {[regexp {^(delread\~|delwrite\~)\s(\S+)\s*\S*$} $tag -> any name]} {
        set which del
    }

    if {$name != 0} {
        foreach key $kind($which) {
            if {[lsearch -all -inline -glob $::all_externals [list $key $name]] eq ""} {
                lappend ::all_externals [list $key $name]
                set ::all_externals [lsort $::all_externals]
            }
        }
    }
}

proc ::completion::choose_selected {} {
    if {[::completion::valid]} {
        set selected [.pop.f.lb curselection]
        ::completion::popup_destroy 1
        ::completion::replace_text [lindex $::completions $selected]
        set ::current_text [lindex $::completions $selected]
        ::completion::disable
        focus -force $::current_canvas
        set ::focus "canvas"
    }
}

# keys with listbox focus
proc ::completion::lb_keys {key} {
    set ::completion_text_updated 0
    if {[regexp {^[a-zA-Z0-9~/\._\+]{1}$} $key]} {
        ::completion::insert_key $key; return
    }
    switch -- $key {
        "space"     { ::completion::insert_key " " } ;# search
        "Return"    { ::completion::choose_selected }
        "BackSpace" { ::completion::chop } ;# search
    }
}

# keys from textbox
proc ::completion::text_keys {key} {
    set ::completion_text_updated 0
    switch -- $key {
        "plus"   { set key "+" }
        "minus"   { set key "-" }
        "Escape" { ::completion::popup_destroy 1 }
    }
    if {[regexp {^[a-zA-Z0-9~/\._\+\-\*]{1}$} $key]} {
        ::completion::search
    } elseif {$key eq "space"} {
        ::completion::search
    } elseif {$key eq "BackSpace"} {
        after 10; ::completion::search ;# FIXME
    } elseif {$key eq "Return"} {
        ::completion::choose_or_unedit
    }
}

proc ::completion::insert_key {key} {
    scan $key %c keynum
    pdsend "pd key 1 $keynum 0"
    append ::current_text $key
    # for some reason this does not work without passing the arg
    ::completion::search $::current_text
    focus -force $::toplevel
    focus -force $::current_canvas ;# FIXME
    set ::focus "canvas"
    pdtk_text_editing $::toplevel $::current_tag 1
    set ::completion_text_updated 0
}

proc ::completion::erase_text {} {
    # simulate backspace keys
    set i [expr {[string length $::erase_text] + 2}] ;# FIXME
    while {--$i > 0} {
        event generate $::toplevel [expr {"<BackSpace>"}]
#        pdsend "$::toplevel key 1 8 0"
#        pdsend "$::toplevel key 0 8 0"
        incr i -1
    }
}

# replace text from object box
proc ::completion::replace_text {args} {
    set text ""
    ::completion::erase_text
    # in case of spaces
    foreach arg $args { set text [concat $text $arg] }

    foreach c [split $text ""] {
        event generate $::toplevel [switch $c {
                " " { expr {"<space>"} }
                "-" { expr {"<minus>"} }
                "<" { expr {"<less>"} }
                ">" { expr {"<greater>"} }
                default { expr {$c} }
            }
        ]
    }
#    for {set i 0} {$i < [string length $text]} {incr i 1} {
#        set cha [string index $text $i]
#        scan $cha %c keynum
#        pdsend "pd key 1 $keynum 0"
#        pdsend "pd key 0 $keynum 0"
#    }

    # to be able to erase it later
    set ::erase_text $text
    # nasty hack: the widget does not update his text because we pretend
    # we typed the text although we faked it so pd gets it as well (mmh)
    set ::completion_text_updated 1
    set ::current_text $text
}

proc ::completion::choose_or_unedit {} {
    if {[winfo exists .pop] && [::completion::valid]} {
        ::completion::choose_selected
    } {
        ::completion::text_unedit
    }
}

proc ::completion::text_unedit {} {
    pdsend "$::focused_window reselect"
    set ::new_object 0
    set ::completion_text_updated 0
}

proc ::completion::chop {} {
#    pdsend "$::toplevel key 1 8 0" ;# BackSpace
#    pdsend "$::toplevel key 0 8 0"
    event generate $::toplevel [expr {"<BackSpace>"}]
    set ::current_text [string replace $::current_text end end]
    ::completion::search $::current_text
    if {[winfo exists .pop]} {
        .pop.f.lb selection clear 0 end
        .pop.f.lb selection set 0
    }
    focus -force $::current_canvas
    set ::focus "canvas"
}

proc ::completion::popup_draw {} {
    if {![winfo exists .pop]} {
        set menuheight 32
        if {$::windowingsystem ne "aqua"} { incr menuheight 24 }
        incr menuheight $::completion::config(offset)
        set geom [wm geometry $::toplevel]
        # fix weird bug on osx
        set decoLeft 0
        set decoTop 0
        regexp -- {([0-9]+)x([0-9]+)\+([0-9]+)\+([0-9]+)} $geom -> width height decoLeft decoTop
        set left [expr {$decoLeft + $::editx}]
        set top [expr {$decoTop + $::edity + $menuheight}]

        catch { destroy .pop }
        toplevel .pop
        wm overrideredirect .pop 1
        wm geometry .pop +$left+$top
        raise .pop
        wm attributes .pop -topmost 1

        frame .pop.f -takefocus 0

        pack configure .pop.f
        .pop.f configure -relief solid -borderwidth $::completion::config(border_width) -background gray

        listbox .pop.f.lb \
            -selectmode browse -height $::completion::config(lines) \
            -listvariable ::completions -activestyle none \
            -highlightcolor $::completion::config(fg) \
            -selectbackground $::completion::config(bg) \
            -selectforeground $::completion::config(fg) -width 24 \
            -yscrollcommand [list .pop.f.sb set] -takefocus 1 \
            -disabledforeground #333333

        pack .pop.f.lb -side left -expand 1 -fill both
        .pop.f.lb configure -relief flat \
            -font [list $::completion::config(font) $::completion::config(font_size)] \
            -state normal

        pack .pop.f.lb [scrollbar ".pop.f.sb" -command [list .pop.f.lb yview] -takefocus 0] \
            -side left -fill y -anchor w
        bind .pop.f.lb <Escape> {after idle { ::completion::popup_destroy 1 }}
        bind .pop.f.lb <KeyRelease> { ::completion::lb_keys %K }
        bind .pop.f.lb <ButtonRelease> {after idle {::completion::choose_selected }}
        focus .pop.f.lb
        set ::focus "pop"
        .pop.f.lb selection set 0 0
    }
}

proc ::completion::popup_destroy {{unbind 0}} {
    catch { destroy .pop }
    focus -force $::current_canvas
    set ::focus "canvas"
    if {$unbind} {
        bind $::current_canvas <KeyRelease> {}
    }
}

proc ::completion::scrollbar_check {} {
    if {[winfo exists .pop]} {
        if {[llength $::completions] < $::completion::config(lines)} {
            pack forget .pop.f.sb
        } else {
            pack .pop.f.sb -side left -fill y
        }
    }
}

###########################################################
# overwritten

proc pdtk_text_editing {mytoplevel tag editing} {
    set ::toplevel $mytoplevel
    set tkcanvas [tkcanvas_name $mytoplevel]
    set rectcoords [$tkcanvas bbox $tag]
    if {$rectcoords ne ""} {
        set ::editx  [expr {int([lindex $rectcoords 0])}]
        set ::edity  [expr {int([lindex $rectcoords 3])}]
    }
    if {$editing == 0} {
        selection clear $tkcanvas
        # completion
        ::completion::disable
        ::completion::popup_destroy 1
        set ::completion_text_updated 0
        # store keywords
        if {$::completion::config(save_mode)} {
            set text [$tkcanvas itemcget $::current_tag -text]
            ::completion_store $text
        }
    } {
        set ::editingtext($mytoplevel) $editing
        # completion
        set ::current_canvas $tkcanvas
        if {$tag ne ""} {
            # unbind Keys if new object
            if {$tag ne $::current_tag} {
                bind $::current_canvas <KeyRelease> {}
            }

            set ::current_tag $tag
        }
    }
    set ::new_object $editing
    $tkcanvas focus $tag
    set ::focus "tag"
}

proc ::dialog_font::ok {gfxstub} {
    variable fontsize
    apply $gfxstub $fontsize
    cancel $gfxstub
    # completion
    set ::completion::config(font_size) [expr {$fontsize}];
    # linux only ?
}

############################################################
# utils

# `prefix' from Bruce Hartweg <http://wiki.tcl.tk/44>
proc ::completion::prefix {s1 s2} {
    regexp {^(.*).*\0\1} "$s1\0$s2" all pref
    return $pref
}

proc ::completion::try_common_prefix {} {
    set found 0
    set prefix [::completion::common_prefix]
    if {$prefix ne $::current_text && $prefix ne ""} {
        ::completion::replace_text $prefix
        # prevent errors in pdtk_text_editing
        catch { focus .pop.f.lb }
        set ::current_text $prefix
        set found 1
    }
    return $found
}

proc ::completion::common_prefix {} {
    set prefix ""
    if {[llength $::completions] > 1} {
        set prefix [::completion::prefix \
                        [lindex $::completions 0] \
                        [lindex $::completions end]]
    }
    return $prefix
}

proc ::completion::trimspaces {} {
    set ::current_text [string trimright $::current_text " "]
}

# just in case..
bind all <$::modifier-Key-Return> {pdsend "$::focused_window reselect"}

###########################################################
# main

::completion::init

bind PatchWindow <<Loaded>> {+::completion::loaded %W}
