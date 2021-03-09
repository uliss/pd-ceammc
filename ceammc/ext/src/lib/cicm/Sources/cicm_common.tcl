# PATCHER MOUSE MOTION
proc eobj_canvas_motion {patcher val} {
    set rx [winfo rootx $patcher]
    set ry [winfo rooty $patcher]
    set x  [winfo pointerx .]
    set y  [winfo pointery .]
    pdtk_canvas_motion $patcher [expr $x - $rx] [expr $y - $ry] $val
}

# PATCHER MOUSE DOWN
proc eobj_canvas_down {patcher val} {
    set rx [winfo rootx $patcher]
    set ry [winfo rooty $patcher]
    set x  [winfo pointerx .]
    set y  [winfo pointery .]
    pdtk_canvas_mouse $patcher [expr $x - $rx] [expr $y - $ry] 0 $val
}

# PATCHER MOUSE UP
proc eobj_canvas_up {patcher} {
    set rx [winfo rootx $patcher]
    set ry [winfo rooty $patcher]
    set x  [winfo pointerx .]
    set y  [winfo pointery .]
    pdtk_canvas_mouseup $patcher [expr $x - $rx] [expr $y - $ry] 0
}

# PATCHER MOUSE RIGHT
proc eobj_canvas_right {patcher} {
    set rx [winfo rootx $patcher]
    set ry [winfo rooty $patcher]
    set x  [winfo pointerx .]
    set y  [winfo pointery .]
    pdtk_canvas_rightclick $patcher [expr $x - $rx] [expr $y - $ry] 0
}

# OBJECT SAVE FILE
proc eobj_saveas {name initialfile initialdir} {
    if { ! [file isdirectory $initialdir]} {set initialdir $::env(HOME)}
    set filename [tk_getSaveFile -initialfile $initialfile -initialdir $initialdir -filetypes $::filetypes]
    if {$filename eq ""} { return }

    set extension [file extension $filename]
    set oldfilename $filename

    if {$filename ne $oldfilename && [file exists $filename]} {
        set answer [tk_messageBox -type okcancel -icon question -default cancel-message [_ "$filename" already exists. Do you want to replace it?]]
        if {$answer eq "cancel"} { return }
    }
    set dirname [file dirname $filename]
    set basename [file tail $filename]
    pdsend "$name eobjwriteto [enquote_path $dirname/$basename]"
    set ::filenewdir $dirname
}

# OBJECT OPEN FILE
proc eobj_openfrom {name} {
    if { ! [file isdirectory $::filenewdir]} {
    set ::filenewdir [file normalize $::env(HOME)]
    }
    set files [tk_getOpenFile -multiple false -initialdir $::fileopendir]
    pdsend "$name eobjreadfrom [enquote_path $files]"
}

# RGBA TO HEX
proc eobj_rgba_to_hex {red green blue alpha} {
    set nR [expr int( $red * 65025 )]
    set nG [expr int( $green * 65025 )]
    set nB [expr int( $blue * 65025 )]
    set col [format {%4.4x} $nR]
    append col [format {%4.4x} $nG]
    append col [format {%4.4x} $nB]
    return #$col
}

# RGB int list to RGB float list
proc eobj_rgb_int_to_float {red green blue} {
    set nR [expr ( $red / 65025. )]
    set nG [expr ( $green / 65025. )]
    set nB [expr ( $blue / 65025. )]
    if {$nR > 1.} {set nR 1.}
    if {$nG > 1.} {set nG 1.}
    if {$nB > 1.} {set nB 1.}
    return [concat $nR $nG $nB]
}

proc ceammc_create_label_font_bold {font_name} {
    set font [ttk::style lookup TLabel -font]
    set family [font configure $font -family]
    set size [font configure $font -size]
    set slant [font configure $font -slant]
    set underline [font configure TkDefaultFont -underline]
    return [font create $font_name -family $family -size $size -weight bold -slant $slant -underline $underline]
}

proc ceammc_category_icon {state} {
    if {$state ne 0} {
        return ceammc_image_category_opened
    } {
        return ceammc_image_category_closed
    }
}

proc ceammc_category_apply {id var_name_state controls} {
    global $var_name_state
    set v [expr $$var_name_state]
    $id configure -image [ceammc_category_icon $v]

    foreach w $controls {
        if {$v eq 0 } {
            grid remove $w
        } {
            grid config $w
        }
    }

    event generate . <Expose>
    update
}

proc ceammc_category_toggle {id var_name_state controls} {
    global $var_name_state
    set v [expr $$var_name_state]
    if {$v ne 0} { set $var_name_state 0 } { set $var_name_state 1 }

    ceammc_category_apply $id $var_name_state $controls
}

