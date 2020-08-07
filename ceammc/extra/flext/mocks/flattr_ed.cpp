/*
flext - C++ layer for Max and Pure Data externals

Copyright (c) 2001-2020 Thomas Grill (gr@grrrr.org)
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "license.txt," in this distribution.
*/

/*! \file flattr_ed.cpp
    \brief Attribute editor (property dialog) for PD
*/

#ifndef __FLEXT_ATTR_ED_CPP
#define __FLEXT_ATTR_ED_CPP

#include "flext.h"

#include "flpushns.h"

#if FLEXT_SYS == FLEXT_SYS_PD 

#ifdef _MSC_VER
#pragma warning( disable : 4091 ) 
#endif


#if defined(FLEXT_ATTRHIDE) || PD_MINOR_VERSION < 37
#define __FLEXT_WIDGETBEHAVIOR
#endif

//////////////////////////////////////////////////////
#ifdef __FLEXT_WIDGETBEHAVIOR
// we need non-public headers!
#pragma message("Attention: non-public headers used - binary is bound to a specific version")

#include <g_canvas.h>

/*
#ifdef PD_DEVEL_VERSION
#define __FLEXT_CLONEWIDGET
#endif
*/

#ifndef __FLEXT_CLONEWIDGET
#include <m_imp.h>
#endif

#endif
//////////////////////////////////////////////////////


#include <string.h>
#include <stdio.h>


#ifdef FLEXT_ATTRHIDE
#ifndef __FLEXT_CLONEWIDGET
static t_visfn ori_vis = NULL;
static t_selectfn ori_select = NULL;
#endif
#endif


#ifdef FLEXT_ATTRHIDE
#define ST_DISABLED ""
#else
#define ST_DISABLED " -state disabled"
#endif

#ifdef __APPLE__
static const char * FLEXT_HELP_FONT_BIG = "Helvetica 14 bold";
static const char * FLEXT_HELP_FONT_NORMAL = "Helvetica 12";
#elif __WIN32
static const char * FLEXT_HELP_FONT_BIG = "Verdana 11 bold";
static const char * FLEXT_HELP_FONT_NORMAL = "Verdana 10";
#else
static const char * FLEXT_HELP_FONT_BIG = "Sans 12 bold";
static const char * FLEXT_HELP_FONT_NORMAL = "Sans 10";
#endif


#ifndef FLEXT_NOATTREDIT

