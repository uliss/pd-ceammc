namespace eval ::ui {
namespace eval cnv {

variable png_support True

proc create_image {name w h} {
    if { !$::ui::cnv::png_support } { return }
    if { [catch { image create photo ${name} -width $w -height $h } ] } {
        ::pdwindow::error "\[ui.canvas\] no PNG image support, can't create canvas\n"
        set ::ui::cnv::png_support False
    }
}

proc update {cnv id w h name data} {
    set c [::ceammc::ui::widget_canvas $cnv $id]
    set t [::ceammc::ui::widget_tag $id]
    $c delete $t

    if { $::ui::cnv::png_support } {
        ${name} put ${data}
        $c create image 0 0 -anchor nw -image $name -tags $t
    } else {
        $c create rectangle 1 1 [expr $w-1] [expr $h-1] -outline black -width 1 -fill grey -tags $t
    }
}

}
}
