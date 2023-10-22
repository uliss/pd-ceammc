# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#     * Redistributions of source code must retain the above copyright
#       notice, this list of conditions and the following disclaimer.
#     * Redistributions in binary form must reproduce the above copyright
#       notice, this list of conditions and the following disclaimer in the
#       documentation and/or other materials provided with the distribution.
#     * Neither the name of the <organization> nor the
#       names of its contributors may be used to endorse or promote products
#       derived from this software without specific prior written permission.
# 
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
# DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
# ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


# ==========================================

# Copyright (c) 2011-2023 yvan, henri, porres, serge, ceammc
# The original version was developed by Yvan Volochine in 2011. 
# Henri Augusto embraced in 2018. Porres took over in 2023 
# 
# https://github.com/porres/completion-plugin

package require Tcl 8.5
package require pd_menucommands 0.1

namespace eval ::dialog_path:: {
    variable use_standard_paths_button 1
    variable verbose_button 0
    variable docspath ""
    variable installpath ""
    namespace export pdtk_path_dialog
}

namespace eval ::completion:: {
    variable ::completion::config
#    variable external_filetype ""
}

###########################################################
# overwritten
rename pdtk_text_editing pdtk_text_editing_old

############################################################
# GLOBALS

set ::completion::plugin_version "0.50.0"

# default
set ::completion::config(max_lines) 15
set ::completion::config(n_lines) $::completion::config(max_lines)
if {$::windowingsystem eq "aqua"} {
    set ::completion::config(font) "Menlo"
} else {
    set ::completion::config(font) "DejaVu Sans Mono"
}
set ::completion::config(font_size) 12 ;# actually load patche's font size now
set ::completion::config(font_weight) "normal"
set ::completion::config(bg) blue
set ::completion::config(auto_complete_libs) 0
#set ::completion::config(bg) "#0a85fe"
#set ::completion::config(skipbg) "#0ad871"
#set ::completion::config(monobg) "#9832ff"
#set ::completion::config(offset) 0
#set ::completion::config(max_scan_depth) 1
#set ::completion::config(save_mode) 1 ;# save keywords (s/r/array/table/...)

# some nice colors to try: #0a85fe #0ad871 #9832ff ; those are great: #ff9831 #ff00ee #012345

# private variables

set ::completion::plugin_path ""
set ::completion::toplevel ""
set ::completion::current_canvas ""
set ::completion::current_tag ""
set ::completion::current_text ""
set ::completion::erase_text ""
set ::completion::completions {"(empty)"}
set ::completion::new_object false
set ::completion::editx 0
set ::completion::edity 0
set ::completion::focus ""
set ::completion::completion_text_updated 0
set ::completion::is_shift_down 0
set ::completion::is_ctrl_down 0
set ::completion::is_alt_down 0
set ::completion::waiting_trigger_keyrelease 0

# =========== [DEBUG mode on/off] ============
#1 = true 0 = false
set ::::completion::completion_debug 0 ;
# debug categories
set ::completion::debug_loaded_externals 1 ;#prints loaded externals
set ::completion::debug_entering_procs 1 ;#prints a message when entering a proc
set ::completion::debug_key_event 1 ;#prints a message when a key event is processed
set ::completion::debug_searches 1 ;#messages about the performed searches
set ::completion::debug_popup_gui 1 ;#messages related to the popup containing the code suggestions
set ::completion::debug_char_manipulation 1 ;#messages related to what we are doing with the text on the obj boxes (inserting/deleting chars)
# set ::completion::debug_unique_names 0 ;#messages related to storing [send/receive] names [tabread] names and alike.
set ::completion::debug_settings 1 ;#messages related to storing settings to a file.
set ::completion::debug_prefix 1 ;#messages related to adding prefix.

# (0 = normal / 1 = skipping)
set ::completion::current_search_mode 0

set ::completion::all_externals {}

#set ::completion::loaded_libs {}

set ::completion::loaded_paths {}

#useful function for debugging
proc ::completion::msg_debug {dbgMsg {debugKey "none"}} {
    switch -- $debugKey {
        "none" {}
        "loaded_externals" { if { !$::completion::debug_loaded_externals } { return  } }
        "entering_procs" { if { !$::completion::debug_entering_procs } { return  } }
        "key_event" { if { !$::completion::debug_key_event } { return  } }
        "searches" { if { !$::completion::debug_searches } { return  } }
        "popup_gui" { if { !$::completion::debug_popup_gui } { return  } }
        "char_manipulation" { if { !$::completion::debug_char_manipulation } { return  } }
        "settings" { if { !$::completion::debug_settings } { return  } }
#        "unique_names" { if { !$::completion::debug_unique_names } { return  } }
#        "prefix" { if { !$::completion::debug_prefix } { return  } }
    }
    if { $::::completion::completion_debug } {
        ::pdwindow::post "autocmpl_dbg: $dbgMsg\n"
    }
}

# This function sends keydown messages to pd
# It is better to use a separate function instead of hardcoded pdsend messages like Yvan was doing because the pd tcl api might change. 
# In fact when i took the project that was one of the major bugs with it. It was using pdsend "pd key 1 $keynum 0" which where not working.
# So using functions (procs) promotes mantainability because you only have to change their implementation to fix the code after api changes.
proc ::completion::sendKeyDown {keynum} {
    pdsend "[winfo toplevel $::completion::current_canvas] key 1 $keynum 0"
}

# This function sends keydown and then keyup messages to pd
proc ::completion::sendKeyDownAndUp {keynum} {
    pdsend "[winfo toplevel $::completion::current_canvas] key 1 $keynum 0"
    pdsend "[winfo toplevel $::completion::current_canvas] key 0 $keynum 0"
}

#called once upon plugin initialization
proc ::completion::init {} {
#    variable external_filetype
    set ::completion::plugin_path "$::current_plugin_loadpath"
    ::pdwindow::post "------------- completion-plugin -------------\n"
    ::pdwindow::post "\n"
    ::pdwindow::post "Version: $::completion::plugin_version\n"
    ::completion::read_config
    #::completion::read_extras
    # file types for each OS https://github.com/pure-data/externals-howto#library
#    switch -- $::windowingsystem {
#        "aqua"  { set external_filetype {*.pd_darwin *.d_fat *.d_i386 *.d_amd64 *.d_arm64} }
#        "win32" { set external_filetype {*.dll *.m_i386 *.m_amd64} }
#        "x11"   { set external_filetype {*.pd_linux *.l_fat *.l_i386 *.d_amd64 *.d_arm *.d_arm64} }
#    }
    if {[catch {bind "completion-plugin" <$completion::config(hotkey)> {::completion::trigger; break;}} err]} {
        ::pdwindow::post "\n---Error while trying to bind the completion plugin hotkey---\n"
        ::pdwindow::post "      hotkey: $::completion::config(hotkey)\n"
        ::pdwindow::post "      err: $err\n\n"
    }
    ::completion::scan_all_completions
    ::completion::init_options_menu
}

proc ::completion::scan_all_completions {} {
    set initTime [clock milliseconds]

# Vanilla internal objects
    set ::completion::all_externals { 
        bang trigger route swap print float int value symbol makefilename send receive \
        pack unpack list append {list prepend} {list store} {list split} {list trim} {list length} {list fromsymbol} {list tosymbol} \
        tabread tabread4 tabwrite soundfiler table {array define} {array size} {array sum} {array get} {array set} {array quantile} {array random} {array max} {array min} \
        qlist textfile {text define} {text get} {text set} {text insert} {text delete} {text size} {text tolist} {text fromlist} {text search} {text sequence} \
        {file handle} {file define} {file mkdir} {file which} {file glob} {file stat} {file isdirectory} {file isfile} {file size} {file copy} {file move} {file delete} {file split} {file splitex} {file join} {file splitname} \
        delay pipe metro line timer cputime realtime \
        select change spigot moses until \
        expr clip random + - * / max min > >= < <= == != div mod && || & | << >> sin cos tan atan atan2 wrap abs sqrt exp log pow \
        mtof ftom rmstodb dbtorms powtodb dbtopow \
        midiin midiout notein noteout ctlin ctlout pgmin pgmout bendin bendout touchin touchout polytouchin polytouchout sysexin midirealtimein makenote stripnote poly oscparse oscformat \
        openpanel savepanel key keyup keyname netsend netreceive fudiparse fudiformat bag trace \
        adc~ dac~ sig~ line~ vline~ threshold~ env~ snapshot~ vsnapsot~ bang~ samphold~ samplerate~ send~ receive~ throw~ catch~ readsf~ writesf~ print~ \
        fft~ ifft~ rfft~ irfft~ expr~ fexpr~ +~ -~ *~ /~ max~ min~ clip~ sqrt~ rsqrt~ wrap~ pow~ exp~ log~ abs~ \
        mtof~ ftom~ rmstodb~ dbtorms~ powtodb~ dbtopow~ \
        noise~ phasor~ cos~ osc~ tabosc4~ tabplay~ tabwrite~ tabread~ tabread4~ tabsend~ tabreceive~ \
        vcf~ hip~ lop~ slop~ bp~ biquad~ rpole~ rzero~ rzero_rev~ cpole~ czero~ czero_rev~ \
        delwrite~ delread~ delread4~ \
        loadbang declare savestate clone pdcontrol pd inlet inlet~ outlet outlet~ namecanvas block~ switch~ \
        struct drawpolygon filledpolygon drawcurve filledcurve drawnumber drawsymbol drawtext plot scalar pointer get set element getsize setsize append \
        sigmund~ bonk~ choice hilbert~ complex-mod~ loop~ lrshift~ pd~ stdout rev1~ rev2~ rev3~ bob~ output~
    }
    ::completion::add_user_externals
    ::completion::add_user_customcompletions

    # clear the loaded_libs because it was only used to scan 
    # the right objects located in multi-object distributions
#    set ::completion::loaded_libs {}
    set ::completion::loaded_paths {}

    set ::completion::all_externals [lsort -unique $::completion::all_externals]
#    ::completion::add_special_messages ;#AFTER sorting
    
    set count [llength $::completion::all_externals]
    ::pdwindow::post "found $count suggestions\n"
    ::pdwindow::post "\n"
    ::pdwindow::post "------------- completion-plugin -------------\n"

#    set finalTime [clock milliseconds]
#    set delta [expr {$finalTime-$initTime}]
#    ::pdwindow::post "\[completion-plugin\] loading time took $delta ms\n"
}