//! generate the script for the property dialog
FLEXT_TEMPLATE
void tclscript()
{
    static bool havecode = false;
    if(havecode) return;
    else havecode = true;

    sys_vgui(const_cast<char *>(
        "proc flext_escatoms {lst} {\n"
            "set tmp {}\n"
            "foreach a $lst {\n"
//                "set a [regsub {\\\\} $a \\\\\\\\]\n"  // replace \ with \\  ... must be first
                "set a [regsub {\\$} $a \\\\$]\n"  // replace $ with \$
//                "set a [regsub {\\{} $a \\\\\\{]\n"  // replace { with \{
//                "set a [regsub {\\}} $a \\\\\\}]\n"  // replace } with \}
//                "set a [regsub {\\ } $a \\\\\\ ]\n"  // replace space with \space
                "set a [regsub {,} $a \\\\,]\n"  // replace , with \,
                "set a [regsub {;} $a \\\\\\;]\n"  // replace ; with \;
                "lappend tmp $a\n"
            "}\n"
            "return $tmp\n"
        "}\n")
    );
    sys_vgui(const_cast<char *>(
        "proc flext_makevalue {id ix} {\n"
            // strip "." from the TK id to make a variable name suffix
            "set vid [string trimleft $id .]\n"

            "set var_attr_name [concat [concat var_name_$ix]_$vid ]\n"
            "set var_attr_init [concat [concat var_init_$ix]_$vid ]\n"
            "set var_attr_val [concat [concat var_val_$ix]_$vid ]\n"
            "set var_attr_save [concat [concat var_save_$ix]_$vid ]\n"
            "set var_attr_type [concat [concat var_type_$ix]_$vid ]\n"

            "global $var_attr_name $var_attr_init $var_attr_val $var_attr_save $var_attr_type\n"

            "set lst {}\n"

            "if { [expr $$var_attr_type] != 0 } {\n"
                // attribute is puttable

                "lappend lst [eval concat $$var_attr_name]\n" 

                // process current value
                "set tmp [flext_escatoms [eval concat $$var_attr_val]]\n"
                "set lst [concat $lst [llength $tmp] $tmp]\n" 

                // process init value
                "set tmp [flext_escatoms [eval concat $$var_attr_init]]\n"
                "set lst [concat $lst [llength $tmp] $tmp]\n" 

                "lappend lst [eval concat $$var_attr_save]\n" 
            "}\n"

            // return list
            "return $lst\n" 
        "}\n")
    );
    sys_vgui(const_cast<char *>(
        "proc flext_apply {id ix} {\n"
            "set lst [flext_makevalue $id $ix]\n"
            "set lst [eval concat $lst]\n" // remove curly braces from character escaping
            "pdsend [concat $id attributedialog $lst \\;]\n"
        "}\n"

        "proc flext_applyall {id alen} {\n"
            // make a list of the attribute values (including save flags)

            "set lst {}\n"
            "for {set ix 1} {$ix <= $alen} {incr ix} {\n"
                "set lst [concat $lst [flext_makevalue $id $ix]]\n" 
            "}\n"
            "set lst [eval concat $lst]\n" // remove curly braces from character escaping

            "pdsend [concat $id attributedialog $lst \\;]\n"
        "}\n"

        "proc flext_cancel {id} {\n"
            "pdsend [concat $id cancel \\;]\n"
        "}\n"

        "proc flext_ok {id alen} {\n"
            "flext_applyall $id $alen\n"
            "flext_cancel $id\n"
        "}\n")
    );
    sys_vgui(
        "proc flext_help {id} {\n"
            "toplevel $id.hw\n"
            "wm title $id.hw \"Flext attribute editor help\"\n"

            "ttk::frame $id.hw.top_frame\n"
            "pack $id.hw.top_frame -fill both -expand 1\n"
            "ttk::frame $id.hw.top_frame.buttons\n"
            "pack $id.hw.top_frame.buttons -side bottom -fill x -pady 2m\n"

            "tk::text $id.hw.top_frame.text -relief sunken -bd 1 -yscrollcommand \"$id.hw.top_frame.scroll set\" -setgrid 1 -width 80 -height 10 -wrap word\n"
            "ttk::scrollbar $id.hw.top_frame.scroll -command \"$id.hw.top_frame.text yview\"\n"
            "pack $id.hw.top_frame.scroll -side right -fill y\n"
            "pack $id.hw.top_frame.text -expand yes -fill both\n"

            "ttk::button $id.hw.top_frame.buttons.ok -text OK -command \"destroy $id.hw\"\n"
            "pack $id.hw.top_frame.buttons.ok -side left -expand 1\n"
            "bind $id.hw {<KeyPress-Escape>} \"destroy $id.hw\"\n"

            "$id.hw.top_frame.text tag configure big -font { %s }\n"
            "$id.hw.top_frame.text configure -font { %s }\n"
            "$id.hw.top_frame.text insert end \""
                "The flext attribute editor lets you query or change attribute values exposed by an external object. \" big \"\n\n"
                "Local variable names ($-values) will only be saved as such for init values. "
                "Alternatively, # can be used instead of $.\n"
                "Ctrl-Button on a text field will open an editor window where text can be entered more comfortably.\n"
            "\"\n"
            "$id.hw.top_frame.text configure -state disabled\n"
        "}\n", FLEXT_HELP_FONT_BIG, FLEXT_HELP_FONT_NORMAL
    );
    sys_vgui(const_cast<char *>(
        "proc flext_copyval {dst src} {\n"
            "global $src $dst\n"
            "set $dst [expr $$src]\n"
        "}\n"

        "proc flext_textcopy {id idtxt var} {\n"
            "global $var\n"
            "set txt [eval $idtxt get 0.0 end]\n"
            // strip newline characters
            "set tmp {}\n"
            "foreach t $txt { lappend tmp [string trim $t] }\n"
            "set $var $tmp\n"
            "destroy $id\n"
        "}\n")
    );
    sys_vgui(const_cast<char *>(
        "proc flext_textzoom {id var title attr edit} {\n"
            "global $var\n"
            "toplevel $id.w\n"
            "wm title $id.w [concat $title \" @\" $attr]\n"
//            "wm iconname $w \"text\"\n"
//            "positionWindow $id.w\n"

            "ttk::frame $id.w.buttons\n"
            "pack $id.w.buttons -side bottom -fill x -pady 2m\n"

            "tk::text $id.w.text -relief sunken -bd 1 -yscrollcommand \"$id.w.scroll set\" -setgrid 1 -width 80 -height 20\n"
            "ttk::scrollbar $id.w.scroll -command \"$id.w.text yview\"\n"
            "pack $id.w.scroll -side right -fill y\n"
            "pack $id.w.text -expand yes -fill both\n"

            // insert text with newlines
            "set txt [split [expr $$var] ,]\n"
            "set lines [llength $txt]\n"
            "for {set ix 0} {$ix < ($lines-1)} {incr ix} {\n"
                "$id.w.text insert end [string trim [lindex $txt $ix] ]\n"
                "$id.w.text insert end \" ,\\n\"\n"
            "}\n"
            "$id.w.text insert end [string trim [lindex $txt end] ]\n"

            "$id.w.text mark set insert 0.0\n"

            "if { $edit != 0 } then {\n"
                "button $id.w.buttons.ok -text OK -command \"flext_textcopy $id.w $id.w.text $var\"\n"
                "pack $id.w.buttons.ok -side left -expand 1\n"
//              "bind $id.w {<Shift-KeyPress-Return>} \"flext_textcopy $id.w $id.w.text $var\"\n"
            "} "
            "else { $id.w.text configure -state disabled }\n"

            "ttk::button $id.w.buttons.cancel -text Cancel -command \"destroy $id.w\"\n"
            "pack $id.w.buttons.cancel -side left -expand 1\n"
            "bind $id.w {<KeyPress-Escape>} \"destroy $id.w\"\n"
        "}\n")
    );
    sys_vgui(const_cast<char *>(
        "proc pdtk_flext_dialog {id title attrlist} {\n"
                "set vid [string trimleft $id .]\n"
                "set alen [expr [llength $attrlist] / 6 ]\n"

                "toplevel $id\n"
                "wm title $id $title\n" 
                "wm protocol $id WM_DELETE_WINDOW [concat flext_cancel $id]\n"

                "ttk::frame $id.tf\n"
                "pack $id.tf -fill both -expand 1\n"
                "ttk::frame $id.tf.frame\n"
                "set row 0\n"

                // set grow parameters
                "grid columnconfigure $id.tf.frame 0 -weight 1\n"  // label
                "grid columnconfigure $id.tf.frame {1 4} -weight 3\n" // value entry
                "grid columnconfigure $id.tf.frame {2 3} -weight 0\n"  // copy buttons
                "grid columnconfigure $id.tf.frame 5 -weight 1\n"  // apply button
                "grid columnconfigure $id.tf.frame {6 7 8} -weight 0\n" // radio buttons

                "grid rowconfigure $id.tf.frame {0 1} -weight 0\n"

                // set column labels
                "ttk::label $id.tf.frame.label -text {attribute}\n"
                "ttk::label $id.tf.frame.init  -text {initial value}\n"
                "ttk::label $id.tf.frame.copy  -text {copy}\n"
                "ttk::label $id.tf.frame.val   -text {current value}\n"
                "ttk::label $id.tf.frame.apply -text {}\n" // why must this be empty?
                "foreach {i txt} {0 {don't\rsave} 1 {do\rinit} 2 {always\rsave} } {\n"
                    "ttk::label $id.tf.frame.b$i -text $txt\n"
                "}\n"

                "grid config $id.tf.frame.label -column 0 -row $row \n"
                "grid config $id.tf.frame.init  -column 1 -row $row \n"
                "grid config $id.tf.frame.copy  -column 2 -columnspan 2 -row $row \n"
                "grid config $id.tf.frame.val   -column 4 -row $row \n"
                "grid config $id.tf.frame.apply  -column 5 -row $row \n"
                "foreach i {0 1 2} { grid config $id.tf.frame.b$i -column [expr $i + 6] -row $row }\n"
                "incr row\n"

                // Separator
                "ttk::frame $id.tf.frame.sep -relief ridge -borderwidth 1 -height 2\n"
                "grid config $id.tf.frame.sep -column 0 -columnspan 9 -row $row -pady 2 -sticky {snew}\n"
                "incr row\n")
    );
    sys_vgui(const_cast<char *>(
                "set ix 1\n"
                "foreach {an av ai atp asv afl} $attrlist {\n"
                    "grid rowconfigure $id.tf.frame $row -weight 1\n"

                    // get attribute name
                    "set var_attr_name [concat [concat var_name_$ix]_$vid ]\n"
                    "global $var_attr_name\n"
                    "set $var_attr_name $an\n"

                    // get attribute init value (list)
                    "set var_attr_init [concat [concat var_init_$ix]_$vid ]\n"
                    "global $var_attr_init\n"
                    "set $var_attr_init $ai\n"

                    // get attribute value (list)
                    "set var_attr_val [concat [concat var_val_$ix]_$vid ]\n"
                    "global $var_attr_val\n"
                    "set $var_attr_val $av\n"

                    // get save flag
                    "set var_attr_save [concat [concat var_save_$ix]_$vid ]\n"
                    "global $var_attr_save\n"
                    "set $var_attr_save $asv\n"

                    // get type flag
                    "set var_attr_type [concat [concat var_type_$ix]_$vid ]\n"
                    "global $var_attr_type\n"
                    "set $var_attr_type $afl\n"

                    // add dialog elements to window

                    // attribute label
                    "ttk::label $id.tf.frame.label-$ix -text \"$an :\"\n"
                    "grid config $id.tf.frame.label-$ix -column 0 -row $row -padx 5 -sticky {e}\n")
    );
    sys_vgui(const_cast<char *>(
                    "if { $afl != 0 } {\n"
                        // attribute is puttable

                        // entry field for initial value
                        // entry field for current value

                        // choose entry field type
                        "switch $atp {\n"
                            "0 - 1 {\n"  // int or float
                                "ttk::entry $id.tf.frame.init-$ix -textvariable $var_attr_init" ST_DISABLED "\n"
                                "ttk::entry $id.tf.frame.val-$ix -textvariable $var_attr_val\n"
                            "}\n"
                            "2 {\n"  // boolean
                                "ttk::checkbutton $id.tf.frame.init-$ix -variable $var_attr_init" ST_DISABLED "\n"
                                "ttk::checkbutton $id.tf.frame.val-$ix -variable $var_attr_val\n"
                            "}\n"
                            "3 {\n"  // symbol
                                "ttk::entry $id.tf.frame.init-$ix -textvariable $var_attr_init" ST_DISABLED "\n"
                                "ttk::entry $id.tf.frame.val-$ix -textvariable $var_attr_val\n"
                            "}\n"
                            "4 - 5 {\n"  // list or unknown
                                "ttk::entry $id.tf.frame.init-$ix -textvariable $var_attr_init" ST_DISABLED "\n"
                                "bind $id.tf.frame.init-$ix {<Control-Button-1>} \" flext_textzoom $id.tf.frame.init-$ix $var_attr_init { $title } $an 1\"\n"
                                "ttk::entry $id.tf.frame.val-$ix -textvariable $var_attr_val\n"
                                "bind $id.tf.frame.val-$ix {<Control-Button-1>} \" flext_textzoom $id.tf.frame.val-$ix $var_attr_val { $title } $an 1\"\n"
                            "}\n"
                        "}\n"

                        "grid config $id.tf.frame.init-$ix  -column 1 -row $row -padx 5 -sticky {ew}\n"
                        "grid config $id.tf.frame.val-$ix   -column 4 -row $row -padx 5 -sticky {ew}\n"

                        // copy buttons
                        "ttk::button $id.tf.frame.b2i-$ix -text {<-} -width 2 -command \" flext_copyval $var_attr_init $var_attr_val \"" ST_DISABLED "\n"
                        "grid config $id.tf.frame.b2i-$ix  -column 2 -row $row  -sticky {ew}\n"
                        "ttk::button $id.tf.frame.b2c-$ix -text {->} -width 2 -command \" flext_copyval $var_attr_val $var_attr_init \"\n"
                        "grid config $id.tf.frame.b2c-$ix  -column 3 -row $row  -sticky {ew}\n"

                        // apply button
                        "ttk::button $id.tf.frame.apply-$ix -text {Apply} -command \" flext_apply $id $ix \"\n"
                        "grid config $id.tf.frame.apply-$ix -column 5 -row $row  -sticky {ew}\n"

                        // radiobuttons
                        "foreach {i c} {0 black 1 blue 2 red} {\n"
                            "ttk::radiobutton $id.tf.frame.b$i-$ix -value $i -variable $var_attr_save" ST_DISABLED "\n"
                            "grid config $id.tf.frame.b$i-$ix -column [expr $i + 6] -row $row\n"
                        "}\n")
    );
    sys_vgui(const_cast<char *>(
                    "} else {\n"
                        // attribute is gettable only

                        // entry field for current value (read-only)

                        // choose display field type
                        "switch $atp {\n"
                            "0 - 1 {\n"  // int or float
                                "ttk::entry $id.tf.frame.val-$ix -textvariable $var_attr_val -state disabled\n"
                            "}\n"
                            "2 {\n"  // boolean
                                "ttk::checkbutton $id.tf.frame.val-$ix -variable $var_attr_val -state disabled\n"
                            "}\n"
                            "3 {\n"  // symbol
                                "ttk::entry $id.tf.frame.val-$ix -textvariable $var_attr_val -state disabled\n"
                            "}\n"
                            "4 - 5 {\n"  // list or unknown
                                "ttk::entry $id.tf.frame.val-$ix -textvariable $var_attr_val -state disabled\n"
                                "bind $id.tf.frame.val-$ix {<Control-Button-1>} \" flext_textzoom $id.tf.frame.val-$ix $var_attr_val { $title } $an 0\"\n"
                            "}\n"
                        "}\n"

//                      "ttk::entry $id.tf.fval.val-$ix -textvariable $var_attr_val -state disabled\n"
                        "grid config $id.tf.frame.val-$ix -column 4 -row $row -padx 5 -sticky {ew}\n"

                        "ttk::label $id.tf.frame.readonly-$ix -text \"read-only\"\n"
                        "grid config $id.tf.frame.readonly-$ix -column 6 -columnspan 3 -row $row -padx 5 -sticky {ew}\n"
                    "}\n"

                    // increase counter
                    "incr ix\n"
                    "incr row\n"
                "}\n"

                // empty space
                "grid rowconfigure $id.tf.frame $row -weight 1\n"
                "ttk::frame $id.tf.frame.dummy\n"
                "grid config $id.tf.frame.dummy -column 0 -columnspan 9 -row $row\n"
                "incr row\n")
    );
    sys_vgui(const_cast<char *>(
                // Separator
                "ttk::frame $id.tf.sep2 -relief ridge -borderwidth 1 -height 2\n"

                // Buttons
                "ttk::frame $id.tf.buttonframe\n"

                "ttk::button $id.tf.buttonframe.cancel -text {Leave} -command \" flext_cancel $id \"\n"
                "ttk::button $id.tf.buttonframe.apply -text {Apply all} -command \" flext_applyall $id $alen \"\n"
                "ttk::button $id.tf.buttonframe.ok -text {Apply & Leave} -command \" flext_ok $id $alen \"\n"
                "ttk::button $id.tf.buttonframe.help -text {Help} -width 10 -command \" flext_help $id \"\n"

                "grid columnconfigure $id.tf.buttonframe {0 1 2 3} -weight 1\n"
                "grid config $id.tf.buttonframe.cancel $id.tf.buttonframe.apply $id.tf.buttonframe.ok $id.tf.buttonframe.help -padx 2 -sticky {snew}\n"

//                "scrollbar $id.tf.scroll -command \"$id.tf.frame yview\"\n"

                "pack $id.tf.buttonframe $id.tf.sep2 -pady 2 -expand 0 -side bottom -fill x\n"
//                "pack $id.tf.scroll -side right -fill y\n"
                "pack $id.tf.frame -expand 1 -side top -fill both\n"

                // Key bindings
                "bind $id {<KeyPress-Escape>} \" flext_cancel $id \"\n"
                "bind $id {<KeyPress-Return>} \" flext_ok $id $alen \"\n"
                "bind $id {<Shift-KeyPress-Return>} \" flext_applyall $id $alen \"\n"
        "}\n")
    );
}

