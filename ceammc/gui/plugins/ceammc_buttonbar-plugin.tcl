# this plugin creates a buttonbar on a patch window when that patch
# window is in Edit Mode

# this GUI plugin removes the menubars from any patch window that is
# not in Edit Mode.  Also, if a patch is switched to Run Mode, the
# menubar will be removed.

# try enabling base64 if possible
catch {package require base64}
# preferences
package require pd_guiprefs

namespace eval ::btnbar:: {
    variable show;   # if show button bar
    variable hide_in_runmode;
    variable states;  # current button bar show states
}

set ::btnbar::show true
set ::btnbar::hide_in_runmode true
set ::btnbar::states [dict create]

namespace eval ::btnbar::utils { }

# tooltips
if { [catch {package require tooltip} ] } {
    proc ::btnbar::tooltip {id msg} {}
} {
    proc ::btnbar::tooltip {id msg} { tooltip::tooltip $id $msg }
}

# helper utils
proc ::btnbar::utils::bool {value {fallback 0}} {
    catch {set fallback [expr bool($value) ] } stdout
    return $fallback
}

proc ::btnbar::addTopLevel { id } {
    if {! [dict exists $::btnbar::states $id] } {
        dict append ::btnbar::states $id $::btnbar::show
        ::btnbar::showhide $id
    }
}

proc ::btnbar::removeTopLevel { id } {
    if { [dict exists $::btnbar::states $id] } { dict unset ::btnbar::states $id }
}

proc ::btnbar::getState { id } {
    if { [dict exists $::btnbar::states $id] } {
        return [dict get $::btnbar::states $id]
    } {
        return 0
    }
}

proc ::btnbar::setState { id val } {
    if { [dict exists $::btnbar::states $id] } { dict set ::btnbar::states $id $val }
}

if { [ catch { set ::btnbar::hide_in_runmode [::pd_guiprefs::read btnbar] } stdout ] } {
    set ::btnbar::hide_in_runmode true
    set ::btnbar::show true
} {
    # read config values
    set ::btnbar::show [::btnbar::utils::bool [::pd_guiprefs::read btnbar_show] 1]
    set ::btnbar::hide_in_runmode [::btnbar::utils::bool [::pd_guiprefs::read btnbar_hide_in_runmode] 1]
}

proc ::btnbar::init_options_menu {} {
    if {$::windowingsystem eq "aqua"} {
        set mymenu .menubar.apple.preferences
    } else {
        set mymenu .menubar.file.preferences
    }

    if { [catch {
        $mymenu entryconfigure [_ "Button Bar"] -command {::btnbar::show_options_gui}
    } _ ] } {
        $mymenu add separator
        $mymenu add command -label [_ "Button Bar"] -command {::btnbar::show_options_gui}
    }
}

proc ::btnbar::write_config {} {
    ::pd_guiprefs::write btnbar_show $::btnbar::show
    ::pd_guiprefs::write btnbar_hide_in_runmode $::btnbar::hide_in_runmode
}

proc ::btnbar::show_options_gui {} {
    if {[winfo exists .options]} {
        focus .options
        return
    }

    toplevel .options -background $::pd_colors::window_background
    wm title .options [_ "Button Bar Settings"]

    ttk::frame .options.f

    ttk::checkbutton .options.f.show -variable ::btnbar::show -onvalue 1 -offvalue 0 -command {
        if {$::btnbar::show} {
            .options.f.hide_in_runmode configure -state normal
        } {
            .options.f.hide_in_runmode configure -state disable
        }
    }
    ttk::label .options.f.show_label -text [_ "Show" ]

    ttk::checkbutton .options.f.hide_in_runmode -variable ::btnbar::hide_in_runmode -onvalue 1 -offvalue 0
    ttk::label .options.f.hide_in_runmode_label -text [_ "Hide in runmode" ]

    # buttons
    ttk::button .options.f.save_btn -text [_ "Save settings" ] -command ::btnbar::write_config

    set padding 2

    # ::::GRID::::

    # setup main frame stuff
    grid .options.f -column 0 -row 0
    set current_row 0

    # show
    grid .options.f.show_label -column 0 -row $current_row -padx $padding -pady $padding -sticky "w"
    grid .options.f.show -column 1 -row $current_row -padx $padding -pady $padding -sticky "w"
    incr current_row

    # hide in runmode
    grid .options.f.hide_in_runmode_label -column 0 -row $current_row -padx $padding -pady $padding -sticky "w"
    grid .options.f.hide_in_runmode -column 1 -row $current_row -padx $padding -pady $padding -sticky "w"
    incr current_row

    # buttons
    grid .options.f.save_btn -column 1 -row $current_row -padx $padding -pady 8 -sticky "w"
}

proc make_pd_button {mytoplevel name tooltip} {
    button $mytoplevel.buttonbar.$name -image buttonimage$name \
        -relief flat -borderwidth 0 -highlightthickness 0 \
        -highlightcolor grey -highlightbackground grey -padx 1 -pady 1 \
        -command "menu_send_float \$::focused_window $name 0"
    pack $mytoplevel.buttonbar.$name -side left -padx 0 -pady 0
    ::btnbar::tooltip $mytoplevel.buttonbar.$name $tooltip
}

proc make_iemgui_button {mytoplevel name tooltip} {
    button $mytoplevel.buttonbar.$name -image buttonimage$name \
        -relief flat -borderwidth 0 -highlightthickness 0 \
        -highlightcolor grey -highlightbackground grey -padx 1 -pady 1 \
        -command "menu_send \$::focused_window $name"
    pack $mytoplevel.buttonbar.$name -side left -padx 0 -pady 0
    ::btnbar::tooltip $mytoplevel.buttonbar.$name $tooltip
}

