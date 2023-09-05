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
#include "ui_circular_meter.h"
#include "ceammc_convert.h"
#include "ceammc_ui.h"
#include "ui_circular_meter.tcl.h"

#include <boost/container/static_vector.hpp>

constexpr t_float MIN_DB_VALUE = -90;
constexpr int MIN_UI_DB_VALUE = -50;
constexpr int DEF_REFRESH_MS = 100;

UICircularMeter::UICircularMeter()
    : clock_([this]() {
        if (pd_getdspstate()) {
            calc();
            reset();
            clock_.delay(prop_interval_ms);
        }
    })
    , prop_color_cold(rgba_grey)
    , prop_color_tepid(rgba_green)
    , prop_color_warm(rgba_green)
    , prop_color_hot(rgba_red)
    , prop_color_over(rgba_red)
    , prop_interval_ms(DEF_REFRESH_MS)
    , prop_nchan(DEF_NCHAN)
    , prop_offset(DEF_OFFSET)
    , prop_clockwise(0)
    , num_samples_(0)
    , start_clock_(false)
{
    out_rms_.fill(MIN_UI_DB_VALUE);
    out_peak_.fill(MIN_UI_DB_VALUE);
    out_over_.fill(0);
}

void UICircularMeter::init(t_symbol* name, const AtomListView& args, bool usePresets)
{
    UIDspObject::init(name, args, usePresets);

    int n = args.intAt(0, -1);

    if (n > 0) {
        prop_nchan = clip<int>(n, MIN_NCHAN, MAX_NCHAN);
    }

    setInputs(prop_nchan);
}

void UICircularMeter::okSize(t_rect* newrect)
{
    newrect->w = pd_clip_min(newrect->w, 8.);
    newrect->h = pd_clip_min(newrect->h, 8.);
}

void UICircularMeter::paint()
{
    sys_vgui("ui::cm::delete #%x %s\n",
        asEBox(), asEBox()->b_drawing_id->s_name);

    sys_vgui("ui::cm::create #%x %s "
             "%d %d "
             "#%6.6x #%6.6x #%6.6x #%6.6x #%6.6x #%6.6x "
             "%d %d %d",
        asEBox(), asEBox()->b_drawing_id->s_name,
        (int)width(), (int)height(),
        rgba_to_hex_int(prop_color_border),
        rgba_to_hex_int(prop_color_cold),
        rgba_to_hex_int(prop_color_tepid),
        rgba_to_hex_int(prop_color_warm),
        rgba_to_hex_int(prop_color_hot),
        rgba_to_hex_int(prop_color_over),
        prop_nchan, prop_offset, prop_clockwise);

    if (prop_angles.size() != prop_nchan)
        propSetAngles(prop_angles);

    sys_vgui(" [list ");
    for (int i = 0; i < prop_nchan; i++) {
        sys_vgui(" %0.1f ", prop_angles.floatAt(i, 0));
    }
    sys_vgui("] [list ");
    for (int i = 0; i < prop_nchan; i++) {
        sys_vgui(" %d %d %d", (int)out_rms_[i], (int)out_peak_[i], (int)out_over_[i]);
    }

    sys_vgui("]\n");
}

void UICircularMeter::dspInit()
{
    dspSetup(prop_nchan, 0);
}

void UICircularMeter::dspOn(double samplerate, long blocksize)
{
    reset();
    start_clock_ = true;
    UIDspObject::dspOn(samplerate, blocksize);
}

void UICircularMeter::dspProcess(t_sample** ins, long n_ins, t_sample** outs, long n_outs, long sampleframes)
{
    const size_t BS = blocksize();

    for (long ch = 0; ch < n_ins; ch++) {
        for (size_t k = 0; k < BS; k++) {
            auto s = ins[ch][k];
            // find peak
            double t = fabs(s);
            if (t > raw_peak_[ch])
                raw_peak_[ch] = t;

            raw_sum2_[ch] += double(s) * double(s);
        }
    }

    num_samples_ += BS;

    if (start_clock_) {
        start_clock_ = false;
        clock_.delay(0);
    }
}

void UICircularMeter::propSetNumChan(t_int n)
{
    prop_nchan = clip<int>(n, MIN_NCHAN, MAX_NCHAN);
    auto state = canvas_suspend_dsp();
    eobj_resize_inputs(asEObj(), prop_nchan);
    canvas_update_dsp();
    canvas_resume_dsp(state);
}

void UICircularMeter::propSetOffset(t_int off)
{
    prop_offset = clip<int>(off, MIN_OFFSET, MAX_OFFSET);
    auto state = canvas_suspend_dsp();
    eobj_resize_inputs(asEObj(), prop_nchan);
    canvas_update_dsp();
    canvas_resume_dsp(state);
}