#endif


#ifdef __FLEXT_WIDGETBEHAVIOR
static t_widgetbehavior widgetbehavior; 
#endif

FLEXT_TEMPIMPL(void FLEXT_CLASSDEF(flext_base))::SetGfx(t_classid c)
{
	t_class *cl = getClass(c);
    // widgetbehavior struct MUST be resident... (static is just ok here)

#ifdef __FLEXT_WIDGETBEHAVIOR
#ifndef __FLEXT_CLONEWIDGET
    widgetbehavior.w_visfn =        cl->c_wb->w_visfn; 
    widgetbehavior.w_selectfn =     cl->c_wb->w_selectfn; 
    widgetbehavior.w_getrectfn =    cl->c_wb->w_getrectfn; 
    widgetbehavior.w_displacefn =   cl->c_wb->w_displacefn; 
    widgetbehavior.w_activatefn =   cl->c_wb->w_activatefn; 
    widgetbehavior.w_deletefn =     cl->c_wb->w_deletefn; 
    widgetbehavior.w_selectfn =     cl->c_wb->w_selectfn;
    widgetbehavior.w_clickfn =      cl->c_wb->w_clickfn;
#else
    widgetbehavior.w_visfn =        text_widgetbehavior.w_visfn; 
    widgetbehavior.w_selectfn =     text_widgetbehavior.w_selectfn; 
    widgetbehavior.w_getrectfn =    text_widgetbehavior.w_getrectfn; 
    widgetbehavior.w_displacefn =   text_widgetbehavior.w_displacefn; 
    widgetbehavior.w_activatefn =   text_widgetbehavior.w_activatefn; 
    widgetbehavior.w_deletefn =     text_widgetbehavior.w_deletefn; 
    widgetbehavior.w_selectfn =     text_widgetbehavior.w_selectfn;
    widgetbehavior.w_clickfn =      text_widgetbehavior.w_clickfn;
#endif
#endif

#ifdef FLEXT_ATTRHIDE

#ifndef __FLEXT_CLONEWIDGET
    ori_vis = widgetbehavior.w_visfn; 
    ori_select = widgetbehavior.w_selectfn; 
#endif
    widgetbehavior.w_visfn =        (t_visfn)cb_GfxVis;
    widgetbehavior.w_selectfn =     (t_selectfn)cb_GfxSelect; 

#if PD_MINOR_VERSION >= 37
    class_setsavefn(cl,(t_savefn)cb_GfxSave);
#else
    widgetbehavior.w_savefn =       (t_savefn)cb_GfxSave;
#endif

#endif // FLEXT_ATTRHIDE


#ifndef FLEXT_NOATTREDIT

#if PD_MINOR_VERSION >= 37
    class_setpropertiesfn(cl,(t_propertiesfn)cb_GfxProperties);
#else
    widgetbehavior.w_propertiesfn = (t_propertiesfn)cb_GfxProperties;
#endif

    FLEXT_TEMPINST(tclscript)();
#endif // FLEXT_NOATTREDIT

#ifdef __FLEXT_WIDGETBEHAVIOR
    class_setwidget(cl, &widgetbehavior);
#endif
}