proc make_ceammc_button {mytoplevel name tooltip} {
    button $mytoplevel.buttonbar.$name -image buttonimage$name \
        -relief flat -borderwidth 0 -highlightthickness 0 \
        -highlightcolor grey -highlightbackground grey -padx 1 -pady 1 \
        -command "menu_send \$::focused_window ui.$name"
    pack $mytoplevel.buttonbar.$name -side left -padx 0 -pady 0
    ::btnbar::tooltip $mytoplevel.buttonbar.$name $tooltip
}

proc make_ceammc_button_tilde {mytoplevel name tooltip} {
    button $mytoplevel.buttonbar.$name -image buttonimage$name \
        -relief flat -borderwidth 0 -highlightthickness 0 \
        -highlightcolor grey -highlightbackground grey -padx 1 -pady 1 \
        -command "menu_send \$::focused_window ui.$name~"
    pack $mytoplevel.buttonbar.$name -side left -padx 0 -pady 0
    ::btnbar::tooltip $mytoplevel.buttonbar.$name $tooltip
}

proc ::btnbar::init {mytoplevel} {
    ttk::frame $mytoplevel.buttonbar -cursor arrow
    make_pd_button $mytoplevel obj [_ "Object"]
    make_pd_button $mytoplevel msg [_ "Message"]
    make_pd_button $mytoplevel floatatom [_ "Number box"]
    make_pd_button $mytoplevel symbolatom [_ "Symbol box"]
    make_pd_button $mytoplevel text [_ "Comment"]
    make_iemgui_button $mytoplevel bng [_ "Button"]
    make_iemgui_button $mytoplevel toggle [_ "Toggle"]
    make_iemgui_button $mytoplevel numbox [_ "Number box2"]
    make_iemgui_button $mytoplevel hslider [_ "Horizontal slider"]
    make_iemgui_button $mytoplevel vslider [_ "Vertial slider"]
    make_iemgui_button $mytoplevel hradio  [_ "Horizontal radio"]
    make_iemgui_button $mytoplevel vradio  [_ "Vertical radio"]
    make_iemgui_button $mytoplevel mycnv   [_ "Canvas"]
    make_iemgui_button $mytoplevel menuarray [_ "Array"]
    make_ceammc_button $mytoplevel knob [_ "Knob"]
    make_ceammc_button $mytoplevel sliders [_ "Sliders"]
    make_ceammc_button $mytoplevel slider2d [_ "Slider 2D"]
    make_ceammc_button $mytoplevel keyboard [_ "Keyboard"]
    make_ceammc_button $mytoplevel env "Breakpoint function"
    make_ceammc_button $mytoplevel display [_ "Display"]
    make_ceammc_button_tilde $mytoplevel scope [_ "Scope"]
    make_ceammc_button_tilde $mytoplevel spectroscope [_ "Spectroscope"]
    make_ceammc_button $mytoplevel preset [_ "Presets"]
}

proc ::btnbar::show {mytoplevel} {
    set state [::btnbar::getState $mytoplevel]

    if {$state} {
        set tkcanvas [tkcanvas_name $mytoplevel]
        pack forget $tkcanvas
        pack $mytoplevel.buttonbar -side top -fill x
        pack $tkcanvas -side top -expand 1 -fill both
        ::btnbar::setState $mytoplevel false
    }
}

proc ::btnbar::hide {mytoplevel} {
    set state [::btnbar::getState $mytoplevel]

    if {!$state} {
        pack forget $mytoplevel.buttonbar
        ::btnbar::setState $mytoplevel true
    }
}

proc ::btnbar::enable {mytoplevel} {
    ::btnbar::show $mytoplevel

    foreach child [winfo children $mytoplevel.buttonbar] {
        catch { $child configure -state normal }
    }
}

proc ::btnbar::disable {mytoplevel} {
    foreach child [winfo children $mytoplevel.buttonbar] {
        catch { $child configure -state disabled }
    }
}

proc ::btnbar::showhide {mytoplevel} {
    # populate button bar with entries
    if { ! [winfo exists $mytoplevel.buttonbar]} {
        ::btnbar::init $mytoplevel
    }

    if {!$::btnbar::show} {
        catch { ::btnbar::hide $mytoplevel }
        return;
    }

    if {$::editmode($mytoplevel)} {
        if {$::btnbar::hide_in_runmode} {
            ::btnbar::show $mytoplevel
        } {
            ::btnbar::enable $mytoplevel
        }
    } {
        if {$::btnbar::hide_in_runmode} {
            ::btnbar::hide $mytoplevel
        } {
            ::btnbar::disable $mytoplevel
        }
    }
}

bind PatchWindow <Expose>  {+::btnbar::addTopLevel %W}
bind PatchWindow <Destroy> {+::btnbar::removeTopLevel %W}
bind PatchWindow <<EditMode>> {+::btnbar::showhide %W}

