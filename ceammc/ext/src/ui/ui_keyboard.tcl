namespace eval ui {

proc keyboard_is_black_key {k} {
    set k [expr $k % 12]
    return [expr (($k - ($k > 4 ? 1 : 0)) % 2) == 1]
}

proc keyboard_delete_keys {id cnv} {
    $cnv delete ${id}_#all
}

proc keyboard_create_white_keys {id cnv numkeys wkcolor acolor bdcolor w h bits} {
    set wh [expr $h+1]
    set wi 0

    for {set i 0} {$i < $numkeys} {incr i} {
        if {[keyboard_is_black_key $i]} { continue }
        set bit [string index $bits $i]
        if {$bit == 1} { set fc $acolor } { set fc $wkcolor }

        $cnv create rectangle [expr $wi*$w] 0 [expr ($wi+1)*$w] $wh -fill $fc -outline $bdcolor \
            -tags ${id}_#all

        incr wi
    }
}

proc keyboard_create_black_keys {id cnv numkeys bkcolor acolor bdcolor w h bits} {
    set bh [expr $h*0.6]
    set kstep [expr 0.5*$w]
    set koff0 [expr 0.2*$w]
    set koff1 [expr 0.6*$w]

    for {set i 0} {$i < $numkeys} {incr i} {
        if { [keyboard_is_black_key $i] } {
            set kn [expr $i % 12]
            set ko [expr $i / 12]
            set x [expr $i*$kstep + (($kn > 4 ? 1 : 0) + 2 * $ko) * $kstep + $koff0]

            set bit [string index $bits $i]
            if {$bit == 1} { set fc $acolor } { set fc $bkcolor }

            $cnv create rectangle $x 0 [expr $x + $koff1] $bh -fill $fc -outline $bdcolor \
                -tags ${id}_#all
        }
    }
}

proc keyboard_create_hkeys {id cnv numkeys wkcolor bkcolor acolor bdcolor w h bits} {
    # one pixel fix
    set kh [expr $h]

    keyboard_create_white_keys $id $cnv $numkeys $wkcolor $acolor $bdcolor $w $kh $bits
    keyboard_create_black_keys $id $cnv $numkeys $bkcolor $acolor $bdcolor $w $kh $bits
}

}
