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

# ==========================================

# The original version was developed by Yvan Volochine a long time ago (as the time of writing: 7 years ago)
# I then embraced the project and i'm willing to mantain it for as long as i can. 
# Right now i'm kind of new to .tcl so i'm trying to stay true to the original code.
# When reasonable i will comment talking about changes i did and why.
#
# https://github.com/HenriAugusto/completion-plugin
#
# Henri Augusto


package require Tcl 8.5
package require pd_menucommands 0.1

namespace eval ::completion:: {
    variable ::completion::config
    variable external_filetype ""
}

###########################################################
# overwritten
rename pdtk_text_editing pdtk_text_editing_old

############################################################
# GLOBALS

set ::completion::plugin_version "0.47.1"

# default
set ::completion::config(save_mode) 1 ;# save keywords (s/r/array/table/...)
set ::completion::config(max_lines) 20
set ::completion::config(font) "DejaVu Sans Mono"
set ::completion::config(font_size) 8 ;# should load pd's default
set ::completion::config(bg) "#0a85fe"
set ::completion::config(skipbg) "#0ad871"
set ::completion::config(monobg) "#9832ff"
set ::completion::config(fg) white
set ::completion::config(offset) 0
set ::completion::config(max_scan_depth) 1
set ::completion::config(auto_complete_libs) 1

# some nice colors to try: #0a85fe #0ad871 #9832ff ; those are great: #ff9831 #ff00ee #012345

# private variables

set ::completion_plugin_path ""
set ::toplevel ""
set ::current_canvas ""
set ::current_tag ""
set ::current_text ""
set ::erase_text ""
set ::completions {"(empty)"}
set ::new_object false
set ::editx 0
set ::edity 0
set ::focus ""
set ::completion_text_updated 0
set ::is_shift_down 0
set ::is_ctrl_down 0
set ::is_alt_down 0
set ::waiting_trigger_keyrelease 0
# =========== [DEBUG mode on/off] ============
#1 = true 0 = false
set ::completion_debug 0 ;
# debug categories
set ::debug_loaded_externals 0 ;#prints loaded externals
set ::debug_entering_procs 1 ;#prints a message when entering a proc
set ::debug_key_event 0 ;#prints a message when a key event is processed
set ::debug_searches 0 ;#messages about the performed searches
set ::debug_popup_gui 0 ;#messages related to the popup containing the code suggestions
set ::debug_char_manipulation 0 ;#messages related to what we are doing with the text on the obj boxes (inserting/deleting chars)
set ::debug_unique_names 0 ;#messages related to storing [send/receive] names [tabread] names and alike.
set ::debug_settings 1 ;#messages related to storing [send/receive] names [tabread] names and alike.
set ::debug_prefix 0 ;#messages related to storing [send/receive] names [tabread] names and alike.

#0 = normal
#1 = skipping
#2 = monolithic
set ::current_search_mode 0

# all pd VANILLA objects
set ::all_externals {}

set ::monolithic_externals {}

set ::loaded_libs {}

#useful function for debugging
proc ::completion::debug_msg {dbgMsg {debugKey "none"}} {
    switch -- $debugKey {
        "none" {}
        "loaded_externals" { if { !$::debug_loaded_externals } { return  } }
        "entering_procs" { if { !$::debug_entering_procs } { return  } }
        "key_event" { if { !$::debug_key_event } { return  } }
        "searches" { if { !$::debug_searches } { return  } }
        "popup_gui" { if { !$::debug_popup_gui } { return  } }
        "char_manipulation" { if { !$::debug_char_manipulation } { return  } }
        "unique_names" { if { !$::debug_unique_names } { return  } }
        "settings" { if { !$::debug_settings } { return  } }
        "prefix" { if { !$::debug_prefix } { return  } }
    }
    if { $::completion_debug } {
        ::pdwindow::post "autocmpl_dbg: $dbgMsg\n"
    }
}

# This function sends keydown messages to pd
# It is better to use a separate function instead of hardcoded pdsend messages like Yvan was doing because the pd tcl api might change. 
# In fact when i took the project that was one of the major bugs with it. It was using pdsend "pd key 1 $keynum 0" which where not working.
# So using functions (procs) promotes mantainability because you only have to change their implementation to fix the code after api changes.
proc ::completion::sendKeyDown {keynum} {
    pdsend "[winfo toplevel $::current_canvas] key 1 $keynum 0"
}

# This function sends keydown and then keyup messages to pd
proc ::completion::sendKeyDownAndUp {keynum} {
    pdsend "[winfo toplevel $::current_canvas] key 1 $keynum 0"
    pdsend "[winfo toplevel $::current_canvas] key 0 $keynum 0"
}

#called once upon plugin initialization
proc ::completion::init {} {
    set initTime [clock milliseconds]
    variable external_filetype
    set ::completion_plugin_path "$::current_plugin_loadpath"
    ::pdwindow::post "\[completion-plugin\] version $::completion::plugin_version\n"
    ::completion::read_config
    #::completion::read_extras
    switch -- $::windowingsystem {
        "aqua"  { set external_filetype *.pd_darwin }
        "win32" { set external_filetype *.dll}
        "x11"   { set external_filetype *.pd_linux }
    }
    if {[catch {bind "completion-plugin" <$completion::config(hotkey)> {::completion::trigger; break;}} err]} {
        ::pdwindow::post "\n---Error while trying to bind the completion plugin hotkey---\n"
        ::pdwindow::post "      hotkey: $::completion::config(hotkey)\n"
        ::pdwindow::post "      err: $err\n\n"
    }
    
    
    ::completion::scan_all_completions
    ::completion::init_options_menu
    set finalTime [clock milliseconds]
    set delta [expr {$finalTime-$initTime}]
    set count [llength $::all_externals]
    set count [expr {$count+[llength $::monolithic_externals]}]
    ::pdwindow::post "\[completion-plugin\] loaded $count completions in $delta milliseconds\n"
}

