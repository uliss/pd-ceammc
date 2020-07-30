# the message dialog panel is a bit unusual in that it is created directly by
# the Tcl 'pd-gui'. Most dialog panels are created by sending a message to
# 'pd', which then sends a message to 'pd-gui' to create the panel.  This is
# similar to the Find dialog panel.

package provide dialog_message 0.1

package require pd_bindings

namespace eval ::dialog_message:: {
    variable message_history {"pd dsp 1"}
    variable history_position 0

    namespace export open_message_dialog
}

proc ::dialog_message::get_history {direction} {
    variable message_history
    variable history_position

    incr history_position $direction
    if {$history_position < 0} {set history_position 0}
    if {$history_position > [llength $message_history]} {
        set history_position [llength $message_history]
    }
    # ceammc: pad
    .message.f.pad.entry delete 0 end
    .message.f.pad.entry insert 0 \
        [lindex $message_history end-[expr $history_position - 1]]
}

# mytoplevel isn't used here, but is kept for compatibility with other dialog ok procs
proc ::dialog_message::ok {mytoplevel} {
    variable message_history
    # ceammc: pad
    set message [.message.f.pad.entry get]
    if {$message ne ""} {
        pdsend $message
        lappend message_history $message
        # ceammc: pad
        .message.f.pad.entry delete 0 end
    }
}

# mytoplevel isn't used here, but is kept for compatibility with other dialog cancel procs
proc ::dialog_message::cancel {mytoplevel} {
    wm withdraw .message
}

# the message panel is opened from the menu and key bindings
proc ::dialog_message::open_message_dialog {mytoplevel} {
    if {[winfo exists .message]} {
        wm deiconify .message
        raise .message
        # ceammc: pad
        focus .message.f.pad.entry
    } else {
        create_dialog $mytoplevel
    }
}

proc ::dialog_message::create_dialog {mytoplevel} {
    toplevel .message -class DialogWindow
    wm group .message .
    wm transient .message
    wm title .message [_ "Send a Pd message"]
    wm geometry .message =400x80+150+150
    wm resizable .message 1 0
    wm minsize .message 250 80
    .message configure -menu $::dialog_menubar
    .message configure -padx 0 -pady 0
    ::pd_bindings::dialog_bindings .message "message"
    # not all Tcl/Tk versions or platforms support -topmost, so catch the error
    catch {wm attributes $id -topmost 1}

    # ceammc: ttk
    ttk::frame .message.f
    ttk::frame .message.f.pad
    ttk::label .message.f.pad.semicolon -text ";"
    # ceammc: width
    ttk::entry .message.f.pad.entry -width 35
    focus .message.f.pad.entry
    ttk::label .message.f.pad.label -text [_ "(use arrow keys for history)"]

    grid .message.f -row 0 -column 0
    grid .message.f.pad -row 0 -column 0 -padx 10 -pady 10
    grid .message.f.pad.semicolon -row 1 -column 0 -padx 5 -pady 5
    grid .message.f.pad.entry -row 1 -column 1 -padx 5 -pady 5
    grid .message.f.pad.label -row 2 -column 1 -padx 5 -pady 5

    bind .message <$::modifier-Key-w> "::dialog_message::cancel %W"
    bind .message.f.pad.entry <Up> "::dialog_message::get_history 1"
    bind .message.f.pad.entry <Down> "::dialog_message::get_history -1"
}
