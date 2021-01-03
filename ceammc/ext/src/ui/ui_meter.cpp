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
#include "ui_meter.h"
#include "ceammc_convert.h"
#include "ceammc_ui.h"

static const t_float MIN_DB_VALUE = -90;
static const int NUM_LEDS = 13;
static const int LED_STEP = 3;
static const int RMS_BAR_PADDING = 3;

UIMeter::UIMeter()
    : clock_(this, &UIMeter::clockTick)
    , prop_color_cold(rgba_grey)
    , prop_color_tepid(rgba_green)
    , prop_color_warm(rgba_green)
    , prop_color_hot(rgba_red)
    , prop_color_over(rgba_red)
    , prop_interval_ms(50)
    , led_layer_(asEBox(), gensym("led_layer"))
    , raw_peak_(0)
    , raw_square_sum_(0)
    , num_samples_(0)
    , overload_(0)
    , rms_dbfs_(-90)
    , peak_dbfs_(-90)
    , start_clock_(false)
    , is_horizontal_(false)
{
    createOutlet();
}

void UIMeter::okSize(t_rect* newrect)
{
    newrect->width = pd_clip_min(newrect->width, 8.);
    newrect->height = pd_clip_min(newrect->height, 8.);

    is_horizontal_ = newrect->width > newrect->height;

    if (is_horizontal_)
        newrect->width = pd_clip_min(newrect->width, 50.);
    else
        newrect->height = pd_clip_min(newrect->height, 50.);
}

void UIMeter::paint()
{
    drawBackground();
    drawLeds();
}

void UIMeter::drawBackground()
{
    const t_rect r = rect();
    UIPainter p = bg_layer_.painter(r);

    if (!p)
        return;

    p.setColor(prop_color_border);
    if (is_horizontal_) {
        float ratio = r.width / NUM_LEDS;
        for (int i = 1; i < NUM_LEDS; i++) {
            float x = roundf(i * ratio);
            p.drawLine(x, -1, x, r.height + 1);
        }

    } else {
        float ratio = r.height / NUM_LEDS;
        for (int i = 1; i < NUM_LEDS; i++) {
            float y = roundf(i * ratio);
            p.drawLine(-1, y, r.width + 1, y);
        }
    }
}

static inline float led2Db(int ledIdx)
{
    return -1 * (NUM_LEDS - ledIdx) * LED_STEP;
}

void UIMeter::drawLeds()
{
    const t_rect r = rect();
    UIPainter p = led_layer_.painter(r);
    if (!p)
        return;

    // draw rms
    for (int i = 0; i < NUM_LEDS; i++) {
        const float db = led2Db(i);

        if (rms_dbfs_ > db) {
            const float diff = rms_dbfs_ - db;
            const bool top = (diff < LED_STEP);

            p.setColor(dbfsToColor(db));

            if (is_horizontal_) {
                const float led_space = r.width / NUM_LEDS;
                const int top_offset = roundf((top ? (1 - (diff / LED_STEP)) : 0) * led_space);
                const int led_x = roundf(i * led_space) + 1;
                const int next_led_x = roundf((i + 1) * led_space);
                const int led_w = next_led_x - led_x - top_offset;

                p.drawRect(led_x, RMS_BAR_PADDING, led_w, r.height - 2 * RMS_BAR_PADDING);
            } else {
                const float led_space = r.height / NUM_LEDS;
                const float top_offset = (top ? (1 - (diff / LED_STEP)) : 0) * led_space;
                const int led_y = roundf((NUM_LEDS - i - 1) * led_space + top_offset) + 1;
                const int next_led_y = roundf((NUM_LEDS - i) * led_space);
                const int led_h = next_led_y - led_y;

                p.drawRect(RMS_BAR_PADDING, led_y, r.width - 2 * RMS_BAR_PADDING, led_h);
            }

            p.fill();
        }
    }

    // draw peak
    p.setColor(dbfsToColor(peak_dbfs_));
    p.setLineWidth(2);
    if (is_horizontal_) {
        float x = convert::lin2lin<float>(peak_dbfs_, -(LED_STEP * NUM_LEDS), 0, 0, r.width);
        x = std::min<float>(roundf(x), r.width);
        p.drawLine(x, -1, x, r.height + 1);
    } else {
        float y = convert::lin2lin<float>(peak_dbfs_, -(LED_STEP * NUM_LEDS), 0, r.height, 0);
        y = std::max<float>(0, roundf(y));
        p.drawLine(-1, y, r.width + 1, y);
    }

    // draw overload
    if (overload_) {
        p.setColor(prop_color_over);
        if (is_horizontal_) {
            const float led_space = r.width / NUM_LEDS;
            const int led_x = roundf((NUM_LEDS - 1) * led_space) + 1;
            const int next_led_x = roundf(NUM_LEDS * led_space);
            const int led_w = next_led_x - led_x;

            p.drawRect(led_x, RMS_BAR_PADDING, led_w, r.height - 2 * RMS_BAR_PADDING);
        } else {
            const float led_space = r.height / NUM_LEDS;
            const int led_y = 1;
            const int next_led_y = roundf(led_space);
            const int led_h = next_led_y - led_y;

            p.drawRect(RMS_BAR_PADDING, led_y, r.width - 2 * RMS_BAR_PADDING, led_h);
        }

        p.fill();
    }
}

