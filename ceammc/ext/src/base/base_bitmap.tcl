# test file
namespace eval ceammc {
namespace eval img {

proc free {img_id} {
    catch {image delete "img${img_id}"}
}

proc create {img_id data} {
    catch {image delete "img${img_id}"}
    image create photo "img${img_id}" -data $data
}

proc show {img_id} {
    set w .i${img_id}
    catch {destroy $w}
    toplevel $w
    wm title $w [_ {[bitmap] image view}]
    wm minsize $w 100 100
#    wm iconname $w "cscroll"
#    positionWindow $w
    set c $w.c

    set img img${img_id}
    set imgw [image width  $img]
    set imgh [image height $img]

    # set minimal dimensions
    set max_width  [expr round([winfo screenwidth $w] * 0.75)]
    set max_height [expr round([winfo screenheight $w] * 0.75)]
    set win_width  [expr min($max_width,  max(200, $imgw)) + 30]
    set win_height [expr min($max_height, max(200, $imgh)) + 30]
    wm geometry $w ${win_width}x${win_height}

    set imgw2 [expr [image width  $img] * 0.5]
    set imgh2 [expr [image height $img] * 0.5]

    frame $w.grid
    ttk::scrollbar $w.hscroll -orient horizontal -command "$c xview"
    ttk::scrollbar $w.vscroll -orient vertical   -command "$c yview"
    canvas $c -relief solid -borderwidth 0 -scrollregion "0 0 $imgw $imgh" \
            -xscrollcommand "$w.hscroll set" \
            -yscrollcommand "$w.vscroll set" \
            -width $imgw \
            -height $imgh

#    pack $w.hscroll -side bottom -fill x -padx 1 -pady 1
#    pack $w.vscroll -side right  -fill y -padx 1 -pady 1
#    pack $c -side left -expand 0 -fill none -padx 1 -pady 1
    pack $w.grid -expand yes -fill both -padx 1 -pady 1
    grid rowconfig    $w.grid 0 -weight 1 -minsize 0
    grid columnconfig $w.grid 0 -weight 1 -minsize 0

    grid $c -padx 1 -in $w.grid -pady 1 \
        -row 0 -column 0 -sticky {}
    grid $w.vscroll -in $w.grid -padx 1 -pady 1 \
        -row 0 -column 1 -sticky news
    grid $w.hscroll -in $w.grid -padx 1 -pady 1 \
        -row 1 -column 0 -sticky news

    # create image
    $c create image 0 0 \
        -anchor nw \
        -image $img

    # mousewheel
    if {([tk windowingsystem] eq "aqua") && ![package vsatisfies [package provide Tk] 8.7-]} {
        bind $c <MouseWheel> {
            %W yview scroll [expr {-%D}] units
        }
        bind $c <Option-MouseWheel> {
            %W yview scroll [expr {-0.5*%D}] units
        }
        bind $c <Shift-MouseWheel> {
            %W xview scroll [expr {-%D}] units
        }
        bind $c <Shift-Option-MouseWheel> {
            %W xview scroll [expr {-0.5*%D}] units
        }
    } else {
        # We must make sure that positive and negative movements are rounded
        # equally to integers, avoiding the problem that
        #     (int)1/-30 = -1,
        # but
        #     (int)-1/-30 = 0
        # The following code ensures equal +/- behaviour.
        bind $c <MouseWheel> {
            if {%D >= 0} {
                %W yview scroll [expr {%D/-30}] units
            } else {
                %W yview scroll [expr {(%D-29)/-30}] units
            }
        }
        bind $c <Option-MouseWheel> {
            if {%D >= 0} {
                %W yview scroll [expr {%D/-12}] units
            } else {
                %W yview scroll [expr {(%D-11)/-12}] units
            }
        }
        bind $c <Shift-MouseWheel> {
            if {%D >= 0} {
                %W xview scroll [expr {%D/-30}] units
            } else {
                %W xview scroll [expr {(%D-29)/-30}] units
            }
        }
        bind $c <Shift-Option-MouseWheel> {
            if {%D >= 0} {
                %W xview scroll [expr {%D/-12}] units
            } else {
                %W xview scroll [expr {(%D-11)/-12}] units
            }
        }
    }

    if {[tk windowingsystem] eq "x11" && ![package vsatisfies [package provide Tk] 8.7-]} {
        # Support for mousewheels on Linux/Unix commonly comes through mapping
        # the wheel to the extended buttons.  If you have a mousewheel, find
        # Linux configuration info at:
        #	https://linuxreviews.org/HOWTO_change_the_mouse_speed_in_X
        bind $c <Button-4> {
            if {!$tk_strictMotif} {
                %W yview scroll -5 units
            }
        }
        bind $c <Shift-Button-4> {
            if {!$tk_strictMotif} {
                %W xview scroll -5 units
            }
        }
        bind $c <Button-5> {
            if {!$tk_strictMotif} {
                %W yview scroll 5 units
            }
        }
        bind $c <Shift-Button-5> {
            if {!$tk_strictMotif} {
                %W xview scroll 5 units
            }
        }
    }
}

}
# namespace img
}
# namespace ceam
