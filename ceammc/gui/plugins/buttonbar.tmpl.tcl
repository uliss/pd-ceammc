# this plugin creates a buttonbar on a patch window when that patch
# window is in Edit Mode

# this GUI plugin removes the menubars from any patch window that is
# not in Edit Mode.  Also, if a patch is switched to Run Mode, the
# menubar will be removed.

# try enabling base64 if possible
catch {package require base64}
# preferences
package require pd_guiprefs

namespace eval ::btnbar:: {
    variable show;   # if show button bar
    variable hide_in_runmode;
    variable states;  # current button bar show states
}

set ::btnbar::show true
set ::btnbar::hide_in_runmode true
set ::btnbar::states [dict create]

namespace eval ::btnbar::utils { }

# tooltips
if { [catch {package require tooltip} ] } {
    proc ::btnbar::tooltip {id msg} {}
} {
    proc ::btnbar::tooltip {id msg} { tooltip::tooltip $id $msg }
}

# helper utils
proc ::btnbar::utils::bool {value {fallback 0}} {
    catch {set fallback [expr bool($value) ] } stdout
    return $fallback
}

proc ::btnbar::addTopLevel { id } {
    if {! [dict exists $::btnbar::states $id] } {
        dict append ::btnbar::states $id $::btnbar::show
        ::btnbar::showhide $id
    }
}

proc ::btnbar::removeTopLevel { id } {
    if { [dict exists $::btnbar::states $id] } { dict unset ::btnbar::states $id }
}

proc ::btnbar::getState { id } {
    if { [dict exists $::btnbar::states $id] } {
        return [dict get $::btnbar::states $id]
    } {
        return 0
    }
}

proc ::btnbar::setState { id val } {
    if { [dict exists $::btnbar::states $id] } { dict set ::btnbar::states $id $val }
}

if { [ catch { set ::btnbar::hide_in_runmode [::pd_guiprefs::read btnbar] } stdout ] } {
    set ::btnbar::hide_in_runmode true
    set ::btnbar::show true
} {
    # read config values
    set ::btnbar::show [::btnbar::utils::bool [::pd_guiprefs::read btnbar_show] 1]
    set ::btnbar::hide_in_runmode [::btnbar::utils::bool [::pd_guiprefs::read btnbar_hide_in_runmode] 1]
}

proc ::btnbar::init_options_menu {} {
    if {$::windowingsystem eq "aqua"} {
        set mymenu .menubar.apple.preferences
    } else {
        set mymenu .menubar.file.preferences
    }

    if { [catch {
        $mymenu entryconfigure [_ "Button Bar"] -command {::btnbar::show_options_gui}
    } _ ] } {
        $mymenu add separator
        $mymenu add command -label [_ "Button Bar"] -command {::btnbar::show_options_gui}
    }
}

proc ::btnbar::write_config {} {
    ::pd_guiprefs::write btnbar_show $::btnbar::show
    ::pd_guiprefs::write btnbar_hide_in_runmode $::btnbar::hide_in_runmode
}

proc ::btnbar::show_options_gui {} {
    if {[winfo exists .options]} {
        focus .options
        return
    }

    toplevel .options
    wm title .options [_ "Button Bar Settings"]

    frame .options.f -padx 5 -pady 5 -background #ECECEC

    ttk::checkbutton .options.f.show -variable ::btnbar::show -onvalue 1 -offvalue 0 -command {
        if {$::btnbar::show} {
            .options.f.hide_in_runmode configure -state normal
        } {
            .options.f.hide_in_runmode configure -state disable
        }
    }
    ttk::label .options.f.show_label -text [_ "Show" ]

    ttk::checkbutton .options.f.hide_in_runmode -variable ::btnbar::hide_in_runmode -onvalue 1 -offvalue 0
    ttk::label .options.f.hide_in_runmode_label -text [_ "Hide in runmode" ]

    # buttons
    ttk::button .options.f.save_btn -text [_ "Save settings" ] -command ::btnbar::write_config

    set padding 2

    # ::::GRID::::

    # setup main frame stuff
    grid .options.f -column 0 -row 0
    set current_row 0

    # show
    grid .options.f.show_label -column 0 -row $current_row -padx $padding -pady $padding -sticky "w"
    grid .options.f.show -column 1 -row $current_row -padx $padding -pady $padding -sticky "w"
    incr current_row

    # hide in runmode
    grid .options.f.hide_in_runmode_label -column 0 -row $current_row -padx $padding -pady $padding -sticky "w"
    grid .options.f.hide_in_runmode -column 1 -row $current_row -padx $padding -pady $padding -sticky "w"
    incr current_row

    # buttons
    grid .options.f.save_btn -column 1 -row $current_row -padx $padding -pady 8 -sticky "w"
}

proc make_pd_button {mytoplevel name tooltip} {
    button $mytoplevel.buttonbar.$name -image buttonimage$name \
        -relief flat -borderwidth 0 -highlightthickness 0 \
        -highlightcolor grey -highlightbackground grey -padx 1 -pady 1 \
        -command "menu_send_float \$::focused_window $name 0"
    pack $mytoplevel.buttonbar.$name -side left -padx 0 -pady 0
    ::btnbar::tooltip $mytoplevel.buttonbar.$name $tooltip
}