#ifndef FLEXT_NOATTREDIT

FLEXT_TEMPLATE
size_t escapeit(char *dst,size_t maxlen,const char *src)
{
    char *d;
    for(d = dst; *src && (d-dst) < (int)maxlen; ++src) {
        if(*src == '%')
            *(d++) = '%',*(d++) = '%';
        else
            *(d++) = *src;
    }
    *d = 0;
    return d-dst;
}

FLEXT_TEMPIMPL(void FLEXT_CLASSDEF(flext_base))::cb_GfxProperties(flext_hdr *c, t_glist *)
{
    flext_base *th = thisObject(c);
    char buf[1000];

     // beginning of proc
    sys_vgui(const_cast<char *>("proc pdtk_flext_dialog_%p {title} {\n"),th);

    sys_vgui(const_cast<char *>("pdtk_flext_dialog $title {\n"));

    // add title
    t_text *x = (t_text *)c;
    FLEXT_ASSERT(x->te_binbuf);

    int argc = binbuf_getnatom(x->te_binbuf);
    t_atom *argv = binbuf_getvec(x->te_binbuf);

    PrintList(argc,argv,buf,sizeof(buf));
    sys_vgui(const_cast<char *>("%s } {\n"),buf);

    AtomListStatic<32> la;
    th->ListAttrib(la);
    int cnt = la.Count();

    for(int i = 0; i < cnt; ++i) {
        const t_symbol *sym = GetSymbol(la[i]); 

        // get attribute
        AttrItem *gattr = th->FindAttrib(sym,true);
        // get puttable attribute
        AttrItem *pattr = gattr?gattr->Counterpart():th->FindAttrib(sym,false);

        // get flags
        int sv;
        const AtomList *initdata;
        const AttrData *a = th->attrdata->find(sym);
//        AttrDataCont::iterator it = th->attrdata->find(sym);
//        if(it == th->attrdata->end())
        if(!a)
            sv = 0,initdata = NULL;
        else {
//            const AttrData &a = *it.data();
            if(a->IsSaved())
                sv = 2;
            else if(a->IsInit())
                sv = 1;
            else 
                sv = 0;
            initdata = a->IsInitValue()?&a->GetInitValue():NULL;
        }

        // get attribute type
        int tp;
        bool list;
        switch((gattr?gattr:pattr)->argtp) {
            case a_int: tp = 0; list = false; break;
            case a_float: tp = 1; list = false; break;
            case a_bool: tp = 2; list = false; break;
            case a_symbol: tp = 3; list = true; break;
            case a_list: 
            case a_LIST: tp = 4; list = true; break;
            default: 
                tp = 5; list = true; 
                FLEXT_ASSERT(false);
        }

        sys_vgui(const_cast<char *>(list?"%s {":"%s "),GetString(sym));

        AtomListStatic<32> lv;
        if(gattr) { // gettable attribute is present
            // Retrieve attribute value
            th->GetAttrib(sym,gattr,lv);

            char *b = buf; *b = 0;
            for(int j = 0; j < lv.Count(); ++j) {
                char tmp[100];
                PrintAtom(lv[j],tmp,sizeof tmp);
                b += FLEXT_TEMPINST(escapeit)(b,sizeof(buf)+buf-b,tmp);
                if(j < lv.Count()-1) { *(b++) = ' '; *b = 0; }
            }
            sys_vgui(const_cast<char *>("%s"),buf);
        }
        else
            sys_vgui(const_cast<char *>("{}"));

        sys_vgui(const_cast<char *>(list?"} {":" "));

        if(pattr) {
            // if there is initialization data take this, otherwise take the current data
            const AtomList &lp = initdata?*initdata:static_cast<const AtomList &>(lv);

            char *b = buf; *b = 0;
            for(int j = 0; j < lp.Count(); ++j) {
                char tmp[256];
                PrintAtom(lp[j],tmp,sizeof(tmp)); 
                b += FLEXT_TEMPINST(escapeit)(b,sizeof(buf)+buf-b,tmp);
                if(j < lp.Count()-1) { *(b++) = ' '; *b = 0; }
            }
            sys_vgui(const_cast<char *>("%s"),buf);
        }
        else
            sys_vgui(const_cast<char *>("{}"));


        sys_vgui(const_cast<char *>(list?"} %i %i %i \n":" %i %i %i \n"),tp,sv,pattr?(pattr->BothExist()?2:1):0);
    }

    sys_vgui(const_cast<char *>(" } }\n")); // end of proc

    STD::sprintf(buf,"pdtk_flext_dialog_%p %%s\n",th);
    gfxstub_new((t_pd *)th->thisHdr(), th->thisHdr(),buf);

    //! \todo delete proc in TCL space
}

