# META NAME drop-patch
# META DESCRIPTION drop patch-files onto patches to create objects
# META DESCRIPTION drop patch-files onto Pd-console to open patches

# META AUTHOR IOhannes m zmölnig <zmoelnig@iem.at>
# META AUTHOR Patrice Colet <colet.patrice@free.fr>
# META AUTHOR Hans-Christoph Steiner <eighthave@users.sourceforge.net>
# 2018 - 2021 Modifications by Oliver Stotz and Lucas Cordiviola 
 
::pdwindow::post "\[dnd-plugin\] version 0.3.1\n"
::pdwindow::post "\[dnd-plugin\] Drag and Drop on Pd window or patch canvas\n"

#lappend ::auto_path $::current_plugin_loadpath
set dir [file join $::current_plugin_loadpath ../tkdnd]
source [file join $dir pkgIndex.tcl]

package require tkdnd

namespace eval ::dnd_object_create {
    variable x 0
    variable y 0
}

namespace eval ::text_on_patch {
    variable x 0
    variable y 0
}

#------------------------------------------------------------------------------#
# create an object using the dropped filename

proc ::dnd_object_create::bind_to_canvas {mytoplevel} {
    ::tkdnd::drop_target register $mytoplevel DND_Files
    bind $mytoplevel <<DropPosition>> {+::dnd_object_create::setxy %X %Y}
    bind $mytoplevel <<Drop:DND_Files>> {::dnd_object_create::dropped_object_files %W %D}
}


proc ::dnd_object_create::setxy {newx newy} {
    variable x $newx
    variable y $newy
    return "copy"
}


proc ::dnd_object_create::open_dropped_files {files} {
    foreach file $files {
        open_file $file
    }
}

# ---------------------------- 2021 Modification ---------------------------- #
# convert string to ascii values
proc ::dnd_object_create::string_to_ascii {string_value} {
    set map {}
    set result [lrepeat [string length $string_value] DUMMY]
    set idx 0
    foreach c [split $string_value ""] {
        if {![dict exists $map $c]} {
            scan $c %c ch
            dict set map $c $ch
        }
        lset result $idx [dict get $map $c]
        incr idx
    }
    return $result
}

# prepend white spaces with a backslash
proc ::dnd_object_create::correct_spaces {string_value} {
    if {[ regexp {^(?:[0-9]+|[a-zA-Z]+)$} $string_value ]} {
        set result \\$string_value
    } else {
        set result [regsub -all {\s+} $string_value "\\\ "]
    }
    return $result
}

# ----------------------------End 2021 Modification ------------------------- #

proc ::dnd_object_create::dropped_object_files {mytoplevel files} {
    foreach file $files {
	set ext  [file extension $file]
	set obj  [file rootname [file tail $file]]
	set dir  [file dirname $file]
        if {$ext == ".pd"} {
            set found 0
            foreach pathdir [concat $::sys_searchpath $::sys_staticpath] {
                ## if pathdir is relative, prepend pwd to it
                set pathdir [file normalize $pathdir]
                # check if the dropped file is in a subdirectory of our PATH
                if { [string first $pathdir $dir ] == 0 } {
                    set found 1
                    set obj [string trimleft [file rootname [string range $file [string length $pathdir ] end]] /]
                    ::pdwindow::debug "dropping $obj from $pathdir on $::focused_window\n"
                    ::dnd_object_create::make_object $mytoplevel $obj
                    break
                }
            }

            if { 0 == $found } {
                set patchname_bsl [::dnd_object_create::correct_spaces $file]
                set dropobj [file rootname $patchname_bsl]
                ::pdwindow::debug "dropping $dropobj on $::focused_window\n"
                ::dnd_object_create::make_object $mytoplevel $dropobj
            }

           # 2018 Modification
        } else {
            ::dnd_object_create::send_filename $mytoplevel $file $ext $dir $obj
        }
    }
    return "link"
}

proc ::dnd_object_create::make_object {w obj} {
    variable x
    variable y
    set posx [expr $x - [winfo rootx $w]]
    set posy [expr $y - [winfo rooty $w]]
    pdsend "$w obj $posx $posy $obj"
    return "dropped"
}


# ------------------------- 2018 Modification --------------------------------#

