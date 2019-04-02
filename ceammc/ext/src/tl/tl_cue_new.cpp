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

#include "tl_cue_fn.tcl.h"

#include <inttypes.h>

using namespace tl;

static const int CUE_Y_POS = 2;
static const int LINE_BOTTOM_MARGIN = 5;
static const int LINE_WIDTH = 1;

static void tl_cue_displace(t_gobj* z, t_glist* glist, int dx, int dy)
{
    t_ebox* x = (t_ebox*)z;
    x->b_rect.y = CUE_Y_POS;
    x->b_obj.o_obj.te_ypix = CUE_Y_POS;
    ebox_wdisplace(z, glist, dx, 0);

    reinterpret_cast<TlCue*>(x)->updatePos();
}

size_t TlCue::ref_counter_ = 0;

TlCue::TlCue()
    : data_(canvas(), asPdObject())
    , font_(gensym(FONT_FAMILY), FONT_SIZE)
    , txt_(font_.font(), ColorRGBA::black(), ETEXT_LEFT, ETEXT_JLEFT, ETEXT_NOWRAP)
    , bind_sym_(gensym("tl_cue_canvas_resize"))
    , first_draw_(true)
    , line_created_(false)
{
    CueStorage::add(&data_);

    // canvas can be equal nullptr
    if (canvas() != nullptr) {
        bindTo(bind_sym_);
        if (++ref_counter_ == 1)
            sys_gui("::ceammc::tl::startpolling\n");
    }
}

TlCue::~TlCue()
{
    CueStorage::remove(&data_);
    CueStorage::sort(canvas());
    CueStorage::enumerate(canvas());
    redrawCues();

    if (canvas()) {
        unbindFrom(bind_sym_);

        if (--ref_counter_ == 0)
            sys_gui("::ceammc::tl::stoppolling\n");
    }

    deleteLine();
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
    // update all cues on first draw
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

    createLine();
}

void TlCue::updatePos()
{
    if (updateCues())
        redrawCues();

    updateLine();
}

void TlCue::onZoom(t_float z)
{
    UIObject::onZoom(z);

    deleteLine();
}

void TlCue::m_updateLine(const AtomList& l)
{
    if (l.symbolAt(0, &s_) != asEBox()->b_canvas_id)
        return;

    updateLine();
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

void TlCue::createLine()
{
    if (line_created_ || !asEBox()->b_canvas_id)
        return;

    sys_vgui("%s create line %d %d %d %d -width %d -fill #%6.6x -tags .x%lx_CUE_LINE\n",
        asEBox()->b_canvas_id->s_name,
        int(x() - 2 * LINE_WIDTH), CUE_Y_POS,
        int(x() - 2 * LINE_WIDTH),
        lineHeight(), LINE_WIDTH,
        rgba_to_hex_int(prop_color_border), this);

    line_created_ = true;
}

void TlCue::deleteLine()
{
    if (!line_created_ || !asEBox()->b_canvas_id)
        return;

    sys_vgui("%s delete .x%lx_CUE_LINE\n", asEBox()->b_canvas_id->s_name, this);
    line_created_ = false;
}

void TlCue::updateLine()
{
    if (!line_created_ || !asEBox()->b_canvas_id)
        return;

    sys_vgui("%s coords .x%lx_CUE_LINE %d %d %d %d\n",
        asEBox()->b_canvas_id->s_name, this,
        int(x() - 2 * LINE_WIDTH), CUE_Y_POS,
        int(x() - 2 * LINE_WIDTH), lineHeight());
}

int TlCue::lineHeight() const
{
    t_canvas* cnv = canvas();
    if (!cnv)
        return 0;

    int res = (cnv->gl_screeny2 - cnv->gl_screeny1) - LINE_BOTTOM_MARGIN;
    if (cnv->gl_edit)
        res -= 32;

    return res;
}

void TlCue::setup()
{
    UIObjectFactory<TlCue> obj("tl.cue", EBOX_GROWNO | EBOX_IGNORELOCKCLICK, CLASS_NOINLET);

    obj.setDefaultSize(45, 15);
    obj.hideProperty("size");
    obj.hideProperty("send");
    obj.hideProperty("receive");

    obj.setPropertyDefaultValue(PROP_BORDER_COLOR, DEFAULT_ACTIVE_COLOR);

    obj.pd_class->c_widget.w_displacefn = tl_cue_displace;
    obj.addMethod("update_line", &TlCue::m_updateLine);
}

void setup_tl_cue()
{
    sys_gui(tl_cue_fn_tcl);
    TlCue::setup();
}
