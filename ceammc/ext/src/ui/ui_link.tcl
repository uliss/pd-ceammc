#/* Copyright (c) 2005 krzYszcz and others.
# * For information on usage and redistribution, and for a DISCLAIMER OF ALL
# * WARRANTIES, see the file, "LICENSE.txt," in this distribution.  */

namespace eval ::ui {

namespace eval link {
    variable font_size 11
    variable font_family "Helvetica"
}

switch -- $::windowingsystem {
    "aqua" {
        set link::font_family "Menlo"
    }
    "x11"  {
        set link::font_family "DejaVu Sans Mono"
    }
    "win32" {
        set link::font_family "DejaVu Sans Mono"
    }
}

proc link_type_font { zoom } { return "$link::font_family [expr $zoom * $link::font_size] normal roman" }

proc link_open {filename dir} {
    if {[string first "://" $filename] > -1} {
        menu_openfile $filename
    } elseif {[file pathtype $filename] eq "absolute"} {
        if {[file extension $filename] eq ".pd"} {
            set dir [file dirname $filename]
            set name [file tail $filename]
            menu_doc_open $dir $name
        } else {
            menu_openfile $filename
        }
    } elseif {[file exists [file join $dir $filename]]} {
        set fullpath [file normalize [file join $dir $filename]]
        set dir [file dirname $fullpath]
        set filename [file tail $fullpath]
        menu_doc_open $dir $filename
   } elseif {[file exists [file join $::sys_libdir doc 5.reference $filename]]} {
        # search in core doc dir
        set fullpath [file normalize [file join $::sys_libdir doc 5.reference $filename]]
        set dir [file dirname $fullpath]
        set filename [file tail $fullpath]
        menu_doc_open $dir $filename
    } elseif {[file exists [file join $::sys_libdir extra $filename]]} {
        # search in external doc dir
        set fullpath [file normalize [file join $::sys_libdir extra $filename]]
        set dir [file dirname $fullpath]
        set filename [file tail $fullpath]
        menu_doc_open $dir $filename
    } else {
        bell ; # beep on error to provide instant feedback
        pdtk_post "ERROR: file not found: $filename at $dir"
        pdtk_post ""
    }
}

proc link_update {cnv id rid w h zoom txt_color hover_color url txt} {
    set c [::ceammc::ui::widget_canvas $cnv $id]
    set t [::ceammc::ui::widget_tag $id]
    $c delete $t

    set x 0
    set txpad [expr $zoom*2+1]
    set typad [expr $zoom*2+1]
    set tx $txpad
    set ty $typad
    set ft [link_type_font $zoom]

    $c create text $tx $ty -text $txt -anchor nw -justify left \
        -font $ft -fill $txt_color -width 0 -tags $t

    # bind mouse events
    set onenter "$c itemconfigure $t -fill $hover_color"
    bind $c <Enter> $onenter
    set onleave "$c itemconfigure $t -fill $txt_color"
    bind $c <Leave> $onleave

    # tooltip
    ceammc_tooltip $c $url

    # calc text bbox
    lassign [$c bbox $t] tx0 ty0 tx1 ty1
    set tw [expr $tx1 - $tx0]
    set th [expr abs($ty1 - $ty0)]

    # calc new width
    set nw $w
    set dispw [expr $tw + (1*$txpad)]
    if { $dispw != $nw } { set nw $dispw }

    # calc new height
    set nh 5
    set disph [expr $th + $typad]
    if { $disph > $nh } { set nh $disph }

    if { $nh != $h || $nw != $w } {
        pdsend "$rid .resize $nw $nh"
    }

}

}
