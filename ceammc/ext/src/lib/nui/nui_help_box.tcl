namespace eval ::nui {
    namespace eval help_box {
        proc tag {id} {return "#hb${id}"}

        proc create {cnv model id x y _w h title text_width is_open lines} {
            set c [::nui::widget_canvas $cnv $model]
            set t [tag $id]

            set title_x [expr {$x + 4}]
            set title_y [expr {$h * 0.5}]

            $c create text $title_x $title_y -text $title -anchor w -width $text_width -tags $t
        }

        proc update {cnv model id x y w h title text_width is_open lines} {
            set c [::nui::widget_canvas $cnv $model]
            set t [tag $id]

            set title_x [expr {$x + 4}]
            set title_y [expr {$h * 0.5}]

            $c coords $t $title_x $title_y
            $c itemconfigure $t -text $title -width $text_width -tags $t
        }

        proc erase {cnv model id} {
            ::nui::item_erase $cnv $model [tag $id]
        }

        proc move {cnv model id x y} {
            ::nui::item_move $cnv $model [tag $id] $x $y
        }
    }
}
