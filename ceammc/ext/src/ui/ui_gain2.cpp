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
#include "ui_gain2.h"
#include "ceammc_dsp_ui.h"

UIGain2::UIGain2()
{
}

void UIGain2::init(t_symbol* name, const AtomList& args, bool usePresets)
{
    UIDspObject::init(name, args, usePresets);
    dspSetup(2, 2);
}

void UIGain2::dspProcess(t_sample** ins, long n_ins, t_sample** outs, long n_outs, long sampleframes)
{
    const float v = ampValue();

    for (long i = 0; i < sampleframes; i++) {
        auto amp = smooth_.get(v);
        auto x0 = ins[0][i] * amp;
        auto x1 = ins[1][i] * amp;

        outs[0][i] = x0;
        outs[1][i] = x1;
    }
}

void UIGain2::setup()
{
    UIDspFactory<UIGain2> obj("ui.gain2~");

    obj.addProperty("knob_color", _("Knob Color"), DEFAULT_ACTIVE_COLOR, &UIGain2::prop_color_knob);
    obj.addProperty("db", &UIGain2::dbValue, &UIGain2::setDbValue);
    obj.setPropertyDefaultValue("db", "-60");
    obj.addProperty("amp", &UIGain2::ampValue, &UIGain2::setAmpValue);
    obj.addIntProperty("max", _("Maximum value"), 0, &UIGain2::prop_max);
    obj.addIntProperty("min", _("Minimum value"), -60, &UIGain2::prop_min);
    obj.setPropertyRange("max", -12, 12);
    obj.setPropertyRange("min", -90, -30);
    obj.addBoolProperty("show_range", _("Show range"), true, &UIGain2::prop_show_range);
    obj.addBoolProperty("output_value", _("Output value"), false, &UIGain2::prop_output_value);

    obj.setDefaultSize(15, 120);
    obj.usePresets();
    obj.useBang();

    obj.useMouseEvents(UI_MOUSE_DOWN | UI_MOUSE_DRAG | UI_MOUSE_DBL_CLICK);
    obj.addMethod("+", &UIGain2::m_plus);
    obj.addMethod("-", &UIGain2::m_minus);
    obj.addMethod("++", &UIGain2::m_inc);
    obj.addMethod("--", &UIGain2::m_dec);
    obj.addMethod("set", &UIGain2::setDbValue);
}

void setup_ui_gain2()
{
    UIGain2::setup();
}
