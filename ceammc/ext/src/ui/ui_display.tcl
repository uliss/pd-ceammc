namespace eval ::ui {

namespace eval display {
    variable font_size 11
    variable font_family "Helvetica"
}

switch -- $::windowingsystem {
    "aqua" { }
    "x11"  { }
    "win32" {
        set display::font_size 7
    }
}

proc display_type_font { zoom } { return "$display::font_family [expr $zoom * $display::font_size] normal roman" }

proc display_single_line { type } {
    switch $type {
        bang    { return 1 }
        float   { return 1 }
        symbol  { return 1 }
        default { return 0 }
    }
}

proc display_txt_width { c tag } {
   lassign [$c bbox $tag] tx0 ty0 tx1 ty1
   return [expr $tx1 - $tx0]
}

proc display_txt_height { c tag } {
   lassign [$c bbox $tag] tx0 ty0 tx1 ty1
   return [expr abs($ty1 - $ty0)]
}

proc display_copy {txt} {
    clipboard clear
    clipboard append $txt
}

proc display_update {cnv id rid w h zoom hauto bdcolor bgcolor txtcolor typecolor showtype type txt} {
    set c [::ceammc::ui::widget_canvas $cnv $id]
    set t [::ceammc::ui::widget_tag $id]
    $c delete $t

    # set bg
    ::ceammc::ui::widget_bg $cnv $id $bgcolor

    set txpad [expr $zoom*2+1]
    set typad [expr $zoom*2+1]
    set tx $txpad
    set ty $typad
    set ft [display_type_font $zoom]

    set x 0
    set typeh 0
    if { $showtype == 1 } {
        set ttag "${t}_t"
        # draw type text
        $c create text $tx $ty -text $type -anchor nw -justify left \
            -font $ft -fill $txtcolor -width 0 -tags [list $t $ttag]

        set typew [display_txt_width $c $ttag]
        set typeh [display_txt_height $c $ttag]
        set x [expr (2*$txpad) + $typew]
        # draw type filled rectangle
        set ttagr "${t}_tr"
        $c create rectangle 0 0 $x $h -fill $typecolor -outline $typecolor -width $zoom -tags [list $t $ttagr]
        $c raise $ttag $ttagr
        # draw type text separator line
        $c create line $x 0 $x $h -fill $bdcolor -width $zoom -tag $t
    }

    # draw main text
    set tx [expr $x + $txpad]
    set ty $typad
    if {$hauto == 1} {
        set tw 0
        if { ![display_single_line $type] } {
            set minw [expr 150*$zoom]
            set tw $minw
        }
    } {
        set tw [expr $w - ($tx + $txpad)]
    }
    set tt "${t}_m"
    $c create text $tx $ty -text $txt -anchor nw -justify left \
        -font $ft -fill $txtcolor -width $tw -tags [list $t $tt]

    if {$hauto == 1} {
        switch $type {
            bang {
                # resize to min size
                pdsend "$rid .resize [expr $tx+15] 10"
            }
            float {
                set tw [display_txt_width $c $tt]
                set dispw [expr $tx + $tw + (2*$txpad)]
                if { $dispw != $w } {
                    # resize to min height
                    pdsend "$rid .resize $dispw 10"
                }
            }
            symbol {
                set tw [display_txt_width $c $tt]
                set dispw [expr $tx + $tw + (2*$txpad)]
                if { $dispw != $w } {
                    # resize to min height
                    pdsend "$rid .resize $dispw 10"
                }
            }
            default {
                set nw $w
                set tw [display_txt_width $c $tt]
                set dispw [expr $tx + $tw + (2*$txpad)]
                if { $dispw != $w } { set nw $dispw }

                set nh 5
                set th [display_txt_height $c $tt]
                set disph [expr $th + $typad]
                if { $typeh > $nh } { set nh $typeh }
                if { $disph > $nh } { set nh $disph }
                if { $nh != $h || $nw != $w } {
                    pdsend "$rid .resize $nw $nh"
                }
            }
        }
    }
}
}
