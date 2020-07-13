/*****************************************************************************
 * Copyright 2017 Serge Poltavsky. All rights reserved.
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

#include "ceammc_factory.h"

extern "C" {
#include "g_canvas.h"
#include "m_imp.h"
}

void ceammc::factory_bind_inlet_tooltip(t_glist* glist, t_object* x, size_t idx, const char* str)
{
    static bool tooltip_init = false;
    if (!tooltip_init) {
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

        tooltip_init = true;
    }

    if (!str || str[0] == '\0')
        return;

    const size_t N = obj_ninlets(x);
    if (idx >= N) {
        LIB_LOG << "invalid inlet index: " << idx;
        return;
    }

    const char* tag = rtext_gettag(glist_findrtext(glist, x));
    sys_vgui("::ceammc_tt::txt .x%lx.c %si%d 1 \"%s\"\n",
        glist, tag, (int)idx, str);
}

void ceammc::factory_bind_outlet_tooltip(t_glist* glist, t_object* x, size_t idx, const char* str)
{
    if (!str || str[0] == '\0')
        return;

    const size_t N = obj_noutlets(x);
    if (idx >= N) {
        LIB_LOG << "invalid outlet index: " << idx;
        return;
    }

    const char* tag = rtext_gettag(glist_findrtext(glist, x));
    sys_vgui("::ceammc_tt::txt .x%lx.c %so%d 0 \"%s\"\n",
        glist, tag, (int)idx, str);
}

ceammc::factory_visfn ceammc::factory_set_widget_behavior(t_class* c, t_widgetbehavior* wb, factory_visfn new_fn)
{
    *wb = *c->c_wb;
    factory_visfn old = wb->w_visfn;
    wb->w_visfn = new_fn;
    class_setwidget(c, wb);
    return old;
}

ceammc::factory_wb_pointer ceammc::factory_create_wb(t_class* c)
{
    return { new t_widgetbehavior(*c->c_wb), factory_free_wb };
}

void ceammc::factory_free_wb(t_widgetbehavior* wb)
{
    delete wb;
}