FLEXT_TEMPIMPL(bool FLEXT_CLASSDEF(flext_base))::cb_AttrDialog(flext_base *th,int argc,const t_atom *argv)
{
    for(int i = 0; i < argc; ) {
        FLEXT_ASSERT(IsSymbol(argv[i]));

        // get name
        const t_symbol *aname = GetSymbol(argv[i]);
        i++;

        // get current value
        FLEXT_ASSERT(CanbeInt(argv[i]));
        int ccnt,coffs;
        ccnt = GetAInt(argv[i]);
        coffs = ++i;
        i += ccnt;

        // get init value
        FLEXT_ASSERT(CanbeInt(argv[i]));
        int icnt,ioffs;
        icnt = GetAInt(argv[i]);
        ioffs = ++i;
        i += icnt;

        FLEXT_ASSERT(i < argc);
        int sv = GetAInt(argv[i]);
        ++i;

        // find puttable attribute
        AttrItem *attr = th->FindAttrib(aname,false);
        if(attr) {
            bool ret = th->SetAttrib(aname,attr,ccnt,argv+coffs);
            FLEXT_ASSERT(ret);

            AttrData *a = th->attrdata->find(aname);
            if(sv >= 1) {
                // if data not present create it
                if(!a) {
                    AttrData *old = th->attrdata->insert(aname,a = new AttrData);
                    FLEXT_ASSERT(!old);
                }

                a->SetSave(sv == 2);
                a->SetInit(true);
                a->SetInitValue(icnt,argv+ioffs);
            }
            else {
                if(a) {
                    // if data is present reset flags
                    a->SetSave(false);
                    a->SetInit(false);

                    // let init data as is
                }
            }
        }
        else {
            post("%s - Attribute %s can't be set",th->thisName(),GetString(aname));
        }
    }
    return true;
}