void UIMeter::dspInit()
{
    dspSetup(1, 0);
}

void UIMeter::dspOn(double samplerate, long blocksize)
{
    reset();
    start_clock_ = true;
    UIDspObject::dspOn(samplerate, blocksize);
}

void UIMeter::dspProcess(t_sample** ins, long n_ins, t_sample** outs, long n_outs, long sampleframes)
{
    const size_t BS = blocksize();

    for (size_t i = 0; i < BS; i++) {
        t_sample s = ins[0][i];
        // find peak
        double t = fabs(s);
        if (t > raw_peak_)
            raw_peak_ = t;

        raw_square_sum_ += double(s) * double(s);
    }

    num_samples_ += BS;

    if (start_clock_) {
        start_clock_ = false;
        clock_.delay(0);
    }
}

void UIMeter::setup()
{
    UIObjectFactory<UIMeter> obj("ui.meter~", EBOX_GROWINDI | EBOX_IGNORELOCKCLICK);
    obj.addAlias("ui.m~");
    obj.hideLabelInner();

    obj.setDefaultSize(15, 120);

    obj.addColorProperty("cold_color", _("Cold signal color"), "0 0.6 0 1", &UIMeter::prop_color_cold);
    obj.addColorProperty("tepid_color", _("Tepid signal color"), "0.6 0.73 0 1", &UIMeter::prop_color_tepid);
    obj.addColorProperty("warm_color", _("Warm signal color"), ".85 .85 0 1", &UIMeter::prop_color_warm);
    obj.addColorProperty("hot_color", _("Hot signal color"), "1 0.6 0 1", &UIMeter::prop_color_hot);
    obj.addColorProperty("over_color", _("Overload signal color"), "1 0 0 1", &UIMeter::prop_color_over);

    obj.addIntProperty("interval", _("Refresh interval (ms)"), 50, &UIMeter::prop_interval_ms, _("Main"));
    obj.setPropertyUnits("interval", "msec");
}

void UIMeter::calc()
{
    double rms = num_samples_ ? sqrt(raw_square_sum_ / num_samples_) : 0;
    rms_dbfs_ = (rms > 0) ? convert::amp2dbfs(rms) : MIN_DB_VALUE;
    peak_dbfs_ = (raw_peak_ > 0) ? convert::amp2dbfs(raw_peak_) : MIN_DB_VALUE;

    if (peak_dbfs_ >= 0)
        overload_ = 1;
    else if (overload_ > 0)
        overload_++;

    // reset overload after 1 second
    if (overload_ >= 1000. / prop_interval_ms)
        overload_ = 0;

    led_layer_.invalidate();
    redraw();
}

void UIMeter::reset()
{
    raw_peak_ = 0;
    raw_square_sum_ = 0;
    num_samples_ = 0;
}

void UIMeter::clockTick()
{
    if (canvas_dspstate) {
        calc();
        output();
        reset();
        clock_.delay(prop_interval_ms);
    }
}

void UIMeter::output()
{
    Atom res[2];
    res[0] = rms_dbfs_;
    res[1] = peak_dbfs_;
    listTo(0, AtomListView(res, 2));
    send(AtomListView(res, 2));
}

const t_rgba& UIMeter::dbfsToColor(int dbfs) const
{
    dbfs = clip<int>(dbfs, -LED_STEP * NUM_LEDS, 0);

    if (dbfs < -30)
        return prop_color_cold;
    else if (dbfs < -21)
        return prop_color_tepid;
    else if (dbfs < -12)
        return prop_color_warm;
    else if (dbfs < -3)
        return prop_color_hot;
    else
        return prop_color_over;
}

void setup_ui_meter()
{
    UIMeter::setup();
}
