
package provide dialog_gatom 0.1

package require wheredoesthisgo

namespace eval ::dialog_gatom:: {
    variable sizes {0 8 10 12 16 24 36}
    namespace export pdtk_gatom_dialog
}

# array for communicating the position of the radiobuttons (Tk's
# radiobutton widget requires this to be global)
array set gatomlabel_radio {}

############ pdtk_gatom_dialog -- run a gatom dialog #########

proc ::dialog_gatom::escape {sym} {
    if {[string length $sym] == 0} {
        set ret "-"
    } else {
        if {[string equal -length 1 $sym "-"]} {
            set ret [string replace $sym 0 0 "--"]
        } else {
            set ret $sym
        }
    }
    return [string map {"$" {\$}} [unspace_text $ret]]
}

proc ::dialog_gatom::unescape {sym} {
    if {[string equal -length 1 $sym "-"]} {
        set ret [string replace $sym 0 0 ""]
    } else {
        set ret $sym
    }
    return [respace_text $ret]
}

proc ::dialog_gatom::apply {mytoplevel} {
    global gatomlabel_radio

    pdsend "$mytoplevel param \
        [$mytoplevel.width.entry get] \
        [$mytoplevel.limits.lower.entry get] \
        [$mytoplevel.limits.upper.entry get] \
        [::dialog_gatom::escape [$mytoplevel.gatomlabel.name.entry get]] \
        $gatomlabel_radio($mytoplevel) \
        [::dialog_gatom::escape [$mytoplevel.s_r.receive.entry get]] \
        [::dialog_gatom::escape [$mytoplevel.s_r.send.entry get]] \
        $::dialog_gatom::fontsize"
}

proc ::dialog_gatom::cancel {mytoplevel} {
    pdsend "$mytoplevel cancel"
}

proc ::dialog_gatom::ok {mytoplevel} {
    ::dialog_gatom::apply $mytoplevel
    ::dialog_gatom::cancel $mytoplevel
}

# set up the panel with the info from pd
proc ::dialog_gatom::pdtk_gatom_dialog {mytoplevel initwidth initlower initupper \
    initgatomlabel_radio  initgatomlabel initreceive initsend  fontsize} {

    global gatomlabel_radio
    set gatomlabel_radio($mytoplevel) $initgatomlabel_radio
    set ::dialog_gatom::fontsize $fontsize
    if {[winfo exists $mytoplevel]} {
        wm deiconify $mytoplevel
        raise $mytoplevel
        focus $mytoplevel
    } else {
        create_dialog $mytoplevel
    }

    $mytoplevel.width.entry insert 0 $initwidth
    $mytoplevel.width.entry selection range 0 end
    $mytoplevel.limits.lower.entry insert 0 $initlower
    $mytoplevel.limits.upper.entry insert 0 $initupper
    if {$initgatomlabel ne "-"} {
        $mytoplevel.gatomlabel.name.entry insert 0 \
            [::dialog_gatom::unescape $initgatomlabel]
    }
    set gatomlabel_radio($mytoplevel) $initgatomlabel_radio
        if {$initsend ne "-"} {
        $mytoplevel.s_r.send.entry insert 0 \
            [::dialog_gatom::unescape $initsend]
    }
    if {$initreceive ne "-"} {
        $mytoplevel.s_r.receive.entry insert 0 \
            [::dialog_gatom::unescape $initreceive]
    }
    $mytoplevel.width.entry select range 0 end
    focus $mytoplevel.width.entry
}