# create images
if { $::tcl_version >= 8.6 } {
    image create photo ceammc_image_category_opened -width 18 -height 18 \
        -data "iVBORw0KGgoAAAANSUhEUgAAABIAAAASCAQAAAD8x0bcAAAAQklEQVR4AWMYDoAFnYcJNBnuMFjDebYMtxk0MBVJMtxg+MxgA1XyBciTBLMxlQElbfAoQShDKMGjDKYErzIgHFEAAGLzEOwIrN0jAAAAAElFTkSuQmCC"
    image create photo ceammc_image_category_closed -width 18 -height 18 \
        -data "iVBORw0KGgoAAAANSUhEUgAAABIAAAASCAQAAAD8x0bcAAAAPUlEQVR4AWMY9ICNoZCBlZAiH4b/DGsIK8sjTlkBccoqgcqW41fCy3CE4Q9DKO2UIIArw3egEoJAkmGIAABqKxDmokl7sAAAAABJRU5ErkJggg=="
} else {
    image create photo ceammc_image_category_opened -width 18 -height 18 \
        -data "R0lGODlhEgASAPAAAAAAAAAAACH5BAEAAAAAIf8LSW1hZ2VNYWdpY2sOZ2FtbWE9MC40NTQ1NDUALAAAAAASABIAAAIahI+py+0Po2RB1RauyY8D32UaBE7miabqmhQAOw=="
    image create photo ceammc_image_category_closed -width 18 -height 18 \
        -data "R0lGODlhEgASAPAAAAAAAAAAACH5BAEAAAAAIf8LSW1hZ2VNYWdpY2sOZ2FtbWE9MC40NTQ1NDUALAAAAAASABIAAAIYhI+py+0PowpBHmoNzjt2+D2hmJXmiToFADs="
}

# create font for categories in dialog
ceammc_create_label_font_bold CICMCategoryFont

# show tooltips
if { [catch {package require tooltip} ] } {
    proc ceammc_tooltip {id msg} {}
} {
    proc ceammc_tooltip {id msg} {
        tooltip::tooltip $id $msg
    }
}

