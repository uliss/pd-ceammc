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
#include "tl_cue_new.h"
#include "ceammc_ui.h"

using namespace tl;

static const int CUE_Y_POS = 2;

static void tl_cue_displace(t_gobj* z, t_glist* glist, int dx, int dy)
{
    t_ebox* x = (t_ebox*)z;
    x->b_rect.y = CUE_Y_POS;
    x->b_obj.o_obj.te_ypix = CUE_Y_POS;
    ebox_wdisplace(z, glist, dx, 0);

    reinterpret_cast<TlCue*>(x)->updatePos();
}

TlCue::TlCue()
    : data_(canvas(), asPdObject())
    , font_(gensym(FONT_FAMILY), FONT_SIZE)
    , txt_(font_.font(), ColorRGBA::black(), ETEXT_LEFT, ETEXT_JLEFT, ETEXT_NOWRAP)
    , first_draw_(true)
{
    CueStorage::add(&data_);
}

TlCue::~TlCue()
{
    CueStorage::remove(&data_);
    CueStorage::sort(canvas());
    CueStorage::enumerate(canvas());
    redrawCues();
}

void TlCue::init(t_symbol* name, const AtomList& args, bool usePresets)
{
    UIObject::init(name, args, usePresets);
    auto ebox = asEBox();
    ebox->b_rect.y = CUE_Y_POS;
    ebox->b_obj.o_obj.te_ypix = CUE_Y_POS;
}

void TlCue::okSize(t_rect* newrect)
{
    newrect->height = 15;
    newrect->width = 45;

    auto x = asEBox();
    x->b_rect.y = CUE_Y_POS;
    x->b_obj.o_obj.te_ypix = CUE_Y_POS;
}

void TlCue::setDrawParams(t_object* obj, t_edrawparams* params)
{
    UIObject::setDrawParams(obj, params);
    params->d_borderthickness = CUE_Y_POS;
}

void TlCue::paint(t_object* view)
{
    if (first_draw_) {
        first_draw_ = false;
        data_.setXPos(x());

        if (y() != CUE_Y_POS)
            ebox_pos(asEBox(), x(), CUE_Y_POS);

        if (!isLayoutFinished())
            return;

        updateCues();
        redrawCues();
    }

    UIPainter p = bg_layer_.painter(rect());
    if (p) {
        txt_.set(data_.name().c_str(), 3, height() / 2, width(), height());
        p.drawText(txt_);
    }
}

void TlCue::setup()
{
    UIObjectFactory<TlCue> obj("tl.ncue", EBOX_GROWNO | EBOX_IGNORELOCKCLICK, CLASS_NOINLET);

    obj.setDefaultSize(45, 15);
    obj.hideProperty("size");
    obj.hideProperty("send");
    obj.hideProperty("receive");

    obj.setPropertyDefaultValue(PROP_BORDER_COLOR, DEFAULT_ACTIVE_COLOR);

    obj.pd_class->c_widget.w_displacefn = tl_cue_displace;
}

void TlCue::updatePos()
{
    if (updateCues())
        redrawCues();
}

bool TlCue::updateCues()
{
    CueList* lst = CueStorage::cueList(canvas());
    if (lst == 0)
        return false;

    // we do need update all
    for (size_t i = 0; i < lst->size(); i++) {
        void* obj = lst->at(i)->object();
        TlCue* c = reinterpret_cast<TlCue*>(obj);
        c->data_.setXPos(c->x());
    }

    CueStorage::sort(canvas());
    // return true if enumeration changed
    return CueStorage::enumerate(canvas());
}

void TlCue::redrawCues()
{
    CueList* lst = CueStorage::cueList(canvas());
    if (lst == 0)
        return;

    for (size_t i = 0; i < lst->size(); i++) {
        TlCue* c = reinterpret_cast<TlCue*>(lst->at(i)->object());
        c->bg_layer_.invalidate();
        c->redrawInnerArea();
    }
}

bool TlCue::isLayoutFinished()
{
    CueList* lst = CueStorage::cueList(canvas());
    for (size_t i = 0; i < lst->size(); i++) {
        TlCue* c = reinterpret_cast<TlCue*>(lst->at(i)->object());
        if (c->x() == 0.f)
            return false;
    }

    return true;
}

void setup_tl_cue()
{
    TlCue::setup();
}
