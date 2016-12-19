# this plugin creates a buttonbar on a patch window when that patch
# window is in Edit Mode

# this GUI plugin removes the menubars from any patch window that is
# not in Edit Mode.  Also, if a patch is switched to Run Mode, the
# menubar will be removed.

package require base64
package require tooltip

proc make_pd_button {mytoplevel name tooltip} {
    button $mytoplevel.buttonbar.$name -image buttonimage$name \
        -relief solid -borderwidth 0 -highlightthickness 0 \
        -highlightcolor grey -highlightbackground grey -padx 1 -pady 1 \
        -command "menu_send_float \$::focused_window $name 0"
    pack $mytoplevel.buttonbar.$name -side left -padx 0 -pady 0
    tooltip::tooltip $mytoplevel.buttonbar.$name $tooltip
}

proc make_iemgui_button {mytoplevel name tooltip} {
    button $mytoplevel.buttonbar.$name -image buttonimage$name \
        -relief solid -borderwidth 1 -highlightthickness 0 \
        -highlightcolor grey -highlightbackground grey -padx 1 -pady 1 \
        -command "menu_send \$::focused_window $name"
    pack $mytoplevel.buttonbar.$name -side left -padx 0 -pady 0
    tooltip::tooltip $mytoplevel.buttonbar.$name $tooltip
}

proc showhide_buttonbar {mytoplevel} {
    if { ! [winfo exists $mytoplevel.buttonbar]} {
        frame $mytoplevel.buttonbar -cursor arrow -background grey \
            -pady 0 -padx 0
        make_pd_button $mytoplevel obj [_ "Object"]
        make_pd_button $mytoplevel msg "Message"
        make_pd_button $mytoplevel floatatom "Number box"
        make_pd_button $mytoplevel symbolatom "Symbol box"
        make_pd_button $mytoplevel text "Comment"
        make_iemgui_button $mytoplevel bng "Button"
        make_iemgui_button $mytoplevel toggle "Toggle"
        make_iemgui_button $mytoplevel numbox "Number box2"
        make_iemgui_button $mytoplevel hslider "Horizontal slider"
        make_iemgui_button $mytoplevel vslider "Vertial slider"
        make_iemgui_button $mytoplevel hradio  "Horizontal radio"
        make_iemgui_button $mytoplevel vradio  "Vertical radio"
        make_iemgui_button $mytoplevel mycnv   "Canvas"
        make_iemgui_button $mytoplevel menuarray "Array"
        make_iemgui_button $mytoplevel vumeter "VU meter"
    }
    if {$::editmode($mytoplevel)} {
        set tkcanvas [tkcanvas_name $mytoplevel]
        pack forget $tkcanvas
        pack $mytoplevel.buttonbar -side top -fill x
        pack $tkcanvas -side top -expand 1 -fill both
    } else {
        pack forget $mytoplevel.buttonbar
    }
}

bind PatchWindow <FocusIn> {+showhide_buttonbar %W}
bind PatchWindow <<EditMode>> {+showhide_buttonbar %W}

set bngdata {R0lGODdhIAAgAIQUAAAAAAEBAQEDAgUKCwsTExwnKB0oKQdhcQdlc3NvcQaqyAasygax0AjJ7Kmp
qQnU+cC+vsDAwN7e3vz8/P///////////////////////////////////////////////ywAAAAA
IAAgAAAFpyAljmRpnmiqrmzrvnAsz3Rtr0Cu73wAA5OgJFEYDAoJSXAC+AUdBMSi0VggBJFg8wWM
ABSPsJgBcDB/EgFDzH4oCJKtC5A4tNuIhLwFMCzubAsFeywAAw2AYg0DhDiHiWGLjSp9f5CCkyl0
dpB5mSgAEgRggAwCcU4OAGttCgBZnydAExACB1NVBwRmZ1xLQwZGBklLsSYBPMk6PjfNzs/Q0dLT
1DQhADs=}
image create photo buttonimagebng -data $bngdata

set floatatomdata {R0lGODdhIAAgAKUqAAAAAAEBAQMDAwYGBgkJCQoKCgsLCw4ODhMTExoaGiUlJTk5OUFBQUhISFVV
VVhYWF1dXV9fX2BgYHNzc5OTk56enq+vr7Gxsbe3t729vcDAwMTExMjIyMvLy9/f3+Dg4Obm5unp
6evr6/Hx8fb29vf39/r6+vz8/P39/f7+/v//////////////////////////////////////////
/////////////////////////////////////////////ywAAAAAIAAgAAAGgUCVcEgsGo/IpHLJ
bDqf0Kh0Sq1ar9EAYMvtegsfJ+CpQWiaY7KZmVaflW0RqfkpLMemh0LQQd9VFgspFQYnbH8UGSMH
AyKHcCkMHBAYESGPSWMTDhIoCZd/cCoXAyUeBCaYSGMoDw0AG36iQiAjYktaXrq7AVi+v8DBwsPE
xcZJQQA7}
image create photo buttonimagefloatatom -data $floatatomdata