namespace eval ::ceammc {
namespace eval ui {

# border
proc border_tag { id }   { return "bd${id}" }
proc border_w { w zoom } { expr $w * $zoom }
proc border_h { h zoom } { expr $h * $zoom }

proc border_x { zoom } { expr ($zoom == 1) ? 0 : 1 }
proc border_y { zoom } { expr ($zoom == 1) ? 0 : 1 }

proc border_draw { cnv id w h zoom color } {
    set tag [border_tag $id]
    set c [widget_canvas $cnv $id]
    set tags [$c find withtag $tag]
    # draw params

    set x [border_x $zoom]
    set y [border_y $zoom]
    set w [border_w $w $zoom]
    set h [border_h $h $zoom]

    if { [llength $tags] == 0 } {
        $c create rectangle $x $y $w $h -outline $color -width $zoom -tags $tag
    } else {
        set item [lindex $tags 0]
        $c coords $item $x $y $w $h
        $c itemconfigure $item -outline $color -width $zoom
    }

    $c raise $tag
}

# xlets
proc inlets_tag { id } { return "i${id}" }
proc inlets_tag_idx { id idx } { return "i${id}#${idx}" }

proc inlets_delete { cnv id } {
    set c [widget_canvas $cnv $id]
    $c delete [inlets_tag $id]
}

proc xlet_w { zoom } { expr 6*$zoom + 1 }

proc xlet_h { type zoom } {
    if { $type == "_" } { expr $zoom } { expr ($zoom==1) ? 2 : 4 }
}

proc xlet_color { type } {
    if { $type == "_" } { return "#000000" } { return "#2200AA" }
}

proc xlet_x { w i n zoom } {
    set ww [expr $w - [xlet_w $zoom]]
    return [expr round(($ww/($n-1.0)) * $i)]
}

proc inlet_draw { cnv id x y zoom type {idx 0} } {
    set c [widget_canvas $cnv $id]
    set x1 [expr $x + [xlet_w $zoom]]
    set y1 [expr $y + [xlet_h $type $zoom]]
    set color [xlet_color $type]

    set tags [inlets_tag $id]
    lappend tags [inlets_tag_idx $id $idx]

    $c create rectangle $x $y $x1 $y1 -fill $color -outline $color -width 1 -tags $tags
    $c raise $tags
}

proc inlets_draw { cnv id w h zoom str } {
    inlets_delete $cnv $id

    set c [widget_canvas $cnv $id]
    set w [border_w $w $zoom]
    set h [border_h $h $zoom]

    set n [string length $str]
    if { $n == 1 } {
        inlet_draw $cnv $id 0 0 $zoom $str 0
    } elseif { $n > 1 } {
        set i 0
        foreach inlet [split $str {}] {
            set x [xlet_x $w $i $n $zoom]
            inlet_draw $cnv $id $x 0 $zoom $inlet $i
            incr i
        }
    }
}

proc inlet_tooltip { cnv id idx str } {
    set c [widget_canvas $cnv $id]
    set win [widget_window $cnv $id]
    set tag [inlets_tag_idx $id $idx]
    xlet_tooltip::create $c $win $cnv $tag 1 $str
}

proc outlets_tag { id } { return "o${id}" }
proc outlets_tag_idx { id idx } { return "o${id}#${idx}" }

proc outlets_delete { cnv id } {
    set c [widget_canvas $cnv $id]
    $c delete [outlets_tag $id]
}

proc outlet_tooltip { cnv id idx str } {
    set c [widget_canvas $cnv $id]
    set win [widget_window $cnv $id]
    set tag [outlets_tag_idx $id $idx]
    xlet_tooltip::create $c $win $cnv $tag 0 $str
}

proc outlet_draw { cnv id x y w h zoom type {idx 0} } {
    set c [widget_canvas $cnv $id]
    set x1 [expr $x + [xlet_w $zoom]]
    set y0  [expr $h - [xlet_h $type $zoom]]
    set y1 [expr $h]
    set color [xlet_color $type]

    set tags [outlets_tag $id]
    lappend tags [outlets_tag_idx $id $idx]

    $c create rectangle $x $y0 $x1 $y1 -fill $color -outline $color -width 1 -tags $tags
    $c raise $tags
}

proc outlets_draw { cnv id w h zoom str } {
    outlets_delete $cnv $id

    set c [widget_canvas $cnv $id]
    set w [border_w $w $zoom]
    set h [border_h $h $zoom]

    set n [string length $str]
    if { $n == 1 } {
        outlet_draw $cnv $id 0 0 $w $h $zoom $str 0
    } elseif { $n > 1 } {
        set i 0
        foreach inlet [split $str {}] {
            set x [xlet_x $w $i $n $zoom]
            outlet_draw $cnv $id $x 0 $w $h $zoom $inlet $i
            incr i
        }
    }
}

# widget
proc widget_canvas { cnv id } {
    return "${cnv}.ecanvas${id}"
}

proc widget_window { cnv id } {
    return "${cnv}.ewindow${id}"
}

proc widget_w   { w zoom } { expr $w * $zoom + 1 }
proc widget_h   { h zoom } { expr $h * $zoom + 1 }
proc widget_tag { id } { return "${id}_#all" }

proc create_widget { id cnv w h zoom } {
    namespace eval "ebox$id" {}
    destroy $cnv

    set w [widget_w $w $zoom]
    set h [widget_h $h $zoom]

    canvas $cnv -width $w -height $h -bd 0 \
        -highlightthickness 0 -insertborderwidth 0 \
        -state normal -takefocus 1 -insertwidth 0 -confine 0
}

proc create_window { cnv id x y w h zoom } {
    set win [widget_window $cnv $id]
    set c [widget_canvas $cnv $id]
    set w [widget_w $w $zoom]
    set h [widget_h $h $zoom]

    $cnv create window $x $y -anchor nw -window $c -tags $win -width $w -height $h
}

proc widget_resize { cnv id w h zoom } {
    set win [widget_window $cnv $id]

    $cnv itemconfigure $win \
        -width [widget_w $w $zoom] \
        -height [widget_h $h $zoom]

    # update border, it should exists
    set c [widget_canvas $cnv $id]
    set bd [border_tag $id]
    set x [border_x $zoom]
    set y [border_y $zoom]
    set w [border_w $w $zoom]
    set h [border_h $h $zoom]

    $c coords $bd $x $y $w $h
}

proc widget_pos { cnv id x y } {
    set win [widget_window $cnv $id]
    $cnv coords $win $x $y
}

proc widget_move { cnv id x y } {
    set win [widget_window $cnv $id]
    $cnv move $win $x $y
}

proc widget_select { cnv id color } {
    set c [widget_canvas $cnv $id]
    set tag [border_tag $id]
    $c itemconfigure $tag -outline $color
}

proc widget_lower { cnv id } {
    lower [widget_canvas $cnv $id]
}

proc widget_raise { cnv id } {
    raise [widget_canvas $cnv $id]
}

proc widget_focus { cnv id } {
    focus [widget_canvas $cnv $id]
}

proc widget_delete { cnv id } {
    destroy %s [widget_canvas $cnv $id]
}

proc widget_bg { cnv id color } {
    set c [widget_canvas $cnv $id]
    $c configure -bg $color
}

proc label_tag { id } { return "${id}_lbl" }
proc label_makefont { font size } { return "$font $size roman normal" }

proc label_delete { cnv id inner } {
    set t [label_tag $id]
    if { $inner == 1 } {
        set c [widget_canvas $cnv $id]
        $c delete $t
    } else {
        $cnv delete $t
    }
}

proc label_canvas { cnv id inner } {
    if { $inner == 1 } {
        return [widget_canvas $cnv $id]
    } else { return $cnv }
}

proc label_create { cnv id x y inner anchor align font fontsize color text } {
    set t [label_tag $id]
    set all [widget_tag $id]
    set c [label_canvas $cnv $id $inner]

    $c create text $x $y -anchor $anchor -justify $align \
        -fill $color -font [label_makefont $font $fontsize] \
        -text "$text" -tags $t
}

proc label_pos { cnv id x y inner anchor align } {
    set t [label_tag $id]
    set c [label_canvas $cnv $id $inner]

    $c coords $t $x $y
    $c itemconfigure $t -anchor $anchor -justify $align
}

proc label_font { cnv id inner font fontsize } {
    set t [label_tag $id]
    set c [label_canvas $cnv $id $inner]
    $c itemconfigure $t -font [label_makefont $font $fontsize]
}

proc label_color { cnv id inner color } {
    set t [label_tag $id]
    set c [label_canvas $cnv $id $inner]
    $c itemconfigure $t -fill $color
}

proc label_text { cnv id inner text } {
    set t [label_tag $id]
    set c [label_canvas $cnv $id $inner]
    $c itemconfigure $t -text $text
}

proc label_inner_sync { cnv id inner } {
    if { $inner == 1 } {
        set t [label_tag $id]
        set c [label_canvas $cnv $id $inner]
        $c raise $t
    }
}

proc mouse_events_bind { cnv id target args }  {
    set c [widget_canvas $cnv $id]
    foreach name $args {
        set ev "ceammc_bind_mouse_$name"
        $ev $c $target
    }
}

proc mouse_cursor { cnv id name }  {
    set c [widget_canvas $cnv $id]
    $c configure -cursor $name
}

# from ttk::bindMouseWheel
proc bindMouseWheel {bindtag callback} {
    switch -- [tk windowingsystem] {
        x11 {
            bind $bindtag <ButtonPress-4> "$callback -1"
            bind $bindtag <ButtonPress-5> "$callback +1"
        }
        win32 {
            bind $bindtag <MouseWheel> [append callback { [expr {-(%D/120)}]}]
        }
        aqua {
            bind $bindtag <MouseWheel> [append callback { [expr {-(%D)}]} ]
        }
    }
}

proc spinboxScroll {id delta} {
    if {[string equal [focus] "$id"]} { ttk::spinbox::MouseWheel $id $delta }
    return -code break
}

proc comboboxScroll {id delta} {
    if {[string equal [focus] "$id"]} { ttk::combobox::Scroll $id $delta }
    return -code break
}

# https://wiki.tcl-lang.org/page/A+scrolled+frame
# sframe.tcl
# Paul Walton
# Create a ttk-compatible, scrollable frame widget.
#   Usage:
#       sframe new <path> ?-toplevel true?  ?-anchor nsew?
#       -> <path>
#
#       sframe content <path>
#       -> <path of child frame where the content should go>
namespace eval sframe {
   # Create a scrollable frame or window.
   proc new {path args} {
       # Use the ttk theme's background for the canvas and toplevel
       set bg [ttk::style lookup TFrame -background]
       if { [ttk::style theme use] eq "aqua" } {
           # Use a specific color on the aqua theme as 'ttk::style lookup' is not accurate.
           set bg "#e9e9e9"
       }

       # Create the main frame or toplevel.
       if { [dict exists $args -toplevel]  &&  [dict get $args -toplevel] } {
           toplevel $path -bg $bg
       } else {
           ttk::frame $path
       }

       # Create a scrollable canvas with scrollbars which will always be the same size as the main frame.
       set canvas [canvas $path.canvas -bg $bg -bd 0 -highlightthickness 0 -yscrollcommand [list $path.scrolly set]]
       ttk::scrollbar $path.scrolly -orient vertical -command [list $canvas yview]

       # Create a container frame which will always be the same size as the canvas or content, whichever is greater.
       # This allows the child content frame to be properly packed and also is a surefire way to use the proper ttk background.
       set container [ttk::frame $canvas.container]
       pack propagate $container 0

       # Create the content frame. Its size will be determined by its contents. This is useful for determining if the
       # scrollbars need to be shown.
       set content [ttk::frame $container.content]

       # Pack the content frame and place the container as a canvas item.
       set anchor "n"
       if { [dict exists $args -anchor] } {
           set anchor [dict get $args -anchor]
       }
       pack $content -anchor $anchor
       $canvas create window 0 0 -window $container -anchor nw

       # Grid the scrollable canvas sans scrollbars within the main frame.
       grid $canvas -row 0 -column 0 -sticky nsw
       grid rowconfigure    $path 0 -weight 1
       grid columnconfigure $path 0 -weight 1

       # Make adjustments when the sframe is resized or the contents change size.
       bind $path.canvas <Expose> [list ::ceammc::ui::sframe::resize $path]

       # Mousewheel bindings for scrolling.
       bind [winfo toplevel $path] <MouseWheel> [list ::ceammc::ui::sframe::scroll $path yview %W %D]
       return $path
   }


