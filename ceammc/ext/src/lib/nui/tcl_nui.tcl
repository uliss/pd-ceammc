namespace eval ::nui {

variable tkdnd_version
variable tkdnd_found 0

if { [catch {set tkdnd_version [package require tkdnd]} ] } {
    ::pdwindow::debug {[ceammc] no tkdnd found\n}
} {
    ::pdwindow::debug "\[ceammc\] tkdnd version: $tkdnd_version\n"
    set tkdnd_found 1
}

proc pd_canvas {cnv} { return ".x${cnv}.c" }

namespace eval utils {
    proc fix_mac_state {n} { if { $n > 256 } { return [expr $n & 0xFF ] } { return $n } }

    proc fix_win32_alt {n} {
        if { $n & 131072 } {
            incr n -131072
            set n [expr $n | 16]
        }
        return $n
    }

    proc fix_win32_state {n} {
        if { $n & 32 } { incr n -32 }
        return [expr 0xFF & [fix_win32_alt $n]]
    }

    proc send_mouse_move {obj x y mod} {
        switch -- $::windowingsystem {
            "aqua" {
                # disable mouse dragging for buttons other then first
                if { $mod < 512 } { pdsend "$obj mousemove $x $y [fix_mac_state $mod]" }
            } "win32" {
                set mod [fix_win32_alt $mod]
                if { $mod < 512 } { pdsend "$obj mousemove $x $y [fix_win32_state $mod]" }
            } "default" {
                pdsend "$obj mousemove $x $y $mod"
            }
        }
    }
}

proc widget_canvas {cnv id} { return ".x${cnv}.c.cnv${id}" }
proc widget_window {cnv id} { return ".x${cnv}.c.win${id}" }
proc widget_w   {w zoom} { expr $w * $zoom + 1 }
proc widget_h   {h zoom} { expr $h * $zoom + 1 }
proc widget_tag {id} { return "#${id}" }

proc canvas_motion {cnv id val} {
    set c  [pd_canvas $cnv]
    set rx [winfo rootx $c]
    set ry [winfo rooty $c]
    set x  [winfo pointerx .]
    set y  [winfo pointery .]
    pdtk_canvas_motion $c [expr $x - $rx] [expr $y - $ry] $val
}

proc canvas_down {cnv id val} {
    set c  [pd_canvas $cnv]
    set rx [winfo rootx $c]
    set ry [winfo rooty $c]
    set x  [winfo pointerx .]
    set y  [winfo pointery .]
    pdtk_canvas_mouse $c [expr $x - $rx] [expr $y - $ry] 0 $val
}

proc canvas_up {cnv id} {
    set c  [pd_canvas $cnv]
    set rx [winfo rootx $c]
    set ry [winfo rooty $c]
    set x  [winfo pointerx .]
    set y  [winfo pointery .]
    pdtk_canvas_mouseup $c [expr $x - $rx] [expr $y - $ry] 0
}

proc canvas_right {cnv id} {
    set c  [pd_canvas $cnv]
    set rx [winfo rootx $c]
    set ry [winfo rooty $c]
    set x  [winfo pointerx .]
    set y  [winfo pointery .]
    pdtk_canvas_rightclick $c [expr $x - $rx] [expr $y - $ry] 0
}

proc widget_create { cnv id x y w h zoom } {
    set win [widget_window $cnv $id]
    set c [widget_canvas $cnv $id]
    set w [widget_w $w $zoom]
    set h [widget_h $h $zoom]

    namespace eval "nui$id" {}
    destroy $c
    canvas $c -width $w -height $h -bd 0 \
        -highlightthickness 0 -insertborderwidth 0 \
        -state normal -takefocus 1 -insertwidth 0 -confine 0

    $c configure -bg #AAAAAA

    [pd_canvas $cnv] create window $x $y -anchor nw -window $c \
        -tags $win -width $w -height $h
}

proc widget_bg { cnv id color } {
    set c [widget_canvas $cnv $id]
    $c configure -bg $color
}

proc widget_erase { cnv id } {
    set win [widget_window $cnv $id]
    set c [widget_canvas $cnv $id]

    [pd_canvas $cnv] delete $win
    destroy $c
}

proc widget_resize { cnv id w h zoom } {
    set win [widget_window $cnv $id]
    set c [widget_canvas $cnv $id]

    set w [widget_w $w $zoom]
    set h [widget_h $h $zoom]

    $c configure -width $w -height $h
    [pd_canvas $cnv] itemconfigure $win -width $w -height $h
}

proc widget_move { cnv id x y } {
    set c [pd_canvas $cnv]
    set win [widget_window $cnv $id]
    $c coords $win $x $y
}

proc widget_focus { cnv id } {
    focus [widget_canvas $cnv $id]
}

proc widget_mouse_bind { cnv id target args }  {
    set c [widget_canvas $cnv $id]
    foreach name $args {
        set ev "widget_mouse_${name}_bind"
        $ev $c "#$target"
    }
}

proc widget_cursor { cnv id name }  {
    set c [widget_canvas $cnv $id]
    $c configure -cursor $name
}

proc widget_mouse_move_bind  {id obj} { bind $id <Motion> [subst {+::nui::utils::send_mouse_move $obj %x %y %s}] }
proc widget_mouse_enter_bind {id obj} { bind $id <Enter>  [subst {+pdsend "$obj mouseenter"}] }
proc widget_mouse_leave_bind {id obj} { bind $id <Leave>  [subst {+pdsend "$obj mouseleave"}] }

proc widget_mouse_down_bind {id obj} {
    switch -- $::windowingsystem {
        "win32" {
            bind $id <ButtonPress-1> [subst -nocommands {+pdsend "$obj mousedown %x %y %X %Y [ceammc_fix_win32_state %s]"}]
        } "default" {
            bind $id <ButtonPress-1> [subst {+pdsend "$obj mousedown %x %y %X %Y %s"}]
        }
    }
}

# sends #obj mouseup X Y MOD
proc widget_mouse_up_bind {id obj} {
    switch -- $::windowingsystem {
        "aqua" {
            bind $id <ButtonRelease-1> [subst -nocommands {+pdsend "$obj mouseup %x %y [ceammc_fix_macos_state %s]"}]
        } "win32" {
            bind $id <ButtonRelease-1> [subst -nocommands {+pdsend "$obj mouseup %x %y [ceammc_fix_win32_state %s]"}]
        } "default" {
            bind $id <ButtonRelease-1> [subst {+pdsend "$obj mouseup %x %y %s"}]
        }
    }
}

# sends #obj rightclick X Y ABSX ABSY MOD
proc widget_mouse_right_bind {id obj} {
    switch -- $::windowingsystem {
        "aqua" {
            bind $id <ButtonPress-2> [subst {+pdsend "$obj rightclick %x %y %X %Y %s"}]
            bind $id <Control-ButtonPress-1> [subst {+pdsend "$obj rightclick %x %y %X %Y %s"}]
        } "win32" {
            bind $id <ButtonPress-3> [subst -nocommands {+pdsend "$obj rightclick %x %y %X %Y [ceammc_fix_win32_state %s]"}]
        } "default" {
            bind $id <ButtonPress-3> [subst {+pdsend "$obj rightclick %x %y %X %Y %s"}]
        }
    }
}

proc widget_dnd_drop_bind_files {cnv id} {
    variable tkdnd_found
    if { !$tkdnd_found } { return }

    set c [widget_canvas $cnv $id]
    tkdnd::drop_target register $c DND_Files
    bind $c <<Drop:DND_Files>> [
        subst {
            pdsend "#$id dropfiles %D"
            return %A
        }
    ]
}

proc widget_dnd_drop_bind_text {cnv id} {
    variable tkdnd_found
    if { !$tkdnd_found } { return }

    set c [widget_canvas $cnv $id]
    tkdnd::drop_target register $c DND_Text
    bind $c <<Drop:DND_Text>> [
        subst {
            pdsend "#$id droptext %D"
            return %A
        }
    ]
}

if { [catch {package require tooltip} ] } {
    proc widget_tooltip { cnv model id msg } {}
} {
    proc widget_tooltip { cnv model id msg } {
        set c [widget_canvas $cnv $model]
        tooltip::tooltip $c -item $id $msg
    }
}

proc item_erase { cnv model tags } {
    set c [widget_canvas $cnv $model]
    $c delete $tags
}

proc item_move { cnv model tag x y } {
    set c [widget_canvas $cnv $model]

    lassign [$c coords $tag] x0 y0 x1 y1
    set w [expr $x1-$x0]
    set h [expr $y1-$y0]
    $c coords $tag $x $y [expr $x+$w] [expr $y+$h]
}

namespace eval xlet {
    proc tag     { id isout }   { if { $isout } { return "o${id}" }      { return "i${id}" } }
    proc tag_idx { id n isout } { if { $isout } { return "o${id}#${n}" } { return "i${id}#${n}" } }
    proc width  { zoom } { expr 6*$zoom + 1 }
    proc height { type zoom } { if { $type == "_" } { expr $zoom } { expr ($zoom==1) ? 2 : 4 } }
    proc xpos   { w i n zoom } {
        set ww [expr $w - [width $zoom]]
        return [expr round(($ww/($n-1.0)) * $i)]
    }
    proc out_ypos { h type zoom } { return [expr $h - [height $type $zoom]] }

    proc draw_single { cnv id x h zoom type idx ctl_color sig_color isout } {
        set c [::nui::widget_canvas $cnv $id]
        if { $isout } { set y [out_ypos $h $type $zoom] } { set y 0 }
        set x1 [expr $x + [::nui::xlet::width $zoom]]
        set y1 [expr $y + [::nui::xlet::height $type $zoom]]
        set t [tag $id $isout]
        lappend t [tag_idx $id $idx $isout]

        if {$type == "_"} { set color $ctl_color } { set color $sig_color }
        $c create rectangle $x $y $x1 $y1 -fill $color -outline $color -width 1 -tags $t
    }

    proc erase_all { cnv id isout } {
        set c [::nui::widget_canvas $cnv $id]
        $c delete [tag $id $isout]
    }

    proc draw_multiple { cnv id w h zoom str ctl_color sig_color isout } {
        set c [::nui::widget_canvas $cnv $id]
        $c delete [tag $id $isout]

        set n [string length $str]
        if { $n == 1 } {
            draw_single $cnv $id 0 $h $zoom $str 0 $ctl_color $sig_color $isout
        } elseif { $n > 1 } {
            set i 0
            foreach inlet [split $str {}] {
                set x [::nui::xlet::xpos $w $i $n $zoom]
                draw_single $cnv $id $x $h $zoom $inlet $i $ctl_color $sig_color $isout
                incr i
            }
        }
    }

    proc tooltip { cnv id idx str } {
        set c [::nui::widget_canvas $cnv $id]
        set win [::nui::widget_window $cnv $id]
        set tag [tag_idx $id $idx]
        xlet_tooltip::create $c $win $cnv $tag 1 $str
    }
}

namespace eval box {
    proc tag_border { id } { return "#b${id}" }
    proc tag_fill   { id } { return "#b${id}_bg" }

    proc create { cnv model id x y w h zoom border_color fill_color ctl_color sig_color border_width inlets outlets } {
        set c [::nui::widget_canvas $cnv $model]
        set tb [tag_border $id]
        set tf [tag_fill $id]
        # zoomed border correction
        if { $zoom > 1} {
            set x0 [expr $x+($zoom)/2]
            set y0 [expr $y+($zoom)/2]
        } {
            set x0 $x
            set y0 $y
        }
        set x1 [expr $x+$w]
        set y1 [expr $y+$h]

        $c create rectangle $x0 $y0 $x1 $y1 \
            -fill {} -outline $border_color -width $border_width -tags $tb

        $c create rectangle $x0 $y0 $x1 $y1 \
            -fill $fill_color -outline {} -width 0 -tags $tf
        $c lower $tf

        ::nui::xlet::draw_multiple $cnv $model $w $h $zoom $inlets $ctl_color $sig_color false
        ::nui::xlet::draw_multiple $cnv $model $w $h $zoom $outlets $ctl_color $sig_color true
    }

    proc update { cnv model id w h zoom border_color fill_color ctl_color sig_color border_width inlets outlets } {
        set c [::nui::widget_canvas $cnv $model]
        set tb [tag_border $id]
        set tf [tag_fill $id]

        $c itemconfigure $tb -outline $border_color
        $c itemconfigure $tf -fill $fill_color

        lassign [$c coords $tb] x0 y0 x1 y1
        $c coords $tb $x0 $y0 [expr $x0+$w] [expr $y0+$h]
        $c coords $tf $x0 $y0 [expr $x0+$w] [expr $y0+$h]

        ::nui::xlet::draw_multiple $cnv $model $w $h $zoom $inlets $ctl_color $sig_color false
        ::nui::xlet::draw_multiple $cnv $model $w $h $zoom $outlets $ctl_color $sig_color true
    }

    proc erase { cnv model id } {
        ::nui::item_erase $cnv $model [tag $id]
    }

    proc move  { cnv model id x y } {
        ::nui::item_move $cnv $model [tag $id] $x $y
    }
}

namespace eval frame {
    proc tag { id } { return "#f${id}" }

    proc create { cnv model id x y w h border_color fill_color width } {
        set c [::nui::widget_canvas $cnv $model]
        set t [tag $id]
        $c create rectangle $x $y [expr $x+$w] [expr $y+$h] \
            -fill $fill_color -outline $border_color -width $width -tags $t
    }

    proc update { cnv model id w h border_color fill_color } {
        set c [::nui::widget_canvas $cnv $model]
        set t [tag $id]
        $c itemconfigure $t -outline $border_color -fill $fill_color
        lassign [$c coords $t] x0 y0 x1 y1
        $c coords $t $x0 $y0 [expr $x0+$w] [expr $y0+$h]
    }

    proc erase { cnv model id } {
        ::nui::item_erase $cnv $model [tag $id]
    }

    proc move  { cnv model id x y } {
        ::nui::item_move $cnv $model [tag $id] $x $y
    }
}

namespace eval toggle {
    proc tag_all  { id } { return "#tgl${id}" }
    proc tag_box  { id } { return "#tgl${id}_box" }
    proc tag_knob { id } { return "#tgl${id}_kn" }

    proc draw_knob { c id color x y w h zoom } {
        set t [tag_knob $id]
        set ta [tag_all $id]
        set xpad [expr floor($w * 0.1)]
        set ypad [expr floor($h * 0.1)]
        if {$xpad > 16} { set xpad 16 }
        if {$ypad > 16} { set ypad 16 }

        if {$zoom == 1} {set corr 1} {set corr -1}

        set x0 [expr $x + $xpad + $corr]
        set y0 [expr $y + $ypad + $corr]
        set x1 [expr $x + $w - $xpad]
        set y1 [expr $y + $h - $ypad]
        set lw [expr 1 + $zoom]

        if {$w>30} {set lw [expr $lw + $xpad*0.5]}

        $c create line $x0 $y0 $x1 $y1 -fill $color -width $lw -capstyle butt -tags [list $t $ta]
        $c create line $x0 $y1 $x1 $y0 -fill $color -width $lw -capstyle butt -tags [list $t $ta]
    }

    proc create { cnv model id x y w h zoom value out_color fill_color knob_color } {
        set c [::nui::widget_canvas $cnv $model]
        set ta [tag_all $id]
        set tb [tag_box $id]

        # draw box
        $c create rectangle $x $y [expr $x+$w] [expr $y+$h] \
            -fill $fill_color -outline $out_color -width $zoom -tags [list $ta $tb]

        if { $value } { draw_knob $c $id $knob_color $x $y $w $h $zoom }
    }

    proc update { cnv model id zoom value out_color fill_color knob_color } {
        set c [::nui::widget_canvas $cnv $model]
        set tb [tag_box $id]

        # update box
        $c itemconfigure $tb -fill $fill_color -outline $out_color

        lassign [$c coords $tb] x0 y0 x1 y1
        set w [expr $x1-$x0]
        set h [expr $y1-$y0]
        $c delete [tag_knob $id]
        if { $value > 0 } { draw_knob $c $id $knob_color $x0 $y0 $w $h $zoom }
    }

    proc erase { cnv model id } {
        set c [::nui::widget_canvas $cnv $model]
        $c delete [tag_all $id]
    }
}

namespace eval bar {
    proc tag  { id } { return "#bar${id}" }

    proc hcreate { cnv model id x y w h pos fill_color } {
        set c [::nui::widget_canvas $cnv $model]
        set t [tag $id]

        set w [expr $pos*$w]
        $c create rectangle $x $y [expr $x+$w] [expr $y+$h] \
            -fill $fill_color -outline $fill_color -width 1 -tags $t
    }

    proc hupdate { cnv model id w h pos fill_color } {
        set c [::nui::widget_canvas $cnv $model]
        set t [tag $id]

        $c itemconfigure $t -fill $fill_color -outline $fill_color

        lassign [$c coords $t] x0 y0 x1 y1
        set x1 [expr $x0 + $w*$pos]
        $c coords $t $x0 $y0 $x1 $y1
    }
}

}
