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

proc buttonbar_theme_suffix {mytoplevel} {
    if {[tk::windowingsystem] eq "aqua"} then {
        if {[tk::unsupported::MacWindowStyle isdark $mytoplevel]} {
            return "dark"
        } else {
            return "light"
        }
    } else {
        return "light"
    }
}

proc make_pd_button {mytoplevel name tooltip} {
    set theme [buttonbar_theme_suffix $mytoplevel]

    button $mytoplevel.buttonbar.$name -image btn_image_${name}_${theme} \
        -relief flat -borderwidth 0 -highlightthickness 0 \
        -highlightcolor $::pd_colors::window_background -highlightbackground $::pd_colors::window_background \
        -padx 1 -pady 1 \
        -command "menu_send_float \$::focused_window $name 0"
    pack $mytoplevel.buttonbar.$name -side left -padx 0 -pady 0
    ::btnbar::tooltip $mytoplevel.buttonbar.$name $tooltip
}

proc make_iemgui_button {mytoplevel name tooltip} {
    set theme [buttonbar_theme_suffix $mytoplevel]

    button $mytoplevel.buttonbar.$name -image btn_image_${name}_${theme} \
        -relief flat -borderwidth 0 -highlightthickness 0 \
        -highlightcolor $::pd_colors::window_background -highlightbackground $::pd_colors::window_background \
        -padx 1 -pady 1 \
        -command "menu_send \$::focused_window $name"
    pack $mytoplevel.buttonbar.$name -side left -padx 0 -pady 0
    ::btnbar::tooltip $mytoplevel.buttonbar.$name $tooltip
}

proc make_ceammc_button {mytoplevel name tooltip} {
    set theme [buttonbar_theme_suffix $mytoplevel]

    button $mytoplevel.buttonbar.$name -image btn_image_${name}_${theme} \
        -relief flat -borderwidth 0 -highlightthickness 0 \
        -highlightcolor $::pd_colors::window_background -highlightbackground $::pd_colors::window_background \
        -padx 1 -pady 1 \
        -command "menu_send \$::focused_window ui.$name"
    pack $mytoplevel.buttonbar.$name -side left -padx 0 -pady 0
    ::btnbar::tooltip $mytoplevel.buttonbar.$name $tooltip
}

proc make_ceammc_button_tilde {mytoplevel name tooltip} {
    set theme [buttonbar_theme_suffix $mytoplevel]

    button $mytoplevel.buttonbar.$name -image btn_image_${name}_${theme} \
        -relief flat -borderwidth 0 -highlightthickness 0 \
        -highlightcolor $::pd_colors::window_background -highlightbackground $::pd_colors::window_background \
        -padx 1 -pady 1 \
        -command "menu_send \$::focused_window ui.$name~"
    pack $mytoplevel.buttonbar.$name -side left -padx 0 -pady 0
    ::btnbar::tooltip $mytoplevel.buttonbar.$name $tooltip
}