#endif // FLEXT_NOATTREDIT


#ifdef FLEXT_ATTRHIDE

template<typename=void>
void BinbufAdd(t_binbuf *b,const t_atom &at,bool transdoll)
{
    if(transdoll && at.a_type == A_DOLLAR) {
        char tbuf[MAXPDSTRING];
        sprintf(tbuf, "$%d", at.a_w.w_index);
        binbuf_addv(b,"s",flext::MakeSymbol(tbuf));
    }
    else if(transdoll && at.a_type == A_DOLLSYM) {
        char tbuf[MAXPDSTRING];
        sprintf(tbuf, "$%s", at.a_w.w_symbol->s_name);
        binbuf_addv(b,"s",flext::MakeSymbol(tbuf));
    }
    else
        binbuf_add(b,1,const_cast<t_atom *>(&at));
}

FLEXT_TEMPIMPL(void FLEXT_CLASSDEF(flext_base))::BinbufArgs(t_binbuf *b,t_binbuf *args,bool withname,bool transdoll)
{
    int argc = binbuf_getnatom(args);
    t_atom *argv = binbuf_getvec(args);
    int i,cnt = CheckAttrib(argc,argv);
    // process the creation arguments
    for(i = withname?0:1; i < cnt; ++i) BinbufAdd(b,argv[i],transdoll);
}

