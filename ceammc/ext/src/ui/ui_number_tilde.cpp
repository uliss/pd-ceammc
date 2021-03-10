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
#include "ui_number_tilde.h"
#include "ceammc_ui.h"
#include "ui_number_tilde.tcl.h"

constexpr int MIN_PRECISION = 0;
constexpr int MAX_PRECISION = 16;
constexpr int DEF_PRECISION = 6;

UINumberTilde::UINumberTilde()
    : clock_([this]() { redraw(); })
    , value_(0)
    , last_redraw_time_(0)
    , text_value_()
    , prop_color_text(rgba_black)
    , prop_color_active(rgba_blue)
    , prop_max_decimal(-1)
    , prop_interval(50)
{
}

void UINumberTilde::init(t_symbol* name, const AtomListView &args, bool usePresets)
{
    UIDspObject::init(name, args, usePresets);
    dspSetup(1, 1);
}

void UINumberTilde::okSize(t_rect* newrect)
{
    newrect->height = 15;
    newrect->width = pd_clip_min(newrect->width, sys_fontwidth(fontSizeZoomed()) * 3 + 8);
}

void UINumberTilde::paint()
{
    sys_vgui("::ui::number_update %s %lx "
             "%d %d %d #%6.6x #%6.6x #%6.6x {%s}\n",
        asEBox()->b_canvas_id->s_name, asEBox(),
        (int)width(), (int)height(), (int)zoom(),
        rgba_to_hex_int(prop_color_border), rgba_to_hex_int(prop_color_active), rgba_to_hex_int(prop_color_text),
        text_value_);
}

void UINumberTilde::dspProcess(t_sample** ins, long n_ins, t_sample** outs, long n_outs, long sampleframes)
{
    t_sample* in = ins[0];
    t_sample* out = outs[0];

    value_ = in[0];

    for (long i = 0; i < sampleframes; i += 8) {
        out[i + 0] = in[i + 0];
        out[i + 1] = in[i + 1];
        out[i + 2] = in[i + 2];
        out[i + 3] = in[i + 3];
        out[i + 4] = in[i + 4];
        out[i + 5] = in[i + 5];
        out[i + 6] = in[i + 6];
        out[i + 7] = in[i + 7];
    }

    updateTextValue();
}

/// note: called from DSP
void UINumberTilde::updateTextValue()
{
    double t = clock_gettimesince(last_redraw_time_);
    if (t > prop_interval) {
        char buf[BUFSIZE] = "";
        sprintf(buf, "%.*f", prop_max_decimal, value_);

        if (strcmp(buf, text_value_) != 0) {
            memcpy(text_value_, buf, BUFSIZE);
            last_redraw_time_ = clock_getlogicaltime();
            clock_.delay(0);
        }
    }
}

void UINumberTilde::setup()
{
    sys_gui(ui_number_tilde_tcl);

    UIObjectFactory<UINumberTilde> obj("ui.number~", EBOX_GROWINDI | EBOX_IGNORELOCKCLICK);
    obj.addAlias("ui.n~");
    obj.setDefaultSize(80, 15);
    obj.hideLabelInner();

    obj.addColorProperty(PROP_TEXT_COLOR, _("Text color"), "0.9 0.9 0.9 1", &UINumberTilde::prop_color_text);
    obj.addColorProperty(PROP_ACTIVE_COLOR, _("Text color"), DEFAULT_ACTIVE_COLOR, &UINumberTilde::prop_color_active);
    obj.addIntProperty("precision", _("Decimal precision"), DEF_PRECISION, &UINumberTilde::prop_max_decimal, _("Main"));
    obj.setPropertyRange("precision", MIN_PRECISION, MAX_PRECISION);

    obj.addIntProperty("interval", _("Refresh interval (ms)"), 50, &UINumberTilde::prop_interval, _("Main"));
    obj.setPropertyRange("interval", 20, 1000);
    obj.setPropertyUnits("interval", "msec");

    obj.setPropertyDefaultValue(PROP_BACKGROUND_COLOR, "0.3 0.3 0.3 1");
    obj.setPropertyDefaultValue(PROP_BORDER_COLOR, DEFAULT_TEXT_COLOR);
    obj.hideProperty("send");
    obj.hideProperty("receive");
}

void setup_ui_number_tilde()
{
    UINumberTilde::setup();
}
