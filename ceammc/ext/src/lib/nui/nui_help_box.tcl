namespace eval ::nui {
namespace eval help_box {
    proc tag { id } { return "#hb${id}" }

    proc create { cnv model id x y title text_width is_open lines } {
        set c [::nui::widget_canvas $cnv $model]
        set t [tag $id]

        $c create text $x $y -text $title -anchor nw -width $text_width -tags $t
    }

    proc update { cnv model id x y title text_width is_open lines } {
        set c [::nui::widget_canvas $cnv $model]
        set t [tag $id]

        $c coords $t $x $y
        $c itemconfigure $t -text $title -anchor nw -width $text_width -tags $t
    }

    proc erase { cnv model id } {
        ::nui::item_erase $cnv $model [tag $id]
    }

    proc move  { cnv model id x y } {
        ::nui::item_move $cnv $model [tag $id] $x $y
    }
}
}
