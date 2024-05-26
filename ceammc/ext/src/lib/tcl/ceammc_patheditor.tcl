package provide ceammc_patheditor 0.1

namespace eval ::ceammc {
namespace eval patheditor {

    proc open {name title geometry path} {
        set w ".$name"
        toplevel $w
        wm title $w "$title"
        wm geometry $w $geometry
        #wm protocol $name WM_DELETE_WINDOW "ceammc::tableeditor::close $name 1"
        set f [ttk::frame $w.f -padding 3]

        set path_frame [ttk::frame $f.pf -padding 3]
        set pe [ttk::entry $path_frame.e -state enabled]
        $pe insert 0 $path
        set pb [ttk::button $path_frame.b -text [_ "Choose"] -command "::ceammc::patheditor::open_dialog $w $path $pe"]

        # cancel/save buttons
        set btn_frame  [ttk::frame $f.bf -padding 3]
        set bc [ttk::button $btn_frame.c -text [_ "Cancel"] -command "destroy $w" ]
        set bs [ttk::button $btn_frame.s -text [_ "Save"]   -command "::ceammc::patheditor::save $name $pe" ]

        pack $f -expand yes -fill both
        pack $path_frame -expand yes -fill x
        pack $pe -expand yes -fill x -side left
        pack $pb -side right
        pack $btn_frame -side bottom -expand yes -fill x
        pack $bc -side left
        pack $bs -side right

        bind $w <KeyPress-Escape> "destroy $w"
    }

    proc open_dialog {w path entry} {
        set new_path [tk_chooseDirectory -initialdir [file dirname $path] -mustexist 1 -parent $w]
        $entry delete 0 end
        $entry insert 0 $new_path
#        set dirty($name) 0
    }

    proc save {name entry} {
        set w ".$name"
        pdsend "$name .tcl_callback [$entry get]"
        destroy $w
    }
}
}
