// this is autogenerated file: do not edit
// clang-format off
#ifndef cicm_common_tcl_h_
#define cicm_common_tcl_h_
const char* cicm_common_tcl = 
"proc eobj_canvas_motion {patcher val} {\n"
"    set rx [winfo rootx $patcher]\n"
"    set ry [winfo rooty $patcher]\n"
"    set x  [winfo pointerx .]\n"
"    set y  [winfo pointery .]\n"
"    pdtk_canvas_motion $patcher [expr $x - $rx] [expr $y - $ry] $val\n"
"}\n"
"proc eobj_canvas_down {patcher val} {\n"
"    set rx [winfo rootx $patcher]\n"
"    set ry [winfo rooty $patcher]\n"
"    set x  [winfo pointerx .]\n"
"    set y  [winfo pointery .]\n"
"    pdtk_canvas_mouse $patcher [expr $x - $rx] [expr $y - $ry] 0 $val\n"
"}\n"
"proc eobj_canvas_up {patcher} {\n"
"    set rx [winfo rootx $patcher]\n"
"    set ry [winfo rooty $patcher]\n"
"    set x  [winfo pointerx .]\n"
"    set y  [winfo pointery .]\n"
"    pdtk_canvas_mouseup $patcher [expr $x - $rx] [expr $y - $ry] 0\n"
"}\n"
"proc eobj_canvas_right {patcher} {\n"
"    set rx [winfo rootx $patcher]\n"
"    set ry [winfo rooty $patcher]\n"
"    set x  [winfo pointerx .]\n"
"    set y  [winfo pointery .]\n"
"    pdtk_canvas_rightclick $patcher [expr $x - $rx] [expr $y - $ry] 0\n"
"}\n"
"proc eobj_saveas {name initialfile initialdir} {\n"
"    if { ! [file isdirectory $initialdir]} {set initialdir $::env(HOME)}\n"
"    set filename [tk_getSaveFile -initialfile $initialfile -initialdir $initialdir -filetypes $::filetypes]\n"
"    if {$filename eq \"\"} { return }\n"
"    set extension [file extension $filename]\n"
"    set oldfilename $filename\n"
"    if {$filename ne $oldfilename && [file exists $filename]} {\n"
"        set answer [tk_messageBox -type okcancel -icon question -default cancel-message [_ \"$filename\" already exists. Do you want to replace it?]]\n"
"        if {$answer eq \"cancel\"} { return }\n"
"    }\n"
"    set dirname [file dirname $filename]\n"
"    set basename [file tail $filename]\n"
"    pdsend \"$name eobjwriteto [enquote_path $dirname/$basename]\"\n"
"    set ::filenewdir $dirname\n"
"}\n"
"proc eobj_openfrom {name} {\n"
"    if { ! [file isdirectory $::filenewdir]} {\n"
"    set ::filenewdir [file normalize $::env(HOME)]\n"
"    }\n"
"    set files [tk_getOpenFile -multiple false -initialdir $::fileopendir]\n"
"    pdsend \"$name eobjreadfrom [enquote_path $files]\"\n"
"}\n"
"proc eobj_rgba_to_hex {red green blue alpha} {\n"
"    set nR [expr int( $red * 65025 )]\n"
"    set nG [expr int( $green * 65025 )]\n"
"    set nB [expr int( $blue * 65025 )]\n"
"    set col [format {%4.4x} $nR]\n"
"    append col [format {%4.4x} $nG]\n"
"    append col [format {%4.4x} $nB]\n"
"    return #$col\n"
"}\n"
"proc eobj_rgb_int_to_float {red green blue} {\n"
"    set nR [expr ( $red / 65025. )]\n"
"    set nG [expr ( $green / 65025. )]\n"
"    set nB [expr ( $blue / 65025. )]\n"
"    if {$nR > 1.} {set nR 1.}\n"
"    if {$nG > 1.} {set nG 1.}\n"
"    if {$nB > 1.} {set nB 1.}\n"
"    return [concat $nR $nG $nB]\n"
"}\n"
"proc ceammc_create_label_font_bold {font_name} {\n"
"    set font [ttk::style lookup TLabel -font]\n"
"    set family [font configure $font -family]\n"
"    set size [font configure $font -size]\n"
"    set slant [font configure $font -slant]\n"
"    set underline [font configure TkDefaultFont -underline]\n"
"    return [font create $font_name -family $family -size $size -weight bold -slant $slant -underline $underline]\n"
"}\n"
"proc ceammc_category_icon {state} {\n"
"    if {$state ne 0} {\n"
"        return ceammc_image_category_opened\n"
"    } {\n"
"        return ceammc_image_category_closed\n"
"    }\n"
"}\n"
"proc ceammc_category_apply {id var_name_state controls} {\n"
"    global $var_name_state\n"
"    set v [expr $$var_name_state]\n"
"    $id configure -image [ceammc_category_icon $v]\n"
"    foreach w $controls {\n"
"        if {$v eq 0 } {\n"
"            grid remove $w\n"
"        } {\n"
"            grid config $w\n"
"        }\n"
"    }\n"
"    event generate . <Expose>\n"
"    update\n"
"}\n"
"proc ceammc_category_toggle {id var_name_state controls} {\n"
"    global $var_name_state\n"
"    set v [expr $$var_name_state]\n"
"    if {$v ne 0} { set $var_name_state 0 } { set $var_name_state 1 }\n"
"    ceammc_category_apply $id $var_name_state $controls\n"
"}\n"
"if { $::tcl_version >= 8.6 } {\n"
"    image create photo ceammc_image_category_opened -width 18 -height 18 \\\n"
"        -data \"iVBORw0KGgoAAAANSUhEUgAAABIAAAASCAQAAAD8x0bcAAAAQklEQVR4AWMYDoAFnYcJNBnuMFjDebYMtxk0MBVJMtxg+MxgA1XyBciTBLMxlQElbfAoQShDKMGjDKYErzIgHFEAAGLzEOwIrN0jAAAAAElFTkSuQmCC\"\n"
"    image create photo ceammc_image_category_closed -width 18 -height 18 \\\n"
"        -data \"iVBORw0KGgoAAAANSUhEUgAAABIAAAASCAQAAAD8x0bcAAAAPUlEQVR4AWMY9ICNoZCBlZAiH4b/DGsIK8sjTlkBccoqgcqW41fCy3CE4Q9DKO2UIIArw3egEoJAkmGIAABqKxDmokl7sAAAAABJRU5ErkJggg==\"\n"
"} else {\n"
"    image create photo ceammc_image_category_opened -width 18 -height 18 \\\n"
"        -data \"R0lGODlhEgASAPAAAAAAAAAAACH5BAEAAAAAIf8LSW1hZ2VNYWdpY2sOZ2FtbWE9MC40NTQ1NDUALAAAAAASABIAAAIahI+py+0Po2RB1RauyY8D32UaBE7miabqmhQAOw==\"\n"
"    image create photo ceammc_image_category_closed -width 18 -height 18 \\\n"
"        -data \"R0lGODlhEgASAPAAAAAAAAAAACH5BAEAAAAAIf8LSW1hZ2VNYWdpY2sOZ2FtbWE9MC40NTQ1NDUALAAAAAASABIAAAIYhI+py+0PowpBHmoNzjt2+D2hmJXmiToFADs=\"\n"
"}\n"
"ceammc_create_label_font_bold CICMCategoryFont\n"
"if { [catch {package require tooltip} ] } {\n"
"    proc ceammc_tooltip {id msg} {}\n"
"} {\n"
"    proc ceammc_tooltip {id msg} {\n"
"        tooltip::tooltip $id $msg\n"
"    }\n"
"}\n"
"namespace eval ::ceammc {\n"
"namespace eval ui {\n"
"proc border_tag { id } {\n"
"    return \"bd${id}\"\n"
"}\n"
"proc border_w { w zoom } {\n"
"    if { $zoom } {\n"
"        expr ($w * $zoom) + $zoom\n"
"    } else {\n"
"        expr ($w * $zoom) + $zoom + 1\n"
"    }\n"
"}\n"
"proc border_h { h zoom } {\n"
"    if { $zoom } {\n"
"        expr ($h * $zoom) + $zoom\n"
"    } else {\n"
"        expr ($h * $zoom) + $zoom + 1\n"
"    }\n"
"}\n"
"proc border_x { zoom } { expr ($zoom == 1) ? 0 : 1 }\n"
"proc border_y { zoom } { expr ($zoom == 1) ? 0 : 1 }\n"
"proc border_draw { cnv id w h zoom color } {\n"
"    set tag [border_tag $id]\n"
"    set c [widget_canvas $cnv $id]\n"
"    set tags [$c find withtag $tag]\n"
"    # draw params\n"
"    set wd $zoom\n"
"    set x [border_x $zoom]\n"
"    set y [border_y $zoom]\n"
"    set w [border_w $w $zoom]\n"
"    set h [border_h $h $zoom]\n"
"    if { [llength $tags] == 0 } {\n"
"        $c create rectangle $x $y $w $h -outline $color -width $wd -tags $tag\n"
"    } else {\n"
"        set item [lindex $tags 0]\n"
"        $c coords $item $x $y $w $h\n"
"        $c itemconfigure $item -outline $color -width $wd\n"
"    }\n"
"}\n"
"proc widget_canvas { cnv id } {\n"
"    return \"${cnv}.ecanvas${id}\"\n"
"}\n"
"proc widget_window { cnv id } {\n"
"    return \"${cnv}.ewindow${id}\"\n"
"}\n"
"proc widget_w { w zoom } { expr ($w * $zoom) + (2*$zoom) }\n"
"proc widget_h { h zoom } { expr ($h * $zoom) + (2*$zoom) }\n"
"proc create_widget { id cnv w h zoom } {\n"
"    namespace eval \"ebox$id\" {}\n"
"    destroy $cnv\n"
"    set w [widget_w $w $zoom]\n"
"    set h [widget_h $h $zoom]\n"
"    canvas $cnv -width $w -height $h -bd 0 \\\n"
"        -highlightthickness 0 -insertborderwidth 0 \\\n"
"        -state normal -takefocus 1 -insertwidth 0 -confine 0\n"
"}\n"
"proc create_window { cnv id x y w h zoom } {\n"
"    set win [widget_window $cnv $id]\n"
"    set c [widget_canvas $cnv $id]\n"
"    set w [widget_w $w $zoom]\n"
"    set h [widget_h $h $zoom]\n"
"    $cnv create window $x $y -anchor nw -window $c -tags $win -width $w -height $h\n"
"}\n"
"proc widget_resize { cnv id w h zoom } {\n"
"    set win [widget_window $cnv $id]\n"
"    $cnv itemconfigure $win \\\n"
"        -width [widget_w $w $zoom] \\\n"
"        -height [widget_h $h $zoom]\n"
"    # update border, it should exists\n"
"    set c [widget_canvas $cnv $id]\n"
"    set bd [border_tag $id]\n"
"    set x [border_x $zoom]\n"
"    set y [border_y $zoom]\n"
"    set w [border_w $w $zoom]\n"
"    set h [border_h $h $zoom]\n"
"    $c coords $bd $x $y $w $h\n"
"}\n"
"proc widget_pos { cnv id x y } {\n"
"    set win [widget_window $cnv $id]\n"
"    $cnv coords $win $x $y\n"
"}\n"
"proc widget_select { cnv id color } {\n"
"    set c [widget_canvas $cnv $id]\n"
"    set tag [border_tag $id]\n"
"    $c itemconfigure $tag -outline $color\n"
"}\n"
"proc widget_lower { cnv id } {\n"
"    lower [widget_canvas $cnv $id]\n"
"}\n"
"proc widget_bg { cnv id color } {\n"
"    set c [widget_canvas $cnv $id]\n"
"    $c configure -bg $color\n"
"}\n"
"proc bindMouseWheel {bindtag callback} {\n"
"    switch -- [tk windowingsystem] {\n"
"        x11 {\n"
"            bind $bindtag <ButtonPress-4> \"$callback -1\"\n"
"            bind $bindtag <ButtonPress-5> \"$callback +1\"\n"
"        }\n"
"        win32 {\n"
"            bind $bindtag <MouseWheel> [append callback { [expr {-(%D/120)}]}]\n"
"        }\n"
"        aqua {\n"
"            bind $bindtag <MouseWheel> [append callback { [expr {-(%D)}]} ]\n"
"        }\n"
"    }\n"
"}\n"
"proc spinboxScroll {id delta} {\n"
"    if {[string equal [focus] \"$id\"]} { ttk::spinbox::MouseWheel $id $delta }\n"
"    return -code break\n"
"}\n"
"proc comboboxScroll {id delta} {\n"
"    if {[string equal [focus] \"$id\"]} { ttk::combobox::Scroll $id $delta }\n"
"    return -code break\n"
"}\n"
"namespace eval sframe {\n"
"   # Create a scrollable frame or window.\n"
"   proc new {path args} {\n"
"       # Use the ttk theme's background for the canvas and toplevel\n"
"       set bg [ttk::style lookup TFrame -background]\n"
"       if { [ttk::style theme use] eq \"aqua\" } {\n"
"           # Use a specific color on the aqua theme as 'ttk::style lookup' is not accurate.\n"
"           set bg \"#e9e9e9\"\n"
"       }\n"
"       # Create the main frame or toplevel.\n"
"       if { [dict exists $args -toplevel]  &&  [dict get $args -toplevel] } {\n"
"           toplevel $path -bg $bg\n"
"       } else {\n"
"           ttk::frame $path\n"
"       }\n"
"       # Create a scrollable canvas with scrollbars which will always be the same size as the main frame.\n"
"       set canvas [canvas $path.canvas -bg $bg -bd 0 -highlightthickness 0 -yscrollcommand [list $path.scrolly set]]\n"
"       ttk::scrollbar $path.scrolly -orient vertical -command [list $canvas yview]\n"
"       # Create a container frame which will always be the same size as the canvas or content, whichever is greater.\n"
"       # This allows the child content frame to be properly packed and also is a surefire way to use the proper ttk background.\n"
"       set container [ttk::frame $canvas.container]\n"
"       pack propagate $container 0\n"
"       # Create the content frame. Its size will be determined by its contents. This is useful for determining if the\n"
"       # scrollbars need to be shown.\n"
"       set content [ttk::frame $container.content]\n"
"       # Pack the content frame and place the container as a canvas item.\n"
"       set anchor \"n\"\n"
"       if { [dict exists $args -anchor] } {\n"
"           set anchor [dict get $args -anchor]\n"
"       }\n"
"       pack $content -anchor $anchor\n"
"       $canvas create window 0 0 -window $container -anchor nw\n"
"       # Grid the scrollable canvas sans scrollbars within the main frame.\n"
"       grid $canvas -row 0 -column 0 -sticky nsw\n"
"       grid rowconfigure    $path 0 -weight 1\n"
"       grid columnconfigure $path 0 -weight 1\n"
"       # Make adjustments when the sframe is resized or the contents change size.\n"
"       bind $path.canvas <Expose> [list ::ceammc::ui::sframe::resize $path]\n"
"       # Mousewheel bindings for scrolling.\n"
"       bind [winfo toplevel $path] <MouseWheel> [list ::ceammc::ui::sframe::scroll $path yview %W %D]\n"
"       return $path\n"
"   }\n"
"   # Given the toplevel path of an sframe widget, return the path of the child frame suitable for content.\n"
"   proc content {path} {\n"
"       return $path.canvas.container.content\n"
"   }\n"
"   # Make adjustments when the the sframe is resized or the contents change size.\n"
"   proc resize {path} {\n"
"       set canvas    $path.canvas\n"
"       set container $canvas.container\n"
"       set content   $container.content\n"
"       # Set the size of the container. At a minimum use the same width & height as the canvas.\n"
"       set width  [winfo width $canvas]\n"
"       set height [winfo height $canvas]\n"
"       # If the requested width or height of the content frame is greater then use that width or height.\n"
"       if { [winfo reqwidth $content] > $width } {\n"
"           set width [winfo reqwidth $content]\n"
"       }\n"
"       if { [winfo reqheight $content] > $height } {\n"
"           set height [winfo reqheight $content]\n"
"       }\n"
"       $canvas configure -width $width -height $height\n"
"       # $container configure -width $width -height $height\n"
"       # Configure the canvas's scroll region to match the height and width of the container.\n"
"       $canvas configure -scrollregion [list 0 0 $width $height]\n"
"       # Vertical scrolling.\n"
"       if { [winfo reqheight $content] > [winfo height $canvas] } {\n"
"           grid $path.scrolly -row 0 -column 1 -sticky ns\n"
"       } else {\n"
"           grid forget $path.scrolly\n"
"       }\n"
"       return\n"
"   }\n"
"   # Handle mousewheel scrolling.\n"
"   proc scroll {path view W D} {\n"
"       if { [winfo exists $path.canvas]  &&  [string match $path.canvas* $W] } {\n"
"           $path.canvas $view scroll [expr {-$D}] units\n"
"       }\n"
"       return -code break\n"
"   }\n"
"}\n"
"namespace eval xlet_tooltip {\n"
"    variable msg {}\n"
"    variable text_id {}\n"
"    variable is_active 0\n"
"    proc create {widget win cnv tag xlet text} {\n"
"        $widget bind $tag <Enter> [list ::ceammc::ui::xlet_tooltip::on_enter $widget $win $cnv $tag $xlet $text]\n"
"        $widget bind $tag <Leave> [list ::ceammc::ui::xlet_tooltip::on_leave $cnv]\n"
"    }\n"
"    proc on_enter {widget win cnv tag xlet text} {\n"
"        variable msg\n"
"        variable is_active\n"
"        set msg $text\n"
"        set is_active 1\n"
"        after 500 ::ceammc::ui::xlet_tooltip::show $widget $win $cnv $tag $xlet\n"
"    }\n"
"    proc on_leave {cnv} {\n"
"        variable is_active\n"
"        set is_active 0\n"
"        after 50 ::ceammc::ui::xlet_tooltip::hide $cnv\n"
"    }\n"
"    proc show {widget win cnv tag xlet} {\n"
"        variable is_active\n"
"        variable msg\n"
"        variable text_id\n"
"        if {$is_active == 0} return\n"
"        $cnv delete \"cicm_tt\"\n"
"        foreach {rx - - ry} [$widget bbox $tag] break\n"
"        foreach {cx cy - -} [$cnv bbox $win] break\n"
"        if [info exists ry] {\n"
"            if {$xlet == 0} {\n"
"               set text_id [$cnv create text [expr $cx+$rx] [expr $cy+$ry+7] -text $msg -font TkTooltipFont -anchor nw -tag \"cicm_tt\"]\n"
"            } else {\n"
"               set text_id [$cnv create text [expr $cx+$rx] [expr $cy+$ry-10] -text $msg -font TkTooltipFont -anchor sw -tag \"cicm_tt\"]\n"
"            }\n"
"            foreach {tx0 ty0 tx1 ty1} [$cnv bbox $text_id] break\n"
"            $cnv create rect [expr $tx0-2] [expr $ty0-1] [expr $tx1+2] [expr $ty1+1] -fill lightblue -tag \"cicm_tt\"\n"
"            $cnv raise $text_id\n"
"            $cnv bind $text_id <Leave> [list ::ceammc::ui::xlet_tooltip::on_leave $cnv]\n"
"        }\n"
"    }\n"
"    proc hide {cnv} {\n"
"        variable is_active\n"
"        if {$is_active == 1} return\n"
"        $cnv delete \"cicm_tt\"\n"
"    }\n"
"}\n"
"}\n"
"}\n"
;
#endif
// clang-format on
