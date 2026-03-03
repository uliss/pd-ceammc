namespace eval ::nui {
    namespace eval help_box {
        proc tag_all {id} {return "#hb${id}"}
        proc tag_open {id} {return "#hb${id}_open"}

        proc create {cnv model id x y _w h title title_color text_width} {
            set c [::nui::widget_canvas $cnv $model]
            set t [tag_all $id]

            set title_x [expr {$x + 4}]
            set title_y [expr {$h * 0.5}]

            $c create text $title_x $title_y -text "${title} ▶" -fill $title_color \
                -anchor w -width $text_width -tags $t
        }

        proc update {cnv model id x y w h title title_color text_width is_open lines} {
            set c [::nui::widget_canvas $cnv $model]
            set t [tag_all $id]

            set title_x [expr {$x + 4}]
            set title_y [expr {$h * 0.5}]
            $c coords $t $title_x $title_y

            if {$is_open} {
                set title "${title} ▼"
            } else {
                set title "${title} ▶"
            }

            $c itemconfigure $t -text $title -fill $title_color -width $text_width -tags $t

            set OPEN [tag_open $id]
            set ALL [tag_all $id]
            # parent canvas
            set pc [::nui::pd_canvas $cnv]
            # remove popup if exists
            $pc delete $OPEN

            if {$is_open} {
                set zoom 1
                set win [::nui::widget_window $cnv $model]
                set wcnv [::nui::widget_canvas $cnv $model]

                set nitems [llength $lines]
                lassign [$pc coords $win] gx menu_y
                set menu_x [expr {$gx + $w}]
                set txt_left [expr {$menu_x + 5}]
                set txt_top [expr {$menu_y + 5}]


                # draw lines
                for {set i 0} {$i < $nitems} {incr i} {
                    # line text
                    set line_id [$pc create text $txt_left $txt_top \
                        -text [lindex $lines $i] \
                        -width $text_width \
                        -anchor nw \
                        -tags [list $ALL $OPEN]]

                    set line_bbox [$pc bbox $line_id]
                    lassign $line_bbox _ _ _ y2
                    set txt_top [expr {$y2 + 5}]
                }

                # draw rect
                set box [$pc create rectangle $menu_x $menu_y [expr {$menu_x + $text_width + 100}] $txt_top \
                    -fill #FFAA00 -outline #00AAFF -tags [list $ALL $OPEN]]
                $pc lower $box
                $pc raise $OPEN
            }
        }

        proc erase {cnv model id} {
            ::nui::item_erase $cnv $model [tag_all $id]

            set OPEN [tag_open $id]
            # clear parent canvas: remove menu popup
            set pc [::nui::pd_canvas $cnv]
            $pc delete $OPEN
        }

        proc move {cnv model id x y} {
            ::nui::item_move $cnv $model [tag_all $id] $x $y
        }
    }
}
