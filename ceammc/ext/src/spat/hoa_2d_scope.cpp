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
#include "ceammc_dsp_ui.h"

static const int MIN_SIZE = 50;
static const int HOA_DISPLAY_NPOINTS = 128;
static const float HOA_CONTRAST_DARKER = 0.2;
static const float HOA_CONTRAST_LIGHTER = 0.2;

Hoa2dScope::Hoa2dScope()
    : order_(3)
    , nharm_(0)
    , clock_(this, &Hoa2dScope::tick)
    , start_clock_(false)
    , harm_layer_(asEBox(), gensym("harm_layer"))
{
//    scope_.reset(new Scope2d(order_, HOA_DISPLAY_NPOINTS));
}

void Hoa2dScope::init(t_symbol* s, const AtomList& lst, bool usePresets)
{
    UIDspObject::init(s, lst, usePresets);

    // first positional argument handling
    if (!lst.empty() && lst[0].isFloat())
        propSetOrder(clip<t_float>(lst[0].asFloat(), HOA_MIN_ORDER, HOA_MAX_ORDER));

    dspSetup(nharm_, 0);
}

void Hoa2dScope::okSize(t_rect* newrect)
{
    newrect->width = pd_clip_min(newrect->width, MIN_SIZE);
    newrect->height = pd_clip_min(newrect->height, MIN_SIZE);
}

void Hoa2dScope::paint()
{
    drawBackground();
    drawHarmonics();
}

t_float Hoa2dScope::propOrder() const
{
    return order_;
}

void Hoa2dScope::propSetOrder(t_float v)
{
    auto order = clip<int>(v, HOA_MIN_ORDER, HOA_MAX_ORDER);

    if (!scope_ || (order != scope_->getDecompositionOrder())) {
        int dspState = canvas_suspend_dsp();
        scope_.reset(new Scope2d(order, HOA_DISPLAY_NPOINTS));
        order_ = scope_->getDecompositionOrder();
        nharm_ = scope_->getNumberOfHarmonics();

        in_buf_.resize(nharm_ * HOA_DEFAULT_BLOCK_SIZE);

        eobj_resize_inputs(asEBox(), nharm_);
        canvas_update_dsp();
        canvas_resume_dsp(dspState);
    }
}

void Hoa2dScope::tick()
{
    scope_->process(&in_buf_[0]);

    harm_layer_.invalidate();
    redraw();
    if (canvas_dspstate)
        clock_.delay(100);
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

void Hoa2dScope::setup()
{
    UIDspFactory<Hoa2dScope> obj("hoa.scope~", EBOX_IGNORELOCKCLICK | EBOX_GROWLINK);
    obj.usePresets();

    obj.addProperty("order", &Hoa2dScope::propOrder, &Hoa2dScope::propSetOrder);
    obj.setPropertyDefaultValue("order", "3");
    obj.showProperty("order");
    obj.setPropertyRange("order", HOA_MIN_ORDER, HOA_MAX_ORDER);

    obj.addProperty("gain", _("Gain"), 1.f, &Hoa2dScope::prop_gain_, "Main");
    obj.setPropertyMin("gain", 0);

    obj.setDefaultSize(60, 60);
}

void Hoa2dScope::drawBackground()
{
    const auto r = rect();
    UIPainter p = bg_layer_.painter(r);

    if (!p)
        return;

    t_matrix transform;
    t_rgba black = rgba_addContrast(prop_color_background, -HOA_CONTRAST_DARKER);
    t_rgba white = rgba_addContrast(prop_color_background, HOA_CONTRAST_LIGHTER);

    auto center = width() / 2;
    auto radius = center * 0.95;

    egraphics_matrix_init(&transform, 1, 0, 0, -1, center, center);
    egraphics_set_matrix(p.layer(), &transform);

    double angle, x1, x2, y1, y2, cosa, sina;
    for (int i = 0; i < (order_ * 2 + 2); i++) {
        angle = ((double)(i - 0.5) / (order_ * 2 + 2) * HOA_2PI);
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

        egraphics_stroke_preserve(p.layer());
        p.setColor(black);
        p.setLineWidth(1);
    }

    p.stroke();

    for (int i = 5; i > 0; i--) {
        p.setLineWidth(3);
        p.setColor(white);
        p.drawCircle(0, 0, i * 0.2 * radius);
        p.stroke();
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

    t_matrix transform;
    egraphics_rotate(p.layer(), HOA_PI);
    p.setLineWidth(1);
    auto center = width() * 0.5;
    auto radius = center * 0.95;
    egraphics_matrix_init(&transform, 1, 0, 0, -1, center, center);
    egraphics_set_matrix(p.layer(), &transform);

    // positive harmonics
    char pathLength = 0;
    p.setColor(rgba_red);

    for (size_t i = 0; i < scope_->getNumberOfPoints(); i++) {
        if (scope_->getPointValue(i) >= 0) {
            if (!pathLength) {
                p.moveTo(scope_->getPointAbscissa(i) * radius, scope_->getPointOrdinate(i) * radius);
                pathLength++;
            } else {
                p.drawLineTo(scope_->getPointAbscissa(i) * radius, scope_->getPointOrdinate(i) * radius);
            }
        }
    }

    egraphics_close_path(p.layer());
    if (pathLength)
        p.stroke();

    // negative harmonics
    pathLength = 0;
    p.setColor(rgba_blue);

    for (size_t i = 0; i < scope_->getNumberOfPoints(); i++) {
        if (scope_->getPointValue(i) < 0) {
            if (!pathLength) {
                p.moveTo(scope_->getPointAbscissa(i) * radius, scope_->getPointOrdinate(i) * radius);
                pathLength++;
            } else {
                //                egraphics_curve_to(p.l);
                p.drawLineTo(scope_->getPointAbscissa(i) * radius, scope_->getPointOrdinate(i) * radius);
            }
        }
    }

    egraphics_close_path(p.layer());
    if (pathLength)
        p.stroke();
}

void setup_spat_hoa_scope2d()
{
    Hoa2dScope::setup();
}
