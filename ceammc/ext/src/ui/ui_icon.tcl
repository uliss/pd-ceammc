namespace eval ::ui {
namespace eval icon {

variable png_support True

proc create_image {name data18 data24 data36 data48} {
    if { !$::ui::icon::png_support } { return }
    if { [catch { image create photo ${name}_18 -width 18 -height 18 -data $data18
                  image create photo ${name}_24 -width 24 -height 24 -data $data24
                  image create photo ${name}_36 -width 36 -height 36 -data $data36
                  image create photo ${name}_48 -width 48 -height 48 -data $data48}] } {
        ::pdwindow::error "\[ui.icon\] no PNG image support, can't create icons\n"
        set ::ui::icon::png_support False
    }
}

proc update {cnv id w h name color} {
    set c [::ceammc::ui::widget_canvas $cnv $id]
    set t [::ceammc::ui::widget_tag $id]
    $c delete $t

    if { $::ui::icon::png_support } {
        $c create rectangle 0 0 $w $h -outline {} -width 0 -fill $color -tags $t
        $c create image 0 0 -anchor nw -image $name -tags $t
    } else {
        $c create rectangle 1 1 [expr $w-1] [expr $h-1] -outline black -width 1 -fill grey -tags $t
    }
}

}
}
