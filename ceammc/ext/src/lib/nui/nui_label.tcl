namespace eval ::nui {
namespace eval label {
    proc tag { id } { return "#l${id}" }

    proc create { cnv model id x y width anchor font color txt } {
        set c [::nui::widget_canvas $cnv $model]
        set t [tag $id]

        $c create text $x $y -fill $color -text $txt -font $font -anchor $anchor -width $width -tags $t
    }

    proc update { cnv model id x y width anchor font color txt } {
        set c [::nui::widget_canvas $cnv $model]
        set t [tag $id]

        $c coords $t $x $y
        $c itemconfigure $t -fill $color -text $txt -font $font -anchor $anchor -width $width -tags $t
    }

    proc erase { cnv model id } {
        ::nui::item_erase $cnv $model [tag $id]
    }

    proc move  { cnv model id x y } {
        ::nui::item_move $cnv $model [tag $id] $x $y
    }
}
}
