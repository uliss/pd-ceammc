/*****************************************************************************
 * Copyright 2019 Serge Poltavsky. All rights reserved.
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
#include "system_cursor.h"
#include "ceammc_canvas.h"
#include "ceammc_factory.h"
#include "system_cursor.tcl.h"

#include <cinttypes>

extern "C" {
#include "g_canvas.h"
#include "m_imp.h"
}

static t_symbol* SYM_CURSOR_BIND;

int SystemCursor::instances_polling_ = 0;

SystemCursor::SystemCursor(const PdArgs& args)
    : BaseObject(args)
    , is_polling_(false)
    , clock_(this, &SystemCursor::clockTick)
    , relative_(nullptr)
{
    createOutlet();

    char buf[MAXPDSTRING];
    sprintf(buf, "#%" PRIxPTR, reinterpret_cast<uintptr_t>(this));
    bindReceive(gensym(buf));

    relative_ = new BoolProperty("@relative", false);
    addProperty(relative_);
}

SystemCursor::~SystemCursor()
{
    if (is_polling_) {
        instances_polling_--;
        checkPolling();

        pd_unbind(&owner()->te_g.g_pd, SYM_CURSOR_BIND);
    }
}

void SystemCursor::onBang()
{
    sys_vgui("pdsend \"%s .motion [winfo pointerxy .]\"\n", receive()->s_name);
}

void SystemCursor::onFloat(t_float f)
{
    if (f == 1 && !is_polling_) {
        is_polling_ = true;
        pd_bind(&owner()->te_g.g_pd, SYM_CURSOR_BIND);
        startPolling();
    } else if (f == 0 && is_polling_) {
        is_polling_ = false;
        stopPolling();

        // when float arrived after mouse click on a toggle
        // we have to unbind on a next clock tick
        clock_.delay(0);
    }
}

void SystemCursor::m_button(t_symbol* s, const AtomListView& args)
{
    static t_symbol* SYM = gensym("button");

    if (is_polling_)
        anyTo(0, SYM, args);
}

void SystemCursor::m_motion(t_symbol* s, const AtomListView& args)
{
    static t_symbol* SYM = gensym("motion");

    if (!relative_->value()) {
        anyTo(0, SYM, args);
    } else {
        if (checkArgs(args, ARG_FLOAT, ARG_FLOAT)) {
            t_canvas* cnv = canvas_getrootfor(canvas());
            auto r = canvas_info_rect(cnv);
            anyTo(0, SYM, { args[0].asFloat() - r.x, args[1].asFloat() - r.y });
        }
    }
}

void SystemCursor::m_wheel(t_symbol* s, const AtomListView& args)
{
    static t_symbol* SYM = gensym("mousewheel");

    if (is_polling_)
        anyTo(0, SYM, args);
}

void SystemCursor::clockTick()
{
    pd_unbind(&owner()->te_g.g_pd, SYM_CURSOR_BIND);
}

void SystemCursor::checkPolling()
{
    /* if no more objects are listening, stop sending the events */
    if (instances_polling_ == 0)
        sys_gui("::ceammc::cursor::stoppolling\n");
    /* if this is the first instance to start, set up Tcl binding and polling */
    else if (instances_polling_ == 1)
        sys_gui("::ceammc::cursor::startpolling\n");
}

void SystemCursor::startPolling()
{
    instances_polling_++;
    checkPolling();
}

void SystemCursor::stopPolling()
{
    instances_polling_--;
    checkPolling();
}

void setup_system_cursor()
{
    SYM_CURSOR_BIND = gensym("#ceammc_cursor_class_receive");

    ObjectFactory<SystemCursor> obj("system.cursor");
    obj.addMethod(".button", &SystemCursor::m_button);
    obj.addMethod(".motion", &SystemCursor::m_motion);
    obj.addMethod(".mousewheel", &SystemCursor::m_wheel);

    sys_gui(system_cursor_tcl);
    sys_vgui("::ceammc::cursor::setup %s\n", SYM_CURSOR_BIND->s_name);
}
