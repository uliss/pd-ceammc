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

static t_symbol* SYM_BPF;
static t_symbol* SYM_BPFQ;
static t_symbol* SYM_HIGHSHELF;
static t_symbol* SYM_HPF;
static t_symbol* SYM_LIN;
static t_symbol* SYM_LOG10;
static t_symbol* SYM_LOWSHELF;
static t_symbol* SYM_LPF;
static t_symbol* SYM_NOTCH;
static t_symbol* SYM_PEAK_EQ;
static t_symbol* SYM_RAD;

static t_symbol* PROP_FREQ;
static t_symbol* PROP_GAIN;
static t_symbol* PROP_Q;
static t_symbol* PROP_TYPE;
static t_symbol* PROP_SCALE;

constexpr int MIN_LIN_FREQ = 0;
constexpr int MAX_LIN_FREQ = 20000;
constexpr float MIN_LOG10_FREQ = 10;
constexpr float MAX_LOG10_FREQ = 20000;
constexpr float MIN_Q = 0.5;
constexpr float MAX_Q = 32;

static_assert(std::is_same<UIFilter::Array, flt::ArrayBA<double>>::value, "same arrays");

UIFilter::UIFilter()
    : b0_(1)
    , b1_(0)
    , b2_(0)
    , a1_(0)
    , a2_(0)
    , knob_pt_ {}
    , prop_type(SYM_LPF)
    , prop_scale(SYM_LIN)
    , prop_freq(1000)
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
    const auto Fs = sys_getsr();
    const auto w = flt::freq2ang<float>(prop_freq, Fs);

    const auto bw = calcBandwidth(q(), w);

    sys_vgui("ui::filter_update %s %lx %d %d %d "
             "#%6.6x #%6.6x #%6.6x #%6.6x #%6.6x "
             "%f %f %f %f %f "
             "%.2f %.2f {%s} {%s} "
             "%.1f %f\n",
        asEBox()->b_canvas_id->s_name, asEBox(), (int)width(), (int)height(), (int)zoom(),
        rgba_to_hex_int(prop_color_background),
        rgba_to_hex_int(prop_color_grid),
        rgba_to_hex_int(prop_color_label),
        rgba_to_hex_int(prop_color_plot),
        rgba_to_hex_int(prop_color_knob),
        b0_, b1_, b2_, a1_, a2_,
        knob_pt_.x * width(), knob_pt_.y * height(),
        prop_scale->s_name, prop_type->s_name,
        q(), bw);
}

void UIFilter::onList(const AtomListView& lv)
{
    if (lv.size() != 5)
        return;

    b0_ = lv[0].asFloat();
    b1_ = lv[1].asFloat();
    b2_ = lv[2].asFloat();
    a1_ = lv[3].asFloat();
    a2_ = lv[4].asFloat();

    redraw();
}

void UIFilter::onMouseDown(t_object* view, const t_pt& pt, const t_pt& /*abs_pt*/, long /*modifiers*/)
{
    saveMousePoint(pt);
    updateCoeffs();
    redraw();
    output();
}

void UIFilter::onMouseWheel(const t_pt& pt, long /*modifiers*/, float delta)
{
    if (prop_type == SYM_PEAK_EQ) {
        prop_peakq = clip<t_float>(prop_peakq * (1 + delta * 0.05), MIN_Q, MAX_Q);
        updateCoeffs();
        redraw();
        output();
    }
}

void UIFilter::onPropChange(t_symbol* name)
{
    if (name == PROP_FREQ
        || name == PROP_Q
        || name == PROP_GAIN
        || name == PROP_TYPE)
        updateCoeffs();

    if (name == PROP_Q) {
        qUpdateKnob();
        updateCoeffs();
    } else if (name == PROP_GAIN) {
        gainUpdateKnob();
        updateCoeffs();
    } else if (name == PROP_FREQ) {
        freqUpdateKnob();
        updateCoeffs();
    } else if (name == PROP_TYPE) {
        updateCoeffs();
    } else if (name == PROP_SCALE) {
        freqUpdateKnob();
        updateCoeffs();
    }

    redraw();
}