   # Given the toplevel path of an sframe widget, return the path of the child frame suitable for content.
   proc content {path} {
       return $path.canvas.container.content
   }

   # Make adjustments when the the sframe is resized or the contents change size.
   proc resize {path} {
       set canvas    $path.canvas
       set container $canvas.container
       set content   $container.content

       # Set the size of the container. At a minimum use the same width & height as the canvas.
       set width  [winfo width $canvas]
       set height [winfo height $canvas]

       # If the requested width or height of the content frame is greater then use that width or height.
       if { [winfo reqwidth $content] > $width } {
           set width [winfo reqwidth $content]
       }
       if { [winfo reqheight $content] > $height } {
           set height [winfo reqheight $content]
       }

       $canvas configure -width $width -height $height
       # $container configure -width $width -height $height

       # Configure the canvas's scroll region to match the height and width of the container.
       $canvas configure -scrollregion [list 0 0 $width $height]

       # Vertical scrolling.
       if { [winfo reqheight $content] > [winfo height $canvas] } {
           grid $path.scrolly -row 0 -column 1 -sticky ns
       } else {
           grid forget $path.scrolly
       }
       return
   }

   # Handle mousewheel scrolling.
   proc scroll {path view W D} {
       if { [winfo exists $path.canvas]  &&  [string match $path.canvas* $W] } {
           $path.canvas $view scroll [expr {-$D}] units
       }
       return -code break
   }
}

# xlets tooltip for UI objects
namespace eval xlet_tooltip {
    variable msg {}
    variable text_id {}
    variable is_active 0

