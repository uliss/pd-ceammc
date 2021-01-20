namespace eval ui {

proc keyboard_is_black_key {k} {
    set k [expr $k % 12]
    return [expr (($k - ($k > 4 ? 1 : 0)) % 2) == 1]
}

proc keyboard_delete_keys {id cnv} {
    $cnv delete ${id}_#all
}

proc keyboard_create_white_keys {id cnv numkeys kcolor acolor bdcolor w h bits} {
    set wh [expr $h+1]
    set wi 0

    for {set i 0} {$i < $numkeys} {incr i} {
        if {[keyboard_is_black_key $i]} { continue }
        set bit [string index $bits $i]
        if {$bit == 1} { set fc $acolor } { set fc $kcolor }

        $cnv create rectangle [expr $wi*$w] 0 [expr ($wi+1)*$w] $wh \
            -fill $fc \
            -outline $bdcolor \
            -tags ${id}_#all

        incr wi
    }
}

proc keyboard_create_black_keys {id cnv numkeys acolor bdcolor w h bits} {
    set bh [expr $h*0.6]
    set kw [expr $w*0.5]

    for {set i 0} {$i < $numkeys} {incr i} {
        if { [keyboard_is_black_key $i] } {
            set kn [expr $i % 12]
            set ko [expr $i / 12]
            set x [expr $i*$kw + (($kn > 4 ? 1 : 0) + 2 * $ko) * $kw + 0.5 * $kw - 1]

            set bit [string index $bits $i]
            if {$bit == 1} { set fc $acolor } { set fc black }

            $cnv create rectangle $x 0 [expr $x + $kw] $bh \
                -fill $fc \
                -outline $bdcolor \
                -tags ${id}_#all
        }
    }
}

proc keyboard_create_hkeys {id cnv numkeys kcolor acolor bdcolor w h bits} {
    # one pixel fix
    set kh [expr $h]

    keyboard_create_white_keys $id $cnv $numkeys $kcolor $acolor $bdcolor $w $kh $bits
    keyboard_create_black_keys $id $cnv $numkeys $acolor $bdcolor $w $kh $bits
}

}