proc ::completion::init_options_menu {} {
    if {$::windowingsystem eq "aqua"} {
        set mymenu .menubar.apple.preferences
    } else {
        set mymenu .menubar.file.preferences    
    }
    if { [catch {
        $mymenu entryconfigure [_ "Auto Complete settings"] -command {::completion::show_options_gui}
    } _ ] } {
        $mymenu add separator
        $mymenu add command -label [_ "Auto Complete settings"] -command {::completion::show_options_gui}
    }
}

#opens the plugin's manual file (as called from the configuration window)
proc ::completion::open_manual_file {} {
    set filename [file join $::completion::plugin_path "manual.pd"]
    open_file "$filename"
}

proc ::completion::show_options_gui {} {
    if {[winfo exists .options]} {
        focus .options
        return
    }
    toplevel .options -background $::pd_colors::window_background
    wm title .options [_ "Pd AutoComplete Settings"]

    ttk::frame .options.f

#    ttk::label .options.f.title_label -text [ _ "PD AutoComplete Settings" ]
#    .options.f.title_label configure -font [list $::completion::config(font) [expr {$::completion::config(font_size)+3}]]
    
#    label .options.f.status_label -text "" -foreground "#cc2222"
#    label .options.f.status_label -text "" -foreground "#222222"

    # COLORS
    #note that we are using KeyRelease bindings because using "-validate key" would not validate in the right time.
    #ex: you would type a valid string #aabbcc and it would be invalid. Then on the next keypress (whichever it was) it would process #aabbcc

    #Options for background color
    label .options.f.click_to_choose_label -text "click to\nchoose"
    
#    label .options.f.bg_label -text "selection color"
#    entry .options.f.bg_entry -width 8
#    frame .options.f.bg_demo -background $::completion::config(bg) -width 40 -height 40
#    bind .options.f.bg_demo <ButtonRelease> { ::completion::user_select_color "bg"}
#    bind .options.f.bg_entry <KeyRelease> { ::completion::gui_options_update_color ".options.f.bg_entry" ".options.f.bg_demo" "bg" }
    

    #Options for skipping mode background color
#    label .options.f.skip_bg_label -text "skipping mode background color"
#    entry .options.f.skip_bg_entry -width 8
#    frame .options.f.skip_bg_demo -background $::completion::config(skipbg) -width 40 -height 40
#        bind .options.f.skip_bg_demo <ButtonRelease> { ::completion::user_select_color "skipbg"}
#    bind .options.f.skip_bg_entry <KeyRelease> { ::completion::gui_options_update_color ".options.f.skip_bg_entry" ".options.f.skip_bg_demo" "skipbg" }   

    #Options for monolithic mode background color
#    label .options.f.mono_bg_label -text "mono-object bkg color"
#    entry .options.f.mono_bg_entry -width 8
#    frame .options.f.mono_bg_demo -background $::completion::config(monobg) -width 40 -height 40
#        bind .options.f.mono_bg_demo <ButtonRelease> { ::completion::user_select_color "monobg"}
#    bind .options.f.mono_bg_entry <KeyRelease> { ::completion::gui_options_update_color ".options.f.mono_bg_entry" ".options.f.mono_bg_demo" "monobg" }

    #Misc
#    checkbutton .options.f.auto_complete_libs -variable ::completion::config(auto_complete_libs) -onvalue 1 -offvalue 0
#    label .options.f.auto_complete_libs_label -text "Include library prefix"

#    spinbox .options.f.number_of_lines -width 6 -from 3 -to 30 -textvariable ::completion::config(max_lines)
#    label .options.f.number_of_lines_label -text "number of lines to display"
    
#    spinbox .options.f.maximum_scan_depth -width 6 -from 0 -to 10 -textvariable ::completion::config(max_scan_depth)
#    label .options.f.maximum_scan_depth_label -text "maximum scan depth"

#    spinbox .options.f.font_size -width 6 -from 7 -to 20 -textvariable ::completion::config(font_size)
#    label .options.f.font_size_label -text "font size"

    #Hotkey
    ttk::label .options.f.hotkeylabel -text [_ "Hotkey (require save&restart)"]
    ttk::combobox .options.f.hotkeyentry -values [list Tab Alt_L Alt_R Control-space] -state readonly
    .options.f.hotkeyentry set "$::completion::config(hotkey)"
    # .options.f.hotkeyentry insert 0 "$::completion::config(hotkey)"
    bind .options.f.hotkeyentry <<ComboboxSelected>> {
        set ::completion::config(hotkey) [.options.f.hotkeyentry get]
    }
    
    #Buttons
    ttk::button .options.f.save_btn -text [_ "Save settings"] -command ::completion::write_config
    # ttk::button .options.f.default_btn -text [_ "Restore factory settings"] -command ::completion::restore_default_option
    ttk::button .options.f.rescan_btn -text [_ "Rescan externals"] -command ::completion::scan_all_completions
    # .options.f.rescan_btn configure -font {-weight bold}
    ttk::button .options.f.manual_btn -text [_ "Open Manual"] -command ::completion::open_manual_file
    #.options.f.manual_btn configure -font {-family courier -size 12 -weight bold -slant italic}
    # .options.f.manual_btn configure -font {-weight bold}

    set padding 2

    # ::::GRID::::

    #setup main frame stuff
    grid .options.f -column 0 -row 0
#    grid .options.f.title_label -column 0 -row 0 -columnspan 3 -padx $padding -pady $padding

    #setup the rest
    set current_row 1

    #auto complete libs
#    grid .options.f.auto_complete_libs_label -column 0 -row $current_row -padx $padding -pady $padding -sticky "e"
#    grid .options.f.auto_complete_libs -column 1 -row $current_row -padx $padding -pady $padding -sticky "w"
#    incr current_row

    #number of lines
#    grid .options.f.number_of_lines_label -column 0 -row $current_row -padx $padding -pady $padding -sticky "e"
#    grid .options.f.number_of_lines -column 1 -row $current_row -padx $padding -pady $padding -sticky "w"
#    incr current_row

    #font size
#    grid .options.f.font_size_label -column 0 -row $current_row -padx $padding -pady $padding -sticky "e"
#    grid .options.f.font_size -column 1 -row $current_row -padx $padding -pady $padding -sticky "w"
#    incr current_row

    #maximum scan depth
#    grid .options.f.maximum_scan_depth_label -column 0 -row $current_row -padx $padding -pady $padding -sticky "e"
#    grid .options.f.maximum_scan_depth -column 1 -row $current_row -padx $padding -pady $padding -sticky "w"

#    grid .options.f.click_to_choose_label -column 2 -row $current_row -padx $padding -pady $padding
#    incr current_row

    # change background color
#    grid .options.f.bg_label -column 0 -row $current_row -padx $padding -pady $padding -sticky "e"
#    grid .options.f.bg_entry -column 1 -row $current_row -padx $padding -pady $padding -sticky "w"
#    grid .options.f.bg_demo -column 2 -row $current_row -padx $padding -pady $padding
#    incr current_row

    # change skip mode background color
#    grid .options.f.skip_bg_label -column 0 -row $current_row -padx $padding -pady $padding -sticky "e"
#    grid .options.f.skip_bg_entry -column 1 -row $current_row -padx $padding -pady $padding -sticky "w"
#    grid .options.f.skip_bg_demo -column 2 -row $current_row -padx $padding -pady $padding
#    incr current_row

    # change mono mode background color
#    grid .options.f.mono_bg_label -column 0 -row $current_row -padx $padding -pady $padding -sticky "e"
#    grid .options.f.mono_bg_entry -column 1 -row $current_row -padx $padding -pady $padding -sticky "w"
#    grid .options.f.mono_bg_demo -column 2 -row $current_row -padx $padding -pady $padding
#    incr current_row
    
    #hotkey stuff
    grid .options.f.hotkeylabel -column 0 -row $current_row -padx $padding -pady $padding
    grid .options.f.hotkeyentry -column 1 -row $current_row -padx $padding -pady $padding
    incr current_row

    # Status labels and buttons
    #Is the status label used?
    #grid .options.f.status_label -column 0 -row $current_row -padx $padding -pady 8 -sticky "e"
    # grid .options.f.default_btn -column 0 -row $current_row -padx $padding -pady 8 -sticky "e"
    grid .options.f.save_btn -column 1 -row $current_row -padx $padding -pady 8 -sticky "w"
    incr current_row
    grid .options.f.rescan_btn -column 0 -row $current_row -padx $padding -pady 4 -sticky "ew"
    grid .options.f.manual_btn -column 1 -row $current_row -padx $padding -pady 8 -sticky "w"

    ::completion::update_options_gui
}

