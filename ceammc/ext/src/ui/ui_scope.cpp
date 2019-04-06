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

#include "ui_scope.h"
#include "ceammc_convert.h"
#include "ceammc_dsp_ui.h"

static const size_t N_SAMPLES = 150;

UIScope::UIScope()
    : data_(N_SAMPLES, 0)
    , redraw_timer_(this, &UIScope::redrawTick)
    , txt_font_(gensym(FONT_FAMILY), FONT_SIZE_SMALL)
    , txt0_(txt_font_.font(), ColorRGBA::black(), ETEXT_UP_LEFT, ETEXT_JLEFT, ETEXT_NOWRAP)
    , txt1_(txt_font_.font(), ColorRGBA::black(), ETEXT_DOWN_LEFT, ETEXT_JLEFT, ETEXT_NOWRAP)
    , txt2_(txt_font_.font(), ColorRGBA::black(), ETEXT_DOWN_LEFT, ETEXT_JLEFT, ETEXT_NOWRAP)
    , txt3_(txt_font_.font(), ColorRGBA::black(), ETEXT_DOWN_LEFT, ETEXT_JLEFT, ETEXT_NOWRAP)
    , txt4_(txt_font_.font(), ColorRGBA::black(), ETEXT_DOWN_LEFT, ETEXT_JLEFT, ETEXT_NOWRAP)
    , scope_layer_(asEBox(), gensym("scope_layer"))
    , freeze_(false)
    , last_redraw_time_(0)
    , window_phase_(0)
    , window_size_(0)
    , prop_color_active(rgba_blue)
    , prop_min(-1)
    , prop_max(1)
    , prop_window(2048)
    , prop_refresh(50)
{
}

void UIScope::okSize(t_rect* newrect)
{
    newrect->height = pd_clip_min(newrect->height, 40);
    newrect->width = pd_clip_min(newrect->width, 40);
}

void UIScope::onPropChange(t_symbol* prop_name)
{
    calcDspVars();

    scope_layer_.invalidate();
    bg_layer_.invalidate();
    redraw();
}

void UIScope::paint()
{
    paintBackground();
    paintScope();
}

void UIScope::paintBackground()
{
    const t_rect r = rect();
    UIPainter p = bg_layer_.painter(r);

    if (!p)
        return;

    updateLabels();

    // first and last labels
    p.drawText(txt0_);
    p.drawText(txt4_);

    // middle label
    if (r.height > 70)
        p.drawText(txt2_);

    if (r.height > 140) {
        p.drawText(txt1_);
        p.drawText(txt3_);
    }

    // draw guide lines
    p.setColor(prop_color_border);
    p.drawLine(0, r.height * 0.25, r.width, r.height * 0.25);
    p.drawLine(0, r.height * 0.5, r.width, r.height * 0.5);
    p.drawLine(0, r.height * 0.75, r.width, r.height * 0.75);
}

void UIScope::paintScope()
{
    // zero range error check
    if (prop_max - prop_min == 0) {
        DSP_ERR << "zero value range";
        return;
    }

    const t_rect r = rect();
    UIPainter p = scope_layer_.painter(r);

    if (!p)
        return;

    p.setColor(prop_color_active);
    p.setLineWidth(1);

    p.moveTo(0, convert::lin2lin<float>(data_[0], prop_min, prop_max, r.height, 0));
    const size_t N = data_.size();
    for (size_t i = 1; i < N; i++) {
        float y = convert::lin2lin<float>(data_[i], prop_min, prop_max, r.height, 0);
        p.drawLineTo(roundf(float(i) / N * r.width), y);
    }

    p.stroke();
}

void UIScope::dspOn(double sr, long bs)
{
    UIDspObject::dspOn(sr, bs);
    calcDspVars();
}

void UIScope::dspProcess(t_sample** ins, long, t_sample**, long, long sampleframes)
{
    if (freeze_)
        return;

    const size_t N = data_.size();
    const size_t PN = std::max<size_t>(window_size_, N);
    const float k = float(N) / PN;

    for (long i = 0; i < sampleframes; i++, window_phase_++) {
        if (window_phase_ >= PN) {
            window_phase_ = 0;

            redraw_timer_.delay(0);
            return;
        }

        size_t idx = roundf(k * window_phase_);
        if (idx < N) {
            data_[idx] = ins[0][i];
        }
    }
}

void UIScope::m_freeze(t_float f)
{
    freeze_ = (f != 0.f);
}

void UIScope::m_scale(t_float f)
{
    if (f <= 0) {
        DSP_ERR << "invalid scale value: " << f;
        return;
    }

    setProperty(gensym("min"), AtomList(f));
    setProperty(gensym("max"), AtomList(-f));
}

void UIScope::onMouseDown(t_object* view, const t_pt& pt, const t_pt& abs_pt, long modifiers)
{
    if (modifiers == EMOD_SHIFT) {
        setProperty(gensym("min"), AtomList(prop_min * 0.8));
        setProperty(gensym("max"), AtomList(prop_max * 0.8));
    } else if (modifiers == EMOD_CTRL) {
        setProperty(gensym("min"), AtomList(prop_min * 1.2));
        setProperty(gensym("max"), AtomList(prop_max * 1.2));
    }
}

void UIScope::onDblClick(t_object* view, const t_pt& pt, long modifiers)
{
    if (modifiers != 0)
        onMouseDown(view, pt, pt, modifiers);
    else
        freeze_ = !freeze_;
}

void UIScope::setup()
{
    UIDspFactory<UIScope> obj("ui.scope~");
    obj.setDefaultSize(150, 100);

    obj.useMouseEvents(UI_MOUSE_DBL_CLICK | UI_MOUSE_DOWN);
    obj.addMethod("freeze", &UIScope::m_freeze);
    obj.addMethod("scale", &UIScope::m_scale);

    obj.addProperty(PROP_ACTIVE_COLOR, _("Active Color"), DEFAULT_ACTIVE_COLOR, &UIScope::prop_color_active);
    obj.addProperty("max", _("Maximum value"), 1, &UIScope::prop_max);
    obj.addProperty("min", _("Minimum value"), -1, &UIScope::prop_min);
    obj.addIntProperty("window", _("Window size"), 2048, &UIScope::prop_window);
    obj.setPropertyRange("window", 512, 4096);
    obj.addIntProperty("refresh", _("Refresh time (ms)"), 40, &UIScope::prop_refresh);
    obj.setPropertyRange("refresh", 10, 1000);

    obj.hideProperty("send");
}

void UIScope::redrawTick()
{
    double t = clock_gettimesince(last_redraw_time_);
    if (t >= prop_refresh) {
        last_redraw_time_ = clock_getlogicaltime();
        scope_layer_.invalidate();
        redrawInnerArea();
    }
}

void UIScope::updateLabels()
{
    updateLabel(txt0_, 1);
    updateLabel(txt1_, 0.75);
    updateLabel(txt2_, 0.5);
    updateLabel(txt3_, 0.25);
    updateLabel(txt4_, 0);
}

void UIScope::updateLabel(UITextLayout& txt, float k)
{
    char buf[64];
    snprintf(buf, 64, "%g", (prop_max - prop_min) * k + prop_min);
    txt.setColor(prop_color_border);
    txt.set(buf, 3, (k == 1) ? 3 : height() * (1 - k) - 2, 100, 20);
}

void UIScope::calcDspVars()
{
    data_.resize(std::max<size_t>(200, width()));
    window_size_ = prop_window;
}

void setup_ui_scope()
{
    UIScope::setup();
}