set hradiodata {R0lGODdhIAAgAMIEAAAAAAEBAQnU+QvU+f///////////////ywAAAAAIAAgAAADVki63P4wykmr
vTjrzbv/YEgEQGma5HkGExC5EPwSQm0PhOzoj2v/uZnE96sFYy1aUXDsJZdMHkPaIBabu+SgiKMq
vAuweJJSmc0skXrNbrvf8Lh8fkkAADs=}
image create photo buttonimagehradio -data $hradiodata

set hsliderdata {R0lGODdhIAAgAMIFAAAAAAEBAQJpfAnU+YDq+v///////////ywAAAAAIAAgAAADS1i63P4wykmr
vTjrzbv/YCg6AWCeaJoGEkARcONGszTcckvdQ07vOEbtMYTwfJDi4yjUTZgLZfMZjEpKqpRguxp5
v+CweEwum8+NBAA7}
image create photo buttonimagehslider -data $hsliderdata

set menuarraydata {R0lGODlhIAAgAPIEAAAAAAEBAQoKCuDg4P///wAAAAAAAAAAACH5BAAAAAAALAAAAAAgACAAAAOG
SLrc/jDKSau9OOsNA/hgKIbBBGynlGYr1BJBac5MrMjP66ylPQCDx88kO8VKAEGQMRDoGoBictDD
1Z6MaOwTDDSjDs9MXFTCwthFymo7s9MK3fcmVsxVQjPhY1fC9xRTN4B4E15mSDSBSomFgSCEEX99
VHGKGH+TOYYjnSNWHKGio6SlpgkAOw==}
image create photo buttonimagemenuarray -data $menuarraydata

set msgdata {R0lGODdhIAAgAMIGAAAAAAEBAQoKChMTE8DAwODg4P///////ywAAAAAIAAgAAADUmi63P4wykmr
vTjrzTP4YCiOotCQAEYMxDkG19qenfykWyEUEL75DqAnIsQUF0dL0rCkLJuTZ0ea2/VqrFvHYHOJ
YLEsA0Umm7botHrNbrvf6AQAOw==}
image create photo buttonimagemsg -data $msgdata

set mycnvdata {R0lGODdhIAAgAIABAEDJ/P///ywAAAAAIAAgAAACOoyPqcvtD6OctNqLs36g+w92U0h+Y1meaKiu
puSycPxGdA3dXkvzse8CroQoYmqmM5I2zKbzCY1KLwUAOw==}
image create photo buttonimagemycnv -data $mycnvdata

set numboxdata {R0lGODdhIAAgAKU5AAAAAAgICAkJCRISEhQUFBcXFx8fHyUlJTo6Ojs7O05OTlRUVFVVVV9fX2Vl
ZWZmZm1tbXNzc3R0dHZ2dnp6eoWFhYaGhoiIiJOTk5SUlJycnJ2dnaOjo6+vr7KysrS0tLu7u7y8
vL29vcDAwMPDw8TExMzMzM3NzdTU1Nvb29zc3N3d3eDg4Obm5ufn5+jo6Orq6uvr6+zs7O7u7vX1
9fb29vr6+vv7+/z8/P///////////////////////////ywAAAAAIAAgAAAGwMCccEgsGo/IpHLJ
bDqf0Kh0Sq1alYCsdssVsJ6AgQhHLpvJosHICUiPz/D0mgnAueEymFlOR4vLNxZZEzZlKwJ9fm8m
ATMvACFmAImKOBgUOBUAF5KUZCQDNw4cKQIbCoVkk0t1ZSoFHTcPHgYlH6hlq1hlMQcYZBoADZaY
uYk0CRFlJwAoNQQgnaw4NwsMN4CCABCpqn0SCDRwLi1wukkAGQd6cO0450gABSnu7vBHXPn6WVf9
/v8AAwocSFBKEAA7}
image create photo buttonimagenumbox -data $numboxdata

set objdata {R0lGODlhIAAgAPEDAAAAAAEBAYCAgP///yH5BAAAAAAALAAAAAAgACAAAAJInI+py+0Po5y02ott
AJyLI3QeCAQNkKGMerGKW8GIPH1r5tj4zkt6f6AZfrFTyogRDpQQJfNBnAETupCoKgJEp9yu9wsO
iyMFADs=}
image create photo buttonimageobj -data $objdata