void UIFilter::onMouseUp(t_object* /*view*/, const t_pt& pt, long /*modifiers*/)
{
    saveMousePoint(pt);
    updateCoeffs();
    redraw();
    output();
}

void UIFilter::onMouseDrag(t_object* /*view*/, const t_pt& pt, long /*modifiers*/)
{
    saveMousePoint(pt);
    updateCoeffs();
    redraw();
    output();
}

void UIFilter::updateCoeffs()
{
    auto Fs = sys_getsr();
    auto w = flt::freq2ang<float>(prop_freq, Fs);

    if (prop_type == SYM_NOTCH) {
        auto c = flt::calc_notch<double>(w, q());
        setBA(c);
    } else if (prop_type == SYM_LPF) {
        auto c = flt::calc_lpf<double>(w, q());
        setBA(c);
    } else if (prop_type == SYM_HPF) {
        auto c = flt::calc_hpf<double>(w, q());
        setBA(c);
    } else if (prop_type == SYM_PEAK_EQ) {
        auto c = flt::calc_peak_eq<double>(w, q(), calcDb());
        setBA(c);
    } else if (prop_type == SYM_LOWSHELF) {
        auto c = flt::calc_lowshelf<double>(w, calcDb(), 1);
        setBA(c);
    } else if (prop_type == SYM_HIGHSHELF) {
        auto c = flt::calc_highshelf<double>(w, calcDb(), 1);
        setBA(c);
    } else if (prop_type == SYM_BPFQ) {
        auto c = flt::calc_bpfq<double>(w, q());
        setBA(c);
    } else if (prop_type == SYM_BPF) {
        auto c = flt::calc_bpf<double>(w, q());
        setBA(c);
    }
}

float UIFilter::calcBandwidth(float q, float w) const
{
    float bw = 0;
    if (prop_scale == SYM_RAD)
        bw = flt::q2bandwidth<float>(q, w);
    else
        bw = clip<t_float, MIN_LIN_FREQ, MAX_LIN_FREQ>(flt::q2bandwidth<float>(q, w) * prop_freq);

    return std::isnormal(bw) ? bw : 0;
}

float UIFilter::q() const
{
    if (prop_type == SYM_NOTCH
        || prop_type == SYM_BPFQ
        || prop_type == SYM_BPF) {
        return prop_q;
    } else if (prop_type == SYM_LPF) {
        return std::sqrt(0.5);
    } else if (prop_type == SYM_HPF) {
        return std::sqrt(0.5);
    } else if (prop_type == SYM_PEAK_EQ) {
        return prop_peakq;
    } else
        return 0.1;
}

float UIFilter::calcDb() const
{
    if (prop_type == SYM_PEAK_EQ
        || prop_type == SYM_LOWSHELF
        || prop_type == SYM_HIGHSHELF) {
        return prop_gain;
    } else
        return 0;
}

void UIFilter::saveMousePoint(const t_pt& pt)
{
    knob_pt_.x = clip01<t_float>(pt.x / width());
    knob_pt_.y = clip01<t_float>(pt.y / height());

    knobUpdateFreq();
    knobUpdateGain();
    knobUpdateQ();
}

void UIFilter::output()
{
    Atom res[5] = { b0_, b1_, b2_, a1_, a2_ };
    listTo(0, AtomListView(res, 5));
}

void UIFilter::setBA(const Array& ba)
{
    auto check_nan = [](double v) { return std::isnormal(v) ? v : 0; };

    b0_ = check_nan(ba[0]);
    b1_ = check_nan(ba[1]);
    b2_ = check_nan(ba[2]);
    a1_ = check_nan(ba[4]);
    a2_ = check_nan(ba[5]);
}

