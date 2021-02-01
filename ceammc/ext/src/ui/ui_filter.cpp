/*****************************************************************************
 * Copyright 2021 Serge Poltavsky. All rights reserved.
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
#include "ui_filter.h"
#include "ceammc_convert.h"
#include "ceammc_filter.h"
#include "ceammc_ui.h"
#include "ui_filter.tcl.h"

static t_symbol* SYM_NOTCH;
static t_symbol* SYM_LPF;
static t_symbol* SYM_HPF;
static t_symbol* SYM_PEAK_EQ;
static t_symbol* SYM_LOWSHELF;
static t_symbol* SYM_HIGHSHELF;
static t_symbol* SYM_LIN;
static t_symbol* SYM_LOG10;

constexpr int MIN_LIN_FREQ = 0;
constexpr int MAX_LIN_FREQ = 20000;

static_assert(std::is_same<UIFilter::Array, flt::ArrayBA<double>>::value, "same arrays");

UIFilter::UIFilter()
    : b0_(1)
    , b1_(0)
    , b2_(0)
    , a1_(0)
    , a2_(0)
    , prop_type(SYM_LPF)
    , freq_pt_ {}
    , peak_q_(2)
{
    createOutlet();
}

bool UIFilter::okSize(t_rect* newrect)
{
    newrect->width = pd_clip_min(newrect->width, 100);
    newrect->height = pd_clip_min(newrect->height, 60);
    return true;
}

void UIFilter::paint()
{
    const auto f = calcFrequency();
    const auto Fs = sys_getsr();
    const auto w = flt::freq2ang<float>(f, Fs);

    const auto q = calcQ();
    auto bw = clip<t_float, MIN_LIN_FREQ, MAX_LIN_FREQ>(flt::q2bandwidth<float>(q, w) * f);
    if (!std::isnormal(bw))
        bw = 0;

    sys_vgui("ui::filter_update %s %lx %d %d %d "
             "#%6.6x #%6.6x #%6.6x #%6.6x #%6.6x "
             "%f %f %f %f %f "
             "%.2f %.2f {%s} {%s} "
             "%.1f %d\n",
        asEBox()->b_canvas_id->s_name, asEBox(), (int)width(), (int)height(), (int)zoom(),
        rgba_to_hex_int(prop_color_background),
        rgba_to_hex_int(prop_color_grid),
        rgba_to_hex_int(prop_color_label),
        rgba_to_hex_int(prop_color_plot),
        rgba_to_hex_int(prop_color_knob),
        b0_, b1_, b2_, a1_, a2_,
        freq_pt_.x * width(), freq_pt_.y * height(),
        prop_scale->s_name, prop_type->s_name,
        q, (int)bw);
}

void UIFilter::onList(const AtomListView& lv)
{
    if (lv.size() != 5) {
        return;
    }

    b0_ = lv[0].asFloat();
    b1_ = lv[1].asFloat();
    b2_ = lv[2].asFloat();
    a1_ = lv[3].asFloat();
    a2_ = lv[4].asFloat();

    redraw();
}

void UIFilter::onMouseDown(t_object* view, const t_pt& pt, const t_pt& abs_pt, long modifiers)
{
    saveMousePoint(pt);
    calc();
    redraw();
    output();
}

void UIFilter::onMouseWheel(const t_pt& pt, long modifiers, float delta)
{
    constexpr t_float min_q = 0.5;
    constexpr t_float max_q = 32;

    if (prop_type == SYM_PEAK_EQ) {
        peak_q_ = clip<t_float>(peak_q_ * (1 + delta * 0.05), min_q, max_q);
        calc();
        redraw();
        output();
    }
}

void UIFilter::onMouseUp(t_object* view, const t_pt& pt, long modifiers)
{
    saveMousePoint(pt);
    calc();
    redraw();
    output();
}

void UIFilter::onMouseDrag(t_object* view, const t_pt& pt, long modifiers)
{
    saveMousePoint(pt);
    calc();
    redraw();
    output();
}

void UIFilter::calc()
{
    auto f = calcFrequency();
    auto Fs = sys_getsr();
    auto w = flt::freq2ang<float>(f, Fs);

    if (prop_type == SYM_NOTCH) {
        auto c = flt::calc_notch<double>(w, calcQ());
        setBA(c);
    } else if (prop_type == SYM_LPF) {
        auto c = flt::calc_lpf<double>(w, calcQ());
        setBA(c);
    } else if (prop_type == SYM_HPF) {
        auto c = flt::calc_hpf<double>(w, calcQ());
        setBA(c);
    } else if (prop_type == SYM_PEAK_EQ) {
        auto c = flt::calc_peak_eq<double>(w, calcQ(), calcDb());
        setBA(c);
    } else if (prop_type == SYM_LOWSHELF) {
        auto c = flt::calc_lowshelf<double>(w, calcDb(), 1);
        setBA(c);
    } else if (prop_type == SYM_HIGHSHELF) {
        auto c = flt::calc_highshelf<double>(w, calcDb(), 1);
        setBA(c);
    }
}

float UIFilter::calcFrequency() const
{
    if (prop_scale == SYM_LIN) {
        constexpr float fmin = 0;
        constexpr float fmax = 20000;
        return freq_pt_.x * (fmax - fmin) + fmin;
    } else if (prop_scale == SYM_LOG10) {
        constexpr float fmin = 10;
        constexpr float fmax = 20000;
        constexpr float fnyq = 22100;

        static const float loga = std::log10(fmin);
        static const float logb = std::log10(fmax);
        static const float logr = logb - loga;

        const float fp = (freq_pt_.x * logr) + loga;
        return std::pow(10, fp);
    } else {
        UI_ERR << "unknown scale: " << prop_scale;
        return 1;
    }
}

float UIFilter::calcQ() const
{
    if (prop_type == SYM_NOTCH) {
        auto p2 = convert::lin2lin_clip<float>(freq_pt_.y, 0, 1, 6, -6);
        return std::pow(2, p2);
    } else if (prop_type == SYM_LPF) {
        return std::sqrt(0.5);
    } else if (prop_type == SYM_HPF) {
        return std::sqrt(0.5);
    } else if (prop_type == SYM_PEAK_EQ) {
        return peak_q_;
    } else
        return 0.1;
}

float UIFilter::calcDb() const
{
    if (prop_type == SYM_PEAK_EQ
        || prop_type == SYM_LOWSHELF
        || prop_type == SYM_HIGHSHELF) {
        return convert::lin2lin_clip<float>(freq_pt_.y, 0, 1, 24, -24);
    } else
        return 0;
}

void UIFilter::saveMousePoint(const t_pt& pt)
{
    freq_pt_.x = clip01<t_float>(pt.x / width());
    freq_pt_.y = clip01<t_float>(pt.y / height());
}

void UIFilter::output()
{
    Atom res[5] = { b0_, b1_, b2_, a1_, a2_ };
    listTo(0, AtomListView(res, 5));
}

void UIFilter::setBA(const Array& ba)
{
    b0_ = ba[0];
    b1_ = ba[1];
    b2_ = ba[2];
    a1_ = ba[4];
    a2_ = ba[5];
}

void UIFilter::setup()
{
    SYM_NOTCH = gensym("notch");
    SYM_LPF = gensym("lpf");
    SYM_HPF = gensym("hpf");
    SYM_PEAK_EQ = gensym("peak");
    SYM_LOWSHELF = gensym("lowshelf");
    SYM_HIGHSHELF = gensym("highshelf");
    SYM_LIN = gensym("lin");
    SYM_LOG10 = gensym("log");

    sys_gui(ui_filter_tcl);

    UIObjectFactory<UIFilter> obj("ui.filter", EBOX_GROWINDI);
    obj.hideLabelInner();

    obj.useList();
    obj.setDefaultSize(300, 100);
    obj.useMouseEvents(UI_MOUSE_DOWN | UI_MOUSE_UP | UI_MOUSE_DRAG | UI_MOUSE_WHEEL);
    obj.outputMouseEvents(MouseEventsOutput::DEFAULT_OFF);

    obj.addProperty("grid_color", _("Grid Color"), DEFAULT_BORDER_COLOR, &UIFilter::prop_color_grid);
    obj.addProperty("plot_color", _("Plot Color"), "0 0 0 1", &UIFilter::prop_color_plot);
    obj.addProperty("knob_color", _("Knob Color"), DEFAULT_ACTIVE_COLOR, &UIFilter::prop_color_knob);
    obj.setPropertyDefaultValue("label_color", DEFAULT_BORDER_COLOR);

    obj.addMenuProperty("type",
        _("Filter Type"),
        "notch",
        &UIFilter::prop_type,
        "lpf hpf bpf lowshelf highshelf peak notch",
        _("Main"));

    obj.addMenuProperty("scale",
        _("Scale"),
        "lin",
        &UIFilter::prop_scale,
        "lin log log2 rad",
        _("Main"));
}

void setup_ui_filter()
{
    UIFilter::setup();
}