proc make_iemgui_button {mytoplevel name tooltip} {
    button $mytoplevel.buttonbar.$name -image buttonimage$name \
        -relief flat -borderwidth 0 -highlightthickness 0 \
        -highlightcolor grey -highlightbackground grey -padx 1 -pady 1 \
        -command "menu_send \$::focused_window $name"
    pack $mytoplevel.buttonbar.$name -side left -padx 0 -pady 0
    ::btnbar::tooltip $mytoplevel.buttonbar.$name $tooltip
}

proc make_ceammc_button {mytoplevel name tooltip} {
    button $mytoplevel.buttonbar.$name -image buttonimage$name \
        -relief flat -borderwidth 0 -highlightthickness 0 \
        -highlightcolor grey -highlightbackground grey -padx 1 -pady 1 \
        -command "menu_send \$::focused_window ui.$name"
    pack $mytoplevel.buttonbar.$name -side left -padx 0 -pady 0
    ::btnbar::tooltip $mytoplevel.buttonbar.$name $tooltip
}

proc make_ceammc_button_tilde {mytoplevel name tooltip} {
    button $mytoplevel.buttonbar.$name -image buttonimage$name \
        -relief flat -borderwidth 0 -highlightthickness 0 \
        -highlightcolor grey -highlightbackground grey -padx 1 -pady 1 \
        -command "menu_send \$::focused_window ui.$name~"
    pack $mytoplevel.buttonbar.$name -side left -padx 0 -pady 0
    ::btnbar::tooltip $mytoplevel.buttonbar.$name $tooltip
}

proc ::btnbar::init {mytoplevel} {
    frame $mytoplevel.buttonbar -cursor arrow -background lightgrey
    make_pd_button $mytoplevel obj [_ "Object"]
    make_pd_button $mytoplevel msg "Message"
    make_pd_button $mytoplevel floatatom "Number box"
    make_pd_button $mytoplevel symbolatom "Symbol box"
    make_pd_button $mytoplevel text "Comment"
    make_iemgui_button $mytoplevel bng "Button"
    make_iemgui_button $mytoplevel toggle "Toggle"
    make_iemgui_button $mytoplevel numbox "Number box2"
    make_iemgui_button $mytoplevel hslider "Horizontal slider"
    make_iemgui_button $mytoplevel vslider "Vertial slider"
    make_iemgui_button $mytoplevel hradio  "Horizontal radio"
    make_iemgui_button $mytoplevel vradio  "Vertical radio"
    make_iemgui_button $mytoplevel mycnv   "Canvas"
    make_iemgui_button $mytoplevel menuarray "Array"
    make_ceammc_button $mytoplevel knob "Knob"
    make_ceammc_button $mytoplevel sliders "Sliders"
    make_ceammc_button $mytoplevel slider2d "Slider 2D"
    make_ceammc_button $mytoplevel keyboard "Keyboard"
    make_ceammc_button $mytoplevel env "Breakpoint function"
    make_ceammc_button $mytoplevel display "Display"
    make_ceammc_button_tilde $mytoplevel scope "Scope"
    make_ceammc_button_tilde $mytoplevel spectroscope "Spectroscope"
    make_ceammc_button $mytoplevel preset "Presets"
}

proc ::btnbar::show {mytoplevel} {
    set state [::btnbar::getState $mytoplevel]

    if {$state} {
        set tkcanvas [tkcanvas_name $mytoplevel]
        pack forget $tkcanvas
        pack $mytoplevel.buttonbar -side top -fill x
        pack $tkcanvas -side top -expand 1 -fill both
        ::btnbar::setState $mytoplevel false
    }
}

proc ::btnbar::hide {mytoplevel} {
    set state [::btnbar::getState $mytoplevel]

    if {!$state} {
        pack forget $mytoplevel.buttonbar
        ::btnbar::setState $mytoplevel true
    }
}

proc ::btnbar::enable {mytoplevel} {
    ::btnbar::show $mytoplevel

    foreach child [winfo children $mytoplevel.buttonbar] {
        catch { $child configure -state normal }
    }
}

proc ::btnbar::disable {mytoplevel} {
    foreach child [winfo children $mytoplevel.buttonbar] {
        catch { $child configure -state disabled }
    }
}

proc ::btnbar::showhide {mytoplevel} {
    # populate button bar with entries
    if { ! [winfo exists $mytoplevel.buttonbar]} {
        ::btnbar::init $mytoplevel
    }

    if {!$::btnbar::show} {
        catch { ::btnbar::hide $mytoplevel }
        return;
    }

    if {$::editmode($mytoplevel)} {
        if {$::btnbar::hide_in_runmode} {
            ::btnbar::show $mytoplevel
        } {
            ::btnbar::enable $mytoplevel
        }
    } {
        if {$::btnbar::hide_in_runmode} {
            ::btnbar::hide $mytoplevel
        } {
            ::btnbar::disable $mytoplevel
        }
    }
}

bind PatchWindow <Expose>  {+::btnbar::addTopLevel %W}
bind PatchWindow <Destroy> {+::btnbar::removeTopLevel %W}
bind PatchWindow <<EditMode>> {+::btnbar::showhide %W}

::btnbar::init_options_menu
