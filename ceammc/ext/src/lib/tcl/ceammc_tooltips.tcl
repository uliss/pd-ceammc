namespace eval ::ceammc_tt {
    set txt {}
    set active 0
}
proc ::ceammc_tt::txt {c tag xlet text} {
    $c bind $tag <Enter>  [list ::ceammc_tt::enter $c $tag $xlet $text]
    $c bind $tag <Leave>  [list ::ceammc_tt::leave $c]
 }
proc ::ceammc_tt::show {c tag xlet} {
    if {$::ceammc_tt::active == 0} return
    $c delete ceammc_tt
    foreach {x - - y} [$c bbox $tag] break
    if [info exists y] {
        variable id
        if {$xlet == 0} {
           incr y 7
           set id [$c create text $x $y -text $::ceammc_tt::txt -font TkTooltipFont -anchor nw -tag ceammc_tt]
        } else {
           incr y -10
           set id [$c create text $x $y -text $::ceammc_tt::txt -font TkTooltipFont -anchor sw -tag ceammc_tt]
        }
        foreach {x0 y0 x1 y1} [$c bbox $id] break
        $c create rect [expr $x0-2] [expr $y0-1] [expr $x1+2] [expr $y1+1] -fill lightblue -tag ceammc_tt
        $c raise $id
        $c bind $id <Leave> [list ::ceammc_tt::leave $c]
    }
}
proc ::ceammc_tt::enter {c tag xlet text} {
    set ::ceammc_tt::active 1
    set ::ceammc_tt::txt $text
    after 500 ::ceammc_tt::show $c $tag $xlet
}
proc ::ceammc_tt::delete {c} {
    if {$::ceammc_tt::active == 1} return
    $c delete ceammc_tt
}
proc ::ceammc_tt::leave {c} {
    set ::ceammc_tt::active 0
    after 50 ::ceammc_tt::delete $c
}
