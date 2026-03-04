namespace eval ::nui {
    namespace eval helpbox {
        variable title_margin_top 0

        switch $::windowingsystem {
            "x11" {
                variable title_margin_top 2
            }
            default {}
        }


        proc tag_all {id} {return "#hb${id}"}
        proc tag_open {id} {return "#hb${id}_open"}

        proc make_title_indicator {title indicator indicator_right} {
            if {$indicator_right} {
                return "${title} ${indicator}"
            } else {
                return "${indicator} ${title}"
            }
        }

        proc make_title {title direction_right indicator_right is_open} {
            if {$is_open} {
                return [make_title_indicator $title {▼} $indicator_right]
            } elseif {$direction_right} {
                return [make_title_indicator $title {▶} $indicator_right]
            } else {
                return [make_title_indicator $title {◀} $indicator_right]
            }
        }

        proc create_ui {params} {
            variable title_margin_top
            array set args $params

            set _c [::nui::widget_canvas $args(-cnv) $args(-model)]
            set _t [tag_all $args(-id)]

            set _ztitle_x [expr {$args(-x) + (4 * $args(-zoom))}]
            set _ztitle_y [expr {(($args(-height) * 0.5) + $title_margin_top) * $args(-zoom)}]
            set _ztext_width [expr {$args(-text_width) * $args(-zoom)}]

            set _title [make_title $args(-title) $args(-direction_right) $args(-indicator_right) 0]

            $_c create text $_ztitle_x $_ztitle_y \
                -text $_title \
                -fill $args(-title_color) \
                -font $args(-title_font) \
                -anchor w \
                -width $_ztext_width \
                -tags $_t
        }

        proc update_ui {params} {
            variable title_margin_top
            array set args $params

            set _c [::nui::widget_canvas $args(-cnv) $args(-model)]
            set _t [tag_all $args(-id)]

            set _ztitle_x [expr {$args(-x) + (4 * $args(-zoom))}]
            set _ztitle_y [expr {(($args(-height) * 0.5) + $title_margin_top) * $args(-zoom)}]
            set _ztext_width [expr {$args(-text_width) * $args(-zoom)}]
            $_c coords $_t $_ztitle_x $_ztitle_y

            # set title
            set _title [make_title $args(-title) $args(-direction_right) $args(-indicator_right) $args(-is_open)]
            $_c itemconfigure $_t \
                -text $_title \
                -fill $args(-title_color) \
                -font $args(-title_font) \
                -width $_ztext_width \
                -tags $_t

            set OPEN [tag_open $args(-id)]
            set ALL [tag_all $args(-id)]
            # parent canvas
            set _pc [::nui::pd_canvas $args(-cnv)]
            # remove popup if exists
            $_pc delete $OPEN

            if {$args(-is_open)} {
                set _win [::nui::widget_window $args(-cnv) $args(-model)]

                set _nitems [llength $args(-text_lines)]
                lassign [$_pc coords $_win] _gx _menu_top

                if {$args(-direction_right)} {
                    set _menu_left [expr {$_gx + $args(-width) * $args(-zoom)}]
                } else {
                    set _menu_left [expr {$_gx - $_ztext_width}]
                }

                set _menu_right [expr {$_menu_left + $_ztext_width}]
                set _txt_left [expr {$_menu_left + 5}]
                set _txt_ypos [expr {$_menu_top + 5}]

                # draw lines
                for {set i 0} {$i < $_nitems} {incr i} {
                    # line text
                    set _txt [binary decode base64 [lindex $args(-text_lines) $i]]
                    set _txt_utf8 [encoding convertfrom utf-8 $_txt]
                    set _line_id [$_pc create text $_txt_left $_txt_ypos \
                        -text $_txt_utf8 \
                        -width [expr {$_ztext_width - 10}] \
                        -fill $args(-popup_text_color) \
                        -font $args(-popup_font) \
                        -anchor nw \
                        -tags [list $ALL $OPEN]]

                    set _line_bbox [$_pc bbox $_line_id]
                    lassign $_line_bbox _ _ _ _y2
                    set _txt_ypos [expr {$_y2 + 5}]
                }

                # draw rect
                set _box [$_pc create rectangle $_menu_left $_menu_top $_menu_right $_txt_ypos \
                    -fill $args(-popup_fill_color) \
                    -outline $args(-popup_border_color) \
                    -width $args(-popup_border_width) \
                    -tags [list $ALL $OPEN]]
                $_pc lower $_box
                $_pc raise $OPEN
            }
        }

        proc erase_ui {cnv model id} {
            ::nui::item_erase $cnv $model [tag_all $id]

            set OPEN [tag_open $id]
            # clear parent canvas: remove menu popup
            set pc [::nui::pd_canvas $cnv]
            $pc delete $OPEN
        }

        proc move_ui {cnv model id x y} {
            ::nui::item_move $cnv $model [tag_all $id] $x $y
        }
    }
}