proc ::completion::update_options_gui {} {
#    .options.f.status_label configure -text ""
#    .options.f.bg_demo configure -background $::completion::config(bg)
#    .options.f.skip_bg_demo configure -background $::completion::config(skipbg)
#    .options.f.mono_bg_demo configure -background $::completion::config(monobg)
#    .options.f.bg_entry delete 0 end
#    .options.f.bg_entry insert 0 $::completion::config(bg)
#    .options.f.skip_bg_entry delete 0 end
#    .options.f.skip_bg_entry insert 0 $::completion::config(skipbg)
#    .options.f.mono_bg_entry delete 0 end
#    .options.f.mono_bg_entry insert 0 $::completion::config(monobg)
    .options.f.hotkeyentry delete 0 end
    .options.f.hotkeyentry insert 0 $::completion::config(hotkey)
}

proc ::completion::restore_default_option {} {
    set ::completion::config(hotkey) "Tab"
#    set ::completion::config(max_lines) 15
#    set ::completion::config(n_lines) $::completion::config(max_lines)
    if {$::windowingsystem eq "aqua"} {
        set ::completion::config(font) "Menlo"
    } else {
        set ::completion::config(font) "DejaVu Sans Mono"    
    }
#    set ::completion::config(font_size) 12
#    set ::completion::config(bg) blue
#    set ::completion::config(bg) "#0a85fe"
#    set ::completion::config(skipbg) "#0ad871"
#    set ::completion::config(monobg) "#9832ff"
#    set ::completion::config(offset) 0
#    set ::completion::config(max_scan_depth) 1
#    set ::completion::config(auto_complete_libs) 0
    ::completion::update_options_gui
    ::completion::write_config
}