    proc create {widget win cnv tag xlet text} {
        $widget bind $tag <Enter> [list ::ceammc::ui::xlet_tooltip::on_enter $widget $win $cnv $tag $xlet $text]
        $widget bind $tag <Leave> [list ::ceammc::ui::xlet_tooltip::on_leave $cnv]
    }

    proc on_enter {widget win cnv tag xlet text} {
        variable msg
        variable is_active

        set msg $text
        set is_active 1
        after 500 ::ceammc::ui::xlet_tooltip::show $widget $win $cnv $tag $xlet
    }

    proc on_leave {cnv} {
        variable is_active

        set is_active 0
        after 50 ::ceammc::ui::xlet_tooltip::hide $cnv
    }

    proc show {widget win cnv tag xlet} {
        variable is_active
        variable msg
        variable text_id

        if {$is_active == 0} return
        $cnv delete "cicm_tt"
        foreach {rx - - ry} [$widget bbox $tag] break
        foreach {cx cy - -} [$cnv bbox $win] break
        if [info exists ry] {
            if {$xlet == 0} {
               set text_id [$cnv create text [expr $cx+$rx] [expr $cy+$ry+7] -text $msg -font TkTooltipFont -anchor nw -tag "cicm_tt"]
            } else {
               set text_id [$cnv create text [expr $cx+$rx] [expr $cy+$ry-10] -text $msg -font TkTooltipFont -anchor sw -tag "cicm_tt"]
            }
            foreach {tx0 ty0 tx1 ty1} [$cnv bbox $text_id] break
            $cnv create rect [expr $tx0-2] [expr $ty0-1] [expr $tx1+2] [expr $ty1+1] -fill lightblue -tag "cicm_tt"
            $cnv raise $text_id
            $cnv bind $text_id <Leave> [list ::ceammc::ui::xlet_tooltip::on_leave $cnv]
        }
    }

    proc hide {cnv} {
        variable is_active

        if {$is_active == 1} return
        $cnv delete "cicm_tt"
    }
}
}
}
