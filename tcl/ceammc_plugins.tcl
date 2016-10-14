proc load_plugin_script {filename} {
    global errorInfo

    set basename [file tail $filename]
    if {[lsearch $::loaded_plugins $basename] > -1} {
        ::pdwindow::post [_ "'$basename' already loaded, ignoring: '$filename'\n"]
        return
    }

    ::pdwindow::debug [_ "Loading plugin: $filename\n"]
    set tclfile [open $filename]
    set tclcode [read $tclfile]
    close $tclfile
    if {[catch {uplevel #0 $tclcode} errorname]} {
        ::pdwindow::error "-----------\n"
        ::pdwindow::error [_ "UNHANDLED ERROR: $errorInfo\n"]
        ::pdwindow::error [_ "FAILED TO LOAD $filename\n"]
        ::pdwindow::error "-----------\n"
    } else {
        lappend ::loaded_plugins $basename
    }
}

load_plugin_script [file join $::sys_guidir ceammc_editmode_plugin.tcl]