void UICircularMeter::propSetAngles(const AtomListView& lv)
{
    if (!lv.allOf([](const Atom& a) {
            if (!a.isFloat())
                return false;

            auto f = a.asT<t_float>();
            return f >= -360 && f <= 360;
        })) {
        UI_ERR << "invalid angles: " << lv;
        return;
    }

    using StaticAngleList = boost::container::static_vector<float, MAX_NCHAN>;
    StaticAngleList lst;
    for (auto& a : lv)
        lst.push_back(std::fmod(a.asT<t_float>() + 360, 360));

    std::sort(lst.begin(), lst.end());
    int left = prop_nchan - int(lst.size());
    if (left > 0) {
        auto start = lst.empty() ? 0 : lst.back();
        auto segm = (360.f - start) / (left + 1);
        for (int i = 1; i <= left; i++) {
            lst.push_back(start + segm * i);
        }
    }

    lst.resize(prop_nchan);
    prop_angles.clear();
    for (auto f : lst)
        prop_angles.push_back(f);
}

void UICircularMeter::calc()
{
    for (int i = 0; i < prop_nchan && i < MAX_NCHAN; i++) {
        const double rms = num_samples_ ? sqrt(raw_sum2_[i] / num_samples_) : 0;
        // clip to min UI value: -50db
        out_rms_[i] = clip_min<float, MIN_UI_DB_VALUE>((rms > 0) ? convert::amp2dbfs(rms) : MIN_DB_VALUE);
        auto new_peak = clip_min<float, MIN_UI_DB_VALUE>((raw_peak_[i] > 0) ? convert::amp2dbfs(raw_peak_[i]) : MIN_DB_VALUE);
        out_peak_[i] = new_peak;

        if (new_peak >= 0)
            out_over_[i] = 1; // start overload countter
        else if (out_over_[i] > 0)
            out_over_[i]++;

        // reset overload after 1 second
        if (out_over_[i] >= (1000. / prop_interval_ms))
            out_over_[i] = 0;
    }

    redraw();
}

void UICircularMeter::reset()
{
    raw_peak_.fill(0);
    raw_sum2_.fill(0);
    out_over_.fill(MIN_DB_VALUE);
    num_samples_ = 0;
}

void UICircularMeter::setInputs(int n)
{
    auto dsp_state = canvas_suspend_dsp();
    prop_nchan = clip<int, MIN_NCHAN, MAX_NCHAN>(n);
    eobj_resize_inputs(asEObj(), prop_nchan);
    canvas_update_dsp();
    canvas_resume_dsp(dsp_state);
}

void UICircularMeter::setup()
{
    ui_circular_meter_tcl_output();

    UIObjectFactory<UICircularMeter> obj("ui.cmeter~", EBOX_GROWLINK);
    obj.addAlias("ui.cm~");
    //    obj.useMouseEvents(UI_MOUSE_DBL_CLICK);

    obj.hideLabelInner();

    obj.setDefaultSize(30, 120);

    obj.addColorProperty("cold_color", _("Cold signal color"), "0 0.6 0 1", &UICircularMeter::prop_color_cold);
    obj.addColorProperty("tepid_color", _("Tepid signal color"), "0.6 0.73 0 1", &UICircularMeter::prop_color_tepid);
    obj.addColorProperty("warm_color", _("Warm signal color"), ".85 .85 0 1", &UICircularMeter::prop_color_warm);
    obj.addColorProperty("hot_color", _("Hot signal color"), "1 0.6 0 1", &UICircularMeter::prop_color_hot);
    obj.addColorProperty("over_color", _("Overload signal color"), "1 0 0 1", &UICircularMeter::prop_color_over);

    obj.addIntProperty("interval", _("Refresh interval (ms)"), DEF_REFRESH_MS, &UICircularMeter::prop_interval_ms, _("Main"));
    obj.setPropertyMin("interval", 20);
    obj.setPropertyUnits("interval", "msec");

    obj.addIntProperty("nch", _("Number of channels"), DEF_NCHAN, &UICircularMeter::prop_nchan, _("Main"));
    obj.setPropertyRange("nch", MIN_NCHAN, MAX_NCHAN);
    obj.setPropertyAccessor("nch", &UICircularMeter::propNumChan, &UICircularMeter::propSetNumChan);

    obj.addIntProperty("offset", _("Channel offset"), DEF_OFFSET, &UICircularMeter::prop_offset, _("Main"));
    obj.setPropertyRange("offset", MIN_OFFSET, MAX_OFFSET);
    obj.setPropertyAccessor("offset", &UICircularMeter::propOffset, &UICircularMeter::propSetOffset);

    obj.addBoolProperty("clockwise", _("Clockwise"), false, &UICircularMeter::prop_clockwise, _("Main"));

    obj.addProperty("angles", &UICircularMeter::propAngles, &UICircularMeter::propSetAngles);
    obj.showProperty("angles");
    obj.setPropertyCategory("angles", "Main");
    obj.setPropertyLabel("angles", _("Angles"));
    obj.setPropertySave("angles");
}

void setup_ui_circular_meter()
{
    UICircularMeter::setup();
}
