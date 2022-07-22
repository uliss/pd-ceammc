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
#include "ceammc_ui.h"

UIGain2::UIGain2()
{
}

void UIGain2::init(t_symbol* name, const AtomListView& args, bool usePresets)
{
    UIDspObject::init(name, args, usePresets);

    if (name == gensym("ui.hgain2~"))
        initHorizontal();

    dspSetup(2, 2);
}

void UIGain2::dspProcess(t_sample** ins, long n_ins, t_sample** outs, long n_outs, long sampleframes)
{
#define COPY_SAMPLE(offset)                               \
    {                                                     \
        auto amp = smooth_.get(v);                        \
        auto x0 = ins[0][i + offset] * amp;               \
        auto x1 = ins[1][i + offset] * amp;               \
        outs[0][i + offset] = std::isnormal(x0) ? x0 : 0; \
        outs[1][i + offset] = std::isnormal(x1) ? x1 : 0; \
    }

    const t_float v = ampValue();

    for (long i = 0; i < sampleframes; i += 8) {
        COPY_SAMPLE(0);
        COPY_SAMPLE(1);
        COPY_SAMPLE(2);
        COPY_SAMPLE(3);
        COPY_SAMPLE(4);
        COPY_SAMPLE(5);
        COPY_SAMPLE(6);
        COPY_SAMPLE(7);
    }

#undef COPY_SAMPLE
}

void UIGain2::setup()
{
    static t_symbol* SYM_DB = gensym("db");
    static t_symbol* SYM_MAX = gensym("max");
    static t_symbol* SYM_MIN = gensym("min");

    UIObjectFactory<UIGain2> obj("ui.gain2~");
    obj.addAlias("ui.vgain2~");
    obj.addAlias("ui.hgain2~");

    obj.addColorProperty("knob_color", _("Knob Color"), DEFAULT_ACTIVE_COLOR, &UIGain2::prop_color_knob);
    obj.addHiddenFloatCbProperty("db", &UIGain2::dbValue, &UIGain2::setDbValue);
    obj.setPropertyDefaultValue("db", "-60");
    obj.setPropertyUnits(SYM_DB, SYM_DB);
    obj.addHiddenFloatCbProperty("amp", &UIGain2::ampValue, &UIGain2::setAmpValue);
    obj.addIntProperty("max", _("Maximum value"), 0, &UIGain2::prop_max, _("Bounds"));
    obj.addIntProperty("min", _("Minimum value"), -60, &UIGain2::prop_min, _("Bounds"));
    obj.setPropertyRange("max", -12, 12);
    obj.setPropertyRange("min", -90, -15);
    obj.setPropertyUnits(SYM_MAX, SYM_DB);
    obj.setPropertyUnits(SYM_MIN, SYM_DB);
    obj.addBoolProperty("show_range", _("Show range"), true, &UIGain2::prop_show_range, _("Misc"));
    obj.addBoolProperty("output_value", _("Output value"), false, &UIGain2::prop_output_value, _("Main"));
    obj.addBoolProperty("relative", _("Relative mode"), true, &UIGain2::prop_relative_mode, _("Main"));

    obj.addProperty("midi_channel", _("MIDI channel"), 0, &UIGain2::prop_midi_chn, "MIDI");
    obj.setPropertyRange("midi_channel", 0, 16);
    obj.addProperty("midi_control", _("MIDI control"), -1, &UIGain2::prop_midi_ctl, "MIDI");
    obj.setPropertyRange("midi_control", -1, 127);
    obj.addProperty("midi_pickup", _("MIDI pickup"), true, &UIGain2::prop_pickup_midi, "MIDI");

    obj.setDefaultSize(15, 120);
    obj.usePresets();
    obj.useBang();

    obj.useMouseEvents(UI_MOUSE_DOWN | UI_MOUSE_DRAG | UI_MOUSE_DBL_CLICK | UI_MOUSE_WHEEL);
    obj.outputMouseEvents(MouseEventsOutput::DEFAULT_OFF);
    obj.usePopup();

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
