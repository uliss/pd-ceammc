namespace eval ::ui {

proc open_file {id path} {
    if { [file exists $path] == 0} {
        ::pdwindow::error "\[ui\] file not found: '${path}'\n"
        return
    }

    menu_openfile $path
}

}
