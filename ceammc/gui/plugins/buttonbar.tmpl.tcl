# this plugin creates a buttonbar on a patch window when that patch
# window is in Edit Mode

# this GUI plugin removes the menubars from any patch window that is
# not in Edit Mode.  Also, if a patch is switched to Run Mode, the
# menubar will be removed.

package require base64
package require tooltip

proc make_pd_button {mytoplevel name tooltip} {
    button $mytoplevel.buttonbar.$name -image buttonimage$name \
        -relief flat -borderwidth 1 -highlightthickness 0 \
        -highlightcolor grey -highlightbackground grey -padx 1 -pady 1 \
        -command "menu_send_float \$::focused_window $name 0"
    pack $mytoplevel.buttonbar.$name -side left -padx 0 -pady 0
    tooltip::tooltip $mytoplevel.buttonbar.$name $tooltip
}

proc make_iemgui_button {mytoplevel name tooltip} {
    button $mytoplevel.buttonbar.$name -image buttonimage$name \
        -relief flat -borderwidth 1 -highlightthickness 0 \
        -highlightcolor grey -highlightbackground grey -padx 1 -pady 1 \
        -command "menu_send \$::focused_window $name"
    pack $mytoplevel.buttonbar.$name -side left -padx 0 -pady 0
    tooltip::tooltip $mytoplevel.buttonbar.$name $tooltip
}

proc make_ceammc_button {mytoplevel name tooltip} {
    button $mytoplevel.buttonbar.$name -image buttonimage$name \
        -relief flat -borderwidth 1 -highlightthickness 0 \
        -highlightcolor grey -highlightbackground grey -padx 1 -pady 1 \
        -command "menu_send \$::focused_window ui.$name"
    pack $mytoplevel.buttonbar.$name -side left -padx 0 -pady 0
    tooltip::tooltip $mytoplevel.buttonbar.$name $tooltip
}

proc make_ceammc_button_tilde {mytoplevel name tooltip} {
    button $mytoplevel.buttonbar.$name -image buttonimage$name \
        -relief flat -borderwidth 1 -highlightthickness 0 \
        -highlightcolor grey -highlightbackground grey -padx 1 -pady 1 \
        -command "menu_send \$::focused_window ui.$name~"
    pack $mytoplevel.buttonbar.$name -side left -padx 0 -pady 0
    tooltip::tooltip $mytoplevel.buttonbar.$name $tooltip
}

proc showhide_buttonbar {mytoplevel} {
    if { ! [winfo exists $mytoplevel.buttonbar]} {
        frame $mytoplevel.buttonbar -cursor arrow -background grey \
            -pady 0 -padx 0
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
        make_ceammc_button $mytoplevel bpfunc "Breakpoint function"
        make_ceammc_button $mytoplevel display "Display"
        make_ceammc_button_tilde $mytoplevel scope "Scope"
        make_ceammc_button_tilde $mytoplevel spectroscope "Spectroscope"
        make_ceammc_button $mytoplevel preset "Presets"
    }
    if {$::editmode($mytoplevel)} {
        set tkcanvas [tkcanvas_name $mytoplevel]
        pack forget $tkcanvas
        pack $mytoplevel.buttonbar -side top -fill x
        pack $tkcanvas -side top -expand 1 -fill both
    } else {
        pack forget $mytoplevel.buttonbar
    }
}

bind PatchWindow <FocusIn> {+showhide_buttonbar %W}
bind PatchWindow <<EditMode>> {+showhide_buttonbar %W}

