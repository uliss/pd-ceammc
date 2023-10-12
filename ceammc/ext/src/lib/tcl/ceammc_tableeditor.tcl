package provide ceammc_tableeditor 0.1

if {[catch {package require tablelist_tile}]} {
    ::pdwindow::error "\[TCL\] package error: can't find tablelist_tile package\n"
} else {

namespace eval ::ceammc {
namespace eval tableeditor {

    variable dirty

    proc open {name geometry title font cols} {
        variable dirty
        if {[winfo exists $name]} {
            $name.f.tbl delete 0 end
        } else {
            toplevel $name
            wm title $name $title
            wm geometry $name $geometry
            wm protocol $name WM_DELETE_WINDOW "ceammc::tableeditor::close $name 1"
            set dirty($name) 0

            set f [ttk::frame $name.f]
            scrollbar $f.scroll -command "$name.f.tbl yview"
            set tbl $f.tbl
            tablelist::tablelist $tbl -height 0 -width 0 \
                -yscrollcommand "$name.f.scroll set"

            if {[$tbl cget -selectborderwidth] == 0} {
                $tbl configure -spacing 1
            }

            foreach c $cols {
                $tbl insertcolumnlist end [lrange $c 0 2]
                $tbl columnconfigure end -editable  [lindex $c 3]
                $tbl columnconfigure end -editwindow [lindex $c 4]
            }

            bind $tbl <<TablelistCellUpdated>> "ceammc::tableeditor::setdirty $name 1"

            # layout
            set btn [ttk::button $f.btn -text [_ "Close"] -command "ceammc::tableeditor::close $name 1"]
            pack $btn -side bottom -pady 7p
            pack $tbl -side top -expand yes -fill both
            pack $f.scroll -side right -fill y
            pack $f -expand yes -fill both

            bind $name <$::modifier-Key-s> "ceammc::tableeditor::send $name"
            bind $name <$::modifier-Key-w> "ceammc::tableeditor::close $name 1"
        }
    }

    proc show {name} {
        wm deiconify $name
        raise $name
        focus $name.f.tbl
    }

    proc dodirty {name} {
        variable dirty
        set title [wm title $name]
        set dt [string equal -length 1 $title "*"]
        if {$dirty($name)} {
            if {$dt == 0} {wm title $name "*$title"}
        } else {
            if {$dt} {wm title $name [string range $title 1 end]}
        }
    }

    proc setdirty {name flag} {
        variable dirty
        if {[winfo exists $name]} {
            set dirty($name) $flag
            dodirty $name
        }
    }

    proc doclose {name} {
        destroy $name
        pdsend [concat $name signoff]
    }

    proc append {name contents} {
        $name.f.tbl insert end $contents
    }

    proc clear {name} {
        if {[winfo exists $name]} {
            $name.f.tbl delete 0 end
        }
    }

    proc send {name} {
        variable escape_map
        if {[winfo exists $name]} {
            pdsend [concat $name .clear]
            foreach row [$name.f.tbl get 0 end] {
                set row [string map {"{" {} "}" {}} $row]
                pdsend [concat $name .addline $row]
            }
            pdsend [concat $name .sync]
        }
        setdirty $name 0
    }

    proc close {name ask} {
        variable dirty
        if {[winfo exists $name]} {
            if {$ask && $dirty($name)} {
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