proc ::completion::gui_options_update_color {entryWidget frameWidget configTag} {
    if { [regexp {^\#(\d|[a-f]){6}$} [$entryWidget get]] } {
        set ::completion::config($configTag) [$entryWidget get]
        $frameWidget configure -background $::completion::config($configTag)
        # change color to show it's valid
        $entryWidget configure -foreground #000000
    } else {
        # change color to show it's Invalid
        $entryWidget configure -foreground #de2233
    }
}

# taken from kiosk-plugin.tcl by Iohannes
proc ::completion::read_config {{filename completion.cfg}} {
    if {[file exists $filename]} {
        set fp [open $filename r]
    } else {
        set filename [file join $::completion::plugin_path $filename]
        if {[file exists $filename]} {
            set fp [open $filename r]
        } else {
            ::completion::msg_debug "completion.cfg not found"
            return False
        }
    }
    while {![eof $fp]} {
        set data [gets $fp]
        if { ![regexp {^\w} $data] } {
            continue ;#this line doesn't start with a char
        }
        # if the user provided the key value pair
        ::completion::msg_debug "data length = [llength $data]" "settings"
        if { [llength $data ] == 2} {
            set ::completion::config([lindex $data 0]) [lindex $data 1]
            ::completion::msg_debug "::completion::config([lindex $data 0]) = $::completion::config([lindex $data 0])" "settings"
        } elseif { [llength $data ] > 2} {
            set ::completion::config([lindex $data 0]) [lrange $data 1 end]
            ::completion::msg_debug "::completion::config([lindex $data 0]) = $::completion::config([lindex $data 0])" "settings"
        } else {
            ::completion::msg_debug "ERROR reading ::completion::config([lindex $data 0]) => data = $data" "settings"
        }
    }
    close $fp
    return True
}

proc ::completion::write_config {{filename completion.cfg}} {
    if { [file exists $filename] } {
        set fp [open $filename r]
        set had_to_create_file false
    } else {
        set filename [file join $::completion::plugin_path $filename]
        if {[file exists $filename]} {
            set fp [open $filename r]
            set had_to_create_file false
        } else {
            set fp [open $filename w+]
            set had_to_create_file true ;#file will get overwritten so we need to append to $lines
        }
    }
    #read the lines from the file
    set lines [split [read $fp] "\n"]
    close $fp

    #process the lines
    set lines [::completion::write_config_variable $lines "hotkey"]
#    set lines [::completion::write_config_variable $lines "max_lines"]
#    set lines [::completion::write_config_variable $lines "font"]
#    set lines [::completion::write_config_variable $lines "font_size"]
#    set lines [::completion::write_config_variable $lines "max_scan_depth"]
#    set lines [::completion::write_config_variable $lines "auto_complete_libs"]
#    set lines [::completion::write_config_variable $lines "bg"]
#    set lines [::completion::write_config_variable $lines "fg"]
#    set lines [::completion::write_config_variable $lines "skipbg"]
#    set lines [::completion::write_config_variable $lines "monobg"]
#    set lines [::completion::write_config_variable $lines "offset"]

    #write the file
    set fp [open $filename w]
    if { $had_to_create_file } {
        set lines [linsert $lines 0 "This file was generated by Completion Plugin in the absence of the original file that comes with the plugin.\n"]
    }
    puts $fp [join $lines "\n"]
    close $fp
    #.options.f.status_label configure -text "saved!"
    #after 1000 { .options.f.status_label configure -text "" }
}

proc ::completion::write_config_variable {file_lines name} {
    #https://stackoverflow.com/a/37812995/5818209   for writing to specific lines
    ::completion::msg_debug "Saving config($name)" "settings"
    set pattern ^$name\\s ;# do NOT enclose this in brackets
    ::completion::msg_debug "pattern = $pattern" "settings"
    set index 0
    set found false
    foreach line $file_lines {
        #::completion::msg_debug "line = $line"
        if {[regexp $pattern $line]} {
            ::completion::msg_debug "current variable's line = $line" "settings"
            set file_lines [lreplace $file_lines $index $index "$name $::completion::config($name)"]
            ::completion::msg_debug "line AFTER = $line" "settings"
            set found true
        }
        incr index
    }
    if { !$found } {
        #if there is no line for that variable, write it
        lappend file_lines "$name $::completion::config($name)"
    }
    return $file_lines
}

#proc ::completion::user_select_color {target} {
#    set color [tk_chooseColor -title "Completion Plugin Settings: Choose a color" -initialcolor $::completion::config($target)]
#    if { $color eq ""} { 
#        return 
#    }
#    set ::completion::config($target) $color
#    ::completion::update_options_gui
#}

# this function looks for objects in the current folder and recursively call itself for each subfolder
# we read the subfolders because pd reads the subpatches!
proc ::completion::add_user_externalsOnFolder {{dir .} depth} {
    ::completion::msg_debug "===add_user_externalsOnFolder $dir===" "loaded_externals"
#    variable external_filetype
#    if { [expr {$depth > $::completion::config(max_scan_depth)}] } {
#        return
#    }
    #::completion::msg_debug "external_filetype = $external_filetype" ;#just for debugging
#    ::completion::msg_debug "depth =  $depth" "loaded_externals"

    # i concatenate the result of two globs because for some reason i can't use glob with two patterns.
    # I've tried using: {$external_filetype,*.pd} 
    #List of system depentent (*.pd_darwin, *.dll, *.pd_linux) files on the folder
#    set sys_dependent_files ""
    # search each of extensions available in the OS (for example of macOS, *.pd_darwin,*.d_fat,*.d_i386,*.d_amd64,*.d_arm64)
#    foreach filetype $external_filetype {
#        set external_files [glob -directory $dir -nocomplain -types {f} -- $filetype]
#        if {$sys_dependent_files eq ""} {
#            set sys_dependent_files $external_files 
#        } else {
#            set sys_dependent_files [concat $external_files $sys_dependent_files]
#        }
#    }
#    set all_files [concat $pd_files $sys_dependent_files]
# list of pd files on the folder
    set pd_files [glob -directory $dir -nocomplain -types {f} -- *.pd]
    foreach filepath $pd_files {
        ::completion::msg_debug "     external = $filepath" "loaded_externals"
        set file_tail [file tail $filepath] ;# file extension
        set name_without_extension [file rootname $file_tail]
        set dir_name [file dirname $filepath] 
        set how_many_folders_to_get $depth
        set folder_name [lrange [file split $filepath] end-$how_many_folders_to_get end-1 ]
        set extension_path [join $folder_name \/]
        if {$extension_path ne ""} {
            set extension_path $extension_path\/
        }
#            ::completion::msg_debug "       depth =  $depth" "loaded_externals"
        ::completion::msg_debug "       filepath = $filepath" "loaded_externals"
        ::completion::msg_debug "       dir_name = $dir_name" "loaded_externals"
        ::completion::msg_debug "       folder_name = $folder_name" "loaded_externals"
        ::completion::msg_debug "       extension_path = $extension_path" "loaded_externals"
        ::completion::msg_debug "       file_tail = $file_tail" "loaded_externals"
        ::completion::msg_debug "       name_without_extension = $name_without_extension" "loaded_externals"
        if {[string range $name_without_extension end-4 end] eq "-help"} {
#            ::pdwindow::post "name_without_extension = $name_without_extension\n"
            set external_name [string range $name_without_extension 0 end-5]

        ::completion::msg_debug "       external_name = $external_name" "loaded_externals"
        if {[string match "ceammc*" $extension_path]} { continue }

            lappend ::completion::all_externals $extension_path$external_name
#            lappend ::completion::all_externals $external_name
#            lappend ::completion::loaded_libs $extension_path
        }
    }
    #do the same for each subfolder (recursively)
#    set depth [expr {$depth+1}]
#    foreach subdir [glob -nocomplain -directory $dir -type d *] {
#        if {[string match "ceammc*" $subdir]} { continue }
#        ::completion::add_user_externalsOnFolder $subdir $depth
#    }
}

proc ::completion::search_static_temp {} {
    set pathlist [concat $::sys_staticpath $::sys_temppath]
    foreach pathdir $pathlist {
#        ::pdwindow::post "extra pathdir: $pathdir\n"
        set dir [file normalize $pathdir]
        if { ! [file isdirectory $dir]} {
            continue
        }
        lappend ::completion::loaded_paths $pathdir
        ::pdwindow::post " - scanning: $dir\n"
        ::completion::add_user_externalsOnFolder $dir 0
        foreach subdir [glob -nocomplain -directory $dir -type d *] {
            set folder_name [lrange [file split $pathdir] end end ]
            if { $folder_name eq "extra" } {
                set subdir_name [lrange [file split $subdir] end end ]
#                ::pdwindow::post "extra extra extra -> subdir_name = $subdir_name\n"
                if { $subdir eq "bob~" } { 
                    return 
                } elseif { $subdir_name eq "bonk~" } { 
                    return 
                } elseif { $subdir_name eq "choice" } { 
                    return 
                } elseif { $subdir_name eq "fiddle~" } { 
                    return 
                } elseif { $subdir_name eq "loop~" } { 
                    return 
                } elseif { $subdir_name eq "lrshift~" } { 
                    return 
                } elseif { $subdir_name eq "pd~" } { 
                    return 
                } elseif { $subdir_name eq "pique" } { 
                    return 
                } elseif { $subdir_name eq "sigmund~" } { 
                    return 
                } elseif { $subdir_name eq "stdout" } { 
                    return 
                } else { 
                    lappend ::completion::loaded_paths $subdir
                    ::completion::add_user_externalsOnFolder $subdir 1 
                } 
            } else {
                lappend ::completion::loaded_paths $subdir
                ::completion::add_user_externalsOnFolder $subdir 1
            }
        }
    }   
}

# this proc runs the main search ::completion::add_user_externalsOnFolder into each main folder
proc ::completion::add_user_externals {} {
    ::completion::msg_debug "-----searching add_user_externals-----" "loaded_externals"
    if {[namespace exists ::pd_docsdir] && [::pd_docsdir::externals_path_is_valid]} {
        # new preferred scanning way, faster and without duplicates
        set path [::pd_docsdir::get_externals_path] 
        lappend ::completion::loaded_paths $path
        set dir [file normalize $path]
        ::pdwindow::post " - scanning: $dir\n"
        ::completion::add_user_externalsOnFolder $dir 0
        foreach subdir [glob -nocomplain -directory $dir -type d *] {
            lappend ::completion::loaded_paths $subdir
            ::completion::add_user_externalsOnFolder $subdir 1
        }
    } 
    ::completion::search_static_temp 
    # user added paths
    set searchpaths [concat $::sys_searchpath]
    set ::completion::loaded_paths [lsort -unique $::completion::loaded_paths]
    foreach searchpath $searchpaths {
        set dir [file normalize $searchpath]
        set done_before 0
        foreach paths $::completion::loaded_paths {
            if { $dir eq $paths } {
               set done_before 1
               break
            } 
        }
        if { ! $done_before } {
            # skip ceammc user folders
            if {[string match "ceammc*" $dir]} { continue }
            ::pdwindow::post " - scanning: $dir\n"
            ::completion::add_user_externalsOnFolder $dir 0
            lappend ::completion::loaded_paths $dir
            foreach subdir [glob -nocomplain -directory $dir -type d *] {
                lappend ::completion::loaded_paths $subdir
                ::completion::add_user_externalsOnFolder $subdir 1
            }
        }
    }
    #remove duplicates from the loaded_libs
#    set ::completion::loaded_libs [lsort -unique $::completion::loaded_libs]
}


#adds any completion set in any txt file under "custom_completions"
proc ::completion::add_user_customcompletions {} {
    ::completion::msg_debug "entering add user object list" "entering_procs"
    set userdir [file join $::completion::plugin_path "custom_completions"]
    foreach filename [glob -directory $userdir -nocomplain -types {f} -- *.txt] {
        ::completion::read_completionslist_file $filename
    }
}

# Reads anything located in the .txt files in the subfolders
proc ::completion::read_completionslist_file {afile} {
    if {[file exists $afile]
        && [file readable $afile]
    } {
        set fl [open $afile r]
        while {[gets $fl line] >= 0} {
            if {[string index $line 0] ne ";"
                && [string index $line 0] ne " "
                && [string index $line 0] ne ""
                && [lsearch -exact $::completion::all_externals $line] == -1} {
                lappend ::completion::all_externals $line
            }
        }
        close $fl
    }
}

###########################################################
#                      overwritten                        #
###########################################################
proc pdtk_text_editing {mytoplevel tag editing} {
    ::completion::msg_debug "entering overwritten pdtk text editing" "entering_procs"
    #::completion::msg_debug "   mytoplevel = $mytoplevel"
    #::completion::msg_debug "   tag = $tag"
    #::completion::msg_debug "   editing = $editing"
    set ::completion::toplevel $mytoplevel
    set tkcanvas [tkcanvas_name $mytoplevel]
    set rectcoords [$tkcanvas bbox $tag]
    if {$rectcoords ne ""} {
        set ::completion::editx  [expr {int([lindex $rectcoords 0])}]
        set ::completion::edity  [expr {int([lindex $rectcoords 3])}]
    }
    if {$editing == 0} {
        selection clear $tkcanvas
        # completion
        # Henri: Yvan originally called set_empty_listbox. Doens't seem to make sense. It wouldn't even reset ::completion::current_text
        ::completion::popup_destroy
        set ::completion::completion_text_updated 0
        # store keywords. Henri: i'm disabling that. See developmentGuide.md
        #if {$::completion::config(save_mode)} {
        #    set text [$tkcanvas itemcget $::completion::current_tag -text]
        #    ::completion_store $text
        #}
    } {
        set ::editingtext($mytoplevel) $editing
        # completion
        set ::completion::current_canvas $tkcanvas
        if {$tag ne ""} {
            # unbind Keys if new object
            if {$tag ne $::completion::current_tag} {
                bind $::completion::current_canvas <KeyRelease> {}
            }
            set ::completion::current_tag $tag
        }

        if {[string first "completion-plugin" [bindtags $::completion::current_canvas] ] eq -1} {
            bindtags $::completion::current_canvas "completion-plugin [bindtags $::completion::current_canvas]"
        }
        #delete_if {[string first [bindtags $::completion::current_canvas] "test"] eq -1} {
        #delete_    bindtags $::completion::current_canvas "test [bindtags $::completion::current_canvas]"
        #delete_}
        #delete_#bind $::completion::current_canvas <$completion::config(hotkey)> {+::completion::trigger;break;}
        #delete_::pdwindow::post "-----EDITING------\n"
        #delete_::pdwindow::post "current_canvas: $::completion::current_canvas\n"
        #delete_set dbg [bindtags $::completion::current_canvas]
        #delete_::pdwindow::post "bindtags: $dbg\n"
    }
    set ::completion::new_object $editing
    $tkcanvas focus $tag
    set ::completion::focus "tag"
}

# this is called when the user enters the auto completion mode
proc ::completion::trigger {} {
    ::completion::msg_debug "===entering trigger===" "entering_procs"

    set font_info [$::completion::current_canvas itemcget $::completion::current_tag -font]
    set fontface [lindex $font_info 0]
    set size [lindex $font_info 1]
    set fontsize 12
    if { $size ne "" } {
        set fontsize [expr {$size * -1}]
    }
#    set fontsize [expr {[lindex $font_info 1] * -1}]
    set fontweight [lindex $font_info 2]

    set ::completion::config(font) $fontface 
    set ::completion::config(font_size) $fontsize 
    set ::completion::config(font_weight) $fontweight 

    set ::completion::waiting_trigger_keyrelease 1
        
    set ::completion::is_shift_down 0
    set ::completion::is_ctrl_down 0
    set ::completion::is_alt_down 0
    if {$::completion::current_canvas ne ""
        && $::completion::current_text eq ""
        && ! $::completion::completion_text_updated
    } {
        #this code is responsible for reading any text already present in the object when you enter the autocomplete mode
        set ::completion::current_text [$::completion::current_canvas itemcget $::completion::current_tag -text]

        ::completion::trimspaces
        ::completion::msg_debug "Text that was already in the box = $::completion::current_text\n" "searches"
    }

    ::completion::msg_debug "-----TRIGGER------\n"
    ::completion::msg_debug "current_canvas: $::completion::current_canvas\n"
    set dbg [bindtags $::completion::current_canvas]
    ::completion::msg_debug "bindtags: $dbg\n"

    #if the user is typing into an object box
    if {$::completion::new_object} {

            # detect if the user is typing on an object, message or comment
            set ::tags_on_object_being_edited [$::completion::current_canvas itemcget $::completion::current_tag -tags]
                ::completion::msg_debug "\[$::completion::current_canvas itemcget $::completion::current_tag -tags\] = $::tags_on_object_being_edited"
            set ::type_of_object_being_edited [lindex $::tags_on_object_being_edited 1]
                ::completion::msg_debug "------>::type_of_object_being_edited = $::type_of_object_being_edited \n"
            if { ($::type_of_object_being_edited ne "obj") } {
                ::completion::msg_debug "the completion-plugin does not trigger for objects of type $::type_of_object_being_edited"
                return
            }

            bind $::completion::current_canvas <KeyRelease> {::completion::text_keys %K}
            set completed_because_was_unique 0
            if {![winfo exists .pop]} {
                    ::completion::search $::completion::current_text
                    set listsize [llength $::completion::completions]
                    if {$listsize < $::completion::config(max_lines)} {
                        set ::completion::config(n_lines) $listsize
                    #    ::pdwindow::post "listsize: $listsize\n"
                    } else {
                        set ::completion::config(n_lines) $::completion::config(max_lines)
                    }
                    ::completion::popup_draw
#                    ::completion::try_common_prefix
                    ::completion::update_completions_gui
                    if {[::completion::unique] } {
                        ::completion::choose_selected ;#Henri: was replace_text. This is needed for the three modes
                        ::completion::popup_destroy
                        ::completion::set_empty_listbox
                        set completed_because_was_unique 1
                    }
 #           } else {
                    
#                    if {[::completion::unique]} {
#                        ::completion::choose_selected
#                        set completed_because_was_unique 1
#                    } elseif { [llength $::completion::completions] > 1 } {
#                        if {![::completion::try_common_prefix]} {
#                            ::completion::msg_debug "IF not common prefix\n"
                            #::completion::increment ;#Henri: this would allow to cycle 
                            # through the completions with Tab. I'm disabling that in 
                            # favor of the arrow keys
#                        } else {
#                            ::completion::msg_debug "IF INDEED common prefix\n"
#                        }
 #                   }
            }
            # if the unique completion was used there will be no .pop to bind!
            if { !$completed_because_was_unique } {
                # work in progress
                # bind .pop <FocusOut> {::completion::msg_debug "the user has unfocused the popup"; ::completion::popup_destroy }
                # bind $::completion::current_canvas <FocusOut> {::completion::msg_debug "the user has unfocused the canvas"}
            }
    } else {
        ::completion::msg_debug "the user is NOT typing into an object box" "key_event"
    }
    # this should be time enough for the user to release the keys (so we don't capture the release keys of the plugin hotkey)
    after 200 {
        ::completion::msg_debug "accepting keys\n"
        set ::completion::waiting_trigger_keyrelease 0
    }
}

proc ::completion::skipping_search {{text ""}} {
    #set variables related to skipping_search
    ::completion::msg_debug "::completion::skipping_search($text)" "searches"
    set ::completion::current_search_mode 1
    # do we really need to check if the popup exists?
    if {[winfo exists .pop]} {
        .pop.f.lb configure -selectbackground $::completion::config(bg)
    }
    #do the search
    set text [string range $text 1 end]
    set text [::completion::fix_pattern $text]
    set chars [split $text {}]
    set pattern ""
    foreach char $chars {
        ::completion::msg_debug "--------------char = $char"
        set pattern "$pattern$char.*"
    }
    ::completion::msg_debug "RegExp pattern  = $pattern" "searches"
    ::completion::msg_debug "--------------chars = $chars" "searches"
    set ::completion::completions [lsearch -all -inline -regexp -nocase $::completion::all_externals $pattern]
}

# Searches for matches.
# (this method detects the current search mode and returns after calling the right one 
# if it happens to be normal or skipping.)
proc ::completion::search {{text ""}} {
    ::completion::msg_debug "::completion::search($text)" "searches"
    ::completion::msg_debug "::completion::completion_text_updated = $::completion::completion_text_updated" "searches"
    # without the arg there are some bugs when keys come from listbox ;# what Yvan meant?
    set ::completion::erase_text $::completion::current_text
    #if starts with a . it is a skipping search
    if {[string range $text 0 0] eq "."} {
        ::completion::skipping_search $text
        return
    }
    # Else just do the normal search
    if {[winfo exists .pop.f.lb]} {
        .pop.f.lb configure -selectbackground $::completion::config(bg)
    }
    set ::completion::current_search_mode 0
    if {$text ne ""} {
        ::completion::msg_debug "=searching for $text=" "searches"
        set ::completion::current_text $text
        set ::completion::erase_text $text
        set ::should_restore False
    } elseif { !$::completion::completion_text_updated } {
        ::completion::msg_debug "searching for empty string" "searches"
        #set ::completion::current_text \
            [$::completion::current_canvas itemcget $::completion::current_tag -text]
        set ::previous_current_text $::completion::current_text ;# saves the current text
        ::completion::msg_debug "original current_text: $::completion::current_text" "searches"
        set ::completion::current_text ""
        ::completion::msg_debug "replaced current_text is $::completion::current_text" "searches"
        set ::should_restore True
    }
    ::completion::trimspaces

    # Now this part will always run so you can perform "empty searchs" which will return all objects. In Yvan's code it would clear completions on an "empty search"
    #Yvan was using -glob patterns but they wouldn't match stuff with forward slashes (/)
    #for example if you type "freq" it wouldn't match cyclone/freqshift~
    #using -regexp now allows for that
    #Also i've added case insensitive searching (since PD object creation IS case-insensitive).
    set pattern "$::completion::current_text"
    set pattern [::completion::fix_pattern $pattern]

    set ::completion::completions [lsearch -all -inline -regexp -nocase $::completion::all_externals $pattern]
    if {$::should_restore} {
        set ::completion::current_text $::previous_current_text ;# restores the current text
        ::completion::msg_debug "restored current_text: $::completion::current_text" "searches"
    }
    ::completion::update_completions_gui
    ::completion::msg_debug "SEARCH END! Current text is $::completion::current_text" "searches"
}

# This is a method that edits a string used as a regex pattern escaping chars in order to correcly compile the regexp;
# example: we must escape "++" to "\\+\\+". 
proc ::completion::fix_pattern {pattern} {
        ::completion::msg_debug "================== - pattern = $pattern" "searches"
    set pattern [string map {"+" "\\+"} $pattern]
        ::completion::msg_debug "+ - pattern = $pattern" "searches"
    set pattern [string map {"*" "\\*"} $pattern]
        ::completion::msg_debug "* - pattern = $pattern" "searches"
    set skippingPrefix [string range $pattern 0 0]
        ::completion::msg_debug "skippingPrefix = $skippingPrefix" "searches"
    set skippingString [string range $pattern 1 end]
        ::completion::msg_debug "skippingString = $skippingString" "searches"
    set skippingString [string map {"." "\\."} $skippingString]
        ::completion::msg_debug ". skippingString = $skippingString" "searches"
    set pattern "$skippingPrefix$skippingString"
        ::completion::msg_debug ". - pattern = $pattern" "searches"
    return $pattern
}

proc ::completion::update_completions_gui {} {
    ::completion::msg_debug "entering update_completions_gui" "entering_procs"
    if {[winfo exists .pop.f.lb]} {
        ::completion::scrollbar_check
        if {$::completion::completions == {}} { ::completion::set_empty_listbox }
        if {[llength $::completion::completions] > 1} {
            .pop.f.lb configure -state normal
            .pop.f.lb select clear 0 end
            .pop.f.lb select set 0 0
            .pop.f.lb yview scroll -100 page
        }
    }
}

# I am disabling this (Porres)
# I think it should suggest a completion even if it's just 1!
proc ::completion::unique {} {
    ::completion::msg_debug "entering unique" "entering_procs"
#    return [expr {[llength $::completion::completions] == 1 && [::completion::valid]}]
    return 0
}

proc ::completion::valid {} {
    ::completion::msg_debug "entering valid" "entering_procs"
    return [expr {[lindex $::completion::completions 0] ne "(empty)"}]
}

# this is run when there are no results to display
proc ::completion::set_empty_listbox {} {
    ::completion::msg_debug "entering set_empty_listbox" "entering_procs"
    if {[winfo exists .pop.f.lb]} {
        ::completion::scrollbar_check
        .pop.f.lb configure -state disabled
    }
    set ::completion::completions {"(empty)"}
}

#this proc moves the selection down (incrementing the index)
proc ::completion::increment {{amount 1}} {
    ::completion::msg_debug "entering increment" "entering_procs"
    ::completion::msg_debug "amount = $amount" "popup_gui"
    if {$::completion::focus != "pop"} {
        focus .pop.f.lb
        set ::completion::focus "pop"
    }
    ::completion::msg_debug "bindtags = [bindtags .pop.f.lb]" "popup_gui"
    ::completion::msg_debug "bindings on .pop.f.lb = [bind .pop.f.lb]" "popup_gui"
    set selected [.pop.f.lb curselection]
    ::completion::msg_debug "selected = $selected" "popup_gui"
    
    #if completion list is empty then "selected" will be empty
    if { ![ string is integer -strict $selected] } {
        return
    }
    set updated [expr {($selected + $amount) % [llength $::completion::completions]}]
    ::completion::msg_debug "updated = $updated" "popup_gui"
    .pop.f.lb selection clear 0 end
    .pop.f.lb selection set $updated
    ::completion::msg_debug "curselection after selection set = [.pop.f.lb curselection]" "popup_gui"
    .pop.f.lb see $updated
}

# store keywords (send/receive or array)
#proc ::completion_store {tag} {
    # I'm  (Henri) disabling the unique names completion for now because i don't think it is desireable.
    # While it does detects when the user type a new name it **doesn't** when those names are not 
    # used any more (user closed their containing patch, deleted their objects, etc).
    # Also it doesn't detect those names when the user loads an patch.
    # In future versions we should be able to do that communicating with PD directly.
#    return
#    ::completion::msg_debug "entering completion store" "entering_procs"
#    ::completion::msg_debug "   tag = $tag" "unique_names"
#    set name 0
#    set kind(sr) {s r send receive}
#    set kind(sra) {send~ receive~}
#    set kind(tc) {throw~ catch~}
#    set kind(arr) {tabosc4~ tabplay~ tabread tabread4 \
                         tabread4~ tabread~ tabwrite tabwrite~}
#    set kind(del) {delread~     delwrite~}

#    if {[regexp {^(s|r|send|receive)\s(\S+)$} $tag -> do_not_matter name]} {
#        set which sr
#    }
#    if {[regexp {^(send\~|receive\~)\s(\S+)$} $tag -> do_not_matter name]} {
#        set which sra
#    }
#    if {[regexp {^(throw\~|catch\~)\s(\S+)$} $tag -> do_not_matter name]} {
#        set which tc
#    }
#    if {[regexp {^tab\S+\s(\S+)$} $tag -> name]} {
#        set which arr
#    }
#    if {[regexp {^(delread\~|delwrite\~)\s(\S+)\s*\S*$} $tag -> do_not_matter name]} {
#        set which del
#        ::completion::msg_debug "4 do_not_matter = $do_not_matter" "unique_names"
#        ::completion::msg_debug "4 name = $name" "unique_names"
#    }
#    ::completion::msg_debug "Unique name = $name" "unique_names"
#    if {$name != 0} {
#        foreach key $kind($which) {
#            ::completion::msg_debug "key = $key" "unique_names"
#            if {[lsearch -all -inline -glob $::completion::all_externals [list $key $name]] eq ""} {
#                lappend ::completion::all_externals [list $key $name]
#                set ::completion::all_externals [lsort $::completion::all_externals]
#            }
#        }
#    }
#}

#this is called when the user selects the desired external
proc ::completion::choose_selected {} {
    ::completion::msg_debug "entering choose selected" "entering_procs"
    if {[::completion::valid]} {
        set selected_index [.pop.f.lb curselection]
        ::completion::popup_destroy
        set choosen_item [lindex $::completion::completions $selected_index]
#        set isSpecialMsg [::completion::is_special_msg $choosen_item]
#        if { $isSpecialMsg } {
#            ::completion::erase_text
#            ::completion::delete_obj_onspecialmsg
#        } else {
            ::completion::replace_text $choosen_item            
#        }
        ::completion::msg_debug "----------->Selected word: $choosen_item" "char_manipulation"
        set ::completion::current_text "" ;# clear for next search
        ::completion::set_empty_listbox
        #focus -force $::completion::current_canvas
        #set ::completion::focus "canvas"
        ::completion::msg_debug "end of choose_selected current_text: $::completion::current_text" "char_manipulation"
    }
}

#proc ::completion::delete_obj_onspecialmsg {} {
    # will anybody ever read this mess? heh
    # well, this is still experimental software. I'll clean this up in the future :)
    # (dreaming of pd 1.0)

    #$::completion::current_canvas configure -bg #00ff00
#    set rectangle "$::completion::current_tag"
#    append rectangle "R"
#    ::completion::msg_debug "rectangle = $rectangle\n"
    
#    $::completion::current_canvas itemconfigure $rectangle -fill red


    # mimicking PD messages (using -d 1)
    # pdtk_undomenu $::completion::current_canvas clear no
    # pdtk_undomenu $::completion::current_canvas clear no
    # $::completion::current_canvas itemconfigure $rectangle -fill black
    # $::completion::current_canvas itemconfigure $::completion::current_tag -fill black
    # pdtk_undomenu $::completion::current_canvas clear no

    #$::completion::current_canvas delete $::completion::current_tag ;#THIS ACTUALLY REMOVES THE TEXT THE USER IS TYPING
    #$::completion::current_canvas delete $rectangle ;#THIS removes the rectangle

    #BUT they are created again when i exit exit mode
    #BUT they are created again when i exit exit mode
    #BUT they are created again when i exit exit mode

#    set coords [$::completion::current_canvas coords $rectangle]
#    ::completion::msg_debug "coords = $coords\n"

#        ::completion::msg_debug "::completion::current_canvas = $::completion::current_canvas\n"
#    set winfo_test "[winfo toplevel $::completion::current_canvas]"
#        ::completion::msg_debug "winfo_test = $winfo_test\n"

#    set offset 1 ;# how much we're backing off before starting the selection
#    set x [lindex $coords 0]
#        set x [expr {$x-$offset}]
#    set y [lindex $coords 1]
#        set y [expr {$y-$offset}]
#    set w [expr $offset+1] ;# how much to go right, then
#    set h [expr $offset+1] ;# how much to go down, then
#    ::completion::msg_debug "x = $x\n"
#    ::completion::msg_debug "y = $y\n"
#    ::completion::msg_debug "w = $w\n"
#    ::completion::msg_debug "h = $h\n"
#    ::completion::msg_debug "\[expr \{$x+$w\}\] = [expr {$x+$w}]\n"

    
#    pdsend "[winfo toplevel $::completion::current_canvas] motion $x $y 0"
#    pdsend "[winfo toplevel $::completion::current_canvas] mouse $x $y 1 0"
#    pdsend "[winfo toplevel $::completion::current_canvas] motion [expr {$x+$w}] [expr {$y+$h}] 0"
#    pdsend "[winfo toplevel $::completion::current_canvas] mouseup [expr {$x+$w}] [expr {$y+$h}] 1"

    
#    pdsend "[winfo toplevel $::completion::current_canvas] key 1 127 0" ;#delete = 127
#    pdsend "[winfo toplevel $::completion::current_canvas] key 0 127 0" ;
    #pdsend "[winfo toplevel $::completion::current_canvas] text 0" ;

    #WORK AROUND

    #QUERY INFORMATION ABOUT THE $rectlange position and mimic mouse and keyboard behavior (ghostPatching) by sendin input messages do pd engine to delete the object!

    #$::completion::current_canvas itemconfigure $::completion::current_tag TK_CONFIG_COLOR #ff0000
    
    #$::completion::current_canvas delete "all" ;#delete everything but the selected object is recreated

#}

# The keypressed and key released methods just route their input to this proc and it does the rest
proc ::completion::update_modifiers {key pressed_or_released} {
    switch -- $key {
        "Shift_L"   { set ::completion::is_shift_down $pressed_or_released }
        "Shift_R"   { set ::completion::is_shift_down $pressed_or_released }
        "Control_L" { set ::completion::is_ctrl_down $pressed_or_released }
        "Control_R" { set ::completion::is_ctrl_down $pressed_or_released }
        "Alt_L"     { set ::completion::is_alt_down $pressed_or_released }
        "Alt_R"     { set ::completion::is_alt_down $pressed_or_released }
    }
}

# receives <Key> events while listbox has focus
# some stuff is passed correctly only on KeyRelease and other stuff only on KeyPress
# so that's why there is both a lb_keyrelease and a lb_keypress procs
proc ::completion::keypress {key unicode} {
    ::completion::msg_debug "key pressed was $key.  Unicode = $unicode\n" "key_event"
    ::completion::update_modifiers $key 1
    # this is needed for users with keyboards in languages where ~ is a Multi_Key (ex: portuguese, french, etc) - only tested on PT-BR keyboard
    # tested on Windows 7 with a pt-br keyboard. This unicode "~~" is not caught on key release
    switch -- $unicode {
        "~~" { ::completion::insert_key "~" }
    }
}

# receives <KeyRelease> events while listbox has focus
# some stuff is passed correctly only on KeyRelease and other stuff only on KeyPress
# so that's why there is both a lb_keyrelease and a lb_keypress procs
proc ::completion::lb_keyrelease {key unicode} {
    ::completion::msg_debug "~lb_keys~ key released was $key    unicode = $unicode\n" "key_event"
    # We don't want to receive a key if the user pressed the plugin-activation hotkey.
    # otherwise (let's say the user is using Control+space as the hotkey) when the user activates the plugin it would output a space
    # so when we get the keydown event we wait for the keyrelease and do nothing.
    if {$::completion::waiting_trigger_keyrelease eq 1} {
        ::completion::msg_debug "got the key release. \[$key, $unicode\]\n"
        return
    }
    ::completion::update_modifiers $key 0
    set ::completion::completion_text_updated 0
    #validate keys (currently we can't detect "~" in windows because it results in a "Multi_key")
    if {[regexp {^[a-zA-Z0-9~/\._\+\-]{1}$} $key]} {
        ::completion::insert_key $key; return
    }
    switch -- $key {
        "space"     { ::completion::insert_key " " }
        "Return"    { ::completion::choose_selected }
        "BackSpace" { ::completion::chop }
        "comma" { ::completion::insert_key "," }
        "semicolon" { ::completion::insert_key ";" }
        "period" { ::completion::insert_key "." }
        "underscore" { ::completion::insert_key "_" }
        "equal" { ::completion::insert_key "+" }
        "minus" { ::completion::insert_key "-" }
        "slash" { ::completion::insert_key "/" }
        "exclam" { ::completion::insert_key "!" }
        "at" { ::completion::insert_key "@" }
        "numbersign" { ::completion::insert_key "#" }
        "dollar" { ::completion::insert_key "$" }
        "percent" { ::completion::insert_key "%" }
        "ampersand" { ::completion::insert_key "&" }
        "percent" { ::completion::insert_key "%" }
        "underscore" { ::completion::insert_key "_" }
        "plus" { ::completion::insert_key "+" }
        "minus" { ::completion::insert_key "-" }
    }
    # I've tried adding those but without success
    # maybe i should do like the solution i've used for this: 
    # https://github.com/HenriAugusto/completion-plugin/issues/21
    # "parenleft" { ::completion::insert_key "\(" }
    # "parenright" { ::completion::insert_key "\)" }
    # "bracketleft" { ::completion::insert_key "\[" }
    # "bracketright" { ::completion::insert_key "\]" }
    # "braceleft" { ::completion::insert_key "\{" }
    # "braceright" { ::completion::insert_key "\}" }
    # "backslash" { ::completion::insert_key "\\" }
}

# keys from textbox (the box where you tipe stuff in PD)
proc ::completion::text_keys {key} {
    ::completion::msg_debug "~text_keys~ key pressed was $key\n" "key_event"
    set ::completion::completion_text_updated 0
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

# this inserts the key
proc ::completion::insert_key {key} {
    ::completion::msg_debug "entering ::completion::insert_key" "entering_procs"
    scan $key %c keynum
    # pdsend "pd key 1 $keynum 0" ; notworking
    ::completion::sendKeyDown $keynum
    ::completion::msg_debug "inserting key $keynum" "char_manipulation"

    append ::completion::current_text $key
    # set ::completion::current_text [$::completion::current_canvas itemcget $::completion::current_tag -text] ;# why does this line doesn't work?

    # to debug the right line
    ::completion::search $::completion::current_text
    set ::completion::focus "canvas"
    pdtk_text_editing $::completion::toplevel $::completion::current_tag 1
    set ::completion::completion_text_updated 0
    # for some reason this does not work without passing the arg ;# what Yvan meant?
    #Those lines were making the completion windom vanish!
    #focus -force $::completion::toplevel
    #focus -force $::completion::current_canvas
}

# erases what the user typed since it started the pluging
proc ::completion::erase_text {} {
    ::completion::msg_debug "entering erase text" "entering_procs"
    # simulate backspace keys
    ::completion::msg_debug "erase_text = $::completion::erase_text" "char_manipulation"
    set i [expr {[string length $::completion::erase_text] + 2}] ;# FIXME
    while {--$i > 0} {
        ::completion::sendKeyDownAndUp 8 ;#8 = BackSpace
        incr i -1
    }
}

# this is the proc that types the object name for the user. It runs in two steps
# 1: by erasing what the user typed (calling erase_text)
# 2: typing the match chosen by the user
# Why not just send the remaining chars, you ask? It would not make sense in "skip" search mode!
# You might also wonder why not use
#       pdtk_text_selectall $::completion::current_canvas $::completion::current_tag
#                               OR
#       pdtk_text_set $::completion::current_canvas $::completion::current_tag ""
# to select everything and delete it or directly clear the text object. 
# I've tried it but it doesn't work (idky yet).
proc ::completion::replace_text {args} {
    ::completion::msg_debug "===Entering replace_text" "entering_procs"
    ::completion::erase_text
    set text ""
    if { ( !$::completion::config(auto_complete_libs) && !$::completion::is_shift_down) ||
         (  $::completion::config(auto_complete_libs) &&  $::completion::is_shift_down)
         } {
        set args [split $args /]
        set args [lindex $args end]
    }
    # if there are spaces the args variable will arrive as a list. 
    # Example: {"list" "append" "3" "4" "5"}
    # this foreach concatenates it back to a string: "list append 3 4 5"
    foreach arg $args { set text [concat $text $arg] }
    #for each char send a keydown event to PD to simulate user key presses
    for {set i 0} {$i < [string length $text]} {incr i 1} {
        set cha [string index $text $i]
        # ::completion::msg_debug "current char =  $cha"
        scan $cha %c keynum
        ::completion::sendKeyDown $keynum
    }
    set ::completion::erase_text $text
        ::completion::msg_debug "erase_text = $::completion::erase_text" "char_manipulation"
    # nasty hack: the widget does not update his text because we pretend
    # we typed the text although we faked it so pd gets it as well (mmh)
    set ::completion::completion_text_updated 1
    #set ::completion::current_text "" ; Not needed because choose_selected will empty that
}

#proc ::completion::is_special_msg { msg } {
#    switch -- $msg {
#        "plugin::rescan" {
#             ::completion::scan_all_completions 
#             return 1
#        }
#        "plugin::options" {
#            ::completion::show_options_gui
#            return 1
#        }
#        "plugin::help" {
#            ::completion::open_help_file
#            return 1
#        }
#        "plugin::completion::debug" {
#            set ::::completion::completion_debug [expr {!$::::completion::completion_debug}]
#            return 1
#        }
#    }
#    return 0
#}

#proc ::completion::add_special_messages {} {
#    set ::completion::all_externals [linsert $::completion::all_externals 0 "plugin::completion::debug"]
#    set ::completion::all_externals [linsert $::completion::all_externals 0 "plugin::help"]
#    set ::completion::all_externals [linsert $::completion::all_externals 0 "plugin::options"]
#    set ::completion::all_externals [linsert $::completion::all_externals 0 "plugin::rescan"]
#}

# called when user press Enter
proc ::completion::choose_or_unedit {} {
    ::completion::msg_debug "entering choose or unedit" "entering_procs"
    if {[winfo exists .pop] && [::completion::valid]} {
        ::completion::choose_selected
    } {
        ::completion::text_unedit
    }
}

proc ::completion::text_unedit {} {
    ::completion::msg_debug "entering text unedit" "entering_procs"
    # ceammc fix
    pdsend "$::focused_window reselect"
    set ::completion::new_object 0
    set ::completion::completion_text_updated 0
}

# this is called when the user press the BackSpace key (erases on char)
proc ::completion::chop {} {
    ::completion::msg_debug "entering chop" "entering_procs"
    #if the user press shift+backspace restart search ?????
    if {$::completion::is_shift_down} { 
        ::completion::msg_debug "shift+BackSpace = clearing search" "char_manipulation"
        ::completion::erase_text
        set ::completion::current_text ""
        ::completion::search
        return
    }
    ::completion::sendKeyDownAndUp 8 ;#8 = BackSpace
    #::completion::msg_debug "current_text before chopping $::completion::current_text"
    set ::completion::current_text [string replace $::completion::current_text end end] ;#this removes the last char (?!)
    ::completion::msg_debug "current_text after choping = $::completion::current_text" "char_manipulation"
    #::completion::msg_debug "current_text after chopping $::completion::current_text"
    ::completion::search $::completion::current_text
    #what does it do?
    if {[winfo exists .pop]} {
        .pop.f.lb selection clear 0 end
        .pop.f.lb selection set 0
    }
    # focus -force $::completion::current_canvas ;# THIS IS THE LINE THAT MAKES THE AUTOCOMPLETE VANISH AFTER BACKSPACE
    set ::completion::focus "canvas"
}

proc ::completion::popup_draw {} {
    ::completion::msg_debug "entering popup draw" "entering_procs"
    if {![winfo exists .pop]} {
        set screen_w [winfo screenwidth $::completion::current_canvas]
        set screen_h [winfo screenheight $::completion::current_canvas]
        ::completion::msg_debug "Screen width = $screen_w" "popup_gui"
        #::completion::msg_debug "Screen height = $screen_h"
        set popup_width 40
        set menuheight 32
        if {$::windowingsystem ne "aqua"} { incr menuheight 24 }
#        incr menuheight $::completion::config(offset)
        set geom [wm geometry $::completion::toplevel]
        # fix weird bug on osx
        set decoLeft 0
        set decoTop 0
        regexp -- {([0-9]+)x([0-9]+)\+([0-9]+)\+([0-9]+)} $geom -> \
            width height decoLeft decoTop
        set left [expr {$decoLeft + $::completion::editx}]
        set top [expr {$decoTop + $::completion::edity + $menuheight}]
        ::completion::msg_debug "left = $left" "popup_gui"
        ::completion::msg_debug "top = $top" "popup_gui"
        catch { destroy .pop }
        toplevel .pop
        wm overrideredirect .pop 1 
        if {$::windowingsystem eq "aqua"} {            
            raise .pop ;# without this the gui would NOT be displayed on OS X
        }
        wm geometry .pop +$left+$top
        frame .pop.f -takefocus 0

        pack configure .pop.f
        .pop.f configure -relief solid -borderwidth 1 -background white

        #is this needed?
#        switch -- $::completion::current_search_mode {
#            0 { set currentbackground $::completion::config(bg) }
#            1 { set currentbackground $::completion::config(skipbg) }
#        }

        set currentbackground $::completion::config(bg)
        
        listbox .pop.f.lb \
            -selectmode browse \
            -width $popup_width \
            -height $::completion::config(n_lines) \
            -listvariable ::completion::completions -activestyle none \
            -highlightcolor white \
            -selectbackground $currentbackground \
            -selectforeground white \
            -yscrollcommand [list .pop.f.sb set] -takefocus 1 \
            -disabledforeground #333333

#-font {-family $::completion::config(font) -size $::completion::config(font_size) -weight $::completion::config(font_weight)} \

        pack .pop.f.lb -side left -expand 1 -fill both
        .pop.f.lb configure -relief flat \
            -font [list $::completion::config(font) $::completion::config(font_size)] \
            -state normal

        pack .pop.f.lb [scrollbar ".pop.f.sb" -command [list .pop.f.lb yview] -takefocus 0] \
            -side left -fill y -anchor w
        bind .pop.f.lb <Escape> {after idle { ::completion::popup_destroy 1 }}
        bind .pop.f.lb <KeyRelease> {::completion::lb_keyrelease %K %A}
        bind .pop.f.lb <Key> {after idle {::completion::keypress %K %A}}
        # ButtonReleases:
        # LMB = 1    MMB (click) = 2     RMB = 3   ScrollUp = 4    ScrollDown = 5
        bind .pop.f.lb <ButtonRelease> {
            if { %b eq 1} {
                after idle {::completion::choose_selected}                
            }
        }

        # Overriding the Up and Down key due to a bug:
        # the .pop.f.lb selection set $updated call in ::completion::increment 
        # works but on the next non-overridden Up/Down event it would resume from the last 
        # index BEFORE the ::increment call. It would happend because of the event being dispatch 
        # to the next bindtag which would be ListBox.

        # for that reason  we could remove the ListBox bindtag. It would also avoids strange behaviour with home and end keys (that for some reason can't be overriden)
        # yet if we do this the user wouldn't be able to select the suggestions with the mouse so we leave the ListBox on the bindtags.
        #bindtags .pop.f.lb {.pop.f.lb .pop all} 

        # and then set my own bindings (Those Up and Down binds would override the Up and Down on the ListBox bindtags if they weren't removed)
        bind .pop.f.lb <Up> {::completion::increment -1 ; break}
        bind .pop.f.lb <Down> {::completion::increment 1 ; break}
        bind .pop.f.lb <Control-Up> {after idle {::completion::increment -10} ; break}
        bind .pop.f.lb <Control-Down> {after idle {::completion::increment 10} ; break}

        # I could NOT override the Next and Prior keys (Pg Up and Pg Down) without removing the ListBox bindtag. Strange hmmm probably a bug
        # also even after removing the ListBox from the bindtags those don't work! Only if i hold some modifier (so it becomes more specific).
        # probably part of the same bug
        bind .pop.f.lb <Prior> {after idle {::completion::increment -20} ; break}
        bind .pop.f.lb <Next> {after idle {::completion::increment 20} ; break}

        # bindings for Home and End
        bind .pop.f.lb <Home> {after idle {
            .pop.f.lb selection clear 0 end
            .pop.f.lb selection set 0
            .pop.f.lb see 0
            } ; break}
        bind .pop.f.lb <End> {after idle {
            .pop.f.lb selection clear 0 end
            .pop.f.lb selection set end
            .pop.f.lb see end
            } ; break}
        focus .pop.f.lb
        set ::completion::focus "pop"
        .pop.f.lb selection set 0 0
        ::completion::msg_debug "top = $top" "popup_gui"
        set height [winfo reqheight .pop.f.lb]
        # if the popup windows were going to be displayed partly off-screen let's move it left so it doesn't
        #the width is given in units of 8 pixels
        #https://core.tcl.tk/bwidget/doc/bwidget/BWman/ListBox.html#-width
        if { [expr {$left+$popup_width*8>$screen_w}] } {
            set left [expr {$screen_w-$popup_width*8} ]
            ::completion::msg_debug "left = $left" "popup_gui"
        }
        if {$::windowingsystem eq "win32"} {
            # here we assume the user did not set the taskbark  on the sides and also did not set it's size to be more than 1/7 of the screen
            incr screen_h [ expr {-1*$screen_h/7} ]
        }
        #winfo height window
        #Returns a decimal string giving window's height in pixels. When a window is first created its height will be 1 pixel; the height will eventually be changed by a geometry manager to fulfil the window's needs. If you need the true height immediately after creating a widget, invoke update to force the geometry manager to arrange it, or use winfo reqheight to get the window's requested height instead of its actual height.
        ::completion::msg_debug "@screen_h = $screen_h\n        @height = $height" "popup_gui"
        if { [expr {$top+$height>$screen_h}] } {
            set top [expr {$screen_h-$height} ]
            wm geometry .pop +$left+$top
            #.pop.f.lb configure -+
            ::completion::msg_debug "top = $top" "popup_gui"
        }
    }
}

proc ::completion::popup_destroy {{unbind 0}} {
    ::completion::msg_debug "entering popup_destroy" "entering_procs"
    catch { destroy .pop }
    focus -force $::completion::current_canvas
    set ::completion::focus "canvas"
    if {$unbind} {
        bind $::completion::current_canvas <KeyRelease> {}
    }
    set ::completion::current_text ""
}

# Henri: i don't get exactly what this does. Commenting out those packs seems 
# to have absolutely no effect in my system
# pack documentation: https://www.tcl.tk/man/tcl/TkCmd/pack.htm#M11
proc ::completion::scrollbar_check {} {
    ::completion::msg_debug "entering scrollbar_check" "entering_procs"
    if {[winfo exists .pop]} {
        if {[llength $::completion::completions] < $::completion::config(max_lines)} {
            #::completion::msg_debug "completions < max numer of lines"
            pack forget .pop.f.sb
        } else {
            #::completion::msg_debug "completions >= max numer of lines"
            pack .pop.f.sb -side left -fill y
        }
    }
}

############################################################
# utils

# `prefix' from Bruce Hartweg <http://wiki.tcl.tk/44>
proc ::completion::prefix {s1 s2} {
    regexp {^(.*).*\0\1} "$s1\0$s2" all pref
    ::completion::msg_debug "prefix output = $pref" "prefix"
    return $pref
}

#proc ::completion::try_common_prefix {} {
#    set found 0
#    set prefix [::completion::common_prefix]
#    if {$prefix ne $::completion::current_text && $prefix ne ""} {
#        ::completion::replace_text $prefix
#        # prevent errors in pdtk_text_editing
#        catch { focus .pop.f.lb }
#        set ::completion::current_text $prefix
#        set found 1
#    }
#    ::completion::msg_debug "try common prefix output = $found" "prefix"
#    return $found
#}

#proc ::completion::common_prefix {} {
#    set prefix ""
#    if {[llength $::completion::completions] > 1} {
#        set prefix [::completion::prefix \
#                        [lindex $::completion::completions 0] \
#                        [lindex $::completion::completions end]]
#    }
#    ::completion::msg_debug "common prefix output = $prefix" "prefix"
#    return $prefix
#}

proc ::completion::trimspaces {} {
    set ::completion::current_text [string trimright $::completion::current_text " "]
}

# just for testing purposes. Code would need to become more robust before 
# being used to display stuff for the user
#proc ::completion::msgbox {str} {
#    toplevel .cpMsgBox$str
#    frame .cpMsgBox$str.f
#    label .cpMsgBox$str.f.l -text "$str" -padx 3m -pady 2m
#    button .cpMsgBox$str.f.okbtn -text "okay" -command "destroy .cpMsgBox$str"
    
#    pack .cpMsgBox$str.f
#    pack .cpMsgBox$str.f.l
#    pack .cpMsgBox$str.f.okbtn
#}


# just in case.
# ceammc fix
bind all <$::modifier-Key-Return> {pdsend "$::focused_window reselect"}

###########################################################
# main

::completion::init
