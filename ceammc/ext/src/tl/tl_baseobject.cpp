/*****************************************************************************
 * Copyright 2018 Serge Poltavsky. All rights reserved.
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
#include "tl_baseobject.h"
#include "ceammc_factory.h"
#include "ceammc_timeline.h"

extern "C" {
#include "g_canvas.h"
#include "m_imp.h"
}

using namespace ceammc::tl;

static TlBaseObject* toObj(t_object* x)
{
    typedef ObjectFactory<TlBaseObject>::ObjectProxy ObjectProxy;
    ObjectProxy* obj = reinterpret_cast<ObjectProxy*>(x);
    return obj->impl;
}

static void tl_action(TimelineData* x, int v)
{
    if (v)
        toObj(x->object())->onCueEnter();
    else
        toObj(x->object())->onCueLeave();
}

static void tl_displace(t_gobj* z, _glist* glist, int dx, int dy)
{
    // code from g_text.c
    t_text* x = (t_text*)z;
    x->te_xpix += dx;
    x->te_ypix += dy;
    if (glist_isvisible(glist)) {
        t_rtext* y = glist_findrtext(glist, x);
        rtext_displace(y, dx, dy);
        text_drawborder(x, glist, rtext_gettag(y), rtext_width(y), rtext_height(y), 0);
        canvas_fixlinesfor(glist, x);
    }

    // update pos
    toObj((t_object*)x)->updatePos();
}

TlBaseObject::TlBaseObject(const PdArgs& args)
    : BaseObject(args)
    , tl_(new TimelineData(canvas(), owner()))
    , update_(this, &TlBaseObject::updatePos)
{
    createOutlet();

    tl_->setAction(&tl_action);
    UIStorage::add(tl_.get());
    update_.delay(0);

    createCbSymbolProperty("@cue",
        [this]() -> t_symbol* {
            auto data = tl_->findCue();
            if (!data)
                return &s_;

            return gensym(data->name().c_str());
        });
}

TlBaseObject::~TlBaseObject()
{
    UIStorage::remove(tl_.get());
}

void TlBaseObject::updatePos()
{
    tl_->setXPos(owner()->te_xpix);
}

void TlBaseObject::setDisplaceFn(_class* c)
{
    static t_widgetbehavior wb = *c->c_wb;
    wb.w_displacefn = tl_displace;
    class_setwidget(c, &wb);
}
