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

UINumberTilde::UINumberTilde()
    : clock_(this, &UINumberTilde::updateTextValue)
    , text_(&asEBox()->b_font, ColorRGBA::black(), ETEXT_LEFT, ETEXT_JLEFT, ETEXT_NOWRAP)
    , bg_tilde_(&asEBox()->b_font, ColorRGBA::black(), ETEXT_CENTER, ETEXT_JCENTER, ETEXT_NOWRAP)
    , text_layer_(asEBox(), gensym("text_layer"))
    , value_(0)
    , last_redraw_time_(0)
    , text_value_()
    , prop_color_text(rgba_black)
    , prop_color_active(rgba_blue)
    , prop_max_decimal(-1)
    , prop_interval(50)
{
}

void UINumberTilde::init(t_symbol* name, const AtomList& args, bool usePresets)
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
    const t_rect r = rect();

    // background
    {
        UIPainter p = bg_layer_.painter(r);
        if (p) {
            const float width = r.height * 0.7f;
            p.setLineWidth(1);
            p.setColor(prop_color_border);
            p.moveTo(width, 0);
            p.drawLineTo(width, r.height);
            p.stroke();

            bg_tilde_.setColor(prop_color_active);
            bg_tilde_.set("~", width / 2, r.height / 2, 0, 0);
            p.drawText(bg_tilde_);
        }
    }

    // value
    {
        UIPainter p = text_layer_.painter(r);
        if (p) {
            const float y_off = r.height * 0.5;
            const float x_off = r.height * 0.9;

            text_.setColor(prop_color_text);
            text_.set(text_value_, x_off, y_off, 0, 0);
            p.drawText(text_);
        }
    }
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

    clock_.delay(0);
}

void UINumberTilde::setup()
{
    UIObjectFactory<UINumberTilde> obj("ui.number~", EBOX_GROWINDI | EBOX_IGNORELOCKCLICK);
    obj.addAlias("ui.n~");
    obj.setDefaultSize(80, 15);
    obj.hideLabelInner();

    obj.addColorProperty(PROP_TEXT_COLOR, _("Text color"), "0.9 0.9 0.9 1", &UINumberTilde::prop_color_text);
    obj.addColorProperty(PROP_ACTIVE_COLOR, _("Text color"), DEFAULT_ACTIVE_COLOR, &UINumberTilde::prop_color_active);
    obj.addIntProperty("decimal", _("Decimal precision"), 6, &UINumberTilde::prop_max_decimal, _("Main"));
    obj.setPropertyRange("precision", 0, 6);
    obj.addIntProperty("interval", _("Refresh interval (ms)"), 50, &UINumberTilde::prop_interval, _("Main"));
    obj.setPropertyRange("interval", 20, 1000);
    obj.setPropertyUnits("interval", "msec");

    obj.setPropertyDefaultValue(PROP_BACKGROUND_COLOR, "0.3 0.3 0.3 1");
    obj.setPropertyDefaultValue(PROP_BORDER_COLOR, DEFAULT_TEXT_COLOR);
    obj.hideProperty("send");
    obj.hideProperty("receive");
}

void UINumberTilde::updateTextValue()
{
    double t = clock_gettimesince(last_redraw_time_);
    if (t > prop_interval) {
        switch (prop_max_decimal) {
        case 0:
            sprintf(text_value_, "%i", (int)value_);
            break;
        case 1:
            sprintf(text_value_, "%.1f", value_);
            break;
        case 2:
            sprintf(text_value_, "%.2f", value_);
            break;
        case 3:
            sprintf(text_value_, "%.3f", value_);
            break;
        case 4:
            sprintf(text_value_, "%.4f", value_);
            break;
        case 5:
            sprintf(text_value_, "%.5f", value_);
            break;
        default:
            sprintf(text_value_, "%.6f", value_);
            break;
        }

        last_redraw_time_ = clock_getlogicaltime();
        text_layer_.invalidate();
        redraw();
    }
}

void setup_ui_number_tilde()
{
    UINumberTilde::setup();
}