::btnbar::init_options_menu
set bngdata {R0lGODdhIAAgAIQUAAAAAAEBAQEDAgUKCwsTExwnKB0oKQdhcQdlc3NvcQaqyAasygax0AjJ7KmpqQnU+cC+vsDAwN7e3vz8/P///////////////////////////////////////////////ywAAAAAIAAgAAAFpyAljmRpnmiqrmzrvnAsz3Rtr0Cu73wAA5OgJFEYDAoJSXAC+AUdBMSi0VggBJFg8wWMABSPsJgBcDB/EgFDzH4oCJKtC5A4tNuIhLwFMCzubAsFeywAAw2AYg0DhDiHiWGLjSp9f5CCkyl0dpB5mSgAEgRggAwCcU4OAGttCgBZnydAExACB1NVBwRmZ1xLQwZGBklLsSYBPMk6PjfNzs/Q0dLT1DQhADs=}
image create photo buttonimagebng -data $bngdata

set bpfuncdata {R0lGODlhIAAgAOYAAAAAAP////7+/vv7+/f39/b29vX19fT09PPz8/Ly8vHx8fDw8O/v7+7u7u3t7ezs7Ovr6+rq6unp6ejo6Ofn5+bm5uXl5eTk5OPj4+Li4uHh4eDg4N/f397e3tzc3NnZ2djY2NfX19bW1tHR0c3NzczMzMrKysbGxsXFxcTExMLCwsHBwb+/v76+vr29vby8vLu7u7m5ubi4uLW1tbOzs7Gxsa+vr6ysrKurq6mpqaioqKenp6ampqWlpaSkpKOjo6GhoaCgoJ+fn56enp2dnZycnJubm5qampmZmZaWlpWVlZSUlJOTk5KSkpGRkZCQkIyMjImJiYiIiIWFhYSEhIODg4KCgoGBgX9/f35+fnx8fHt7e3p6enl5eXV1dXR0dHBwcG5ubmxsbGtra2NjY2BgYFJSUlFRUVBQUE5OTkZGRjc3NzExMR4eHv///wAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAACH5BAEAAG4ALAAAAAAgACAAAAf/gAGCg4SFhoeIiYqLjI2HVVRKQ0aUlZaXmERBV4Q/FZ+goaKjolWERqSpqRYVKFynqrGhFARDT7CysgYnSDiwFsDBwsPEFxVMMzSER7mhHQcHHZ8TIVkOOb/E2hcLMjAJF8ZATxU8uLkXBgJpagcGFAtWKRU756sUBxUPWkA+WAQVZkwBqMMeqQkWPLwAg+TTEyoBnNz4VHBQkQoUMmrcuCDCGTMkKkBwcCxMFAgPKlQUxExVhwIryIiowIFDBwcP0EipsKFCD1zhMlAY4EADhggKtpjgGe4ChgkZNlgwVs/ip4wQMrjIgMBYkp0bNS5ogFHlOQsHPqzh8qnFGAkKhljFWhkAFagCJa44kaLCy1IMuehezMgqSI0KPtoIqWAhrGMKZge13ADhQ5YPHWWAkNAz10/JFTIYg7KEJuQHHJqVI3SxgoERXS4weExbY2RBdj+wyVHBmOpQdIkw/iKmA4LfoujyqCCFBXJSNQhpKcMkRo4d2LNr387dRhNH4MOLH0++/KJAADs=}
image create photo buttonimagebpfunc -data $bpfuncdata

set displaydata {R0lGODlhIAAgAMQAAAAAAP///09vgVNpdlhjaUCBoUV7l0p1jCmWxi+RvTWLtDqGqxGl3xqg1yKbzwCu7wBcfwBdfwBRcABScAAxQwAsPAArOwAmNAep51xcXP///wAAAAAAAAAAAAAAAAAAACH5BAEAABoALAAAAAAgACAAAAWrYCCOZGmeaKqubOu+cCzPdG3fuH1JfO//wJ+FNHkYj8ikMgkhLp9QY3NUNDoOjcXzCp2Kig2DAbvAjBGJxSFxNR/QiezDGygaEonDVfFWKAoMBFcOfnoGfnNObwh6WH8ECmUZVwmHBw8EBwyJVA8IAmNXDQMHhwICClcIAwYCDwWunF9RD4hLGAsKUk5RDg5PDAoYuyMVEcfIycrLyhQ5z9DR0tPU1dbX2DghADs=}
image create photo buttonimagedisplay -data $displaydata

set envdata {R0lGODlhIAAgAOYAAAAAAP////7+/vv7+/f39/b29vX19fT09PPz8/Ly8vHx8fDw8O/v7+7u7u3t7ezs7Ovr6+rq6unp6ejo6Ofn5+bm5uXl5eTk5OPj4+Li4uHh4eDg4N/f397e3tzc3NnZ2djY2NfX19bW1tHR0c3NzczMzMrKysbGxsXFxcTExMLCwsHBwb+/v76+vr29vby8vLu7u7m5ubi4uLW1tbOzs7Gxsa+vr6ysrKurq6mpqaioqKenp6ampqWlpaSkpKOjo6GhoaCgoJ+fn56enp2dnZycnJubm5qampmZmZaWlpWVlZSUlJOTk5KSkpGRkZCQkIyMjImJiYiIiIWFhYSEhIODg4KCgoGBgX9/f35+fnx8fHt7e3p6enl5eXV1dXR0dHBwcG5ubmxsbGtra2NjY2BgYFJSUlFRUVBQUE5OTkZGRjc3NzExMR4eHv///wAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAACH5BAEAAG4ALAAAAAAgACAAAAf/gAGCg4SFhoeIiYqLjI2HVVRKQ0aUlZaXmERBV4Q/FZ+goaKjolWERqSpqRYVKFynqrGhFARDT7CysgYnSDiwFsDBwsPEFxVMMzSER7mhHQcHHZ8TIVkOOb/E2hcLMjAJF8ZATxU8uLkXBgJpagcGFAtWKRU756sUBxUPWkA+WAQVZkwBqMMeqQkWPLwAg+TTEyoBnNz4VHBQkQoUMmrcuCDCGTMkKkBwcCxMFAgPKlQUxExVhwIryIiowIFDBwcP0EipsKFCD1zhMlAY4EADhggKtpjgGe4ChgkZNlgwVs/ip4wQMrjIgMBYkp0bNS5ogFHlOQsHPqzh8qnFGAkKhljFWhkAFagCJa44kaLCy1IMuehezMgqSI0KPtoIqWAhrGMKZge13ADhQ5YPHWWAkNAz10/JFTIYg7KEJuQHHJqVI3SxgoERXS4weExbY2RBdj+wyVHBmOpQdIkw/iKmA4LfoujyqCCFBXJSNQhpKcMkRo4d2LNr387dRhNH4MOLH0++/KJAADs=}
image create photo buttonimageenv -data $envdata

set floatatomdata {R0lGODdhIAAgAKUqAAAAAAEBAQMDAwYGBgkJCQoKCgsLCw4ODhMTExoaGiUlJTk5OUFBQUhISFVVVVhYWF1dXV9fX2BgYHNzc5OTk56enq+vr7Gxsbe3t729vcDAwMTExMjIyMvLy9/f3+Dg4Obm5unp6evr6/Hx8fb29vf39/r6+vz8/P39/f7+/v///////////////////////////////////////////////////////////////////////////////////////ywAAAAAIAAgAAAGgUCVcEgsGo/IpHLJbDqf0Kh0Sq1ar9EAYMvtegsfJ+CpQWiaY7KZmVaflW0RqfkpLMemh0LQQd9VFgspFQYnbH8UGSMHAyKHcCkMHBAYESGPSWMTDhIoCZd/cCoXAyUeBCaYSGMoDw0AG36iQiAjYktaXrq7AVi+v8DBwsPExcZJQQA7}
image create photo buttonimagefloatatom -data $floatatomdata

set hradiodata {R0lGODdhIAAgAMIEAAAAAAEBAQnU+QvU+f///////////////ywAAAAAIAAgAAADVki63P4wykmrvTjrzbv/YEgEQGma5HkGExC5EPwSQm0PhOzoj2v/uZnE96sFYy1aUXDsJZdMHkPaIBabu+SgiKMqvAuweJJSmc0skXrNbrvf8Lh8fkkAADs=}
image create photo buttonimagehradio -data $hradiodata

set hsliderdata {R0lGODdhIAAgAMIFAAAAAAEBAQJpfAnU+YDq+v///////////ywAAAAAIAAgAAADS1i63P4wykmrvTjrzbv/YCg6AWCeaJoGEkARcONGszTcckvdQ07vOEbtMYTwfJDi4yjUTZgLZfMZjEpKqpRguxp5v+CweEwum8+NBAA7}
image create photo buttonimagehslider -data $hsliderdata

set keyboarddata {R0lGODlhIAAgANUAAAAAAP////39/fz8/Pv7+/r6+vn5+fj4+Pf39/T09PPz8/Ly8vHx8fDw8O/v7+7u7u3t7dXV1dLS0tHR0dDQ0M/Pz87Ozs3NzcjIyMTExMPDw8LCwsHBwcDAwL+/v21tbWxsbGhoaGJiYv///wAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAACH5BAEAACMALAAAAAAgACAAAAb/wIBwSCwaj8ikcslsHinQC8ZCkVIpV2t1ur1ShpfFgfPJFDIfzsFRcYzLZzhavbgMKQhBR5QJZEQdAgoVCnp8fod/gQhfQhQJBRwhGQMZIRwFDBUMkZOVnpaYCY0Bj52UoZmbp5+olwWjd5CSrpianLStn6Kkprmpt6yWtbC9s6Cvwb/IvLIFHZ4ayavQlNLWr7GOx8TKyMTapdy7qrjf5OG+zOXC6+kJBsu2q8vgxs8iGgIagOx7+vwA9ntnwAMIDQQ0gPBg4FbBgwkhKmT4LoGFDRMaTNhgIcEtixg1htzYMR0DBBAiQECpEsGDCg9Yrkw5syWDXgwONKDQwMDOVgYHXj7QydNn0Z8Hbt7JKTQoTKdDm0p9qtQR06dTo2LdOrRqqataw0Id29QrBbBkK0BIK9Ys2qxsx3q98JZr3LJ2hEiAwrev37+AJTgZTLiw4cOIlQQBADs=}
image create photo buttonimagekeyboard -data $keyboarddata

set knobdata {R0lGODdhIAAgAOMQAEdHRyGy9X2pu6Kioi7B/rW1tWXP+8PDw4/V9dDQ0K7a7tzc3Mjm8ubm5u3t7f7+/iwAAAAAIAAgAAAE3fDJSau9OOvNOx5AOIxkaZ6DNTRs675wm1YrvBxFWRxLPFM1V26wSyRwowLsNwk2FiOeY0q9jXoyVQs6WFC/1WuWJluBz47GaAxk5bzoM1Sp1kIP8fihWyfv4Xlgd31tBQWBcYaETWp4iGd7ixIrAwlxCAh6lFqVaAgEmWiRTJONZwoEBnmjWoZgDAQEDXmKpA8rf1QNsQx5g7Y1A44OBgQKgX+St25dDp/HvkQswGWfoXFcbIwsCwIBqsh801otCgizgUvkMew+6+3w42Tx9MAhACj5KB78/f7/ACMAADs=}
image create photo buttonimageknob -data $knobdata

set menuarraydata {R0lGODlhIAAgAPIEAAAAAAEBAQoKCuDg4P///wAAAAAAAAAAACH5BAAAAAAALAAAAAAgACAAAAOGSLrc/jDKSau9OOsNA/hgKIbBBGynlGYr1BJBac5MrMjP66ylPQCDx88kO8VKAEGQMRDoGoBictDD1Z6MaOwTDDSjDs9MXFTCwthFymo7s9MK3fcmVsxVQjPhY1fC9xRTN4B4E15mSDSBSomFgSCEEX99VHGKGH+TOYYjnSNWHKGio6SlpgkAOw==}
image create photo buttonimagemenuarray -data $menuarraydata

set msgdata {R0lGODdhIAAgAMIGAAAAAAEBAQoKChMTE8DAwODg4P///////ywAAAAAIAAgAAADUmi63P4wykmrvTjrzTP4YCiOotCQAEYMxDkG19qenfykWyEUEL75DqAnIsQUF0dL0rCkLJuTZ0ea2/VqrFvHYHOJYLEsA0Umm7botHrNbrvf6AQAOw==}
image create photo buttonimagemsg -data $msgdata

set mycnvdata {R0lGODdhIAAgAIABAEDJ/P///ywAAAAAIAAgAAACOoyPqcvtD6OctNqLs36g+w92U0h+Y1meaKiupuSycPxGdA3dXkvzse8CroQoYmqmM5I2zKbzCY1KLwUAOw==}
image create photo buttonimagemycnv -data $mycnvdata

set numboxdata {R0lGODdhIAAgAKU5AAAAAAgICAkJCRISEhQUFBcXFx8fHyUlJTo6Ojs7O05OTlRUVFVVVV9fX2VlZWZmZm1tbXNzc3R0dHZ2dnp6eoWFhYaGhoiIiJOTk5SUlJycnJ2dnaOjo6+vr7KysrS0tLu7u7y8vL29vcDAwMPDw8TExMzMzM3NzdTU1Nvb29zc3N3d3eDg4Obm5ufn5+jo6Orq6uvr6+zs7O7u7vX19fb29vr6+vv7+/z8/P///////////////////////////ywAAAAAIAAgAAAGwMCccEgsGo/IpHLJbDqf0Kh0Sq1alYCsdssVsJ6AgQhHLpvJosHICUiPz/D0mgnAueEymFlOR4vLNxZZEzZlKwJ9fm8mATMvACFmAImKOBgUOBUAF5KUZCQDNw4cKQIbCoVkk0t1ZSoFHTcPHgYlH6hlq1hlMQcYZBoADZaYuYk0CRFlJwAoNQQgnaw4NwsMN4CCABCpqn0SCDRwLi1wukkAGQd6cO0450gABSnu7vBHXPn6WVf9/v8AAwocSFBKEAA7}
image create photo buttonimagenumbox -data $numboxdata

set objdata {R0lGODlhIAAgAPEDAAAAAAEBAYCAgP///yH5BAAAAAAALAAAAAAgACAAAAJInI+py+0Po5y02ottAJyLI3QeCAQNkKGMerGKW8GIPH1r5tj4zkt6f6AZfrFTyogRDpQQJfNBnAETupCoKgJEp9yu9wsOiyMFADs=}
image create photo buttonimageobj -data $objdata

set presetdata {R0lGODlhIAAgAMZIAAAAAFgAPiw+fD5MkZFMLJFMPgB0v4NYa3xikZFia2trkaRrYqp0WHSDa3SDumuLkYuDmL+DALV8a7p8YrV8dHSRi3yRxL+LdMSLfJmZmZGekcmvg5G6xMS1tcS6uqrEus7E0uTEnszMzOjJpNLS5NbW1snb7LXk9Lrk9NLf7PDfur/o9OPj4+Tk5PTkuuTk7Ojk5OXl5cns9NLs9Ojo6Nfw9PDs6PTs3+Tw9Nv09PTw19/09PTw3/Hx8ej09PLy8vT05PD08PPz8/T06PD09PT07PT08PT09P///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////yH5BAEKAH8ALAAAAAAgACAAAAf+gEiCg4SFhoeIiYqLjI2Oj4wZAJOUGZaXmJmWiBkinjQ9QjQlnqUiJTRCPTSlGZyeLUeyR0Ixpi0/s0e2Iq6HnTG5uj+mwrM/Lb2cNLIqASizrCLMRxMJK9G+hhk9skYU0LI9nt1GLwcysz3ahRlCsxfhtJ7vRyMzukLshBnUurLS/P2jsW9QhhLGdJE69U+WkBIFBXXC9Y9YqYRHfrBQ9qsUKFELTaFSJY3jNlMoU6qMiKSTypcpWbqcFmpUypGrWr0KVrFYxWQyS7zj4SHFrIUlZpE4BnGZrAUfEET7NMuALoKc6oEIImGWEHpHhhQQUMOrzG5HGFRQoI7cLAc1utY5lQWDyNRpDQHKFEFxmM9hG/eKiDEsWakY9WQZFkwT5M1UOT3J1ES58iZImDNr3sx5cyAAOw==}
image create photo buttonimagepreset -data $presetdata

set scopedata {R0lGODlhIAAgAPcAAAAAAP///5mZmZiYmPLz9e/w8eXm5+Pk5c7x/+Dl59/k5nnb/4Tf/4bf/4HV9Kbn/6rp/7Xr/7vs/8rx/9/2/87h6On5//L6/XrZ+YHf/4LZ9oXa9ond943i/pHl/5Hj/5Di/ZLk/43Y8Zfm/5fl/47X8Jrm/5nl/pbe953m/57m/5je9qDo/6Dn/6Pp/6bp/6Xp/pvY7arq/6no/Z3X663r/7Ds/7Hr/rTt/7Tr/bPq+7nt/7vv/7Xm9r/v/8Xx/8ry/7ze6c/z/9X1/83p8tn2/933/9r0/N31/dLo7+L4/9bp7+b5/+r6/+77//f9/+rw8t7h4pPn/5Xn/6Hq/6Lq/6Tq/6Di9qfr/6jr/6fp/Krh8b3w/7vu/cDx/7vq96/b6MPx/8Ty/8by/8Hm8cXg6M3o8O37//L8/6Ds/6bu/6fr/abn+Lbw/7jx/8by/df3/9z4/9j0+9z1+/D8//D4+p7t/r3x/NH3/9v5//H7/fn+/+X7/+j8/836/635/4OXmY2YmfH+//L+/535/6H6/6P5/6f5/7f6/7v7/7z7/9H8/4SXmLX9/9b+/9z+/97+/+f+/8L+/4eYmI+YmJOZmff///r///z//5aYmPv8/O3u7piZmYeamYmamYqZmI+amZCamY2bmZSbmZqbmf//+////P39/P/9++Xj4f79/P328v728p6amPfz8fDu7ejm5fnw7Pbx7/Ll4O7m4/Xl4PXo5PDk4Pvw7aGamPzy7/z08v/7+vLu7ejk4/n085+ZmJ2ZmfHu7ufk5JmXl42Li/37+/Xz8/Lw8Ojm5v/+/pqZmfr6+vf39/X19fPz8/Dw8O7u7u3t7ebm5uTk5JeXl4uLi////wAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAACH5BAEAANcALAAAAAAgACAAAAj/AAMIHEiwoMGDCBMqXMiw4UFrAjjlGgWqkihSywRo3MiRUzBSnSqFElCM4ICBfxRhMhSp4aJClhI5CmZS4JgFFAI8YLFnIZ0GEQIgMPSpZoAWKgQ2yaBkoZAFcAQ2YlTzjAkgA62EWeiFykBIlGoaAYHE1ClTOW6cWsuWrbILJ7qYVabs5EBiq3qgiCUsWi8yK15FG0w42iZcSzQkwbUp2i9iBIHp6TCjFDNNxi6AkINKk+fPxnjpOFHqsyq7AnMx8TCE4J4RNhA+SSGjIOoAueK8MCjGBUI0J3bYJtgqDw6DTrAMOtikSp/hA0nhyYGKmXVmvOrMmMPrunVeR7RY+75+6nYgO2BmHaDG3leUEkFurWc/LFUMGrXmU5NWjSCgQwS48syAzxzzDBtXsELgM79AwcEXuyzYTH8DTSIJQnBMYRAfUghi0G2UPIIQEyEUUdAEEBx0WyaXyJYFFwXxUIOKJilzymefnYLKGzCYoooma61xx4851kVQNdBMo+SS0xhgSxkOKECLAbAkIEIFyTA5DX9HNgPNl2B+qQsRGJihCzSybLFBAciECY0zFAp0m0GYqNHGQGm4gdBtcxr0AwkDpWDBnkYl1AQDPgQgwQdoEDpQnwYh8kcAhPiR0G0QcaSpR6OIwoknpASj6UbWOGTqqaimquqqCwUEADs=}
image create photo buttonimagescope -data $scopedata

set slider2ddata {R0lGODlhIAAgAOYAAAAAAP///72+v7Gys8DGydH0/+f5/+b4/tHz/dDy/NHy/NT0/dPz/N/4/973/t32/b7U2uf5/s/z/NH0/dDz/NHz/NX2/tP0/NT0/Nn1/N73/cDU2eb5/sfW2svX2tL3/933/ef7/7vFx7zFx8LHyOj8/9P6/+T+/9r//+b//+3///H///n//7GysqeoqNDQz93Y1szHxcO9u8zJyMG+vc3JyMK+vdrW1czHxsG8u8TBwc7Nzb28vO/v7+Li4tbW1tTU1M7Ozs3NzcvLy8DAwLu7u4mJif///wAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAACH5BAEAAEcALAAAAAAgACAAAAfYgAGCg4SFhoeIiYqLjI2HRpCRkpOUlYRGhiYljpedhRQRnIOYo4YVHKKCpKqGFKipqwGxghWhsJ6ECa+isbMBtamyuIOnwb2GCAfGw7S2vMy/u5zHhcC3pYUTztPQCsrXrINBOiMkPD7P2IJEAjEzLT/pqioXFhIiOENCORsoGBcMMPwDKJDBhQUXUvRi8aBBBgI1dryw0eGEBhAXQTzQ4ECDR48PVlATVGSADBougMgTNsgHDAgebvRY6auAgWXqBunCGY7QhxA8ZVUaSjRSsKNIkypdeigQADs=}
image create photo buttonimageslider2d -data $slider2ddata

set slidersdata {R0lGODlhIAAgANUAAAAAAP////39/fz8/Pr6+vn5+fb29vX19fT09PPz8/Ly8vDw8O/v7+7u7uzs7Ofn5+bm5uTk5ODg4N7e3tzc3Nvb29ra2tPT09HR0dDQ0M7OzsvLy8rKysfHx8XFxcTExMHBwcDAwL6+vr29vbu7u7q6urW1tS4uLi0tLSoqKikpKSgoKCcnJyYmJiUlJSQkJCMjIyAgIBsbGxkZGRYWFhUVFREREQ4ODg0NDQwMDAsLCwoKCgkJCf///wAAAAAAACH5BAEAAD0ALAAAAAAgACAAAAb/wIBwSCwaj8ikUomqyZ5QmYtFrVqv1dhJmFN4v+Cw2JswGBAQmrClWLqJK+Gq/UYOMh3JQiVk0esCAwJDAxshFHt9f0aBg0J3HRkDjxofE4kBfnZ4ko+GG5MBA5WXfJmLRIUhoJQfGqGjlpiaR7GvrbeipLOohJW5trC7prRGwbjCssS9yM26yopCjb6uycDD0ZCdz68Cgb/JpdmfyRgEGRkEGNWt4pkH3OUEGxvq1d6xl3GZDN/3/uY0aLBnDp09fUJegJBA8Fy6dQEHQpxXb527FyQoNKTXUGBHidACvBAx4aPJiR4tYhpZEiXIiCdDsowJ0+XBlSRpEkj5oWZINhc5bQrtuRPkxaBEefr0eSmFEB1IIxYYynTBDCFNokCBweKF1xZfvb4AO9arCy110qpdy3ZtEAA7}
image create photo buttonimagesliders -data $slidersdata

set spectroscopedata {R0lGODlhIAAgAPcAAAAAAP////39/YmJif/9//3+//T7/1XG/1bH/1jI/+v1+lfI/1nK/1rM/1rL/1vL/1zL/1zN/17N/2HO/nrW/7/l9uHq7l3O/17O/1/P/WHS/2HR/2LR/2LS/2TQ/2PQ/WXT/2fT/2bS/WjT/2fR/WbQ+2jS/WrU/mvV/2zV/2vR/G7U/3HX/3fX/3bV/HvZ/4Hb/4HX+ZLg/5Xg/6Dj/6Xj/Lbh8sLt/7zk9M7x//j9//b7/WbV/2nV/2vW/2rV/WzW/2/X/3LY/3TZ/3PY/Hbb/3XY/XfZ/3fX/HfX+3vb/3zb/3vZ/X3d/37c/oHe/4Hd/3/X+YXf/4LZ+YTb+4nf/4rg/4vf/4nd+4zh/4rd+orb+Yvc+ZDh/5Di/4/g/Y7e/JPj/5Tj/5Pg/Jnk/5vk/5Xb9Zzl/5/m/53j/KPm/57e96bn/6Xk/KDe9avp/63p/6rl+rPr/7Tr/7jr/bfp+7Tj9L3t/8Pv/8Xu/cLk8M/x/dLz/9Tz/9X0/9Py/dn1/9fz/eD3/9Po8OL3/+T4/9Xo7+j5//D7/+Lq7XDZ/HXd/37f/4Dg/33b+oLf/4Db+n/a+Inh/4ze+pLk/5jm/53n/6Hn/6Lp/6fp/6nq/7Dr/7Xs/7bt/7jt/7vu/7zu/7jk8s3z/9Dz/8Tm8df1/9Lw+tz2/9/3/+P4/+Dz+ef5/+X3/er6/+n4/e77//f9//X4+YHf+o/l/5Tm/5fn/5jo/53o/qHp/7Dj8rnl8cby/8rz/870/8/w+tv3/+37/3rm/5Tp/7fx/9P2/+D5//D8/3fm/4Ho/4Xq/5ft/67x/93q7fT9/+73+efs7ebr7H/q/4ft/4js/4nu/4vu/5jv/6Tx/6Xy/6nx/7r1//L9/5nx/670/7T2/8L3/9X6/4Ty/6T1/8L4/6b3/6n4/4j3/779/93+/+P+/6P+/6P9/6b9/6j9/8b+//f///r9/fj6+f//+P///f3x6fzy6/Pu7Pru6v36+fHu7f79/e7t7e3s7O3t7YqKiv///yH5BAEAAP8ALAAAAAAgACAAAAj/AAMIHEiwoMGDCBMqXMiw4cEBECNKnEixIsEBDhliHLgxY8KOATquguWxIMiOOV6VvMhSwLw9rFzNE0Czps2bNUOy7GeP1CAb+/oJFbov31Ch+Zg9C9pPJ8cALgPRuQVvJk198Hboszkvzx+rTgV2PAXHS4GCiEQJ1HE2wCZOLJ8KRJUpS9uBiEYVWMVHJSw1m+KKHSgIU5UAwHoNbCUKVh8/fi9hEhyWUKVHAQ5ZaiaQmKZXpfqo3IamFmWM/PYZitTBArQYuerd02PGTSg7iejpCoKF3j5+YXXii2XKCQMF8b6AUSXPV5o2dWo4k1eHwQt5+OIF32gAD5QGiACJ/zkiJ8CuM28+zQGUCg0HFMYEpjuJjjObIBgK0eii5E6AME+ggQcnp9wgxQYg/CKQOyD5w44gAZQxAgapXOKFEqAEIMkQLXiSSSdyKMFAB7wEYAA5J6mDCwFqpAACKpeE8YIXhFSxBBCNLEEBGk04sAEahNwhzUnmlFABFiJMoYUjk2AQBQ4mJHHCByFssAUkDkSwBhVGIHNSOIrIMgECTKhABBILjMHFARk4YMIKHfzgQgQjxEECBNOctE4Vi3TAQBVPYPAeGT0g4IADHDCySANLCNGDMD44EM1J7WTBQhEPyECLBgkIMQsPDSAAwgK2WOIAL1dcQEkLDhxz0jlWQE6wCxlKsCFFAy/AgAEEDTRxgCZqeIAIDBLMIEkQ3Jz0zjjlbFNMN9kos8w12gRDjTfgKDPMN8no8I014lSDzXYVlWtuRCulq+667LZ7UEAAOw==}
image create photo buttonimagespectroscope -data $spectroscopedata

set symbolatomdata {R0lGODdhIAAgAKU8AAAAAAEBAQMDAwYGBgoKCgsLCw4ODhERERMTExkZGRoaGi4uLjc3Nz09PT4+PkRERExMTGBgYGhoaHR0dHx8fH9/f4GBgYqKipSUlJycnKSkpKioqKmpqaurq62trbCwsLS0tLe3t76+vsDAwMPDw8XFxcjIyMrKytra2tvb29zc3ODg4OHh4ebm5ujo6Onp6evr6+/v7/Hx8fPz8/T09PX19fb29vf39/n5+fz8/P39/f7+/v///////////////ywAAAAAIAAgAAAGpkCecEgsGo/IpHLJbDqf0Kh0Sq1ar9IAYMvtegkrJ+A5QoyaY7KZmVafle3nirCMP+1GPLrO01UWAichFkIeGRgzRXpEYzIDLkI2Byw5Ci8gBhQwQ4ucPDsdAAwpPBwSJRFDGgAXQp2uPDQ5PBsFOTgJAio8LRMMIjqwcDwoABAGGkIfDzs1DSQ7jHxCMTdCOg4mSa88Wl7f4AFY4+Tl5ufo6errSEEAOw==}
image create photo buttonimagesymbolatom -data $symbolatomdata

set textdata {R0lGODdhIAAgAKU0AAAAAAMDAwgICAoKCgsLCxUVFRcXFxsbGyoqKisrK0BAQEFBQUNDQ0REREhISE9PT1NTU1lZWVpaWmBgYGRkZGZmZm5ubnBwcHl5eYiIiJ2dnaCgoKurq7W1tby8vMHBwcfHx8jIyMvLy87OztLS0tra2t/f3+Li4uXl5ebm5ujo6Onp6e7u7vf39/j4+Pr6+vv7+/z8/P39/f7+/v///////////////////////////////////////////////ywAAAAAIAAgAAAGj0CacEgsGo/IpHLJbDqf0Kh0Sq1ar9isdst9ziKAMCBEAQwYscqGxpmAxSGl66XoxGSQ1NAkUBlQc3UxM0szCx80MxAsRBgAGUKGiEwzCohfDg0aQiUAJ5GWTZWXECcxMIkXABagk4WhiitDIgQtBSOJoZQLIIkSAQkPLwckNB4Ig7xdy8zNzs/Q0dLT1FBBADs=}
image create photo buttonimagetext -data $textdata

set toggledata {R0lGODdhIAAgAOMIAAAAAAEBARXV9xjV+Ijp+L3y+d72+vz8/P///////////////////////////////ywAAAAAIAAgAAAEdxDJSau9OOvNu/9gKI5kaW5Aqq5sAAJHLM+xBLwHYdByMSC3D8xX4BEPQQ/scOwNikhcrNmMCmfEqlVJKwgEUFmys+x9w7Exp8x8atUoJ7QK1yy1zXpm+DT29RgwOjxTP4AXbIQHNiABLI8qLieTlJWWl5iZmiQRADs=}
image create photo buttonimagetoggle -data $toggledata

set vradiodata {R0lGODdhIAAgAMIFAAAAAAEBAQTX/gPY//z8/P///////////ywAAAAAIAAgAAADY1i63P4wykmrvTjrXYH/YMAVAGGeJzCWQ+sKhMqxrhuvRG3LG63fs5xuAOwJfzxNSaCDJTMlFOqJAVk9OGkqqy0quybqJdoVW8hacwfshbLVFLQUPpFPR4HrRzTq+/+AgYIFCQA7}
image create photo buttonimagevradio -data $vradiodata

set vsliderdata {R0lGODdhIAAgAMIFAAAAAAEBAQJpfAnU+YDq+v///////////ywAAAAAIAAgAAADUFi63P4wykmrvTjrEoD/QLAxwFOOytmoI7u4GlzImExfNpqa+szrOeAPFSQSBsikoAc4JpFLoPMZJQ5b100R67hZttqspgPyiHrotHrNziQAADs=}
image create photo buttonimagevslider -data $vsliderdata

set vumeterdata {R0lGODlhIAAgAPIEAAAAAGBgYEXoAOnpAP///wAAAAAAAAAAACH5BAAAAAAALAAAAAAgACAAAAN2SLrc/jDKSaslIOjBuQ7AxWRbN3yhqJBfq6Uqa56vun4zamNuD4syk84W7AxjPdfvUvTUiDjhE5k8AqPG6bWUBe1Y1aWFJSiXrcyP+axNV9tj9Rod16wF9Ao4KdbLzXkUZHNwfm9eUHaEiFQBd4ETez47lJUVCQA7}
image create photo buttonimagevumeter -data $vumeterdata

::pdwindow::debug "\[ceammc\]: ceammc_buttonbar-plugin loaded\n"