FLEXT_TEMPIMPL(void FLEXT_CLASSDEF(flext_base))::BinbufAttr(t_binbuf *b,bool transdoll)
{
    // process the attributes
    AtomListStatic<32> la,lv;
    ListAttrib(la);
    int i,cnt = la.Count();

    for(i = 0; i < cnt; ++i) {
        const t_symbol *sym = GetSymbol(la[i]);
        const AtomList *lref = NULL;

        AttrData *a = attrdata->find(sym);
        if(a) {
            if(a->IsInit() && a->IsInitValue()) {
                lref = &a->GetInitValue();

#if 0 /////////////////////////////////////////////////////////////
                // check for $-parameters
                lv = lref->Count();
                for(int j = 0; j < lref->Count(); ++j) {
                    const char *s = IsSymbol((*lref)[j])?GetString((*lref)[j]):NULL;
                    if(s && s[0] == '$') { // TODO: More refined checking?
                        // prepend a "\"
                        char tmp[256]; *tmp = '\\';
                        strcpy(tmp+1,s);
                        SetString(lv[j],tmp);
                    }
                    else
                        lv[i] = (*lref)[j];
                }

                lref = &lv;
#endif /////////////////////////////////////////////////////////////
            }
//            else if(a.IsSaved()) {
            else if(a->IsSaved()) {
                AttrItem *attr = FindAttrib(sym,true);

                // attribute must be gettable (so that the data can be retrieved) and puttable (so that the data can be inited)
                if(attr && attr->BothExist()) {
                    GetAttrib(sym,attr,lv); 
                    lref = &lv;
                }
            }
        }

        if(lref) {
            char attrname[256]; *attrname= '@';
            // store name
            strcpy(attrname+1,GetString(sym));
            binbuf_addv(b,"s",MakeSymbol(attrname));

            // store value
            for(int j = 0; j < lref->Count(); ++j) BinbufAdd(b,(*lref)[j],transdoll);
        }
    }
}