proc ::btnbar::init {mytoplevel} {
    frame $mytoplevel.buttonbar -cursor arrow -background $::pd_colors::window_background
    make_pd_button $mytoplevel obj                      [_ "Object"]
    make_pd_button $mytoplevel msg                      [_ "Message"]
    make_pd_button $mytoplevel floatatom                [_ "Number box"]
    make_pd_button $mytoplevel symbolatom               [_ "Symbol box"]
    make_pd_button $mytoplevel listbox                  [_ "List box"]
    make_pd_button $mytoplevel text                     [_ "Comment"]
    make_ceammc_button $mytoplevel bng                  [_ "Button"]
    make_ceammc_button $mytoplevel toggle               [_ "Toggle"]
    make_ceammc_button $mytoplevel number               [_ "Number box2"]
    make_ceammc_button $mytoplevel hslider              [_ "Horizontal slider"]
    make_ceammc_button $mytoplevel vslider              [_ "Vertial slider"]
    make_ceammc_button $mytoplevel hradio               [_ "Horizontal radio"]
    make_ceammc_button $mytoplevel vradio               [_ "Vertical radio"]
    make_iemgui_button $mytoplevel mycnv                [_ "Canvas"]
    make_iemgui_button $mytoplevel menuarray            [_ "Array"]
    make_ceammc_button $mytoplevel knob                 [_ "Knob"]
    make_ceammc_button $mytoplevel sliders              [_ "Sliders"]
    make_ceammc_button $mytoplevel slider2d             [_ "Slider 2D"]
    make_ceammc_button $mytoplevel keyboard             [_ "Keyboard"]
    make_ceammc_button $mytoplevel env                  [_ "Envelope"]
    make_ceammc_button $mytoplevel display              [_ "Display"]
    make_ceammc_button_tilde $mytoplevel scope          [_ "Scope"]
    make_ceammc_button_tilde $mytoplevel spectroscope   [_ "Spectroscope"]
    make_ceammc_button $mytoplevel preset               [_ "Presets"]
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
set bngdata_light {R0lGODdhIAAgAIQUAAAAAAEBAQEDAgUKCwsTExwnKB0oKQdhcQdlc3NvcQaqyAasygax0AjJ7KmpqQnU+cC+vsDAwN7e3vz8/P///////////////////////////////////////////////ywAAAAAIAAgAAAFpyAljmRpnmiqrmzrvnAsz3Rtr0Cu73wAA5OgJFEYDAoJSXAC+AUdBMSi0VggBJFg8wWMABSPsJgBcDB/EgFDzH4oCJKtC5A4tNuIhLwFMCzubAsFeywAAw2AYg0DhDiHiWGLjSp9f5CCkyl0dpB5mSgAEgRggAwCcU4OAGttCgBZnydAExACB1NVBwRmZ1xLQwZGBklLsSYBPMk6PjfNzs/Q0dLT1DQhADs=}
set bngdata_dark {iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAIAAAD8GO2jAAAA8UlEQVRIx2M0MjJioCVgYqAxGLVg1ALKAQsuifT0dOJNmTlzJskWMDAwPHjwAM5+LG2+Qcn6C48Iz5c3AfeOyj49CZdSUFAgxwdw8IlXeqptNpz7hVdkib4/g75/9uGpfJ+fUhoHaKYjg6m22Z94pSm1AJfpxMgStuCxtDlB/QTV4LNgg5I1QQsIqsFnwRceEYIWEFSDzwKeL28IWkBQDT4LAu4dJWgBQTX4LEDOTWSrIZBMsw9PJVuWKAv4Pj/FZQqROZlwUcH3+Wnltio8ZRH5FiCXYgoML20fr4NyWBkY8BZwyIBxtNkyasGoBbS3AADIC001mj+B7wAAAABJRU5ErkJggg==}
image create photo btn_image_bng_light -data $bngdata_light
image create photo btn_image_bng_dark -data $bngdata_dark

set displaydata_light {R0lGODlhIAAgAMQAAAAAAP///09vgVNpdlhjaUCBoUV7l0p1jCmWxi+RvTWLtDqGqxGl3xqg1yKbzwCu7wBcfwBdfwBRcABScAAxQwAsPAArOwAmNAep51xcXP///wAAAAAAAAAAAAAAAAAAACH5BAEAABoALAAAAAAgACAAAAWrYCCOZGmeaKqubOu+cCzPdG3fuH1JfO//wJ+FNHkYj8ikMgkhLp9QY3NUNDoOjcXzCp2Kig2DAbvAjBGJxSFxNR/QiezDGygaEonDVfFWKAoMBFcOfnoGfnNObwh6WH8ECmUZVwmHBw8EBwyJVA8IAmNXDQMHhwICClcIAwYCDwWunF9RD4hLGAsKUk5RDg5PDAoYuyMVEcfIycrLyhQ5z9DR0tPU1dbX2DghADs=}
set displaydata_dark {iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAYAAABzenr0AAAAc0lEQVRYw+3WIQ7AIAwF0M8y0+v0Pmhsj4LlDtyvcrMTZMtCRsX+Nw2C9ImmkFT1QGA2BIcAAggIB+zXQyllSdPW2hgAAO5+eznnPNW8984ZIOAZYGbDugxQax1WzgABn78FACAir1bpbBJ/xQQQQMDvASeyvBeOvwxUSAAAAABJRU5ErkJggg==}
image create photo btn_image_display_light -data $displaydata_light
image create photo btn_image_display_dark -data $displaydata_dark

set envdata_light {R0lGODlhIAAgAOYAAAAAAP////7+/vv7+/f39/b29vX19fT09PPz8/Ly8vHx8fDw8O/v7+7u7u3t7ezs7Ovr6+rq6unp6ejo6Ofn5+bm5uXl5eTk5OPj4+Li4uHh4eDg4N/f397e3tzc3NnZ2djY2NfX19bW1tHR0c3NzczMzMrKysbGxsXFxcTExMLCwsHBwb+/v76+vr29vby8vLu7u7m5ubi4uLW1tbOzs7Gxsa+vr6ysrKurq6mpqaioqKenp6ampqWlpaSkpKOjo6GhoaCgoJ+fn56enp2dnZycnJubm5qampmZmZaWlpWVlZSUlJOTk5KSkpGRkZCQkIyMjImJiYiIiIWFhYSEhIODg4KCgoGBgX9/f35+fnx8fHt7e3p6enl5eXV1dXR0dHBwcG5ubmxsbGtra2NjY2BgYFJSUlFRUVBQUE5OTkZGRjc3NzExMR4eHv///wAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAACH5BAEAAG4ALAAAAAAgACAAAAf/gAGCg4SFhoeIiYqLjI2HVVRKQ0aUlZaXmERBV4Q/FZ+goaKjolWERqSpqRYVKFynqrGhFARDT7CysgYnSDiwFsDBwsPEFxVMMzSER7mhHQcHHZ8TIVkOOb/E2hcLMjAJF8ZATxU8uLkXBgJpagcGFAtWKRU756sUBxUPWkA+WAQVZkwBqMMeqQkWPLwAg+TTEyoBnNz4VHBQkQoUMmrcuCDCGTMkKkBwcCxMFAgPKlQUxExVhwIryIiowIFDBwcP0EipsKFCD1zhMlAY4EADhggKtpjgGe4ChgkZNlgwVs/ip4wQMrjIgMBYkp0bNS5ogFHlOQsHPqzh8qnFGAkKhljFWhkAFagCJa44kaLCy1IMuehezMgqSI0KPtoIqWAhrGMKZge13ADhQ5YPHWWAkNAz10/JFTIYg7KEJuQHHJqVI3SxgoERXS4weExbY2RBdj+wyVHBmOpQdIkw/iKmA4LfoujyqCCFBXJSNQhpKcMkRo4d2LNr387dRhNH4MOLH0++/KJAADs=}
set envdata_dark {iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAYAAABzenr0AAAA5klEQVRYw+2XPQ6EIBCFn4bS0gNwBi5ltQeAcgs4A57OdhOz9VbrFkbjHygLyibLVEAzX957M8aMMdYhYuWIXAkgAZDppaqqS5pqrbcBAKBpmtldlmo8i5Z7N6eUulsQovH3Geh6atHymRqXAbxIsbLlMgBZKtwft9G3M1Q4lIFoCpiSG1WB0DbkNv+H8fspBUKrED0DxGcKQqzpfM9/mwKi5d5rOvoeIEczsAUxtWA4uypC9uQ3KWBq5AripYDt0y1LBbwB8eTnTcFhkA5A1r/X0HaAutCgRbiQLRsuK0t/Rgng7wE+P5BjBp1PgysAAAAASUVORK5CYII=}
image create photo btn_image_env_light -data $envdata_light
image create photo btn_image_env_dark -data $envdata_dark

set floatatomdata_light {R0lGODdhIAAgAKUqAAAAAAEBAQMDAwYGBgkJCQoKCgsLCw4ODhMTExoaGiUlJTk5OUFBQUhISFVVVVhYWF1dXV9fX2BgYHNzc5OTk56enq+vr7Gxsbe3t729vcDAwMTExMjIyMvLy9/f3+Dg4Obm5unp6evr6/Hx8fb29vf39/r6+vz8/P39/f7+/v///////////////////////////////////////////////////////////////////////////////////////ywAAAAAIAAgAAAGgUCVcEgsGo/IpHLJbDqf0Kh0Sq1ar9EAYMvtegsfJ+CpQWiaY7KZmVaflW0RqfkpLMemh0LQQd9VFgspFQYnbH8UGSMHAyKHcCkMHBAYESGPSWMTDhIoCZd/cCoXAyUeBCaYSGMoDw0AG36iQiAjYktaXrq7AVi+v8DBwsPExcZJQQA7}
set floatatomdata_dark {iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAIAAAD8GO2jAAAA1klEQVRIx+2VvQ6DIBDH6cniyxiiiY7OPLGzoyGXXORpJEE7kBiltpaKS/W/wAD3g/t85HnOzhSwk3UDLgDgbhFCHLc1jmPf99sAxhgRHWcIITw7kV1ERJ4z4sfAY5wSZCLKssyPwSxELIriw31EnPfvTgLABqDrummadh9orS3LMixNnaqq8h4YrQ6ClCQJIg7DwDnf/covgNnv3/z1b3qRU9M0y42U0iWcUmqZjm3bpmkKAMaYMICz+NrCvGSv6/qC88BaG2UkLKt9BdBa3zP5BlwX8ATLxkv7lcDYgQAAAABJRU5ErkJggg==}
image create photo btn_image_floatatom_light -data $floatatomdata_light
image create photo btn_image_floatatom_dark -data $floatatomdata_dark

set hradiodata_light {R0lGODdhIAAgAMIEAAAAAAEBAQnU+QvU+f///////////////ywAAAAAIAAgAAADVki63P4wykmrvTjrzbv/YEgEQGma5HkGExC5EPwSQm0PhOzoj2v/uZnE96sFYy1aUXDsJZdMHkPaIBabu+SgiKMqvAuweJJSmc0skXrNbrvf8Lh8fkkAADs=}
set hradiodata_dark {iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAIAAAD8GO2jAAAAZUlEQVRIx2M0MjJioCVgYqAxGLVg1IIhYAELnJWenk4tQ2fOnInFAgYGhgcPHuDXqaCgQIwa7D6AgHavNlw6K7dVjaaiUQtGLaCwqKAkNxFrAVouJ6YkIAgYR1sVoxaMWjAMLAAAx+sP3mjWCpAAAAAASUVORK5CYII=}
image create photo btn_image_hradio_light -data $hradiodata_light
image create photo btn_image_hradio_dark -data $hradiodata_dark

set hsliderdata_light {R0lGODdhIAAgAMIFAAAAAAEBAQJpfAnU+YDq+v///////////ywAAAAAIAAgAAADS1i63P4wykmrvTjrzbv/YCg6AWCeaJoGEkARcONGszTcckvdQ07vOEbtMYTwfJDi4yjUTZgLZfMZjEpKqpRguxp5v+CweEwum8+NBAA7}
set hsliderdata_dark {iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAYAAABzenr0AAAAcUlEQVRYw2M0MjL6zzCAgIlhgMGoA0YdMOqAAXcACzInPT2dLpbOnDkTuwMYGBgYHjx4QLbB7V5tDAwMDAyV26pwqlFQUBhNA6MOGHXAqANGHTCIKyNsZTVJlcy1WTBDiNbDONosH3XAqANGHTDQDgAAtFsPGAk8akMAAAAASUVORK5CYII=}
image create photo btn_image_hslider_light -data $hsliderdata_light
image create photo btn_image_hslider_dark -data $hsliderdata_dark

set keyboarddata_light {R0lGODlhIAAgANUAAAAAAP////39/fz8/Pv7+/r6+vn5+fj4+Pf39/T09PPz8/Ly8vHx8fDw8O/v7+7u7u3t7dXV1dLS0tHR0dDQ0M/Pz87Ozs3NzcjIyMTExMPDw8LCwsHBwcDAwL+/v21tbWxsbGhoaGJiYv///wAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAACH5BAEAACMALAAAAAAgACAAAAb/wIBwSCwaj8ikcslsHinQC8ZCkVIpV2t1ur1ShpfFgfPJFDIfzsFRcYzLZzhavbgMKQhBR5QJZEQdAgoVCnp8fod/gQhfQhQJBRwhGQMZIRwFDBUMkZOVnpaYCY0Bj52UoZmbp5+olwWjd5CSrpianLStn6Kkprmpt6yWtbC9s6Cvwb/IvLIFHZ4ayavQlNLWr7GOx8TKyMTapdy7qrjf5OG+zOXC6+kJBsu2q8vgxs8iGgIagOx7+vwA9ntnwAMIDQQ0gPBg4FbBgwkhKmT4LoGFDRMaTNhgIcEtixg1htzYMR0DBBAiQECpEsGDCg9Yrkw5syWDXgwONKDQwMDOVgYHXj7QydNn0Z8Hbt7JKTQoTKdDm0p9qtQR06dTo2LdOrRqqataw0Id29QrBbBkK0BIK9Ys2qxsx3q98JZr3LJ2hEiAwrev37+AJTgZTLiw4cOIlQQBADs=}
set keyboarddata_dark {iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAIAAAD8GO2jAAAAd0lEQVRIx2M0MjJioCVgYqAxGLWAIGCBs9LT06lo7syZM9EtYGBgCA8PhzASEhIgjAULFmByGRgYVq5ciUexp6fnaCSPWjBqwTAti5ABcl7H5JKkGLsPIOUMvLRB4xKjeDSSRy2gImCEN7xoVCczjrbsRi2gGAAA7uEhlSBZIHgAAAAASUVORK5CYII=}
image create photo btn_image_keyboard_light -data $keyboarddata_light
image create photo btn_image_keyboard_dark -data $keyboarddata_dark

set knobdata_light {R0lGODdhIAAgAOMQAEdHRyGy9X2pu6Kioi7B/rW1tWXP+8PDw4/V9dDQ0K7a7tzc3Mjm8ubm5u3t7f7+/iwAAAAAIAAgAAAE3fDJSau9OOvNOx5AOIxkaZ6DNTRs675wm1YrvBxFWRxLPFM1V26wSyRwowLsNwk2FiOeY0q9jXoyVQs6WFC/1WuWJluBz47GaAxk5bzoM1Sp1kIP8fihWyfv4Xlgd31tBQWBcYaETWp4iGd7ixIrAwlxCAh6lFqVaAgEmWiRTJONZwoEBnmjWoZgDAQEDXmKpA8rf1QNsQx5g7Y1A44OBgQKgX+St25dDp/HvkQswGWfoXFcbIwsCwIBqsh801otCgizgUvkMew+6+3w42Tx9MAhACj5KB78/f7/ACMAADs=}
set knobdata_dark {iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAIAAAD8GO2jAAABwklEQVRIx2M0MjJioCVgYqAxGLVg4C1gQeOnp6dTYtzMmTMJWMDAwPDgwQPyTFdQUCDsA1yAl5eXh4eHjY3t379/379/f//+/d+/f8kJIkwgLi7OyckJ5zIzM/Pw8PDw8DAwMDx58uTPnz8UWQDx9ZcvX968eYOijYVFRkZGRkbmw4cPHz58IDMVQUx/9OgRmukMDAx//vx58ODB9+/fBQQEIL4h2YI9ZunpWmm1SpH//v3Dpebly5e/f/8WEREhx4LTIvLmH1684uBt92p7L6CES9nTp08h8USaBfcUHBkYGJyOTarcVsXw5/cMq5Tt1vm4jPjx4wdyKiDKgv0yenB25a563ztHLvCLt3u1fecQwFRMTiS/4hFD5urc2la2s57hP8MEp7KT2sFoin/+/El6HPxB18P893fl9iq759eZ/qPHORMTE8n5wPXtvd2SWpji1ucXYwqSk0x1rm1iYGD4xCNOTGEgKCj4//9/0izg+PmJgYFhqm0+QdPZ2dkhGYLkfJB7cAIDI8M+80z8FkhKSv779+/Hjx8kl0U8X1953Tumf2MLfgsoKosImk4wEwxElYmrYiIbMI627EYtoBgAAJomoA94cimyAAAAAElFTkSuQmCC}
image create photo btn_image_knob_light -data $knobdata_light
image create photo btn_image_knob_dark -data $knobdata_dark

set listboxdata_light {R0lGODlhIAAgAOMIAAAAAAICAgQEBA8PDxQUFMDAwOzs7P///5mZmZmZmZmZmZmZmZmZmZmZmZmZmZmZmSH5BAEKAAgALAAAAAAgACAAAAR18MhJq7046827/2AojmRpVkWqrmy7XoUhz3Rtz4UV33yfU7uesPaTBIfI4iEIIPQAUABPyZTyrFYbdQZ9ygRToAx7DWS1YgP5ZhWccenom0tHT2Lm8VxdJ4rPe1ADewZbSEkoh0NKRi6OjyeRkpOUlZaXmCIRADs=}
set listboxdata_dark {iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAIAAAD8GO2jAAABBklEQVRIx+2VPZKEIBCFad2Qo4CBdu5xNDH3KHIbNTDCSA5iYqg9gbNb86NYbkEwNb5EqrD5oB90QxzHzKcC5lkX4AsAP+tHSulkuWEYtgGbc/+QEMIY4zFFxhghhF8PXhheTDbGRFG0C+j73h7f/8p2O4NgG4CIhxuc5zlJkqIoDreyAVBKEVHTNIeRXdedeAePStO0bVv78eu6ZoxlWXbuBIhIRNM02XMFAJxzznlVVecASinGmNZ6HMeyLC0eICIiAsDpFOV5/ghzVotWEZHW+m+8FxOGodYaACz/LMtyz+fak6WUTmrRe0Vy/5Jf6l3gdfUnD5y0hN1+4NCAq+lfgM8D3ADHHG9QGc3kXAAAAABJRU5ErkJggg==}
image create photo btn_image_listbox_light -data $listboxdata_light
image create photo btn_image_listbox_dark -data $listboxdata_dark

set menuarraydata_light {R0lGODlhIAAgAPIEAAAAAAEBAQoKCuDg4P///wAAAAAAAAAAACH5BAAAAAAALAAAAAAgACAAAAOGSLrc/jDKSau9OOsNA/hgKIbBBGynlGYr1BJBac5MrMjP66ylPQCDx88kO8VKAEGQMRDoGoBictDD1Z6MaOwTDDSjDs9MXFTCwthFymo7s9MK3fcmVsxVQjPhY1fC9xRTN4B4E15mSDSBSomFgSCEEX99VHGKGH+TOYYjnSNWHKGio6SlpgkAOw==}
set menuarraydata_dark {iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAIAAAD8GO2jAAABpklEQVRIx+2VsarCMBSGj9eAs4uTD9DBiDg7CCI0jq6lo4P4BD6CT9C5FJ0lQytuipNwQFsHJ3FxcVanFu4QCL291d5K5S79pzbk75ecNP8pNJtN+KS+4MPKAYki8olSWiwWM/loEASe50UBALDb7TIBNBqN/zsDx3EAwLbtTwEqlQohZDwel0ql185qtbparVIDLpdLr9fb7/ej0ei10zTNx+ORDqCqKiIOh0MA6Ha7k8mEUhprcxyn0+kYhpEOoOv6drv1fV+81ut1TdNemG3bTqzkD0C5XEZEQggAMMYYY4qixC6fMQYAvu/P5/MUgNPpdL1ehTksSulgMJBnG67M7XaLvWjxgMPhEJl6PB4BQNO0fr+PiIg4m80453KCZVmIKH5uofV6HR8VALBYLCKA6XQKAIqi/N6WEOdc8GTd7vf70xKdz+eI3/M8VVUjnlgZhiH24bpuONOSo0LXdcuyEqdxzl3XBYDlchkeL8iWSSmVEfi2NptNu92u1WoyN0m20dZqtfKW+T4gq375tCcHQfAsO8P6yzrCUUHCdyo/5Bzwlr4BLUql5/L4yjsAAAAASUVORK5CYII=}
image create photo btn_image_menuarray_light -data $menuarraydata_light
image create photo btn_image_menuarray_dark -data $menuarraydata_dark

set msgdata_light {R0lGODdhIAAgAMIGAAAAAAEBAQoKChMTE8DAwODg4P///////ywAAAAAIAAgAAADUmi63P4wykmrvTjrzTP4YCiOotCQAEYMxDkG19qenfykWyEUEL75DqAnIsQUF0dL0rCkLJuTZ0ea2/VqrFvHYHOJYLEsA0Umm7botHrNbrvf6AQAOw==}
set msgdata_dark {iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAIAAAD8GO2jAAAAaElEQVRIx+2WMQ7AIAwDScWLMvn/k5/VDaHWU2Wm2hNSxJ0QIqS6e5zMNQ4ngh8I5loBMHJJPgWy/CEA9u3TxZV08x286U6BpOcdRBCBV0BStkvnCaSj1lSx14zdVAuM/0FlLooggnEDmpIlS0m8Hk8AAAAASUVORK5CYII=}
image create photo btn_image_msg_light -data $msgdata_light
image create photo btn_image_msg_dark -data $msgdata_dark

set mycnvdata_light {R0lGODdhIAAgAIABAEDJ/P///ywAAAAAIAAgAAACOoyPqcvtD6OctNqLs36g+w92U0h+Y1meaKiupuSycPxGdA3dXkvzse8CroQoYmqmM5I2zKbzCY1KLwUAOw==}
set mycnvdata_dark {iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAIAAAD8GO2jAAAAPUlEQVRIx2M0MjJioCVgYqAxGLVg1IIhYAELfulzZ88SY4qRsfFoHIxaMGrBqAWD2QLG0VbFqAWjFgwDCwCefARtEYHV5wAAAABJRU5ErkJggg==}
image create photo btn_image_mycnv_light -data $mycnvdata_light
image create photo btn_image_mycnv_dark -data $mycnvdata_dark

set numberdata_light {R0lGODdhIAAgAKU5AAAAAAgICAkJCRISEhQUFBcXFx8fHyUlJTo6Ojs7O05OTlRUVFVVVV9fX2VlZWZmZm1tbXNzc3R0dHZ2dnp6eoWFhYaGhoiIiJOTk5SUlJycnJ2dnaOjo6+vr7KysrS0tLu7u7y8vL29vcDAwMPDw8TExMzMzM3NzdTU1Nvb29zc3N3d3eDg4Obm5ufn5+jo6Orq6uvr6+zs7O7u7vX19fb29vr6+vv7+/z8/P///////////////////////////ywAAAAAIAAgAAAGwMCccEgsGo/IpHLJbDqf0Kh0Sq1alYCsdssVsJ6AgQhHLpvJosHICUiPz/D0mgnAueEymFlOR4vLNxZZEzZlKwJ9fm8mATMvACFmAImKOBgUOBUAF5KUZCQDNw4cKQIbCoVkk0t1ZSoFHTcPHgYlH6hlq1hlMQcYZBoADZaYuYk0CRFlJwAoNQQgnaw4NwsMN4CCABCpqn0SCDRwLi1wukkAGQd6cO0450gABSnu7vBHXPn6WVf9/v8AAwocSFBKEAA7}
set numberdata_dark {iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAIAAAD8GO2jAAAA20lEQVRIx+2WwQnEIBBFJ4N1hDl6to8crCIliBVISrAK2/EYLCTsQZCsazZhM2Ev/tOYwH/yZwYclFLwpBAeVgd0wH2JUs3zzOjrva8BWeu63ncnonZEKaX9P+aIAGDbNgBAxFx8alkWY0w5xhhDCAAghJimSUp5AsgREVEzqBhjBQ4hWGtz7Zwr9TfAEcM5xzmmmbH/Yq1tXvD3PeBq+P82eRxHlp3Ao01hcW9P0XV3rXWZLq31JQARpZSOHKtBklKejtZbRIhY9vmRJnM19iQiXsDQ30Ud0AHwAgLzVEkCHq7qAAAAAElFTkSuQmCC}
image create photo btn_image_number_light -data $numberdata_light
image create photo btn_image_number_dark -data $numberdata_dark

set objdata_light {R0lGODlhIAAgAPEDAAAAAAEBAYCAgP///yH5BAAAAAAALAAAAAAgACAAAAJInI+py+0Po5y02ottAJyLI3QeCAQNkKGMerGKW8GIPH1r5tj4zkt6f6AZfrFTyogRDpQQJfNBnAETupCoKgJEp9yu9wsOiyMFADs=}
set objdata_dark {iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAIAAAD8GO2jAAAAd0lEQVRIx+2WMQ7AIAhFn00XZ1dO4f2P48odOriQYgebmjQtf0MSXuRjMNVaWamNxQrADwC7DUQEUFWglNIPp0KgtWZrJvsOROSUnpWv8HCL+oUuAT49K9urAcCnY0zD5DA5TP7gwgFyzre3jap6F1N8vALwfsABhbM35jrA68cAAAAASUVORK5CYII=}
image create photo btn_image_obj_light -data $objdata_light
image create photo btn_image_obj_dark -data $objdata_dark

set presetdata_light {R0lGODlhIAAgAMZIAAAAAFgAPiw+fD5MkZFMLJFMPgB0v4NYa3xikZFia2trkaRrYqp0WHSDa3SDumuLkYuDmL+DALV8a7p8YrV8dHSRi3yRxL+LdMSLfJmZmZGekcmvg5G6xMS1tcS6uqrEus7E0uTEnszMzOjJpNLS5NbW1snb7LXk9Lrk9NLf7PDfur/o9OPj4+Tk5PTkuuTk7Ojk5OXl5cns9NLs9Ojo6Nfw9PDs6PTs3+Tw9Nv09PTw19/09PTw3/Hx8ej09PLy8vT05PD08PPz8/T06PD09PT07PT08PT09P///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////yH5BAEKAH8ALAAAAAAgACAAAAf+gEiCg4SFhoeIiYqLjI2Oj4wZAJOUGZaXmJmWiBkinjQ9QjQlnqUiJTRCPTSlGZyeLUeyR0Ixpi0/s0e2Iq6HnTG5uj+mwrM/Lb2cNLIqASizrCLMRxMJK9G+hhk9skYU0LI9nt1GLwcysz3ahRlCsxfhtJ7vRyMzukLshBnUurLS/P2jsW9QhhLGdJE69U+WkBIFBXXC9Y9YqYRHfrBQ9qsUKFELTaFSJY3jNlMoU6qMiKSTypcpWbqcFmpUypGrWr0KVrFYxWQyS7zj4SHFrIUlZpE4BnGZrAUfEET7NMuALoKc6oEIImGWEHpHhhQQUMOrzG5HGFRQoI7cLAc1utY5lQWDyNRpDQHKFEFxmM9hG/eKiDEsWakY9WQZFkwT5M1UOT3J1ES58iZImDNr3sx5cyAAOw==}
set presetdata_dark {iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAYAAABzenr0AAACQ0lEQVRYw+1XMWvbQBT+lFbkJKIxxSAbvIUODsFDOTzGEApuli5aOmSKf4Dn/o7iTvGo2QmUgAoeZESmtp68GYKmgBcVSUak6nRnneTGsizwUD840Hvfnd537+59SFKz2YyxQzvAjm1PYOcEXq8Kdrvd0hP1+/38BABgNpsBAAghMAwDuq7DdV2YpokwDDPzo0MFw4srTI9rOHl6xOX9DeRFAACo1+ubVYBZu90GpZT71WoVvV4PjuPAsiwe//XuA+4aLe5P39Qw/fQZnckYpw+3xe4AIYQnPzqjfAAApRSEEL7zZHK8b/DHu0YL0aFSjIBhGIL/+4ezEh9eXK1MzkzANyGg6/qLCxk+Pa4tg98mOHglvlLANyHguu6LCxl+8vSYQiTBy+I5CZimKfjs/NP45f2NEP/z/Cz4aTw3gTAM4TgOP382AMBxHN6K8iJAZzIWjoFZZzLmrVioDS3Lgm3ba3Xg9OEWb39a/9SBwgRYJQaDwdoXyYsAH4dfypHitB7kUcI4juF5HqIogizL0DQNkiRtRyCvEgZBAN/3l7IcRZjP51BVFYqiFCOQVsKkIFFKYds2wjBEHMc8+dfvy8t4fd6C7/tcMbdSwmQHpHHP84Sk1+ctYV4S30oJ0zrA8CiKhHiyCqvwQkp4dEYzVWC4LMvCunQF0vhWSpisAsM1TRN2n65AEt/oEjIlpJRmzj+phJIkQVVV+L6f2b2qqmtbsRQlVBQFhJBydSD5GTUajfhzpVIp9VtR2v8Z7Qn89wT+Aoj9IEQWdAArAAAAAElFTkSuQmCC}
image create photo btn_image_preset_light -data $presetdata_light
image create photo btn_image_preset_dark -data $presetdata_dark

set scopedata_light {R0lGODlhIAAgAPcAAAAAAP///5mZmZiYmPLz9e/w8eXm5+Pk5c7x/+Dl59/k5nnb/4Tf/4bf/4HV9Kbn/6rp/7Xr/7vs/8rx/9/2/87h6On5//L6/XrZ+YHf/4LZ9oXa9ond943i/pHl/5Hj/5Di/ZLk/43Y8Zfm/5fl/47X8Jrm/5nl/pbe953m/57m/5je9qDo/6Dn/6Pp/6bp/6Xp/pvY7arq/6no/Z3X663r/7Ds/7Hr/rTt/7Tr/bPq+7nt/7vv/7Xm9r/v/8Xx/8ry/7ze6c/z/9X1/83p8tn2/933/9r0/N31/dLo7+L4/9bp7+b5/+r6/+77//f9/+rw8t7h4pPn/5Xn/6Hq/6Lq/6Tq/6Di9qfr/6jr/6fp/Krh8b3w/7vu/cDx/7vq96/b6MPx/8Ty/8by/8Hm8cXg6M3o8O37//L8/6Ds/6bu/6fr/abn+Lbw/7jx/8by/df3/9z4/9j0+9z1+/D8//D4+p7t/r3x/NH3/9v5//H7/fn+/+X7/+j8/836/635/4OXmY2YmfH+//L+/535/6H6/6P5/6f5/7f6/7v7/7z7/9H8/4SXmLX9/9b+/9z+/97+/+f+/8L+/4eYmI+YmJOZmff///r///z//5aYmPv8/O3u7piZmYeamYmamYqZmI+amZCamY2bmZSbmZqbmf//+////P39/P/9++Xj4f79/P328v728p6amPfz8fDu7ejm5fnw7Pbx7/Ll4O7m4/Xl4PXo5PDk4Pvw7aGamPzy7/z08v/7+vLu7ejk4/n085+ZmJ2ZmfHu7ufk5JmXl42Li/37+/Xz8/Lw8Ojm5v/+/pqZmfr6+vf39/X19fPz8/Dw8O7u7u3t7ebm5uTk5JeXl4uLi////wAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAACH5BAEAANcALAAAAAAgACAAAAj/AAMIHEiwoMGDCBMqXMiw4UFrAjjlGgWqkihSywRo3MiRUzBSnSqFElCM4ICBfxRhMhSp4aJClhI5CmZS4JgFFAI8YLFnIZ0GEQIgMPSpZoAWKgQ2yaBkoZAFcAQ2YlTzjAkgA62EWeiFykBIlGoaAYHE1ClTOW6cWsuWrbILJ7qYVabs5EBiq3qgiCUsWi8yK15FG0w42iZcSzQkwbUp2i9iBIHp6TCjFDNNxi6AkINKk+fPxnjpOFHqsyq7AnMx8TCE4J4RNhA+SSGjIOoAueK8MCjGBUI0J3bYJtgqDw6DTrAMOtikSp/hA0nhyYGKmXVmvOrMmMPrunVeR7RY+75+6nYgO2BmHaDG3leUEkFurWc/LFUMGrXmU5NWjSCgQwS48syAzxzzDBtXsELgM79AwcEXuyzYTH8DTSIJQnBMYRAfUghi0G2UPIIQEyEUUdAEEBx0WyaXyJYFFwXxUIOKJilzymefnYLKGzCYoooma61xx4851kVQNdBMo+SS0xhgSxkOKECLAbAkIEIFyTA5DX9HNgPNl2B+qQsRGJihCzSybLFBAciECY0zFAp0m0GYqNHGQGm4gdBtcxr0AwkDpWDBnkYl1AQDPgQgwQdoEDpQnwYh8kcAhPiR0G0QcaSpR6OIwoknpASj6UbWOGTqqaimquqqCwUEADs=}
set scopedata_dark {iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAIAAAD8GO2jAAABXklEQVRIx2M0MjJioCVgYqAxGLVg4C1ggbPS09OpaO7MmTPRLWBgYHjw4AEePe1ujZ7PrjIwMMi+vCb8+gouZRd0wgK+nCIziAyurDK4smqWaRQeNdultHHGQbtbIx6dEOczMDCknV6GN9RZcUcyqhya3QZXVkHYj8W18JiPZj0THjlcdhtcWYXLrxd0wtCiB8UC4ddXLuiEYQ+fRxeI8StaBGCJZEwVxIQDHouZCKpgYGB4K6oDjwACfv3zm4AF6EHBwMDAwIA1Xcq+vIYnpVGhqMC0FdOjWCwwuLLqragOUSH+9w8xHmUi6DTMlAcBBQf6qFOa4kpXnD8/oMUzVo8yEU6CuLM3st24PIrFArQkSGT2xuVR7EG0Xc4Av7uw2I3DozjiAJZf8Gds4ddXIEnugk5Y5bYqEiyo3FV/QSfsgk4YA8N/YnLDdimc5SsjvOFFoyqTcbRlN2oBxQAAIniAJ679BrgAAAAASUVORK5CYII=}
image create photo btn_image_scope_light -data $scopedata_light
image create photo btn_image_scope_dark -data $scopedata_dark

set slider2ddata_light {R0lGODlhIAAgAOYAAAAAAP///72+v7Gys8DGydH0/+f5/+b4/tHz/dDy/NHy/NT0/dPz/N/4/973/t32/b7U2uf5/s/z/NH0/dDz/NHz/NX2/tP0/NT0/Nn1/N73/cDU2eb5/sfW2svX2tL3/933/ef7/7vFx7zFx8LHyOj8/9P6/+T+/9r//+b//+3///H///n//7GysqeoqNDQz93Y1szHxcO9u8zJyMG+vc3JyMK+vdrW1czHxsG8u8TBwc7Nzb28vO/v7+Li4tbW1tTU1M7Ozs3NzcvLy8DAwLu7u4mJif///wAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAACH5BAEAAEcALAAAAAAgACAAAAfYgAGCg4SFhoeIiYqLjI2HRpCRkpOUlYRGhiYljpedhRQRnIOYo4YVHKKCpKqGFKipqwGxghWhsJ6ECa+isbMBtamyuIOnwb2GCAfGw7S2vMy/u5zHhcC3pYUTztPQCsrXrINBOiMkPD7P2IJEAjEzLT/pqioXFhIiOENCORsoGBcMMPwDKJDBhQUXUvRi8aBBBgI1dryw0eGEBhAXQTzQ4ECDR48PVlATVGSADBougMgTNsgHDAgebvRY6auAgWXqBunCGY7QhxA8ZVUaSjRSsKNIkypdeigQADs=}
set slider2ddata_dark {iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAIAAAD8GO2jAAAAk0lEQVRIx2M0MjJioCVgYqAxGLVg4C1ggbPS09OpaO7MmTPRLWBgYHjw4AFBne1ebZXbqvCrUVBQGI3kUQuGrwXu7u4H/h50d3cnJydDMhEBC/4e3Llzp7u7O36VM6/Nwm4BwSzK4O4OcT4BlWTn5J07dzow2+/cuXM0FY1aMDBVJlpNhC8TEaEMDhhHW3ajFlAMAMzIJM+oGYZiAAAAAElFTkSuQmCC}
image create photo btn_image_slider2d_light -data $slider2ddata_light
image create photo btn_image_slider2d_dark -data $slider2ddata_dark

set slidersdata_light {R0lGODlhIAAgANUAAAAAAP////39/fz8/Pr6+vn5+fb29vX19fT09PPz8/Ly8vDw8O/v7+7u7uzs7Ofn5+bm5uTk5ODg4N7e3tzc3Nvb29ra2tPT09HR0dDQ0M7OzsvLy8rKysfHx8XFxcTExMHBwcDAwL6+vr29vbu7u7q6urW1tS4uLi0tLSoqKikpKSgoKCcnJyYmJiUlJSQkJCMjIyAgIBsbGxkZGRYWFhUVFREREQ4ODg0NDQwMDAsLCwoKCgkJCf///wAAAAAAACH5BAEAAD0ALAAAAAAgACAAAAb/wIBwSCwaj8ikUomqyZ5QmYtFrVqv1dhJmFN4v+Cw2JswGBAQmrClWLqJK+Gq/UYOMh3JQiVk0esCAwJDAxshFHt9f0aBg0J3HRkDjxofE4kBfnZ4ko+GG5MBA5WXfJmLRIUhoJQfGqGjlpiaR7GvrbeipLOohJW5trC7prRGwbjCssS9yM26yopCjb6uycDD0ZCdz68Cgb/JpdmfyRgEGRkEGNWt4pkH3OUEGxvq1d6xl3GZDN/3/uY0aLBnDp09fUJegJBA8Fy6dQEHQpxXb527FyQoNKTXUGBHidACvBAx4aPJiR4tYhpZEiXIiCdDsowJ0+XBlSRpEkj5oWZINhc5bQrtuRPkxaBEefr0eSmFEB1IIxYYynTBDCFNokCBweKF1xZfvb4AO9arCy110qpdy3ZtEAA7}
set slidersdata_dark {iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAIAAAD8GO2jAAAAo0lEQVRIx2M0MjJioCVgYqAxGLVg4C1ggbPS09OpaO7MmTPRLWBgYHjw4AFVTFdQUBiWcUAq4OHhgTC+fPlCEwscHR1//vzJwMCwa9cumlhApFdYyDaOSK+wkB0y5EcyshvxRyCZFiC7EX8EDvF8QEz8U9kCeNjeunVruJZFWAORmhZYWVkNTCqixGcoFiDHPmbdBAGXL18myQLG0ZbdqAUUAwAJYzKsHtfzCAAAAABJRU5ErkJggg==}
image create photo btn_image_sliders_light -data $slidersdata_light
image create photo btn_image_sliders_dark -data $slidersdata_dark

set spectroscopedata_light {R0lGODlhIAAgAPcAAAAAAP////39/YmJif/9//3+//T7/1XG/1bH/1jI/+v1+lfI/1nK/1rM/1rL/1vL/1zL/1zN/17N/2HO/nrW/7/l9uHq7l3O/17O/1/P/WHS/2HR/2LR/2LS/2TQ/2PQ/WXT/2fT/2bS/WjT/2fR/WbQ+2jS/WrU/mvV/2zV/2vR/G7U/3HX/3fX/3bV/HvZ/4Hb/4HX+ZLg/5Xg/6Dj/6Xj/Lbh8sLt/7zk9M7x//j9//b7/WbV/2nV/2vW/2rV/WzW/2/X/3LY/3TZ/3PY/Hbb/3XY/XfZ/3fX/HfX+3vb/3zb/3vZ/X3d/37c/oHe/4Hd/3/X+YXf/4LZ+YTb+4nf/4rg/4vf/4nd+4zh/4rd+orb+Yvc+ZDh/5Di/4/g/Y7e/JPj/5Tj/5Pg/Jnk/5vk/5Xb9Zzl/5/m/53j/KPm/57e96bn/6Xk/KDe9avp/63p/6rl+rPr/7Tr/7jr/bfp+7Tj9L3t/8Pv/8Xu/cLk8M/x/dLz/9Tz/9X0/9Py/dn1/9fz/eD3/9Po8OL3/+T4/9Xo7+j5//D7/+Lq7XDZ/HXd/37f/4Dg/33b+oLf/4Db+n/a+Inh/4ze+pLk/5jm/53n/6Hn/6Lp/6fp/6nq/7Dr/7Xs/7bt/7jt/7vu/7zu/7jk8s3z/9Dz/8Tm8df1/9Lw+tz2/9/3/+P4/+Dz+ef5/+X3/er6/+n4/e77//f9//X4+YHf+o/l/5Tm/5fn/5jo/53o/qHp/7Dj8rnl8cby/8rz/870/8/w+tv3/+37/3rm/5Tp/7fx/9P2/+D5//D8/3fm/4Ho/4Xq/5ft/67x/93q7fT9/+73+efs7ebr7H/q/4ft/4js/4nu/4vu/5jv/6Tx/6Xy/6nx/7r1//L9/5nx/670/7T2/8L3/9X6/4Ty/6T1/8L4/6b3/6n4/4j3/779/93+/+P+/6P+/6P9/6b9/6j9/8b+//f///r9/fj6+f//+P///f3x6fzy6/Pu7Pru6v36+fHu7f79/e7t7e3s7O3t7YqKiv///yH5BAEAAP8ALAAAAAAgACAAAAj/AAMIHEiwoMGDCBMqXMiw4cEBECNKnEixIsEBDhliHLgxY8KOATquguWxIMiOOV6VvMhSwLw9rFzNE0Czps2bNUOy7GeP1CAb+/oJFbov31Ch+Zg9C9pPJ8cALgPRuQVvJk198Hboszkvzx+rTgV2PAXHS4GCiEQJ1HE2wCZOLJ8KRJUpS9uBiEYVWMVHJSw1m+KKHSgIU5UAwHoNbCUKVh8/fi9hEhyWUKVHAQ5ZaiaQmKZXpfqo3IamFmWM/PYZitTBArQYuerd02PGTSg7iejpCoKF3j5+YXXii2XKCQMF8b6AUSXPV5o2dWo4k1eHwQt5+OIF32gAD5QGiACJ/zkiJ8CuM28+zQGUCg0HFMYEpjuJjjObIBgK0eii5E6AME+ggQcnp9wgxQYg/CKQOyD5w44gAZQxAgapXOKFEqAEIMkQLXiSSSdyKMFAB7wEYAA5J6mDCwFqpAACKpeE8YIXhFSxBBCNLEEBGk04sAEahNwhzUnmlFABFiJMoYUjk2AQBQ4mJHHCByFssAUkDkSwBhVGIHNSOIrIMgECTKhABBILjMHFARk4YMIKHfzgQgQjxEECBNOctE4Vi3TAQBVPYPAeGT0g4IADHDCySANLCNGDMD44EM1J7WTBQhEPyECLBgkIMQsPDSAAwgK2WOIAL1dcQEkLDhxz0jlWQE6wCxlKsCFFAy/AgAEEDTRxgCZqeIAIDBLMIEkQ3Jz0zjjlbFNMN9kos8w12gRDjTfgKDPMN8no8I014lSDzXYVlWtuRCulq+667LZ7UEAAOw==}
set spectroscopedata_dark {iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAIAAAD8GO2jAAABsUlEQVRIx+1Wv0sCYRh+TE9uOCFEHOWW2lz0DzgIxIigaBAc2gyXzlyzckhTCoLQyWwIGi4MaqixIZdoaQqiIeKChkvwwrpOkdNruDjMX0hpQfhO3/e8P57vfT++9/0MLpcLg5QRDFiGBH9PYNJXwWCwj3EzmUwzAQCe5yVJoigKgCRJANKzW8vn65pWU2m4bqMjAERRdDgcAGia7rlEZvJv7iA5ERnwJZPU8B38e4JqdbAEtpen3y5R2pf6DoHt+QFA1h3obsYxYTYXApCcSvQ5g8aIBEH0lIHuk/alvCfRpg5RM5k5Jjx/c5h1B7SycEwYQHpmc9+fak3C1Olci0dLrficcHfgWX2jRsnqOwA2F6Jpmud5TVsqlZxOp6IosFo7EnBMOHYRl2U5OR1ncyERAOAt3ic9URBk7HL72D6G8iugLlzvSYCiKLpvrVYzGo2yLHfMIOsO+PM7ZYvFbrezpys6Pi5cRR7zicmNcklkz9YAgPpsdiT5pZ/X6/WOEw2AdihVVQuFQiNeqVQAwABVVZv8BUFo3BaLxW4EjZOozRS83UVXg7ZiGP7shgQ/lg/FipzesKvDVAAAAABJRU5ErkJggg==}
image create photo btn_image_spectroscope_light -data $spectroscopedata_light
image create photo btn_image_spectroscope_dark -data $spectroscopedata_dark

set symbolatomdata_light {R0lGODdhIAAgAKU8AAAAAAEBAQMDAwYGBgoKCgsLCw4ODhERERMTExkZGRoaGi4uLjc3Nz09PT4+PkRERExMTGBgYGhoaHR0dHx8fH9/f4GBgYqKipSUlJycnKSkpKioqKmpqaurq62trbCwsLS0tLe3t76+vsDAwMPDw8XFxcjIyMrKytra2tvb29zc3ODg4OHh4ebm5ujo6Onp6evr6+/v7/Hx8fPz8/T09PX19fb29vf39/n5+fz8/P39/f7+/v///////////////ywAAAAAIAAgAAAGpkCecEgsGo/IpHLJbDqf0Kh0Sq1ar9IAYMvtegkrJ+A5QoyaY7KZmVafle3nirCMP+1GPLrO01UWAichFkIeGRgzRXpEYzIDLkI2Byw5Ci8gBhQwQ4ucPDsdAAwpPBwSJRFDGgAXQp2uPDQ5PBsFOTgJAio8LRMMIjqwcDwoABAGGkIfDzs1DSQ7jHxCMTdCOg4mSa88Wl7f4AFY4+Tl5ufo6errSEEAOw==}
set symbolatomdata_dark {iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAIAAAD8GO2jAAABHklEQVRIx+2VvW2GMBCGTQSMQucGxI9oXbEDBVNcQ0vlAViDCa6nABoX7MAIWIIUVixk8X2CBIokvA3o7uwHXv+c5fs+uVMf5GY9gH8AsNWDUnrJdEKIfcBu7huilBrzXGyREMIw4/o1MBi3LPKWYQL6LxVFYdt23/c61batKjDqD+0iXQ0AiKgjUkrGGCIyxhzH0WVBEBBCuq776Tkoy5JzHscx57yqKiNrWdbRc6AEAJzzcRzzPFcRRJRS1nW9LEvTNMed2f8DRMyyzPO87fgkSQghYRjqiPKHEJKm6WmLpmmKoggAjnzjuq7nLNIahuH9MAA4dxeddVbtNNd1zwG0uYa28e37PM+vhvy9fnBhS9gHXNIMnqb/AH4r4BOTp3W7HPrD5wAAAABJRU5ErkJggg==}
image create photo btn_image_symbolatom_light -data $symbolatomdata_light
image create photo btn_image_symbolatom_dark -data $symbolatomdata_dark

set textdata_light {R0lGODdhIAAgAKU0AAAAAAMDAwgICAoKCgsLCxUVFRcXFxsbGyoqKisrK0BAQEFBQUNDQ0REREhISE9PT1NTU1lZWVpaWmBgYGRkZGZmZm5ubnBwcHl5eYiIiJ2dnaCgoKurq7W1tby8vMHBwcfHx8jIyMvLy87OztLS0tra2t/f3+Li4uXl5ebm5ujo6Onp6e7u7vf39/j4+Pr6+vv7+/z8/P39/f7+/v///////////////////////////////////////////////ywAAAAAIAAgAAAGj0CacEgsGo/IpHLJbDqf0Kh0Sq1ar9isdst9ziKAMCBEAQwYscqGxpmAxSGl66XoxGSQ1NAkUBlQc3UxM0szCx80MxAsRBgAGUKGiEwzCohfDg0aQiUAJ5GWTZWXECcxMIkXABagk4WhiitDIgQtBSOJoZQLIIkSAQkPLwckNB4Ig7xdy8zNzs/Q0dLT1FBBADs=}
set textdata_dark {iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAIAAAD8GO2jAAAAo0lEQVRIx+2VOw7DIAyGSUEcCA8WI6dHHnwfBpA6oCIrtAmqki71P9lg+PxAYgshmDv1MDdLAQpQwA8Abuczs3QBQK4MFwCMMTnnUkpKaT71EdD3mFkGSRsRiajb3vsY4wggIkQ8qeBUtda50Itn0PNtrcnKVmfwViNfeaO19pshH6Q883ajuqZFzrmOXOnPKoBf6k/l4FnP2vTLVIACFPAPgCeeRT6/Pq7IngAAAABJRU5ErkJggg==}
image create photo btn_image_text_light -data $textdata_light
image create photo btn_image_text_dark -data $textdata_dark

set toggledata_light {R0lGODdhIAAgAOMIAAAAAAEBARXV9xjV+Ijp+L3y+d72+vz8/P///////////////////////////////ywAAAAAIAAgAAAEdxDJSau9OOvNu/9gKI5kaW5Aqq5sAAJHLM+xBLwHYdByMSC3D8xX4BEPQQ/scOwNikhcrNmMCmfEqlVJKwgEUFmys+x9w7Exp8x8atUoJ7QK1yy1zXpm+DT29RgwOjxTP4AXbIQHNiABLI8qLieTlJWWl5iZmiQRADs=}
set toggledata_dark {iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAIAAAD8GO2jAAAAwUlEQVRIx2M0MjJioCVgYqAxGLVg1ALKAQsuifT0dJIMmjlzJmkWMDAwPHjwgEjTFRQUyAyidq+2H+x8uGR/sPO1e7VRFAeFezv6nSuw2vGDna/fuaJwbwdFFnD8/ITVDrjpHD8/UZqKMO0g3nRikymyHSSZTiAVYbUDEjFEmj6YcjI8ZPCkK/ItQA53XOmKfAswY5UkO5hINZ1UOwhYgCdFIqcr8pNp5bYq/GkXvwICFuApI4kHjKPtolELRi1gAADKtXyYrEvWrQAAAABJRU5ErkJggg==}
image create photo btn_image_toggle_light -data $toggledata_light
image create photo btn_image_toggle_dark -data $toggledata_dark

set vradiodata_light {R0lGODdhIAAgAMIFAAAAAAEBAQTX/gPY//z8/P///////////ywAAAAAIAAgAAADY1i63P4wykmrvTjrXYH/YMAVAGGeJzCWQ+sKhMqxrhuvRG3LG63fs5xuAOwJfzxNSaCDJTMlFOqJAVk9OGkqqy0quybqJdoVW8hacwfshbLVFLQUPpFPR4HrRzTq+/+AgYIFCQA7}
set vradiodata_dark {iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAIAAAD8GO2jAAAAZElEQVRIx2M0MjJioCVgYqAxoLkFLHjk0tPTiTRl5syZ5FjAwMDw4MEDgqYrKCgM1iCCg3avNqzilduqRkAqGrVghBcVJKX3weoD/OUMFSwYAoXdqAWjFoxaMBzKIsbRxi8hAADimA/il7QtJQAAAABJRU5ErkJggg==}
image create photo btn_image_vradio_light -data $vradiodata_light
image create photo btn_image_vradio_dark -data $vradiodata_dark

set vsliderdata_light {R0lGODdhIAAgAMIFAAAAAAEBAQJpfAnU+YDq+v///////////ywAAAAAIAAgAAADUFi63P4wykmrvTjrEoD/QLAxwFOOytmoI7u4GlzImExfNpqa+szrOeAPFSQSBsikoAc4JpFLoPMZJQ5b100R67hZttqspgPyiHrotHrNziQAADs=}
set vsliderdata_dark {iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAIAAAD8GO2jAAAAaElEQVRIx2M0MjJioCVgYqAxoLkFLHjk0tPTiTRl5syZ5FjAwMDw4MEDgqYrKCgM6zgYtYCiZMrAwNDu1UY4jV6bRb4FlduqGIhIp6ORPGrBqAWjFoxaMGoBZTUa/jYPMYBxtPFLCAAAQe0NHiKqzm0AAAAASUVORK5CYII=}
image create photo btn_image_vslider_light -data $vsliderdata_light
image create photo btn_image_vslider_dark -data $vsliderdata_dark

::pdwindow::debug "\[ceammc\]: ceammc_buttonbar-plugin loaded\n"