set symbolatomdata {R0lGODdhIAAgAKU8AAAAAAEBAQMDAwYGBgoKCgsLCw4ODhERERMTExkZGRoaGi4uLjc3Nz09PT4+
PkRERExMTGBgYGhoaHR0dHx8fH9/f4GBgYqKipSUlJycnKSkpKioqKmpqaurq62trbCwsLS0tLe3
t76+vsDAwMPDw8XFxcjIyMrKytra2tvb29zc3ODg4OHh4ebm5ujo6Onp6evr6+/v7/Hx8fPz8/T0
9PX19fb29vf39/n5+fz8/P39/f7+/v///////////////ywAAAAAIAAgAAAGpkCecEgsGo/IpHLJ
bDqf0Kh0Sq1ar9IAYMvtegkrJ+A5QoyaY7KZmVafle3nirCMP+1GPLrO01UWAichFkIeGRgzRXpE
YzIDLkI2Byw5Ci8gBhQwQ4ucPDsdAAwpPBwSJRFDGgAXQp2uPDQ5PBsFOTgJAio8LRMMIjqwcDwo
ABAGGkIfDzs1DSQ7jHxCMTdCOg4mSa88Wl7f4AFY4+Tl5ufo6errSEEAOw==}
image create photo buttonimagesymbolatom -data $symbolatomdata

set textdata {R0lGODdhIAAgAKU0AAAAAAMDAwgICAoKCgsLCxUVFRcXFxsbGyoqKisrK0BAQEFBQUNDQ0REREhI
SE9PT1NTU1lZWVpaWmBgYGRkZGZmZm5ubnBwcHl5eYiIiJ2dnaCgoKurq7W1tby8vMHBwcfHx8jI
yMvLy87OztLS0tra2t/f3+Li4uXl5ebm5ujo6Onp6e7u7vf39/j4+Pr6+vv7+/z8/P39/f7+/v//
/////////////////////////////////////////////ywAAAAAIAAgAAAGj0CacEgsGo/IpHLJ
bDqf0Kh0Sq1ar9isdst9ziKAMCBEAQwYscqGxpmAxSGl66XoxGSQ1NAkUBlQc3UxM0szCx80MxAs
RBgAGUKGiEwzCohfDg0aQiUAJ5GWTZWXECcxMIkXABagk4WhiitDIgQtBSOJoZQLIIkSAQkPLwck
NB4Ig7xdy8zNzs/Q0dLT1FBBADs=}
image create photo buttonimagetext -data $textdata

set toggledata {R0lGODdhIAAgAOMIAAAAAAEBARXV9xjV+Ijp+L3y+d72+vz8/P//////////////////////////
/////ywAAAAAIAAgAAAEdxDJSau9OOvNu/9gKI5kaW5Aqq5sAAJHLM+xBLwHYdByMSC3D8xX4BEP
QQ/scOwNikhcrNmMCmfEqlVJKwgEUFmys+x9w7Exp8x8atUoJ7QK1yy1zXpm+DT29RgwOjxTP4AX
bIQHNiABLI8qLieTlJWWl5iZmiQRADs=}
image create photo buttonimagetoggle -data $toggledata

set vradiodata {R0lGODdhIAAgAMIFAAAAAAEBAQTX/gPY//z8/P///////////ywAAAAAIAAgAAADY1i63P4wykmr
vTjrXYH/YMAVAGGeJzCWQ+sKhMqxrhuvRG3LG63fs5xuAOwJfzxNSaCDJTMlFOqJAVk9OGkqqy0q
uybqJdoVW8hacwfshbLVFLQUPpFPR4HrRzTq+/+AgYIFCQA7}
image create photo buttonimagevradio -data $vradiodata

set vsliderdata {R0lGODdhIAAgAMIFAAAAAAEBAQJpfAnU+YDq+v///////////ywAAAAAIAAgAAADUFi63P4wykmr
vTjrEoD/QLAxwFOOytmoI7u4GlzImExfNpqa+szrOeAPFSQSBsikoAc4JpFLoPMZJQ5b100R67hZ
ttqspgPyiHrotHrNziQAADs=}
image create photo buttonimagevslider -data $vsliderdata

set vumeterdata {R0lGODlhIAAgAPIEAAAAAGBgYEXoAOnpAP///wAAAAAAAAAAACH5BAAAAAAALAAAAAAgACAAAAN2
SLrc/jDKSaslIOjBuQ7AxWRbN3yhqJBfq6Uqa56vun4zamNuD4syk84W7AxjPdfvUvTUiDjhE5k8
AqPG6bWUBe1Y1aWFJSiXrcyP+axNV9tj9Rod16wF9Ao4KdbLzXkUZHNwfm9eUHaEiFQBd4ETez47
lJUVCQA7}
image create photo buttonimagevumeter -data $vumeterdata

::pdwindow::debug "\[ceammc\]: ceammc_buttonbar-plugin loaded\n"