proc ::dnd_object_create::send_filename {w file ext dir obj} {
    variable x
    variable y
    set posx [expr $x - [winfo rootx $w]]
    set posy [expr $y - [winfo rooty $w]]
	
    ## same function like Ctrl+M
    ## the definition of this function is in Tcl/pd_connect.tcl, line 50

#------------------------------------------------------------------------------#
# modified by jyg on 14.02.19
# from pdtk_canvas.tcl, lines 276 and following ones

    # get the canvas location that is currently the top left corner in the window
    set tkcanvas [tkcanvas_name $w]
    set scrollregion [$tkcanvas cget -scrollregion]
    set left_xview_pix [expr [lindex [$tkcanvas xview] 0] * [lindex $scrollregion 2] ]
    set top_yview_pix [expr [lindex [$tkcanvas yview] 0]* [lindex $scrollregion 3]]
    # take the absolute mouse coords, substract the root of the canvas
    # window, and add the area that is obscured by scrolling
    set xrel [expr int($x - [winfo rootx $w] + $left_xview_pix)]
    set yrel [expr int($y - [winfo rooty $w] + $top_yview_pix)]
#------------------------------------------------------------------------------#
#---------------------------- 2021 Modification ----------------------------#
	
    set ascii_name [::dnd_object_create::string_to_ascii $obj]
    set ascii_path [::dnd_object_create::string_to_ascii $dir]
    set ascii_fullname [::dnd_object_create::string_to_ascii $file]
    set ascii_ext [::dnd_object_create::string_to_ascii $ext]

    set name_bsl [::dnd_object_create::correct_spaces $obj]
    set path_bsl [::dnd_object_create::correct_spaces $dir]
    set fullname_bsl [::dnd_object_create::correct_spaces $file]
	
    # special case here since $ext starts with a dot, so we have to change the regexp
    if { [ regexp {^.(?:[0-9]+|[a-zA-Z]+)$} $ext ] } {set ext_bsl \\$ext} else {set ext_bsl [regsub -all {\s+} $ext "\\\ "]}
    if { $ext == ""} { set ext_bsl "NULL" }

    ::pd_connect::pdsend "dnd-dropped -ext symbol $ext_bsl, \
        -name symbol $name_bsl, \
        -path symbol $path_bsl, \
        -fullname symbol $fullname_bsl, \
        -window-name symbol $::focused_window, \
        -global-coords list $x $y, \
        -ascii-ext list $ascii_ext, \
        -ascii-name list $ascii_name, \
        -ascii-path list $ascii_path, \
        -ascii-fullname list $ascii_fullname, \
        -drop list $posx $posy $fullname_bsl"

#-------------------------- End of 2021 Modification -----------------------#
# ---------------------- End of 2018 Modification -------------------------#
	
		
#------------------------------------------------------------------------------#
# modified by jyg on 14.02.19
# generate click event where the file has been dropped

    ::pd_connect::pdsend "$w mouse $xrel $yrel 1 0"
    ::pd_connect::pdsend "$w mouseup $xrel $yrel 1"
    # afterclick signal
    ::pd_connect::pdsend "dnd-dropped -done"

#------------------------------------------------------------------------------#


    ::pdwindow::debug "$w file $posx $posy $::focused_window $file \n"
    return "dropped"	
}
#------------------------------------------------------------------------------#


bind PatchWindow <<Loaded>> {+::dnd_object_create::bind_to_canvas %W}
::tkdnd::drop_target register .pdwindow DND_Files
bind .pdwindow <<Drop:DND_Files>> {::dnd_object_create::open_dropped_files %D}

#------------------------------------------------------------------------------#
# create an object using the dropped filename

bind PatchWindow <<Loaded>> {+::text_on_patch::bind_to_dropped_text %W}

proc ::text_on_patch::bind_to_dropped_text {mytoplevel} {
    ::tkdnd::drop_target register $mytoplevel DND_Text
    bind $mytoplevel <<DropPosition>> {+::text_on_patch::setxy %X %Y}
    bind $mytoplevel <<Drop:DND_Text>> {::text_on_patch::make_comments %W %D}
    # TODO bind to DropEnter and DropLeave to make window visually show whether it will accept the drop or not
}

proc ::text_on_patch::setxy {newx newy} {
    variable x $newx
    variable y $newy
    return "copy"
}

proc ::text_on_patch::make_comments {mytoplevel text} {
    variable x
    variable y
    set posx [expr $x - [winfo rootx $mytoplevel]]
    set posy [expr $y - [winfo rooty $mytoplevel]]
    foreach line [split [regsub {\\\;} $text {}] "\n"] {
        if {$line ne ""} {
            set line [string map {"," " \\, " ";" " \\; "} $line]
            set len [expr min(100, [string length $line])]
            pdsend "$mytoplevel text $posx $posy $line, f $len"
        }
        set posy [expr $posy + 20]
    }
    return "copy"
}
