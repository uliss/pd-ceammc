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