proc ::dialog_gatom::create_dialog {mytoplevel} {
    global gatomlabel_radio

    toplevel $mytoplevel -class DialogWindow -background $::pd_colors::window_background
    wm title $mytoplevel [_ "GUI Box Properties"]
    wm group $mytoplevel .
    wm resizable $mytoplevel 0 0
    wm transient $mytoplevel $::focused_window
    $mytoplevel configure -menu $::dialog_menubar
    $mytoplevel configure -padx 0 -pady 0
    ::pd_bindings::dialog_bindings $mytoplevel "gatom"

    # ceammc: ttk
    ttk::frame $mytoplevel.buttonframe
    pack $mytoplevel.buttonframe -side bottom -pady 2m
    # ceammc: ttk button
    ttk::button $mytoplevel.buttonframe.cancel -text [_ "Cancel"] \
        -command "::dialog_gatom::cancel $mytoplevel"
    pack $mytoplevel.buttonframe.cancel -side left -expand 1 -fill x -padx 15 -ipadx 10
    if {$::windowingsystem ne "aqua"} {
        # ceammc: ttk
        ttk::button $mytoplevel.buttonframe.apply -text [_ "Apply"] \
            -command "::dialog_gatom::apply $mytoplevel"
        pack $mytoplevel.buttonframe.apply -side left -expand 1 -fill x -padx 15 -ipadx 10
    }
    # ceammc: ttk
    ttk::button $mytoplevel.buttonframe.ok -text [_ "OK"] \
        -command "::dialog_gatom::ok $mytoplevel" -default active
    pack $mytoplevel.buttonframe.ok -side left -expand 1 -fill x -padx 15 -ipadx 10

    # ceammc: ttk::labelframe
    ttk::labelframe $mytoplevel.s_r -text [_ "Messages"]
    pack $mytoplevel.s_r -side bottom -fill x
    # ceammc: ttk
    ttk::frame $mytoplevel.s_r.send
    pack $mytoplevel.s_r.send -side top -anchor e
    # ceammc: ttk
    ttk::label $mytoplevel.s_r.send.label -text [_ "Send symbol:"]
    ttk::entry $mytoplevel.s_r.send.entry -width 21
    pack $mytoplevel.s_r.send.entry $mytoplevel.s_r.send.label -side right

    # ceammc: ttk
    ttk::frame $mytoplevel.s_r.receive
    pack $mytoplevel.s_r.receive -side top -anchor e
    # ceammc: ttk
    ttk::label $mytoplevel.s_r.receive.label -text [_ "Receive symbol:"]
    ttk::entry $mytoplevel.s_r.receive.entry -width 21
    pack $mytoplevel.s_r.receive.entry $mytoplevel.s_r.receive.label -side right

    ttk::labelframe $mytoplevel.fontsize -text [_ "Font Size"] \
        -width [::msgcat::mcmax "Font Size"] -labelanchor n
    pack $mytoplevel.fontsize -side right -padx 5
    foreach size $::dialog_gatom::sizes {
        if {$size eq 0} {
            set sizetext "auto"
        } else {
            set sizetext $size
        }
        # ceammc: ttk
        ttk::radiobutton $mytoplevel.fontsize.radio$size -value $size -text $sizetext \
            -variable ::dialog_gatom::fontsize
        pack $mytoplevel.fontsize.radio$size -side top -anchor w
    }

    # ceammc: ttk
    ttk::frame $mytoplevel.width -height 7
    pack $mytoplevel.width -side top
    # ceammc: ttk
    ttk::label $mytoplevel.width.label -text [_ "Width:"]
    ttk::entry $mytoplevel.width.entry -width 4
    pack $mytoplevel.width.label $mytoplevel.width.entry -side left

    ttk::labelframe $mytoplevel.limits -text [_ "Limits"]
    pack $mytoplevel.limits -side top -fill x
    # ceammc: ttk
    ttk::frame $mytoplevel.limits.lower
    pack $mytoplevel.limits.lower -side left
    ttk::label $mytoplevel.limits.lower.label -text [_ "Lower:"]
    # ceammc: ttk
    ttk::entry $mytoplevel.limits.lower.entry -width 7
    pack $mytoplevel.limits.lower.label $mytoplevel.limits.lower.entry -side left
    # ceammc: ttk
    ttk::frame $mytoplevel.limits.upper
    pack $mytoplevel.limits.upper -side left
    # ceammc: ttk
    ttk::label $mytoplevel.limits.upper.label -text [_ "Upper:"]
    ttk::entry $mytoplevel.limits.upper.entry -width 7
    pack $mytoplevel.limits.upper.label $mytoplevel.limits.upper.entry -side left

    ttk::labelframe $mytoplevel.gatomlabel -text [_ "Label"]
    pack $mytoplevel.gatomlabel -side top -fill x -pady 5
    ttk::frame $mytoplevel.gatomlabel.name
    pack $mytoplevel.gatomlabel.name -side top
    ttk::entry $mytoplevel.gatomlabel.name.entry -width 33
    pack $mytoplevel.gatomlabel.name.entry -side left
    ttk::frame $mytoplevel.gatomlabel.radio
    pack $mytoplevel.gatomlabel.radio -side top
    ttk::radiobutton $mytoplevel.gatomlabel.radio.left -value 0 -text [_ "Left"] \
        -variable gatomlabel_radio($mytoplevel) -takefocus 0
    ttk::radiobutton $mytoplevel.gatomlabel.radio.right -value 1 -text [_ "Right"] \
        -variable gatomlabel_radio($mytoplevel) -takefocus 0
    ttk::radiobutton $mytoplevel.gatomlabel.radio.top -value 2 -text [_ "Top"] \
        -variable gatomlabel_radio($mytoplevel) -takefocus 0
    ttk::radiobutton $mytoplevel.gatomlabel.radio.bottom -value 3 -text [_ "Bottom"] \
        -variable gatomlabel_radio($mytoplevel) -takefocus 0
    pack $mytoplevel.gatomlabel.radio.left -side left -anchor w
    pack $mytoplevel.gatomlabel.radio.right -side right -anchor w
    pack $mytoplevel.gatomlabel.radio.top -side top -anchor w
    pack $mytoplevel.gatomlabel.radio.bottom -side bottom -anchor w

    # live widget updates on OSX in lieu of Apply button
    if {$::windowingsystem eq "aqua"} {

        # call apply on radiobutton changes
        $mytoplevel.gatomlabel.radio.left config -command [ concat ::dialog_gatom::apply $mytoplevel ]
        $mytoplevel.gatomlabel.radio.right config -command [ concat ::dialog_gatom::apply $mytoplevel ]
        $mytoplevel.gatomlabel.radio.top config -command [ concat ::dialog_gatom::apply $mytoplevel ]
        $mytoplevel.gatomlabel.radio.bottom config -command [ concat ::dialog_gatom::apply $mytoplevel ]

        # allow radiobutton focus
        $mytoplevel.gatomlabel.radio.left config -takefocus 1
        $mytoplevel.gatomlabel.radio.right config -takefocus 1
        $mytoplevel.gatomlabel.radio.top config -takefocus 1
        $mytoplevel.gatomlabel.radio.bottom config -takefocus 1

        # call apply on Return in entry boxes that are in focus & rebind Return to ok button
        bind $mytoplevel.width.entry <KeyPress-Return> "::dialog_gatom::apply_and_rebind_return $mytoplevel"
        bind $mytoplevel.limits.lower.entry <KeyPress-Return> "::dialog_gatom::apply_and_rebind_return $mytoplevel"
        bind $mytoplevel.limits.upper.entry <KeyPress-Return> "::dialog_gatom::apply_and_rebind_return $mytoplevel"
        bind $mytoplevel.gatomlabel.name.entry <KeyPress-Return> "::dialog_gatom::apply_and_rebind_return $mytoplevel"
        bind $mytoplevel.s_r.send.entry <KeyPress-Return> "::dialog_gatom::apply_and_rebind_return $mytoplevel"
        bind $mytoplevel.s_r.receive.entry <KeyPress-Return> "::dialog_gatom::apply_and_rebind_return $mytoplevel"

        # unbind Return from ok button when an entry takes focus
        $mytoplevel.width.entry config -validate focusin -validatecommand "::dialog_gatom::unbind_return $mytoplevel"
        $mytoplevel.width.entry config -validate focusin -validatecommand "::dialog_gatom::unbind_return $mytoplevel"
        $mytoplevel.limits.lower.entry config -validate focusin -validatecommand "::dialog_gatom::unbind_return $mytoplevel"
        $mytoplevel.limits.upper.entry config -validate focusin -validatecommand "::dialog_gatom::unbind_return $mytoplevel"
        $mytoplevel.gatomlabel.name.entry config -validate focusin -validatecommand "::dialog_gatom::unbind_return $mytoplevel"
        $mytoplevel.s_r.send.entry config -validate focusin -validatecommand "::dialog_gatom::unbind_return $mytoplevel"
        $mytoplevel.s_r.receive.entry config -validate focusin -validatecommand "::dialog_gatom::unbind_return $mytoplevel"

        # remove cancel button from focus list since it's not activated on Return
        $mytoplevel.buttonframe.cancel config -takefocus 0

        # show active focus on the ok button as it *is* activated on Return
        $mytoplevel.buttonframe.ok config -default normal
        bind $mytoplevel.buttonframe.ok <FocusIn> "$mytoplevel.buttonframe.ok config -default active"
        bind $mytoplevel.buttonframe.ok <FocusOut> "$mytoplevel.buttonframe.ok config -default normal"

        # since we show the active focus, disable the highlight outline
        # ceammc disable
        # $mytoplevel.buttonframe.ok config -highlightthickness 0
        # $mytoplevel.buttonframe.cancel config -highlightthickness 0
        # ceammc end
    }

    position_over_window $mytoplevel $::focused_window
}

# for live widget updates on OSX
proc ::dialog_gatom::apply_and_rebind_return {mytoplevel} {
    ::dialog_gatom::apply $mytoplevel
    bind $mytoplevel <KeyPress-Return> "::dialog_gatom::ok $mytoplevel"
    focus $mytoplevel.buttonframe.ok
    return 0
}

# for live widget updates on OSX
proc ::dialog_gatom::unbind_return {mytoplevel} {
    bind $mytoplevel <KeyPress-Return> break
    return 1
}
