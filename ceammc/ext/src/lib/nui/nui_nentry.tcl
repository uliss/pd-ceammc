namespace eval ::nui {
namespace eval nentry {
    proc tag_all { id } { return "#ne${id}" }
    proc tag_box { id } { return "#ne${id}_box" }
    proc tag_tri { id } { return "#ne${id}_tri" }
    proc tag_txt { id } { return "#ne${id}_txt" }

    proc create { cnv model id x y w h zoom value font border_color fill_color value_color } {
        set c [::nui::widget_canvas $cnv $model]
        set ALL [tag_all $id]
        set BOX [tag_box $id]
        set TRI [tag_tri $id]
        set TXT [tag_txt $id]

        # draw box
        $c create rectangle $x $y [expr $x+$w] [expr $y+$h] \
            -fill $fill_color -outline $border_color -width $zoom -tags [list $ALL $BOX]

        # draw triangle
        $c create polygone $x $y [expr $x+10] [expr $y+($h*0.5)] $x [expr $y+$h] \
            -fill $fill_color -outline $border_color -width $zoom -tags [list $ALL $TRI]

        # draw text
        $c create text [expr $x+12] $y -fill $value_color -text $value \
            -font $font -anchor w -width $w -tags [list $ALL $TXT]
    }

    proc update { cnv model id x y w h zoom value border_color fill_color value_color } {
        set c [::nui::widget_canvas $cnv $model]
        set ALL [tag_all $id]

        $c coords $t $x $y
        $c itemconfigure $t -fill $color -text $txt -font $font -anchor $anchor -width $width -tags [list $ALL]
    }

    proc erase { cnv model id } {
        set ALL [tag_all $id]
        #set c [::nui::widget_canvas $cnv $model]
        #$c delete [tag_all $id]
        ::nui::item_erase $cnv $model $ALL
    }
}
}
