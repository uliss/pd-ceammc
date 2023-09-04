/*****************************************************************************
 * Copyright 2019 Serge Poltavsky. All rights reserved.
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
#include "hoa_2d_scope.h"
#include "ceammc_convert.h"
#include "ceammc_ui.h"

static const int MIN_SIZE = 20;
static const int HOA_DISPLAY_NPOINTS = 65;
static const float HOA_CONTRAST_DARKER = 0.2;
static const float HOA_CONTRAST_LIGHTER = 0.2;

Hoa2dScope::Hoa2dScope()
    : prop_order_(3)
    , nharm_(0)
    , prop_gain_(1)
    , prop_view_(0)
    , prop_refresh_(100)
    , clock_(this, &Hoa2dScope::tick)
    , start_clock_(false)
    , harm_layer_(asEBox(), gensym("harm_layer"))
{
}

void Hoa2dScope::init(t_symbol* s, const AtomListView& args, bool usePresets)
{
    UIDspObject::init(s, args, usePresets);

    // first positional argument handling
    if (!args.empty() && args[0].isFloat())
        propSetOrder(clip<t_float>(args[0].asFloat(), HOA_MIN_ORDER, HOA_MAX_ORDER));

    dspSetup(nharm_, 0);
}

void Hoa2dScope::okSize(t_rect* newrect)
{
    newrect->w = pd_clip_min(newrect->w, MIN_SIZE);
    newrect->h = pd_clip_min(newrect->h, MIN_SIZE);
}

void Hoa2dScope::paint()
{
    drawBackground();
    drawHarmonics();
}

t_float Hoa2dScope::propOrder() const
{
    return prop_order_;
}

void Hoa2dScope::propSetOrder(t_float v)
{
    auto order = clip<int>(v, HOA_MIN_ORDER, HOA_MAX_ORDER);

    if (!scope_ || (order != scope_->getDecompositionOrder())) {
        int dspState = canvas_suspend_dsp();
        scope_.reset(new Scope2d(order, HOA_DISPLAY_NPOINTS));
        prop_order_ = scope_->getDecompositionOrder();
        nharm_ = scope_->getNumberOfHarmonics();

        in_buf_.resize(nharm_ * HOA_DEFAULT_BLOCK_SIZE);

        eobj_resize_inputs(asEObj(), nharm_);
        canvas_update_dsp();
        canvas_resume_dsp(dspState);
    }
}

t_float Hoa2dScope::propView() const
{
    return prop_view_;
}

void Hoa2dScope::propSetView(t_float angle)
{
    int dspState = canvas_suspend_dsp();
    prop_view_ = wrapFloatMax<float>(angle, 360);
    scope_->setViewRotation(0., 0., convert::degree2rad(prop_view_));
    scope_->computeRendering();
    canvas_resume_dsp(dspState);

    harm_layer_.invalidate();
    bg_layer_.invalidate();
    redraw();
}

void Hoa2dScope::tick()
{
    scope_->process(&in_buf_[0]);

    harm_layer_.invalidate();
    redraw();
    if (pd_getdspstate())
        clock_.delay(prop_refresh_);
}

void Hoa2dScope::dspOn(double samplerate, long blocksize)
{
    in_buf_.resize(scope_->getNumberOfHarmonics() * blocksize);

    UIDspObject::dspOn(samplerate, blocksize);
    start_clock_ = true;
}

void Hoa2dScope::dspProcess(t_sample** ins, long n_ins, t_sample** outs, long n_outs, long sampleframes)
{
    for (long i = 0; i < n_ins; i++)
        Signal::copy(size_t(sampleframes), ins[i], 1, &in_buf_[i], size_t(n_ins));

    Signal::scale(size_t(n_ins * sampleframes), prop_gain_, &in_buf_[0]);

    if (start_clock_) {
        start_clock_ = false;
        clock_.delay(0);
    }
}

void Hoa2dScope::drawBackground()
{
    const auto r = rect();
    UIPainter p = bg_layer_.painter(r);

    if (!p)
        return;

    t_rgba black = rgba_addContrast(prop_color_background, -HOA_CONTRAST_DARKER);
    t_rgba white = rgba_addContrast(prop_color_background, HOA_CONTRAST_LIGHTER);

    const float center = width() / 2;
    const float radius = center * 0.95;

    p.setMatrix({ 1, 0, 0, -1, center, center });

    double angle, x1, x2, y1, y2, cosa, sina;
    for (int i = 0; i < (prop_order_ * 2 + 2); i++) {
        angle = ((double)(i - 0.5) / (prop_order_ * 2 + 2) * HOA_2PI);
        cosa = cos(angle);
        sina = sin(angle);
        x1 = cosa * radius * 0.2;
        y1 = sina * radius * 0.2;
        x2 = cosa * radius;
        y2 = sina * radius;

        p.moveTo(x1, y1);
        p.drawLineTo(x2, y2);
        p.setLineWidth(3);
        p.setColor(white);
        p.strokePreserve();

        p.setColor(black);
        p.setLineWidth(1);
        p.stroke();
    }

    for (int i = 5; i > 0; i--) {
        p.setLineWidth(3);
        p.setColor(white);
        p.drawCircle(0, 0, i * 0.2 * radius);
        p.strokePreserve();

        p.setLineWidth(1);
        p.setColor(black);
        p.drawCircle(0, 0, i * 0.2 * radius);
        p.stroke();
    }
}

void Hoa2dScope::drawHarmonics()
{
    const t_rect r = rect();
    UIPainter p = harm_layer_.painter(r);
    if (!p)
        return;

    const float center = width() * 0.5;
    const float radius = center * 0.95;

    p.rotate(HOA_PI);
    p.setLineWidth(1);
    p.setMatrix({ 1, 0, 0, -1, center, center });

    // positive harmonics
    char pathLength = 0;
    t_pt start_pt;

    p.setColor(prop_ph_color_);
    p.setSmooth(ESMOOTH_BEZIER);

    int prev_x = -1;
    int prev_y = -1;

    for (size_t i = 0; i < scope_->getNumberOfPoints(); i++) {
        if (scope_->getPointValue(i) >= 0) {
            if (!pathLength) {
                start_pt.x = scope_->getPointAbscissa(i) * radius;
                start_pt.y = scope_->getPointOrdinate(i) * radius;
                p.moveTo(start_pt.x, start_pt.y);
                pathLength++;
            } else {
                int x = scope_->getPointAbscissa(i) * radius;
                int y = scope_->getPointOrdinate(i) * radius;
                if (prev_x != x || prev_y != y) {
                    p.drawLineTo(x, y);
                    prev_x = x;
                    prev_y = y;
                }
            }
        }
    }

    p.closePath();
    if (pathLength) {
        p.drawLineTo(start_pt.x, start_pt.y);
        p.stroke();
    }

    // negative harmonics
    pathLength = 0;
    p.setColor(prop_nh_color_);

    prev_x = -1;
    prev_y = -1;

    for (size_t i = 0; i < scope_->getNumberOfPoints(); i++) {
        if (scope_->getPointValue(i) < 0) {
            if (!pathLength) {
                start_pt.x = scope_->getPointAbscissa(i) * radius;
                start_pt.y = scope_->getPointOrdinate(i) * radius;
                p.moveTo(start_pt.x, start_pt.y);
                pathLength++;
            } else {
                int x = scope_->getPointAbscissa(i) * radius;
                int y = scope_->getPointOrdinate(i) * radius;
                if (prev_x != x || prev_y != y) {
                    p.drawLineTo(x, y);
                    prev_x = x;
                    prev_y = y;
                }
            }
        }
    }

    p.closePath();
    if (pathLength) {
        p.drawLineTo(start_pt.x, start_pt.y);
        p.stroke();
    }

    p.setSmooth(ESMOOTH_NONE);
}

void Hoa2dScope::setup()
{
    auto SYM_REFRESH = gensym("refresh");
    auto SYM_MSEC = gensym("msec");

    UIObjectFactory<Hoa2dScope> obj("hoa.scope~", EBOX_IGNORELOCKCLICK | EBOX_GROWLINK);
    obj.setDefaultSize(120, 120);

    // hide some properties
    obj.internalProperty("send");
    obj.internalProperty("receive");

    // @order
    obj.addIntProperty("order", _("Ambisonic Order"), HOA_DEFAULT_ORDER, &Hoa2dScope::prop_order_, "Ambisonic");
    obj.setPropertyAccessor("order", &Hoa2dScope::propOrder, &Hoa2dScope::propSetOrder);
    obj.setPropertyRange("order", HOA_MIN_ORDER, HOA_MAX_ORDER);

    // @view
    obj.addFloatProperty("view", _("View rotation"), 0, &Hoa2dScope::prop_view_, "Ambisonic");
    //    obj.setPropertyRange("view", 0, 360);
    obj.setPropertyAccessor("view", &Hoa2dScope::propView, &Hoa2dScope::propSetView);

    // @gain
    obj.addFloatProperty("gain", _("Gain"), 1.f, &Hoa2dScope::prop_gain_, "Main");
    obj.setPropertyMin("gain", 0);

    // @refresh
    obj.addIntProperty(SYM_REFRESH->s_name, _("Refresh time (ms)"), 100, &Hoa2dScope::prop_refresh_, _("Main"));
    obj.setPropertyRange(SYM_REFRESH->s_name, 20, 1000);
    obj.setPropertyUnits(SYM_REFRESH, SYM_MSEC);

    // @ph_color
    obj.addColorProperty("ph_color", _("Positive Harmonics Color"), "1. 0. 0. 1.", &Hoa2dScope::prop_ph_color_);
    obj.addColorProperty("nh_color", _("Negative Harmonics Color"), "0. 0. 1. 1.", &Hoa2dScope::prop_nh_color_);
}

void setup_spat_hoa_scope2d()
{
    Hoa2dScope::setup();
}
