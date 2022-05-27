/*****************************************************************************
 *
 * Created: 04.12.2016 2016 by Serge Poltavski
 *
 * Copyright 2016 Serge Poltavski. All rights reserved.
 *
 * This file may be distributed under the terms of GNU Public License version
 * 3 (GPL v3) as defined by the Free Software Foundation (FSF). A copy of the
 * license should have been included with this file, or the project in which
 * this file belongs to. You may also find the details of GPL v3 at:
 * http://www.gnu.org/licenses/gpl-3.0.txt
 *
 * If you have any questions regarding the use of this file, feel free to
 * contact the author of this file, or the owner of the project in which
 * this file belongs to.
 *****************************************************************************/
#include "ceammc.h"
#include "config.h"
#include "fmt/format.h"

namespace ceammc {

InvalidOutlet::InvalidOutlet(OutletIdx n) noexcept
    : Exception("")
    , n_(n)
{
}

const char* InvalidOutlet::what() const noexcept
{
    static char buf[64];
    fmt::format_to(buf, FMT_STRING("invalid outlet index: {}"), n_.n);
    return buf;
}

SymbolTable::SymbolTable()
    : s_annotate_fn(gensym(".annotate"))
    , s_propget_fn(gensym(".propget"))
    , s_propset_fn(gensym(".propset"))
    , s_propfind_fn(gensym(".propfind"))
    , s_is_base_obj_fn(gensym(".is_base?"))
    , s_is_cicm_obj_fn(gensym(".is_cicm?"))
    , s_is_flext_obj_fn(gensym(".is_flext?"))
    , s_dump_fn(gensym("dump"))
{
}

const SymbolTable& SymbolTable::instance()
{
    static SymbolTable instance_;
    return instance_;
}

void ceammc_tcl_init_tooltips()
{
    sys_vgui("%s\n",
        "namespace eval ::ceammc_tt {\n"
        "    set txt {}\n"
        "    set active 0\n"
        "}\n"
        "proc ::ceammc_tt::txt {c tag xlet text} {\n"
        "    $c bind $tag <Enter>  [list ::ceammc_tt::enter $c $tag $xlet $text]\n"
        "    $c bind $tag <Leave>  [list ::ceammc_tt::leave $c]\n"
        " }\n"
        "proc ::ceammc_tt::show {c tag xlet} {\n"
        "    if {$::ceammc_tt::active == 0} return\n"
        "    $c delete ceammc_tt\n"
        "    foreach {x - - y} [$c bbox $tag] break\n"
        "    if [info exists y] {\n"
        "        variable id\n"
        "        if {$xlet == 0} { \n"
        "           incr y 7 \n"
        "           set id [$c create text $x $y -text $::ceammc_tt::txt -font TkTooltipFont -anchor nw -tag ceammc_tt]\n"
        "        } else {\n"
        "           incr y -10\n"
        "           set id [$c create text $x $y -text $::ceammc_tt::txt -font TkTooltipFont -anchor sw -tag ceammc_tt]\n"
        "        }\n"
        "        foreach {x0 y0 x1 y1} [$c bbox $id] break\n"
        "        $c create rect [expr $x0-2] [expr $y0-1] [expr $x1+2] [expr $y1+1] -fill lightblue -tag ceammc_tt\n"
        "        $c raise $id\n"
        "        $c bind $id <Leave> [list ::ceammc_tt::leave $c]\n"
        "    }\n"
        "}\n"
        "proc ::ceammc_tt::enter {c tag xlet text} {\n"
        "    set ::ceammc_tt::active 1\n"
        "    set ::ceammc_tt::txt $text\n"
        "    after 500 ::ceammc_tt::show $c $tag $xlet\n"
        "}\n"
        "proc ::ceammc_tt::delete {c} {\n"
        "    if {$::ceammc_tt::active == 1} return\n"
        "    $c delete ceammc_tt\n"
        "}\n"
        "proc ::ceammc_tt::leave {c} {\n"
        "    set ::ceammc_tt::active 0\n"
        "    after 50 ::ceammc_tt::delete $c\n"
        "}");
}

XletGetAnnotationFn ceammc_get_annotation_fn(t_pd* x)
{
    return reinterpret_cast<XletGetAnnotationFn>(zgetfn(x, ceammc::SymbolTable::instance().s_annotate_fn));
}

void ceammc_xlet_bind_tooltip(t_object* x, t_glist* glist, XletType type, const char* xlet_id, const char* txt)
{
    sys_vgui("::ceammc_tt::txt .x%lx.c %s %d \"%s\"\n", glist, xlet_id, type, txt);
}

void ceammc_xlet_bind_tooltip(t_object* x, t_glist* glist, XletGetTclIdFn id_fn, XletGetAnnotationFn ann_fn, XletType type, int xlet_idx)
{
    const char* str = ann_fn(x, type, xlet_idx);
    if (!str || str[0] == '\0')
        return;

    char xlet_tcl_id[128] = "";
    id_fn(glist, x, xlet_tcl_id, sizeof(xlet_tcl_id), type, xlet_idx);
    ceammc_xlet_bind_tooltip(x, glist, type, xlet_tcl_id, str);
}

void ceammc_class_add_propget_fn(t_class* c, PropertyGetFn fn)
{
    class_addmethod(c, reinterpret_cast<t_method>(fn), SymbolTable::instance().s_propget_fn, A_CANT, A_NULL);
}

void ceammc_class_add_propset_fn(t_class* c, PropertySetFn fn)
{
    class_addmethod(c, reinterpret_cast<t_method>(fn), SymbolTable::instance().s_propset_fn, A_CANT, A_NULL);
}

void ceammc_class_add_propfind_fn(t_class* c, PropertyFindFn fn)
{
    class_addmethod(c, reinterpret_cast<t_method>(fn), SymbolTable::instance().s_propfind_fn, A_CANT, A_NULL);
}

PropertyGetFn ceammc_get_propget_fn(t_object* x)
{
    if (!x)
        return nullptr;
    else
        return reinterpret_cast<PropertyGetFn>(zgetfn(&x->te_g.g_pd, SymbolTable::instance().s_propget_fn));
}

PropertySetFn ceammc_get_propset_fn(t_object* x)
{
    if (!x)
        return nullptr;
    else
        return reinterpret_cast<PropertySetFn>(zgetfn(&x->te_g.g_pd, SymbolTable::instance().s_propset_fn));
}

PropertyFindFn ceammc_get_propfind_fn(t_object* x)
{
    if (!x)
        return nullptr;
    else
        return reinterpret_cast<PropertyFindFn>(zgetfn(&x->te_g.g_pd, SymbolTable::instance().s_propfind_fn));
}

}
