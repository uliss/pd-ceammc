namespace eval ::nui {
namespace eval nentry {
    proc tag_all { id }  { return "#ne${id}" }
    proc tag_box { id }  { return "#ne${id}_box" }
    proc tag_tri { id }  { return "#ne${id}_tri" }
    proc tag_txt { id }  { return "#ne${id}_txt" }
    proc tag_edit { id } { return "#ne${id}_edit" }

    array set data {}

    proc create_data { id } {
        set ::nui::nentry::data($id) [dict create]
    }

    proc set_data { id key value } {
        dict set ::nui::nentry::data($id) $key $value
    }

    proc get_data { id key } {
        return [dict get $::nui::nentry::data($id) $key]
    }

    proc create { cnv model id x y w h zoom value font border_color fill_color value_color } {
        set c   [::nui::widget_canvas $cnv $model]
        set ALL [tag_all $id]
        set BOX [tag_box $id]
        set TRI [tag_tri $id]
        set TXT [tag_txt $id]

        # draw box
        $c create rectangle $x $y [expr $x+$w] [expr $y+$h] \
            -fill $fill_color -outline $border_color -width $zoom -tags [list $ALL $BOX]

        # draw triangle
        $c create polygon $x $y [expr $x+10] [expr $y+($h*0.5)] $x [expr $y+$h] \
            -fill $fill_color -outline $border_color -width $zoom -tags [list $ALL $TRI]

        # draw text
        $c create text [expr $x+12] [expr $y+($h*0.5)] -fill $value_color -text $value \
            -font $font -anchor w -tags [list $ALL $TXT]

        bind $c <KeyPress>    [subst {::nui::nentry::edit_type $cnv $model $id %K %N}]
        bind $c <Double-1>    [subst {::nui::nentry::edit_select $cnv $model $id %W}]

        create_data $id
        set_data $id is_edit 0
        set_data $id value $value
    }

    proc update { cnv model id x y w h zoom value font border_color fill_color value_color } {
        set c   [::nui::widget_canvas $cnv $model]
        set ALL [tag_all $id]
        set BOX [tag_box $id]
        set TRI [tag_tri $id]
        set TXT [tag_txt $id]

        $c select clear

        #colors
        $c itemconfigure $BOX -fill $fill_color -outline $border_color
        $c itemconfigure $TRI -fill $fill_color -outline $border_color

        # text update
        $c itemconfigure $TXT -fill $value_color -text $value -font $font
    }

    proc erase { cnv model id } {
        set ALL [tag_all $id]
        #set c [::nui::widget_canvas $cnv $model]
        #$c delete [tag_all $id]
        ::nui::item_erase $cnv $model $ALL
        array unset ::nui::nentry::data($id)
    }

    proc edit_select { cnv model id widget } {
        set c    [::nui::widget_canvas $cnv $model]
        set EDIT [tag_edit $id]
        set ALL  [tag_all $id]
        set TXT  [tag_txt $id]

        $c select from $TXT 0
        $c select to $TXT end
        $c icursor $TXT end
        ::focus $c
        $c focus $TXT
    }

    proc take_focus { cnv model id } {
        pdtk_text_editing [::nui::pd_window $cnv] {} 0
        ::nui::widget_focus $cnv $model
        [::nui::pd_canvas $cnv] focus [::nui::widget_window $cnv $model]
        set c [::nui::widget_canvas $cnv $model]
        $c focus [tag_txt $id]
        ::pdwindow::debug "focus $c [tag_txt $id]\n-> [$c focus]\n"
    }

    proc edit_type { cnv model id keyname keycode } {
#        take_focus $cnv $model $id
        ::pdwindow::debug "key: $keyname, code: $keycode\n"

        set c [::nui::widget_canvas $cnv $model]
        set TXT  [tag_txt $id]

        $c icursor $TXT end

        switch -regexp -- $keyname {
            {[0-9]}     { $c insert $TXT end ${keyname} }
            {period}    { $c insert $TXT end . }
            {plus}      { $c insert $TXT end + }
            {minus}     { $c insert $TXT end - }
            {Return}    { edit_done   $id $c $TXT }
            {Escape}    { edit_cancel $id $c $TXT }
            {BackSpace} { $c dchars $TXT sel.first sel.last }
            {Delete}    { $c dchars $TXT end }
            {Left}      {
                puts [$c itemcget $TXT icursor]
#                $c icursor $TXT 1 0
            }
            {Right}     { $c icursor $TXT 1 0}
            "default" {
                ::pdwindow::debug "$keyname\n"
            }
        }

#        # first key press
        if {[get_data $id is_edit] == 0} {
            set_data $id is_edit 1
            $c icursor $TXT end
        }
#            $c insert $TXT end ${keyname}
#        } else {
#            $c insert $TXT end ${keyname}
#        }

#        set EDIT [tag_edit $id]
#        $c coords $EDIT [$c bbox $txt]
    }

    proc edit_done { id c txt } {
#        set EDIT [tag_edit $id]
#        $c delete $EDIT
        set value [$c itemcget $txt -text]
        $c itemconfigure $txt -text [expr $value*1]
        set_data $id is_edit 1
    }

    proc edit_accept { id c txt } {
        ::pdwindow::debug "OK\n"
        set_data $id value [$c itemcget $txt -text]
        edit_done $id $c $txt
    }

    proc edit_cancel { id c txt } {
        $c itemconfigure $txt -text [get_data $id value]
        edit_done $id $c $txt
    }
}
}
