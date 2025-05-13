namespace eval ::nui {
namespace eval combobox {
    proc tag_all  { id } { return "#cmb${id}" }
    proc tag_box  { id } { return "#cmb${id}_box" }
    proc tag_sep  { id } { return "#cmb${id}_sep" }
    proc tag_txt  { id } { return "#cmb${id}_txt" }
    proc tag_open { id } { return "#cmb${id}_open" }
    proc tag_tri0 { id } { return "#cmb${id}_tri0" }
    proc tag_tri1 { id } { return "#cmb${id}_tri1" }

    proc triangle_up {x y w} {
        return [list $x $y [expr $x+($w*0.5)] [expr $y-($w*0.58)] [expr $x+$w] $y]
    }

    proc triangle_down {x y w} {
        return [list $x $y [expr $x+($w*0.5)] [expr $y+($w*0.58)] [expr $x+$w] $y]
    }

    proc create { cnv model id x y w h zoom current_item font border_color fill_color text_color } {
        set c       [::nui::widget_canvas $cnv $model]
        set ALL     [tag_all $id]
        set BOX     [tag_box $id]
        set TXT     [tag_txt $id]
        set SEP     [tag_sep $id]
        set TRI0    [tag_tri0 $id]
        set TRI1    [tag_tri1 $id]

        set xpad [expr 10*$zoom]
        # right x
        set rx [expr $x+$w]
        # left x
        set lx [expr $rx-$xpad]

        # draw box
        $c create rectangle $x $y $rx [expr $y+$h] \
            -fill $fill_color -outline $border_color -width $zoom -tags [list $ALL $BOX]

        # selection box
        set tri_w  [expr $xpad*0.6]
        set tri_x  [expr $lx+$xpad*0.2]

        # triangles
        # upper triangle
        $c create polygon [triangle_up   $tri_x [expr $h*0.3] $tri_w] \
            -fill $fill_color -outline $border_color -width $zoom -tags [list $ALL $TRI0]
        # lower triangle
        $c create polygon [triangle_down $tri_x [expr $h*0.7] $tri_w] \
            -fill $fill_color -outline $border_color -width $zoom -tags [list $ALL $TRI1]

        # separator
        $c create line $lx $y $lx [expr $y+$h] -fill $border_color -width $zoom -tags [list $ALL $SEP]

        set txt_x [expr $x + 5]
        set txt_y [expr $y+($h*0.5)]

        # text item
        $c create text $txt_x $txt_y -fill $text_color -text $current_item -font $font -anchor w -width [expr $w-10] -tags [list $ALL $TXT]

        bind $c <Up>                [subst {::nui::combobox::prev  $model}]
        bind $c <Down>              [subst {::nui::combobox::next  $model}]
        bind $c <Begin>             [subst {::nui::combobox::begin $model}]
        bind $c <End>               [subst {::nui::combobox::end   $model}]
        bind $c <KeyPress-Return>   [subst {::nui::combobox::open  $model 0 1}]
    }

    proc update { cnv model id x y w h zoom is_open index items font border_color fill_color text_color active_color } {
        set c       [::nui::widget_canvas $cnv $model]
        set ALL     [tag_all $id]
        set BOX     [tag_box $id]
        set TXT     [tag_txt $id]
        set SEP     [tag_sep $id]
        set OPEN    [tag_open $id]
        set TRI0    [tag_tri0 $id]
        set TRI1    [tag_tri1 $id]

        lassign [$c coords $BOX] x0 y0 x1 y1
        set old_w [expr $x1-$x0]
        if { $old_w != $w || $x != $x0 || $y != $y0 } {
            # update size
            set xpad [expr 10*$zoom]
            set rx [expr $x+$w]
            set lx [expr $rx-$xpad]
            $c coords $SEP $lx $y $lx [expr $y+$h]

            # separator position
            $c coords $BOX $x $y $rx  [expr $y+$h]

            # update triangles pos
            set tri_w  [expr $xpad*0.6]
            set tri_x  [expr $lx+$xpad*0.2]
            $c coords $TRI0 [triangle_up   $tri_x [expr $h*0.3] $tri_w]
            $c coords $TRI1 [triangle_down $tri_x [expr $h*0.7] $tri_w]
        }

        # update colors
        $c itemconfigure $BOX   -fill $fill_color -outline $border_color
        $c itemconfigure $SEP   -fill $border_color
        $c itemconfigure $TRI0  -fill $fill_color -outline $border_color
        $c itemconfigure $TRI1  -fill $fill_color -outline $border_color

        # parent canvas
        set pc [::nui::pd_canvas $cnv]
        # remove popup if exists
        $pc delete $OPEN

        if { $is_open } {
            set win  [::nui::widget_window $cnv $model]
            set wcnv [::nui::widget_canvas $cnv $model]

            set nitems [llength $items]
            lassign [$pc coords $win] gx gy
            set gx0 $gx
            set gy0 [expr $gy+$h]
            set gx1 [expr $gx0+$w-(10*$zoom)]
            set gy1 [expr $gy0+($h*$nitems)]

            set txt_left [expr $gx0+5]

            # draw items
            for {set i 0} {$i < $nitems} {incr i} {
                set item_top [expr $gy0+($i*$h)]
                set item_bot [expr $gy0+(($i+1)*$h)]

                set item_color $fill_color
                # highlight active element
                if { $i == $index } { set item_color $active_color }
                # item rect
                set wid [$pc create rectangle $gx0 $item_top $gx1 $item_bot \
                    -fill $item_color -outline $border_color -width $zoom -tags [list $ALL $OPEN]]

                # bind click on item rect
                $pc bind $wid <Button-1>       [subst {::nui::combobox::select $model $i 0}]
                $pc bind $wid <Shift-Button-1> [subst {::nui::combobox::select $model $i 1}]

                # item text
                set ty [expr $gy0+($i+0.5)*$h]
                set wid [$pc create text $txt_left $ty -fill $text_color -text [lindex $items $i] -font $font \
                    -anchor w -width 200 -tags [list $ALL $OPEN]]

                # bind click on item text
                $pc bind $wid <Button-1>       [subst {::nui::combobox::select $model $i 0}]
                $pc bind $wid <Shift-Button-1> [subst {::nui::combobox::select $model $i 1}]

                # draw separator
                if { $i > 0 } {
                    $pc create line $gx0 $item_top $gx1 $item_top -fill $border_color -width $zoom -tags [list $ALL $OPEN]
                }
            }
        } else {
            # update text
            $c itemconfigure $TXT -fill $text_color -text [lindex $items $index] -font $font
        }
    }

    proc erase { cnv model id } {
        set c       [::nui::widget_canvas $cnv $model]
        set ALL     [tag_all $id]
        set OPEN    [tag_open $id]

        # clear widget
        $c delete $ALL

        # clear parent canvas: remove menu popup
        set pc [::nui::pd_canvas $cnv]
        $pc delete $OPEN
    }

    proc open { model state output } {
        set id [::nui::widget_tag $model]
        ::pdsend "$id open $state"
        if { $output } { ::pdsend "$id bang" }
    }

    proc select { model index state } {
        set id [::nui::widget_tag $model]
        ::nui::combobox::open $model 0 0
        ::pdsend "$id $index"
    }

    proc next { model } {
        set id [::nui::widget_tag $model]
        ::pdsend "$id next"
    }

    proc prev { model } {
        set id [::nui::widget_tag $model]
        ::pdsend "$id prev"
    }

    proc begin { model } {
        set id [::nui::widget_tag $model]
        ::pdsend "$id begin"
    }

    proc end { model } {
        set id [::nui::widget_tag $model]
        ::pdsend "$id end"
    }
}
}
