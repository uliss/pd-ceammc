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
#include "ui_meter.tcl.h"

static const t_float MIN_DB_VALUE = -90;

static t_symbol* SYM_HMETER;
static t_symbol* SYM_VMETER;
static t_symbol* SYM_VMETER2;

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

void UIMeter::init(t_symbol* name, const AtomList& args, bool usePresets)
{
    UIDspObject::init(name, args, usePresets);

    if (name == SYM_HMETER) {
        is_horizontal_ = true;
        std::swap(asEBox()->b_rect.width, asEBox()->b_rect.height);
    }
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
    sys_vgui("ui::meter_delete #%x %s\n",
        asEBox(), asEBox()->b_drawing_id->s_name);

    sys_vgui("ui::meter_create #%x %s "
             "%d %d "
             "#%6.6x #%6.6x #%6.6x #%6.6x #%6.6x #%6.6x "
             "%.2f %.2f %d\n",
        asEBox(), asEBox()->b_drawing_id->s_name,
        (int)width(), (int)height(),
        rgba_to_hex_int(prop_color_border),
        rgba_to_hex_int(prop_color_cold),
        rgba_to_hex_int(prop_color_tepid),
        rgba_to_hex_int(prop_color_warm),
        rgba_to_hex_int(prop_color_hot),
        rgba_to_hex_int(prop_color_over),
        rms_dbfs_, peak_dbfs_, overload_);
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

const char* UIMeter::annotateInlet(int) const
{
    return "signal: input";
}

const char* UIMeter::annotateOutlet(int) const
{
    return "list: RMS(db) PEAK(db)";
}

void UIMeter::onDblClick(t_object* view, const t_pt&, long)
{
    t_canvas* c = reinterpret_cast<t_canvas*>(view);
    if (c->gl_edit) {
        is_horizontal_ = !is_horizontal_;
        resize(height() / zoom(), width() / zoom());
    }
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

void UIMeter::setup()
{
    sys_gui(ui_meter_tcl);

    SYM_HMETER = gensym("ui.hm~");
    SYM_VMETER = gensym("ui.vm~");
    SYM_VMETER2 = gensym("ui.m~");

    UIObjectFactory<UIMeter> obj("ui.meter~", EBOX_GROWINDI);
    obj.addAlias(SYM_VMETER->s_name);
    obj.addAlias(SYM_VMETER2->s_name);
    obj.addAlias(SYM_HMETER->s_name);
    obj.useAnnotations();
    obj.useMouseEvents(UI_MOUSE_DBL_CLICK);

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

void setup_ui_meter()
{
    UIMeter::setup();
}