//! Strip the attributes off the object command line
FLEXT_TEMPIMPL(void FLEXT_CLASSDEF(flext_base))::cb_GfxVis(flext_hdr *c, t_glist *gl, int vis)
{
    if(!gl->gl_isgraph || gl->gl_havewindow) {
        // show object if it's not inside a GOP

        flext_base *th = thisObject(c);
        t_text *x = (t_text *)c;
        FLEXT_ASSERT(x->te_binbuf);

        t_binbuf *b = binbuf_new();
        th->BinbufArgs(b,x->te_binbuf,true,false);

        // delete old object box text
        binbuf_free(x->te_binbuf);
        // set new one
        x->te_binbuf = b;

        t_rtext *rt = glist_findrtext(gl,x);
        rtext_retext(rt);

        // now display the changed text with the normal drawing function
    #ifdef __FLEXT_CLONEWIDGET
        text_widgetbehavior.w_visfn((t_gobj *)c,gl,vis);
    #else
        ori_vis((t_gobj *)c,gl,vis);
    #endif
    }
    // else don't show
}

FLEXT_TEMPIMPL(void FLEXT_CLASSDEF(flext_base))::cb_GfxSelect(flext_hdr *c,t_glist *gl,int state)
{
    t_text *x = (t_text *)c;
    flext_base *th = thisObject(c);

    if(!gl->gl_isgraph || gl->gl_havewindow) {
        if(state || !gl->gl_editor->e_textdirty) {
            // change text only on selection
            // OR if text has _not_ been changed 
            // ->  since object will not be recreated we have to get rid
            //     of the attribute text

            FLEXT_ASSERT(x->te_binbuf);

            t_binbuf *b = binbuf_new();
            th->BinbufArgs(b,x->te_binbuf,true,false);
            if(state) th->BinbufAttr(b,false);

            // delete old object box text
            binbuf_free(x->te_binbuf);
            // set new one
            x->te_binbuf = b;

            t_rtext *rt = glist_findrtext(gl,x);
            rtext_retext(rt);

            // fix lines
            canvas_fixlinesfor(gl,x);
        }

        // call original function
        #ifdef __FLEXT_CLONEWIDGET
            text_widgetbehavior.w_selectfn((t_gobj *)c,gl,state);
        #else
            ori_select((t_gobj *)c,gl,state);
        #endif
    }
}

FLEXT_TEMPIMPL(void FLEXT_CLASSDEF(flext_base))::cb_GfxSave(flext_hdr *c, t_binbuf *b)
{
    flext_base *th = thisObject(c);
    t_text *t = (t_text *)c;
    binbuf_addv(b, "ssiis", gensym("#X"),gensym("obj"), t->te_xpix, t->te_ypix,MakeSymbol(th->thisName()));

    // process the object arguments
    th->BinbufArgs(b,t->te_binbuf,false,true);
    // process the attributes
    th->BinbufAttr(b,true);
    // add end sign
    binbuf_addv(b, ";");
}

#endif // FLEXT_ATTRHIDE

#endif // FLEXT_SYS_PD

#include "flpopns.h"

#endif // __FLEXT_ATTR_ED_CPP