void UIFilter::knobUpdateFreq()
{
    if (prop_scale == SYM_LIN) {
        prop_freq = convert::lin2lin<float, 0, 1>(knob_pt_.x, MIN_LIN_FREQ, MAX_LIN_FREQ);
    } else if (prop_scale == SYM_LOG10) {
        static const float loga = std::log10(MIN_LOG10_FREQ);
        static const float logb = std::log10(MAX_LOG10_FREQ);

        const float fp = convert::lin2lin_clip<float, 0, 1>(knob_pt_.x, loga, logb);
        prop_freq = std::pow(10, fp);
    } else if (prop_scale == SYM_RAD) {
        prop_freq = convert::lin2lin_clip<float, 0, 1>(knob_pt_.x, 0, 0.5 * sys_getsr());
    } else {
        UI_ERR << "unknown scale: " << prop_scale;
        prop_freq = 1;
    }
}

void UIFilter::knobUpdateQ()
{
    prop_q = std::pow(2, convert::lin2lin_clip<float>(knob_pt_.y, 0, 1, 6, -6));
}

void UIFilter::knobUpdateGain()
{
    prop_gain = convert::lin2lin_clip<float>(knob_pt_.y, 0, 1, 24, -24);
}

void UIFilter::freqUpdateKnob()
{
    if (prop_scale == SYM_LIN) {
        knob_pt_.x = convert::lin2lin<float, MIN_LIN_FREQ, MAX_LIN_FREQ>(prop_freq, 0, 1);
    } else if (prop_scale == SYM_LOG10) {
        static const float loga = std::log10(MIN_LOG10_FREQ);
        static const float logb = std::log10(MAX_LOG10_FREQ);

        const float fp = std::log10(prop_freq);
        const auto f = convert::lin2lin_clip<float>(fp, loga, logb, 0, 1);
        knob_pt_.x = std::isnormal(f) ? f : 0;
    } else if (prop_scale == SYM_RAD) {
        knob_pt_.x = convert::lin2lin_clip<float>(prop_freq, 0, 0.5 * sys_getsr(), 0, 1);
    } else {
        UI_ERR << "unknown scale: " << prop_scale;
        knob_pt_.x = 0;
    }
}

void UIFilter::qUpdateKnob()
{
    auto y = convert::lin2lin_clip<float, -6, 6>(std::log2(prop_q), 1, 0);
    knob_pt_.y = std::isnormal(y) ? y : 0;
}

void UIFilter::gainUpdateKnob()
{
    knob_pt_.y = convert::lin2lin_clip<float, -24, 24>(prop_gain, 1, 0);
}

void UIFilter::setup()
{
    SYM_BPF = gensym("bpf");
    SYM_BPFQ = gensym("bpfq");
    SYM_HIGHSHELF = gensym("highshelf");
    SYM_HPF = gensym("hpf");
    SYM_LIN = gensym("lin");
    SYM_LOG10 = gensym("log");
    SYM_LOWSHELF = gensym("lowshelf");
    SYM_LPF = gensym("lpf");
    SYM_NOTCH = gensym("notch");
    SYM_PEAK_EQ = gensym("peak");
    SYM_RAD = gensym("rad");

    PROP_FREQ = gensym("freq");
    PROP_Q = gensym("q");
    PROP_GAIN = gensym("gain");
    PROP_TYPE = gensym("type");
    PROP_SCALE = gensym("scale");

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

    obj.addFloatProperty(PROP_FREQ->s_name,
        _("Frequency"), 1000, &UIFilter::prop_freq, _("Main"));
    obj.setPropertyRange(PROP_FREQ->s_name, 0, MAX_LIN_FREQ);
    obj.addFloatProperty(PROP_Q->s_name,
        _("Quality factory"), std::sqrt(0.5), &UIFilter::prop_q, _("Main"));
    obj.addFloatProperty(PROP_GAIN->s_name,
        _("Gain"), 0, &UIFilter::prop_gain, _("Main"));
    obj.addFloatProperty("peakq", _("Peak Q"), 2, &UIFilter::prop_peakq, _("Main"));

    obj.addMenuProperty("type",
        _("Filter Type"),
        "lpf",
        &UIFilter::prop_type,
        "lpf hpf bpf bpfq lowshelf highshelf peak notch",
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
