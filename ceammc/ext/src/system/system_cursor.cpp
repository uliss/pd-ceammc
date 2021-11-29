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
#include "ceammc_convert.h"
#include "ceammc_factory.h"
#include "system_cursor.tcl.h"

#include <cinttypes>

extern "C" {
#include "g_canvas.h"
#include "m_imp.h"
}

constexpr const char* STR_CURSOR_BIND = "#ceammc_cursor_class_receive";

static t_canvas* cursor_canvas_root(t_canvas* cnv)
{
    while (cnv && cnv->gl_owner)
        cnv = cnv->gl_owner;

    return cnv;
}

int SystemCursor::instances_polling_ = 0;

SystemCursor::SystemCursor(const PdArgs& args)
    : BaseObject(args)
    , unbind_([this]() { pd_unbind(&owner()->te_g.g_pd, gensym(STR_CURSOR_BIND)); })
    , relative_(nullptr)
    , normalize_(nullptr)
    , clip_(nullptr)
    , is_polling_(false)
{
    createOutlet();

    char buf[MAXPDSTRING];
    sprintf(buf, "#%" PRIxPTR, reinterpret_cast<uintptr_t>(this));
    bindReceive(gensym(buf));

    relative_ = new BoolProperty("@relative", false);
    addProperty(relative_);

    normalize_ = new BoolProperty("@norm", false);
    addProperty(normalize_);

    clip_ = new BoolProperty("@clip", false);
    addProperty(clip_);
}

SystemCursor::~SystemCursor()
{
    if (is_polling_) {
        instances_polling_--;
        checkPolling();

        pd_unbind(&owner()->te_g.g_pd, gensym(STR_CURSOR_BIND));
    }
}

void SystemCursor::onBang()
{
    // dedicated motion send request
    sys_vgui("::ceammc::cursor::motion %s 1\n", receive()->s_name);
}

void SystemCursor::onFloat(t_float f)
{
    if (f == 1 && !is_polling_) {
        is_polling_ = true;
        // bind to broadcast message
        pd_bind(&owner()->te_g.g_pd, gensym(STR_CURSOR_BIND));
        startPolling();
    } else if (f == 0 && is_polling_) {
        is_polling_ = false;
        stopPolling();

        // when float arrived after mouse click on a toggle
        // we have to unbind from broadcast on a next clock tick
        unbind_.delay(0);
    }
}

void SystemCursor::m_button(t_symbol* s, const AtomListView& lv)
{
    if (is_polling_)
        anyTo(0, gensym("button"), lv);
}

void SystemCursor::m_motion(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT))
        return;

    t_float x = lv[0].asT<t_float>();
    t_float y = lv[1].asT<t_float>();
    t_float w = 0;
    t_float h = 0;
    t_float clipx = 0;
    t_float clipy = 0;

    if (relative_->value()) {
        auto cnv = cursor_canvas_root(canvas());
        if (!cnv)
            return;

        auto wrect = canvas_info_rect(cnv);
        x -= wrect.x;
        y -= wrect.y;
        w = wrect.w;
        h = wrect.h;
    } else {
        w = lv[2].asT<t_float>();
        h = lv[3].asT<t_float>();
    }

    if (normalize_->value()) {
        if (w > 1 && h > 1) {
            x /= (w - 1);
            y /= (h - 1);
        }

        clipx = 1;
        clipy = 1;
    } else {
        clipx = w - 1;
        clipy = h - 1;
    }

    if (clip_->value()) {
        x = clip<t_float>(x, 0, clipx);
        y = clip<t_float>(y, 0, clipy);
    }

    const Atom res[2] = { x, y };
    anyTo(0, gensym("motion"), AtomListView(res, 2));
}

void SystemCursor::m_wheel(t_symbol* s, const AtomListView& lv)
{
    if (is_polling_)
        anyTo(0, gensym("mousewheel"), lv);
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
    ObjectFactory<SystemCursor> obj("system.cursor");
    obj.addMethod(".button", &SystemCursor::m_button);
    obj.addMethod(".motion", &SystemCursor::m_motion);
    obj.addMethod(".mousewheel", &SystemCursor::m_wheel);

    obj.setXletsInfo({ "bang: output cursor XY pos\n"
                       "float: start/stop polling" },
        { "motion X Y\n"
          "button BTN STATE\n"
          "mousewheel DELTA" });

    sys_gui(system_cursor_tcl);
    sys_vgui("::ceammc::cursor::setup %s\n", STR_CURSOR_BIND);
}