proc ::completion::scan_all_completions {} {
    set ::all_externals {hslider vslider bng cnv bang float symbol int send receive select route pack unpack trigger spigot moses until print makefilename change swap value list {list append} {list fromsybmol} {list length} {list prepend} {list split} {list store} {list tosymbol} {list trim} delay metro line timer cputime realtime \
    pipe + - * / pow == != > < >= <= & && | || % << >> mtof powtodb rmstodb ftom dbtopow dbtorms mod div sin cos tan atan atan2 sqrt log exp abs random max min clip wrap notein ctlin pgmin bendin touchin polytouchin midiin sysexin midirealtimein midiclkin noteout ctlout pgmout bendout touchout polytouchout midiout makenote stripnote \
    oscparse oscformat tabread tabread4 tabwrite soundfiler table array loadbang netsend netreceive glist textfile text openpanel savepanel bag poly key keyup keyname declare +~ -~ *~ /~ max~ min~ clip~ sqrt~ rsqrt~ q8_sqrt~ q8_rsqrt~ wrap~ fft~ ifft~ rfft~ rifft~ pow~ log~ exp~ abs~ framp~ mtof~ ftom~ rmstodb~ dbtorms~ dac~ adc~ sig~ line~ vline~ \
    threshdold~ snapshot~ vsnapshot~ bang~ samplerate~ send~ receive~ throw~ catch~ block~ switch~ readsf~ writesf~ phasor~ cos~ osc~ tabwrite~ tabplay~ tabread~ tabread4~ tabosc4~ tabsend~ tabreceive~ vcf~ noise~ env~ hip~ lop~ bp~ biquad~ samphold~ print~ rpole~ rzero~ rzero_rev~ cpole~ czero~ czero_rev~ delwrite~ delread~ delread4~ vd~ inlet outlet inlet~ outlet~ clone \
    struct drawcurve filledcurve drawpolygon filledpolygon plot drawnumber drawsymbol pointer get set element getsize setsize append scalar sigmund~ bonk~ choice hilbert~ complet-mod~ expr expr~ fexpr~ loop~ lrshift~ pd~ stdout~ rev1~ rev2~ rev3~ bob~ namecanvas savestate pdcontrol slop~}
    set ::monolithic_externals {}
    ::completion::add_user_externals
    ::completion::add_user_customcompletions
    ::completion::add_user_monolithiclist
    set ::loaded_libs {} ;#clear the loaded_libs because it was only used to scan the right objects located in multi-object distributions
    set ::all_externals [lsort $::all_externals]
    ::completion::add_special_messages ;#AFTER sorting
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

#opens the plugin's help file (as called from the configuration window)
proc ::completion::open_help_file {} {
    set filename [file join $::completion_plugin_path "README.pd"]
    open_file "$filename"
}

proc ::completion::show_options_gui {} {
    if {[winfo exists .options]} {
        focus .options
        return
    }
    toplevel .options
    wm title .options "Pd AutoComplete Settings"

    frame .options.f -padx 5 -pady 5
    label .options.f.title_label -text "PD AutoComplete Settings"
    .options.f.title_label configure -font [list $::completion::config(font) [expr {$::completion::config(font_size)+3}]]
    
    label .options.f.status_label -text "" -foreground "#cc2222"

    # COLORS
    #note that we are using KeyRelease bindings because using "-validate key" would not validate in the right time.
    #ex: you would type a valid string #aabbcc and it would be invalid. Then on the next keypress (whichever it was) it would process #aabbcc

    #Options for background color
    label .options.f.click_to_choose_label -text "click to\nchoose"
    
    label .options.f.bg_label -text "bkg color"
    entry .options.f.bg_entry -width 8
    frame .options.f.bg_demo -background $::completion::config(bg) -width 40 -height 40
        bind .options.f.bg_demo <ButtonRelease> { ::completion::user_select_color "bg"}
    bind .options.f.bg_entry <KeyRelease> { ::completion::gui_options_update_color ".options.f.bg_entry" ".options.f.bg_demo" "bg" }
    

    #Options for skipping mode background color
    label .options.f.skip_bg_label -text "skipping bkg color"
    entry .options.f.skip_bg_entry -width 8
    frame .options.f.skip_bg_demo -background $::completion::config(skipbg) -width 40 -height 40
        bind .options.f.skip_bg_demo <ButtonRelease> { ::completion::user_select_color "skipbg"}
    bind .options.f.skip_bg_entry <KeyRelease> { ::completion::gui_options_update_color ".options.f.skip_bg_entry" ".options.f.skip_bg_demo" "skipbg" }
    
    #Options for monolithic mode background color
    label .options.f.mono_bg_label -text "mono-object bkg color"
    entry .options.f.mono_bg_entry -width 8
    frame .options.f.mono_bg_demo -background $::completion::config(monobg) -width 40 -height 40
        bind .options.f.mono_bg_demo <ButtonRelease> { ::completion::user_select_color "monobg"}
    bind .options.f.mono_bg_entry <KeyRelease> { ::completion::gui_options_update_color ".options.f.mono_bg_entry" ".options.f.mono_bg_demo" "monobg" }
    

    #Misc
    checkbutton .options.f.auto_complete_libs -variable ::completion::config(auto_complete_libs) -onvalue 1 -offvalue 0
    label .options.f.auto_complete_libs_label -text "auto complete library names"

    spinbox .options.f.number_of_lines -width 6 -from 3 -to 30 -textvariable ::completion::config(max_lines)
    label .options.f.number_of_lines_label -text "number of lines to display"
    
    spinbox .options.f.maximum_scan_depth -width 6 -from 0 -to 10 -textvariable ::completion::config(max_scan_depth)
    label .options.f.maximum_scan_depth_label -text "maximum scan depth"

    spinbox .options.f.font_size -width 6 -from 7 -to 20 -textvariable ::completion::config(font_size)
    label .options.f.font_size_label -text "font size"

    #Hotkey
    label .options.f.hotkeylabel -text "hotkey (require save&restart)"
    entry .options.f.hotkeyentry -width 22
    .options.f.hotkeyentry insert 0 "$::completion::config(hotkey)"
    bind .options.f.hotkeyentry <KeyRelease> {
        set ::completion::config(hotkey) [.options.f.hotkeyentry get]
    }
    
    #Buttons
    button .options.f.save_btn -text "save to file" -command ::completion::write_config
    button .options.f.default_btn -text "default" -command ::completion::restore_default_option
    button .options.f.rescan_btn -text "rescan" -command ::completion::scan_all_completions
    button .options.f.help_btn -text "help" -command ::completion::open_help_file
    #.options.f.help_btn configure -font {-family courier -size 12 -weight bold -slant italic}
    .options.f.help_btn configure -font {-weight bold}


    set padding 2

    # ::::GRID::::

    #setup main frame stuff
    grid .options.f -column 0 -row 0
    grid .options.f.title_label -column 0 -row 0 -columnspan 3 -padx $padding -pady $padding

    #setup the rest
    set current_row 1

    #auto complete libs
    grid .options.f.auto_complete_libs_label -column 0 -row $current_row -padx $padding -pady $padding -sticky "e"
    grid .options.f.auto_complete_libs -column 1 -row $current_row -padx $padding -pady $padding -sticky "w"
    incr current_row

    #number of lines
    grid .options.f.number_of_lines_label -column 0 -row $current_row -padx $padding -pady $padding -sticky "e"
    grid .options.f.number_of_lines -column 1 -row $current_row -padx $padding -pady $padding -sticky "w"
    incr current_row

    #font size
    grid .options.f.font_size_label -column 0 -row $current_row -padx $padding -pady $padding -sticky "e"
    grid .options.f.font_size -column 1 -row $current_row -padx $padding -pady $padding -sticky "w"
    incr current_row

    #maximum scan depth
    grid .options.f.maximum_scan_depth_label -column 0 -row $current_row -padx $padding -pady $padding -sticky "e"
    grid .options.f.maximum_scan_depth -column 1 -row $current_row -padx $padding -pady $padding -sticky "w"

    grid .options.f.click_to_choose_label -column 2 -row $current_row -padx $padding -pady $padding
    incr current_row

    # change background color
    grid .options.f.bg_label -column 0 -row $current_row -padx $padding -pady $padding -sticky "e"
    grid .options.f.bg_entry -column 1 -row $current_row -padx $padding -pady $padding -sticky "w"
    grid .options.f.bg_demo -column 2 -row $current_row -padx $padding -pady $padding
    incr current_row

    # change skip mode background color
    grid .options.f.skip_bg_label -column 0 -row $current_row -padx $padding -pady $padding -sticky "e"
    grid .options.f.skip_bg_entry -column 1 -row $current_row -padx $padding -pady $padding -sticky "w"
    grid .options.f.skip_bg_demo -column 2 -row $current_row -padx $padding -pady $padding
    incr current_row

    # change mono mode background color
    grid .options.f.mono_bg_label -column 0 -row $current_row -padx $padding -pady $padding -sticky "e"
    grid .options.f.mono_bg_entry -column 1 -row $current_row -padx $padding -pady $padding -sticky "w"
    grid .options.f.mono_bg_demo -column 2 -row $current_row -padx $padding -pady $padding
    incr current_row
    
    #hotkey stuff
    grid .options.f.hotkeylabel -column 0 -row $current_row -padx $padding -pady $padding
    grid .options.f.hotkeyentry -column 1 -row $current_row -padx $padding -pady $padding
    incr current_row

    # Status labels and buttons
    #Is the status label used?
    #grid .options.f.status_label -column 0 -row $current_row -padx $padding -pady 8 -sticky "e"
    grid .options.f.default_btn -column 1 -row $current_row -padx $padding -pady 8 -sticky "e"
    grid .options.f.save_btn -column 2 -row $current_row -padx $padding -pady 8 -sticky "w"
    grid .options.f.help_btn -column 0 -row $current_row -padx $padding -pady 8 -sticky "w"
    incr current_row
    grid .options.f.rescan_btn -column 2 -row $current_row -padx $padding -pady 4 -sticky "ew"

    ::completion::update_options_gui
}

proc ::completion::update_options_gui {} {
    .options.f.status_label configure -text ""
    .options.f.bg_demo configure -background $::completion::config(bg)
    .options.f.skip_bg_demo configure -background $::completion::config(skipbg)
    .options.f.mono_bg_demo configure -background $::completion::config(monobg)
    .options.f.bg_entry delete 0 end
    .options.f.bg_entry insert 0 $::completion::config(bg)
    .options.f.skip_bg_entry delete 0 end
    .options.f.skip_bg_entry insert 0 $::completion::config(skipbg)
    .options.f.mono_bg_entry delete 0 end
    .options.f.mono_bg_entry insert 0 $::completion::config(monobg)
    .options.f.hotkeyentry delete 0 end
    .options.f.hotkeyentry insert 0 $::completion::config(hotkey)
}

proc ::completion::restore_default_option {} {
    set ::completion::config(hotkey) "Control-space"
    set ::completion::config(max_lines) 20
    set ::completion::config(font) "DejaVu Sans Mono"
    set ::completion::config(font_size) 8
    set ::completion::config(bg) "#0a85fe"
    set ::completion::config(skipbg) "#0ad871"
    set ::completion::config(monobg) "#9832ff"
    set ::completion::config(fg) white
    set ::completion::config(offset) 0
    set ::completion::config(max_scan_depth) 1
    set ::completion::config(auto_complete_libs) 1
    ::completion::update_options_gui
}

proc ::completion::gui_options_update_color {entryWidget frameWidget configTag} {
    if { [regexp {^\#(\d|[a-f]){6}$} [$entryWidget get]] } {
        set ::completion::config($configTag) [$entryWidget get]
        $frameWidget configure -background $::completion::config($configTag)
        # chagne color to show it's valid
        $entryWidget configure -foreground #000000
    } else {
        # chagne color to show it's INvalid
        $entryWidget configure -foreground #de2233
    }
}

# taken from kiosk-plugin.tcl by Iohannes
proc ::completion::read_config {{filename completion.cfg}} {
    if {[file exists $filename]} {
        set fp [open $filename r]
    } else {
        set filename [file join $::completion_plugin_path $filename]
        if {[file exists $filename]} {
            set fp [open $filename r]
        } else {
            ::completion::debug_msg "completion.cfg not found"
            return False
        }
    }
    while {![eof $fp]} {
        set data [gets $fp]
        if { ![regexp {^\w} $data] } {
            continue ;#this line doesn't start with a char
        }
        # if the user provided the key value pair
        ::completion::debug_msg "data length = [llength $data]" "settings"
        if { [llength $data ] == 2} {
            set ::completion::config([lindex $data 0]) [lindex $data 1]
            ::completion::debug_msg "::completion::config([lindex $data 0]) = $::completion::config([lindex $data 0])" "settings"
        } elseif { [llength $data ] > 2} {
            set ::completion::config([lindex $data 0]) [lrange $data 1 end]
            ::completion::debug_msg "::completion::config([lindex $data 0]) = $::completion::config([lindex $data 0])" "settings"
        } else {
            ::completion::debug_msg "ERROR reading ::completion::config([lindex $data 0]) => data = $data" "settings"
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
        set filename [file join $::completion_plugin_path $filename]
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
    set lines [::completion::write_config_variable $lines "max_lines"]
    set lines [::completion::write_config_variable $lines "font"]
    set lines [::completion::write_config_variable $lines "font_size"]
    set lines [::completion::write_config_variable $lines "max_scan_depth"]
    set lines [::completion::write_config_variable $lines "auto_complete_libs"]
    set lines [::completion::write_config_variable $lines "bg"]
    set lines [::completion::write_config_variable $lines "fg"]
    set lines [::completion::write_config_variable $lines "skipbg"]
    set lines [::completion::write_config_variable $lines "monobg"]
    set lines [::completion::write_config_variable $lines "offset"]

    #write the file
    set fp [open $filename w]
    if { $had_to_create_file } {
        set lines [linsert $lines 0 "This file was generated by PD AutoComplete in the absence of the original file that comes with the plugin.\n"]
    }
    puts $fp [join $lines "\n"]
    close $fp
    #.options.f.status_label configure -text "saved!"
    #after 1000 { .options.f.status_label configure -text "" }
}

proc ::completion::write_config_variable {file_lines name} {
    #https://stackoverflow.com/a/37812995/5818209   for writing to specific lines
    ::completion::debug_msg "Saving config($name)" "settings"
    set pattern ^$name\\s ;# do NOT enclose this in brackets
    ::completion::debug_msg "pattern = $pattern" "settings"
    set index 0
    set found false
    foreach line $file_lines {
        #::completion::debug_msg "line = $line"
        if {[regexp $pattern $line]} {
            ::completion::debug_msg "current variable's line = $line" "settings"
            set file_lines [lreplace $file_lines $index $index "$name $::completion::config($name)"]
            ::completion::debug_msg "line AFTER = $line" "settings"
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

proc ::completion::user_select_color {target} {
    set color [tk_chooseColor -title "AutoComplete settings: Choose a color" -initialcolor $::completion::config($target)]
    if { $color eq ""} { 
        return 
    }
    set ::completion::config($target) $color
    ::completion::update_options_gui
}

# this function looks for objects in the current folder and recursively call itself for each subfolder
# we read the subfolders because pd reads the subpatches!
proc ::completion::add_user_externalsOnFolder {{dir .} depth} {
    variable external_filetype
    if { [expr {$depth > $::completion::config(max_scan_depth)}] } {
        return
    }
    #::completion::debug_msg "external_filetype = $external_filetype" ;#just for debugging
    ::completion::debug_msg "===add_user_externalsOnFolder $dir===" "loaded_externals"
    ::completion::debug_msg "depth =  $depth" "loaded_externals"

    # i concatenate the result of two globs because for some reason i can't use glob with two patterns. I've tried using: {$external_filetype,*.pd}
    # list of pd files on the folder
    set pd_files [glob -directory $dir -nocomplain -types {f} -- *.pd] 
    #List of system depentent (*.pd_darwin, *.dll, *.pd_linux) files on the folder
    set sys_dependent_files [glob -directory $dir -nocomplain -types {f} -- $external_filetype]
    set all_files [concat $pd_files $sys_dependent_files]
    # for both types of files
    foreach filepath $all_files {
        ::completion::debug_msg "     external = $filepath" "loaded_externals"
        set file_tail [file tail $filepath] ;#this one contains the file extension
        set name_without_extension [file rootname $file_tail]
        set dir_name [file dirname $filepath] 
        set how_many_folders_to_get [expr {$depth+0}]
        set folder_name [lrange [file split $filepath] end-$how_many_folders_to_get end-1 ]
        set extension_path [join $folder_name \/]
        if {$extension_path ne ""} {
            set extension_path $extension_path\/
        }
            ::completion::debug_msg "       depth =  $depth" "loaded_externals"
            ::completion::debug_msg "       filepath = $filepath" "loaded_externals"
            ::completion::debug_msg "       dir_name = $dir_name" "loaded_externals"
            ::completion::debug_msg "       folder_name = $folder_name" "loaded_externals"
            ::completion::debug_msg "       extension_path = $extension_path" "loaded_externals"
            ::completion::debug_msg "       file_tail = $file_tail" "loaded_externals"
            ::completion::debug_msg "       name_without_extension = $name_without_extension" "loaded_externals"
        if {[string range $name_without_extension end-4 end] ne "-help"} {
            lappend ::all_externals $extension_path$name_without_extension
            lappend ::loaded_libs $extension_path
        }
    }
    #do the same for each subfolder (recursively)
    set depth [expr {$depth+1}]
    foreach subdir [glob -nocomplain -directory $dir -type d *] {
        ::completion::add_user_externalsOnFolder $subdir $depth
    }
}

# this proc runs the main search ::completion::add_user_externalsOnFolder into each main folder
proc ::completion::add_user_externals {} {
    ::completion::debug_msg "-----searching externals on the following directories:-----" "loaded_externals"
    foreach DIR $::sys_searchpath {
        ::completion::debug_msg "$DIR" "loaded_externals"
    }
    foreach DIR $::sys_staticpath {
        ::completion::debug_msg "static path $DIR" "loaded_externals"
    }
    #for each directory the user set in edit->preferences->path
    set dynamic_and_static_paths [concat $::sys_searchpath $::sys_staticpath]
    set pathlist $::sys_staticpath
    set pathlist $dynamic_and_static_paths
    foreach pathdir $pathlist {
        set dir [file normalize $pathdir]
        if { ! [file isdirectory $dir]} { ;#why Yvan was doing this check?
            continue
        }
        ::completion::add_user_externalsOnFolder $pathdir 0
        #foreach subdir [glob -directory $dir -nocomplain -types {d} *] {
        #    ::completion::add_user_externalsOnFolder $subdir 1
        #}
    }
    #remove duplicates from the loaded_libs
    set ::loaded_libs [lsort -unique $::loaded_libs]
}



#adds any completion set in any txt file under "custom_completions"
proc ::completion::add_user_customcompletions {} {
    ::completion::debug_msg "entering add user object list" "entering_procs"
    set userdir [file join $::completion_plugin_path "custom_completions"]
    foreach filename [glob -directory $userdir -nocomplain -types {f} -- \
                         *.txt] {
        ::completion::read_completionslist_file $filename
    }
}

#reads objects stored into monolithic files (*.pd_darwin, *.dll, *.pd_linux)
proc ::completion::add_user_monolithiclist {} {
    ::completion::debug_msg "entering add user monolithic list" "entering_procs"
        ::completion::debug_msg "::loaded_libs = $::loaded_libs" "loaded_externals"
    set userdir [file join $::completion_plugin_path "monolithic_objects"]

    # for each .txt file in /monolithic_objects
    foreach filename [glob -directory $userdir -nocomplain -types {f} -- \
                         *.txt] {
        #  Slurp up the data file
        set fp [open $filename r]
        set file_data [read $fp]
        foreach line $file_data {

            # gets the lib name from the string
            set lib [lindex [split $line /] 0]
            set lib ${lib}/ ;# turns libName into libName/

            # only if the user actually have that library installed
            if { [expr [lsearch -nocase $::loaded_libs $lib] >= 0 ] } {
                lappend ::monolithic_externals [split $line /]
            }

        }
        close $fp
        ::completion::debug_msg "======monolithic externals=======\n$::monolithic_externals" "loaded_externals"
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
                && [lsearch -exact $::all_externals $line] == -1} {
                lappend ::all_externals $line
            }
        }
        close $fl
    }
}

###########################################################
#                      overwritten                        #
###########################################################
proc pdtk_text_editing {mytoplevel tag editing} {
    ::completion::debug_msg "entering overwritten pdtk text editing" "entering_procs"
    #::completion::debug_msg "   mytoplevel = $mytoplevel"
    #::completion::debug_msg "   tag = $tag"
    #::completion::debug_msg "   editing = $editing"
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
        # Henri: Yvan originally called set_empty_listbox. Doens't seem to make sense. It wouldn't even reset ::current_text
        ::completion::popup_destroy
        set ::completion_text_updated 0
        # store keywords. Henri: i'm disabling that. See developmentGuide.md
        #if {$::completion::config(save_mode)} {
        #    set text [$tkcanvas itemcget $::current_tag -text]
        #    ::completion_store $text
        #}
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

        if {[string first "completion-plugin" [bindtags $::current_canvas] ] eq -1} {
            bindtags $::current_canvas "completion-plugin [bindtags $::current_canvas]"
        }
        #delete_if {[string first [bindtags $::current_canvas] "test"] eq -1} {
        #delete_    bindtags $::current_canvas "test [bindtags $::current_canvas]"
        #delete_}
        #delete_#bind $::current_canvas <$completion::config(hotkey)> {+::completion::trigger;break;}
        #delete_::pdwindow::post "-----EDITING------\n"
        #delete_::pdwindow::post "current_canvas: $::current_canvas\n"
        #delete_set dbg [bindtags $::current_canvas]
        #delete_::pdwindow::post "bindtags: $dbg\n"
    }
    set ::new_object $editing
    $tkcanvas focus $tag
    set ::focus "tag"
}

# this is called when the user enters the auto completion mode
proc ::completion::trigger {} {
    ::completion::debug_msg "===entering trigger===" "entering_procs"
    set ::waiting_trigger_keyrelease 1
        
    set ::is_shift_down 0
    set ::is_ctrl_down 0
    set ::is_alt_down 0
    if {$::current_canvas ne ""
        && $::current_text eq ""
        && ! $::completion_text_updated
    } {
        #this code is responsible for reading any text already present in the object when you enter the autocomplete mode
        set ::current_text [$::current_canvas itemcget $::current_tag -text]
        ::completion::trimspaces
        ::completion::debug_msg "Text that was already in the box = $::current_text\n" "searches"
    }

    ::completion::debug_msg "-----TRIGGER------\n"
    ::completion::debug_msg "current_canvas: $::current_canvas\n"
    set dbg [bindtags $::current_canvas]
    ::completion::debug_msg "bindtags: $dbg\n"

    #if the user is typing into an object box
    if {$::new_object} {

            # detect if the user is typing on an object, message or comment
            set ::tags_on_object_being_edited [$::current_canvas itemcget $::current_tag -tags]
                ::completion::debug_msg "\[$::current_canvas itemcget $::current_tag -tags\] = $::tags_on_object_being_edited"
            set ::type_of_object_being_edited [lindex $::tags_on_object_being_edited 1]
                ::completion::debug_msg "------>::type_of_object_being_edited = $::type_of_object_being_edited \n"
            if { ($::type_of_object_being_edited ne "obj") && ($::type_of_object_being_edited ne "msg") } {
                ::completion::debug_msg "the completion-plugin does not trigger for objects of type $::type_of_object_being_edited"
                return
            }

            bind $::current_canvas <KeyRelease> {::completion::text_keys %K}
            set completed_because_was_unique 0
            if {![winfo exists .pop]} {
                    ::completion::popup_draw
                    ::completion::search $::current_text
                    ::completion::try_common_prefix
                    ::completion::update_completions_gui
                    if {[::completion::unique] } {
                        ::completion::choose_selected ;#Henri: was replace_text. This is needed for the three modes
                        ::completion::popup_destroy
                        ::completion::set_empty_listbox
                        set completed_because_was_unique 1
                    }
            } else {
                    
                    if {[::completion::unique]} {
                        ::completion::choose_selected
                        set completed_because_was_unique 1
                    } elseif { [llength $::completions] > 1 } {
                        if {![::completion::try_common_prefix]} {
                            ::completion::debug_msg "IF not common prefix\n"
                            #::completion::increment ;#Henri: this would allow to cycle through the completions with Tab. I'm disabling that in favor of the arrow keys
                        } else {
                            ::completion::debug_msg "IF INDEED common prefix\n"
                        }
                    }
            }
            # if the unique completion was used there will be no .pop to bind!
            if { !$completed_because_was_unique } {
                # work in progress
                # bind .pop <FocusOut> {::completion::debug_msg "the user has unfocused the popup"; ::completion::popup_destroy }
                # bind $::current_canvas <FocusOut> {::completion::debug_msg "the user has unfocused the canvas"} 
            }
    } else {
        ::completion::debug_msg "the user is NOT typing into an object box" "key_event"
    }
    # this should be time enough for the user to release the keys (so we don't capture the release keys of the plugin hotkey)
    after 200 {
        ::completion::debug_msg "accepting keys\n"
        set ::waiting_trigger_keyrelease 0
    }
}

proc ::completion::monolithic_search {{text ""}} {
    #set variables related to monolithic_search
    ::completion::debug_msg "::completion::monolithic_search($text)" "searches"
    set ::current_search_mode 2
    if {[winfo exists .pop]} {
        .pop.f.lb configure -selectbackground $::completion::config(monobg)
    }
    #do the search
    set text [string range $text 1 end]
    set results {}
    ::completion::debug_msg "::completion::monolithic_search($text)" "searches"
    foreach elem $::monolithic_externals {
        #those are stored into a {libName objName} fashion
        set libraryName [lindex $elem 0]
        set objName [lindex $elem 1]
        ::completion::debug_msg "::completion::monolithic_search\[$libraryName\/$objName\]" "searches"
        lappend results "$libraryName\/$objName"
    }
    #::completion::debug_msg "----------results=\[$results"
    set pattern "$text"
    set pattern [::completion::fix_pattern $pattern]
    set ::completions [lsearch -all -inline -regexp -nocase $results $pattern]
}

proc ::completion::skipping_search {{text ""}} {
    #set variables related to skipping_search
    ::completion::debug_msg "::completion::skipping_search($text)" "searches"
    set ::current_search_mode 1
    # do we really need to check if the popup exists?
    if {[winfo exists .pop]} {
        .pop.f.lb configure -selectbackground $::completion::config(skipbg)
    }
    #do the search
    set text [string range $text 1 end]
    set text [::completion::fix_pattern $text]
    set chars [split $text {}]
    set pattern ""
    foreach char $chars {
        ::completion::debug_msg "--------------char = $char"
        set pattern "$pattern$char.*"
    }
    ::completion::debug_msg "RegExp pattern  = $pattern" "searches"
    ::completion::debug_msg "--------------chars = $chars" "searches"
    set ::completions [lsearch -all -inline -regexp -nocase $::all_externals $pattern]
}

# Searches for matches.
# (this method detects the current search mode and returns after calling the right one it it happens to be monolithic or skipping.)
proc ::completion::search {{text ""}} {
    ::completion::debug_msg "::completion::search($text)" "searches"
    ::completion::debug_msg "::completion_text_updated = $::completion_text_updated" "searches"
    # without the arg there are some bugs when keys come from listbox ;# what Yvan meant?
    set ::erase_text $::current_text
    #if starts with a . it is a skipping search
    #if starts with a , it is a monolithic search
    if {[string range $text 0 0] eq ","} {
        ::completion::monolithic_search $text
        return
    } elseif {[string range $text 0 0] eq "."} {
        ::completion::skipping_search $text
        return
    }
    # Else just do the normal search
    if {[winfo exists .pop.f.lb]} {
        .pop.f.lb configure -selectbackground $::completion::config(bg)
    }
    set ::current_search_mode 0
    if {$text ne ""} {
        ::completion::debug_msg "=searching for $text=" "searches"
        set ::current_text $text
        set ::erase_text $text
        set ::should_restore False
    } elseif { !$::completion_text_updated } {
        ::completion::debug_msg "searching for empty string" "searches"
        #set ::current_text \
            [$::current_canvas itemcget $::current_tag -text]
        set ::previous_current_text $::current_text ;# saves the current text
        ::completion::debug_msg "original current_text: $::current_text" "searches"
        set ::current_text ""
        ::completion::debug_msg "replaced current_text is $::current_text" "searches"
        set ::should_restore True
    }
    ::completion::trimspaces

    # Now this part will always run so you can perform "empty searchs" which will return all objects. In Yvan's code it would clear completions on an "empty search"
    #Yvan was using -glob patterns but they wouldn't match stuff with forward slashes (/)
    #for example if you type "freq" it wouldn't match cyclone/freqshift~
    #using -regexp now allows for that
    #Also i've added case insensitive searching (since PD object creation IS case-insensitive).
    set pattern "$::current_text"
    set pattern [::completion::fix_pattern $pattern]

    set ::completions [lsearch -all -inline -regexp -nocase $::all_externals $pattern]
    if {$::should_restore} {
        set ::current_text $::previous_current_text ;# restores the current text
        ::completion::debug_msg "restored current_text: $::current_text" "searches"
    }
    ::completion::update_completions_gui
    ::completion::debug_msg "SEARCH END! Current text is $::current_text" "searches"
}

# This is a method that edits a string used as a regex pattern escaping chars in order to correcly compile the regexp;
# example: we must escape "++" to "\\+\\+". 
proc ::completion::fix_pattern {pattern} {
        ::completion::debug_msg "================== - pattern = $pattern" "searches"
    set pattern [string map {"+" "\\+"} $pattern]
        ::completion::debug_msg "+ - pattern = $pattern" "searches"
    set pattern [string map {"*" "\\*"} $pattern]
        ::completion::debug_msg "* - pattern = $pattern" "searches"
    set skippingPrefix [string range $pattern 0 0]
        ::completion::debug_msg "skippingPrefix = $skippingPrefix" "searches"
    set skippingString [string range $pattern 1 end]
        ::completion::debug_msg "skippingString = $skippingString" "searches"
    set skippingString [string map {"." "\\."} $skippingString]
        ::completion::debug_msg ". skippingString = $skippingString" "searches"
    set pattern "$skippingPrefix$skippingString"
        ::completion::debug_msg ". - pattern = $pattern" "searches"
    return $pattern
}

proc ::completion::update_completions_gui {} {
    ::completion::debug_msg "entering update_completions_gui" "entering_procs"
    if {[winfo exists .pop.f.lb]} {
        ::completion::scrollbar_check
        if {$::completions == {}} { ::completion::set_empty_listbox }
        if {[llength $::completions] > 1} {
            .pop.f.lb configure -state normal
            .pop.f.lb select clear 0 end
            .pop.f.lb select set 0 0
            .pop.f.lb yview scroll -100 page
        }
    }
}

proc ::completion::unique {} {
    ::completion::debug_msg "entering unique" "entering_procs"
    return [expr {[llength $::completions] == 1
                  && [::completion::valid]}]
}

proc ::completion::valid {} {
    ::completion::debug_msg "entering valid" "entering_procs"
    return [expr {[lindex $::completions 0] ne "(empty)"}]
}

# this is run when there are no results to display
proc ::completion::set_empty_listbox {} {
    ::completion::debug_msg "entering set_empty_listbox" "entering_procs"
    if {[winfo exists .pop.f.lb]} {
        ::completion::scrollbar_check
        .pop.f.lb configure -state disabled
    }
    set ::completions {"(empty)"}
}

#this proc moves the selection down (incrementing the index)
proc ::completion::increment {{amount 1}} {
    ::completion::debug_msg "entering increment" "entering_procs"
    ::completion::debug_msg "amount = $amount" "popup_gui"
    if {$::focus != "pop"} {
        focus .pop.f.lb
        set ::focus "pop"
    }
    ::completion::debug_msg "bindtags = [bindtags .pop.f.lb]" "popup_gui"
    ::completion::debug_msg "bindings on .pop.f.lb = [bind .pop.f.lb]" "popup_gui"
    set selected [.pop.f.lb curselection]
    ::completion::debug_msg "selected = $selected" "popup_gui"
    
    #if completion list is empty then "selected" will be empty
    if { ![ string is integer -strict $selected] } {
        return
    }
    set updated [expr {($selected + $amount) % [llength $::completions]}]
    ::completion::debug_msg "updated = $updated" "popup_gui"
    .pop.f.lb selection clear 0 end
    .pop.f.lb selection set $updated
    ::completion::debug_msg "curselection after selection set = [.pop.f.lb curselection]" "popup_gui"
    .pop.f.lb see $updated
}

# store keywords (send/receive or array)
proc ::completion_store {tag} {
    # I'm disabling the unique names completion for now because i don't think it is desireable.
    # While it does detects when the user type a new name it **doesn't** when those names are not 
    # used any more (user closed their containing patch, deleted their objects, etc).
    # Also it doesn't detect those names when the user loads an patch.
    # In future versions we should be able to do that communicating with PD directly.
    return
    ::completion::debug_msg "entering completion store" "entering_procs"
    ::completion::debug_msg "   tag = $tag" "unique_names"
    set name 0
    set kind(sr) {s r send receive}
    set kind(sra) {send~ receive~}
    set kind(tc) {throw~ catch~}
    set kind(arr) {tabosc4~ tabplay~ tabread tabread4 \
                         tabread4~ tabread~ tabwrite tabwrite~}
    set kind(del) {delread~     delwrite~}

    if {[regexp {^(s|r|send|receive)\s(\S+)$} $tag -> do_not_matter name]} {
        set which sr
    }
    if {[regexp {^(send\~|receive\~)\s(\S+)$} $tag -> do_not_matter name]} {
        set which sra
    }
    if {[regexp {^(throw\~|catch\~)\s(\S+)$} $tag -> do_not_matter name]} {
        set which tc
    }
    if {[regexp {^tab\S+\s(\S+)$} $tag -> name]} {
        set which arr
    }
    if {[regexp {^(delread\~|delwrite\~)\s(\S+)\s*\S*$} $tag -> do_not_matter name]} {
        set which del
        ::completion::debug_msg "4 do_not_matter = $do_not_matter" "unique_names"
        ::completion::debug_msg "4 name = $name" "unique_names"
    }
    ::completion::debug_msg "Unique name = $name" "unique_names"
    if {$name != 0} {
        foreach key $kind($which) {
            ::completion::debug_msg "key = $key" "unique_names"
            if {[lsearch -all -inline -glob $::all_externals [list $key $name]] eq ""} {
                lappend ::all_externals [list $key $name]
                set ::all_externals [lsort $::all_externals]
            }
        }
    }
}

#this is called when the user selects the desired external
proc ::completion::choose_selected {} {
    ::completion::debug_msg "entering choose selected" "entering_procs"
    if {[::completion::valid]} {
        set selected_index [.pop.f.lb curselection]
        ::completion::popup_destroy
        set choosen_item [lindex $::completions $selected_index]
        #if we are on monolithic mode we should not write the "libName/"
        if {$::current_search_mode eq 2} {
            set libName [lindex [split $choosen_item /] 0]
            set choosen_item [lindex [split $choosen_item /] 1]
            # I'm addind this line just to have the option to print the lib name on the console but i don't think this is needed it apperas on the completions list.
            #::pdwindow::post "auto complete: [lindex [split $choosen_item /] 0] is part of the $libName library\n\n"
        }
        set isSpecialMsg [::completion::is_special_msg $choosen_item]
        if { $isSpecialMsg } {
            ::completion::erase_text
            ::completion::delete_obj_onspecialmsg
        } else {
            ::completion::replace_text $choosen_item            
        }
        ::completion::debug_msg "----------->Selected word: $choosen_item" "char_manipulation"
        set ::current_text "" ;# clear for next search
        ::completion::set_empty_listbox
        #focus -force $::current_canvas
        #set ::focus "canvas"
        ::completion::debug_msg "end of choose_selected current_text: $::current_text" "char_manipulation"
    }
}

proc ::completion::delete_obj_onspecialmsg {} {
    # will anybody ever read this mess? heh
    # well, this is still experimental software. I'll clean this up in the future :)
    # (dreaming of pd 1.0)

    #$::current_canvas configure -bg #00ff00
    set rectangle "$::current_tag"
    append rectangle "R"
    ::completion::debug_msg "rectangle = $rectangle\n"
    
    $::current_canvas itemconfigure $rectangle -fill red


    # mimicking PD messages (using -d 1)
    # pdtk_undomenu $::current_canvas clear no
    # pdtk_undomenu $::current_canvas clear no
    # $::current_canvas itemconfigure $rectangle -fill black
    # $::current_canvas itemconfigure $::current_tag -fill black
    # pdtk_undomenu $::current_canvas clear no

    #$::current_canvas delete $::current_tag ;#THIS ACTUALLY REMOVES THE TEXT THE USER IS TYPING
    #$::current_canvas delete $rectangle ;#THIS removes the rectangle

    #BUT they are created again when i exit exit mode
    #BUT they are created again when i exit exit mode
    #BUT they are created again when i exit exit mode

    set coords [$::current_canvas coords $rectangle]
    ::completion::debug_msg "coords = $coords\n"

        ::completion::debug_msg "::current_canvas = $::current_canvas\n"
    set winfo_test "[winfo toplevel $::current_canvas]"
        ::completion::debug_msg "winfo_test = $winfo_test\n"

    set offset 1 ;# how much we're backing off before starting the selection
    set x [lindex $coords 0]
        set x [expr {$x-$offset}]
    set y [lindex $coords 1]
        set y [expr {$y-$offset}]
    set w [expr $offset+1] ;# how much to go right, then
    set h [expr $offset+1] ;# how much to go down, then
    ::completion::debug_msg "x = $x\n"
    ::completion::debug_msg "y = $y\n"
    ::completion::debug_msg "w = $w\n"
    ::completion::debug_msg "h = $h\n"
    ::completion::debug_msg "\[expr \{$x+$w\}\] = [expr {$x+$w}]\n"

    
    pdsend "[winfo toplevel $::current_canvas] motion $x $y 0"
    pdsend "[winfo toplevel $::current_canvas] mouse $x $y 1 0"
    pdsend "[winfo toplevel $::current_canvas] motion [expr {$x+$w}] [expr {$y+$h}] 0"
    pdsend "[winfo toplevel $::current_canvas] mouseup [expr {$x+$w}] [expr {$y+$h}] 1"

    
    pdsend "[winfo toplevel $::current_canvas] key 1 127 0" ;#delete = 127
    pdsend "[winfo toplevel $::current_canvas] key 0 127 0" ;
    #pdsend "[winfo toplevel $::current_canvas] text 0" ;

    #WORK AROUND

    #QUERY INFORMATION ABOUT THE $rectlange position and mimic mouse and keyboard behavior (ghostPatching) by sendin input messages do pd engine to delete the object!

    #$::current_canvas itemconfigure $::current_tag TK_CONFIG_COLOR #ff0000
    
    #$::current_canvas delete "all" ;#delete everything but the selected object is recreated

}

# The keypressed and key released methods just route their input to this proc and it does the rest
proc ::completion::update_modifiers {key pressed_or_released} {
    switch -- $key {
        "Shift_L"   { set ::is_shift_down $pressed_or_released }
        "Shift_R"   { set ::is_shift_down $pressed_or_released }
        "Control_L" { set ::is_ctrl_down $pressed_or_released }
        "Control_R" { set ::is_ctrl_down $pressed_or_released }
        "Alt_L"     { set ::is_alt_down $pressed_or_released }
        "Alt_R"     { set ::is_alt_down $pressed_or_released }
    }
}

# receives <Key> events while listbox has focus
# some stuff is passed correctly only on KeyRelease and other stuff only on KeyPress
# so that's why there is both a lb_keyrelease and a lb_keypress procs
proc ::completion::keypress {key unicode} {
    ::completion::debug_msg "key pressed was $key.  Unicode = $unicode\n" "key_event"
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
    ::completion::debug_msg "~lb_keys~ key released was $key    unicode = $unicode\n" "key_event"
    # We don't want to receive a key if the user pressed the plugin-activation hotkey.
    # otherwise (let's say the user is using Control+space as the hotkey) when the user activates the plugin it would output a space
    # so when we get the keydown event we wait for the keyrelease and do nothing.
    if {$::waiting_trigger_keyrelease eq 1} {
        ::completion::debug_msg "got the key release. \[$key, $unicode\]\n"
        return
    }
    ::completion::update_modifiers $key 0
    set ::completion_text_updated 0
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
    ::completion::debug_msg "~text_keys~ key pressed was $key\n" "key_event"
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

# this inserts the key
proc ::completion::insert_key {key} {
    ::completion::debug_msg "entering ::completion::insert_key" "entering_procs"
    scan $key %c keynum
    # pdsend "pd key 1 $keynum 0" ; notworking
    ::completion::sendKeyDown $keynum
    ::completion::debug_msg "inserting key $keynum" "char_manipulation"

    append ::current_text $key
    # set ::current_text [$::current_canvas itemcget $::current_tag -text] ;# why does this line doesn't work?

    # to debug the right line
    ::completion::search $::current_text
    set ::focus "canvas"
    pdtk_text_editing $::toplevel $::current_tag 1
    set ::completion_text_updated 0    
    # for some reason this does not work without passing the arg ;# what Yvan meant?
    #Those lines were making the completion windom vanish!
    #focus -force $::toplevel 
    #focus -force $::current_canvas
}

# erases what the user typed since it started the pluging
proc ::completion::erase_text {} {
    ::completion::debug_msg "entering erase text" "entering_procs"
    # simulate backspace keys
    ::completion::debug_msg "erase_text = $::erase_text" "char_manipulation"
    set i [expr {[string length $::erase_text] + 2}] ;# FIXME
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
#       pdtk_text_selectall $::current_canvas $::current_tag
#                               OR
#       pdtk_text_set $::current_canvas $::current_tag ""
# to select everything and delete it or directly clear the text object. 
# I've tried it but it doesn't work (idky yet).
proc ::completion::replace_text {args} {
    ::completion::debug_msg "===Entering replace_text" "entering_procs"
    ::completion::erase_text
    set text ""
    if { ( !$::completion::config(auto_complete_libs) && !$::is_shift_down) ||
         (  $::completion::config(auto_complete_libs) &&  $::is_shift_down) 
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
        # ::completion::debug_msg "current char =  $cha"
        scan $cha %c keynum
        ::completion::sendKeyDown $keynum
    }
    set ::erase_text $text
        ::completion::debug_msg "erase_text = $::erase_text" "char_manipulation"
    # nasty hack: the widget does not update his text because we pretend
    # we typed the text although we faked it so pd gets it as well (mmh)
    set ::completion_text_updated 1
    #set ::current_text "" ; Not needed because choose_selected will empty that
}

proc ::completion::is_special_msg { msg } {
    switch -- $msg {
        "plugin::rescan" {
             ::completion::scan_all_completions 
             return 1
        }
        "plugin::options" {
            ::completion::show_options_gui
            return 1
        }
        "plugin::help" {
            ::completion::open_help_file
            return 1
        }
        "plugin::debug" {
            set ::completion_debug [expr {!$::completion_debug}]
            return 1
        }
    }
    return 0
}

proc ::completion::add_special_messages {} {
    set ::all_externals [linsert $::all_externals 0 "plugin::debug"]
    set ::all_externals [linsert $::all_externals 0 "plugin::help"]
    set ::all_externals [linsert $::all_externals 0 "plugin::options"]
    set ::all_externals [linsert $::all_externals 0 "plugin::rescan"]
}

# called when user press Enter
proc ::completion::choose_or_unedit {} {
    ::completion::debug_msg "entering choose or unedit" "entering_procs"
    if {[winfo exists .pop] && [::completion::valid]} {
        ::completion::choose_selected
    } {
        ::completion::text_unedit
    }
}

proc ::completion::text_unedit {} {
    ::completion::debug_msg "entering text unedit" "entering_procs"
    pdsend "$::focused_window reselect"
    set ::new_object 0
    set ::completion_text_updated 0
}

# this is called when the user press the BackSpace key (erases on char)
proc ::completion::chop {} {
    ::completion::debug_msg "entering chop" "entering_procs"
    #if the user press shift+backspace restart search
    if {$::is_shift_down} {
        ::completion::debug_msg "shift+BackSpace = clearing search" "char_manipulation"
        ::completion::erase_text
        set ::current_text ""
        ::completion::search
        return
    }
    ::completion::sendKeyDownAndUp 8 ;#8 = BackSpace
    #::completion::debug_msg "current_text before chopping $::current_text"
    set ::current_text [string replace $::current_text end end] ;#this removes the last char (?!)
    ::completion::debug_msg "current_text after choping = $::current_text" "char_manipulation"
    #::completion::debug_msg "current_text after chopping $::current_text"
    ::completion::search $::current_text
    #what does it do?
    if {[winfo exists .pop]} {
        .pop.f.lb selection clear 0 end
        .pop.f.lb selection set 0
    }
    # focus -force $::current_canvas ;# THIS IS THE LINE THAT MAKES THE AUTOCOMPLETE VANISH AFTER BACKSPACE
    set ::focus "canvas"
}

proc ::completion::popup_draw {} {
    ::completion::debug_msg "entering popup draw" "entering_procs"
    if {![winfo exists .pop]} {
        set screen_w [winfo screenwidth $::current_canvas]
        set screen_h [winfo screenheight $::current_canvas]
        ::completion::debug_msg "Screen width = $screen_w" "popup_gui"
        #::completion::debug_msg "Screen height = $screen_h"
        set popup_width 40
        set menuheight 32
        if {$::windowingsystem ne "aqua"} { incr menuheight 24 }
        incr menuheight $::completion::config(offset)
        set geom [wm geometry $::toplevel]
        # fix weird bug on osx
        set decoLeft 0
        set decoTop 0
        regexp -- {([0-9]+)x([0-9]+)\+([0-9]+)\+([0-9]+)} $geom -> \
            width height decoLeft decoTop
        set left [expr {$decoLeft + $::editx}]
        set top [expr {$decoTop + $::edity + $menuheight}]
        ::completion::debug_msg "left = $left" "popup_gui"
        ::completion::debug_msg "top = $top" "popup_gui"
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
        switch -- $::current_search_mode {
            0 { set currentbackground $::completion::config(bg) }
            1 { set currentbackground $::completion::config(skipbg) }
            2 { set currentbackground $::completion::config(monobg) }
        }
        
        listbox .pop.f.lb \
            -selectmode browse \
            -width $popup_width \
            -height $::completion::config(max_lines) \
            -listvariable ::completions -activestyle none \
            -highlightcolor $::completion::config(fg) \
            -selectbackground $currentbackground \
            -selectforeground $::completion::config(fg) \
            -yscrollcommand [list .pop.f.sb set] -takefocus 1 \
            -disabledforeground #333333

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
        set ::focus "pop"
        .pop.f.lb selection set 0 0
        ::completion::debug_msg "top = $top" "popup_gui"
        set height [winfo reqheight .pop.f.lb]
        # if the popup windows were going to be displayed partly off-screen let's move it left so it doesn't
        #the width is given in units of 8 pixels
        #https://core.tcl.tk/bwidget/doc/bwidget/BWman/ListBox.html#-width
        if { [expr {$left+$popup_width*8>$screen_w}] } {
            set left [expr {$screen_w-$popup_width*8} ]
            ::completion::debug_msg "left = $left" "popup_gui"
        }
        if {$::windowingsystem eq "win32"} {
            # here we assume the user did not set the taskbark  on the sides and also did not set it's size to be more than 1/7 of the screen
            incr screen_h [ expr {-1*$screen_h/7} ]
        }
        #winfo height window
        #Returns a decimal string giving window's height in pixels. When a window is first created its height will be 1 pixel; the height will eventually be changed by a geometry manager to fulfil the window's needs. If you need the true height immediately after creating a widget, invoke update to force the geometry manager to arrange it, or use winfo reqheight to get the window's requested height instead of its actual height.
        ::completion::debug_msg "@screen_h = $screen_h\n        @height = $height" "popup_gui"
        if { [expr {$top+$height>$screen_h}] } {
            set top [expr {$screen_h-$height} ]
            wm geometry .pop +$left+$top
            #.pop.f.lb configure -+
            ::completion::debug_msg "top = $top" "popup_gui"
        }
    }
}

proc ::completion::popup_destroy {{unbind 0}} {
    ::completion::debug_msg "entering popup_destroy" "entering_procs"
    catch { destroy .pop }
    focus -force $::current_canvas
    set ::focus "canvas"
    if {$unbind} {
        bind $::current_canvas <KeyRelease> {}
    }
    set ::current_text ""
}

# Henri: i don't get exactly what this does. Commenting out those packs seems to have absolutely no effect in my system
# pack documentation: https://www.tcl.tk/man/tcl/TkCmd/pack.htm#M11
proc ::completion::scrollbar_check {} {
    ::completion::debug_msg "entering scrollbar_check" "entering_procs"
    if {[winfo exists .pop]} {
        if {[llength $::completions] < $::completion::config(max_lines)} {
            #::completion::debug_msg "completions < max numer of lines"
            pack forget .pop.f.sb
        } else {
            #::completion::debug_msg "completions >= max numer of lines"
            pack .pop.f.sb -side left -fill y
        }
    }
}

############################################################
# utils

# `prefix' from Bruce Hartweg <http://wiki.tcl.tk/44>
proc ::completion::prefix {s1 s2} {
    regexp {^(.*).*\0\1} "$s1\0$s2" all pref
    ::completion::debug_msg "prefix output = $pref" "prefix"
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
    ::completion::debug_msg "try common prefix output = $found" "prefix"
    return $found
}

proc ::completion::common_prefix {} {
    set prefix ""
    if {[llength $::completions] > 1} {
        set prefix [::completion::prefix \
                        [lindex $::completions 0] \
                        [lindex $::completions end]]
    }
    ::completion::debug_msg "common prefix output = $prefix" "prefix"
    return $prefix
}

proc ::completion::trimspaces {} {
    set ::current_text [string trimright $::current_text " "]
}

# just for testing purposes. Code would need to become more robust before 
# being used to display stuff for the user
proc ::completion::msgbox {str} {
    toplevel .cpMsgBox$str
    frame .cpMsgBox$str.f
    label .cpMsgBox$str.f.l -text "$str" -padx 3m -pady 2m
    button .cpMsgBox$str.f.okbtn -text "okay" -command "destroy .cpMsgBox$str"
    
    pack .cpMsgBox$str.f
    pack .cpMsgBox$str.f.l
    pack .cpMsgBox$str.f.okbtn
}


# just in case.
bind all <$::modifier-Key-Return> {pdsend "$::focused_window reselect"}

###########################################################
# main

::completion::init
