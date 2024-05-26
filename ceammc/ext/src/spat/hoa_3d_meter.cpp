/*****************************************************************************
 * Copyright 2023 Serge Poltavski. All rights reserved.
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
#include "hoa_3d_meter.h"
#include "args/argcheck.h"
#include "ceammc_crc32.h"
#include "ceammc_dsp.h"
#include "ceammc_ui.h"
#include "cicm/Sources/egraphics.h"
#include "hoa_common.h"

CEAMMC_DEFINE_SYM_HASH(anticlock)
CEAMMC_DEFINE_SYM_HASH(both)
CEAMMC_DEFINE_SYM_HASH(bottom)
CEAMMC_DEFINE_SYM_HASH(clockwise)
CEAMMC_DEFINE_SYM_HASH(energy)
CEAMMC_DEFINE_SYM_HASH(none)
CEAMMC_DEFINE_SYM_HASH(top)
CEAMMC_DEFINE_SYM_HASH(topnextbottom)
CEAMMC_DEFINE_SYM_HASH(toponbottom)
CEAMMC_DEFINE_SYM_HASH(velocity)

constexpr int MIN_SIZE = 20;
constexpr int DEF_NCHAN = 4;
constexpr int MIN_NCHAN = 1;
constexpr int MAX_NCHAN = 64;
constexpr float HOA_CONTRAST_DARKER = 0.2;
constexpr float HOA_CONTRAST_LIGHTER = 0.2;

using namespace hoa;

Hoa3dMeter::Hoa3dMeter()
    : f_clock([this] { clockTick(); })
    , f_signals(0, hoa::Signal<t_sample>::free)
    , led_layer(this, "led_layer")
    , vec_layer(this, "vec_layer")
{
    f_ramp = 0;
    f_meter.reset(new Meter3d(DEF_NCHAN));
    f_vector.reset(new Vector3d(DEF_NCHAN));
    f_signals.reset(hoa::Signal<t_sample>::alloc(HOA_MAX_PLANEWAVES * HOA_MAXBLKSIZE));

    f_meter->computeRendering();
    f_vector->computeRendering();
    f_startclock = 0;
}

void Hoa3dMeter::init(t_symbol* s, const AtomListView& args, bool usePresets)
{
    UIDspObject::init(s, args, usePresets);

    dspSetup(f_meter->getNumberOfPlanewaves(), 0);
}

void Hoa3dMeter::okSize(t_rect* newrect)
{
    newrect->w = pd_clip_min(newrect->w, MIN_SIZE);
    newrect->h = pd_clip_min(newrect->h, MIN_SIZE);

    double delta1 = newrect->w - rawLastRect().w;
    double delta2 = newrect->h - rawLastRect().h;

    if (f_view == sym_topnextbottom()) {
        if (fabs(delta1) < fabs(delta2))
            newrect->w = newrect->h * 2;
        else
            newrect->h = newrect->w * 0.5;
    } else if (f_view == sym_toponbottom()) {
        if (fabs(delta1) < fabs(delta2))
            newrect->w = newrect->h * 0.5;
        else
            newrect->h = newrect->w * 2;
    } else {
        if (fabs(delta1) < fabs(delta2))
            newrect->w = newrect->h;
        else
            newrect->h = newrect->w;
    }

    rawLastRect() = *newrect;
}

void Hoa3dMeter::paint()
{
    auto r = rect();

    if (f_view == sym_topnextbottom())
        f_center = r.w * 0.25;
    else
        f_center = r.w * 0.5;

    f_radius = f_center * 0.95;

    drawLeds();
    drawVectors();
    drawBackground();
}

void Hoa3dMeter::dspProcess(t_sample** ins, long n_ins, t_sample** outs, long n_outs, long sampleframes)
{
    for (int i = 0; i < n_ins; i++) {
        hoa::Signal<t_sample>::copy(sampleframes, ins[i], 1, f_signals.get() + i, n_ins);
    }

    for (f_ramp = 0; f_ramp < sampleframes; f_ramp++) {
        f_meter->process(f_signals.get() + n_ins * f_ramp);
    }

    if (f_startclock) {
        f_startclock = 0;
        f_clock.delay(0);
    }
}

void Hoa3dMeter::dspOn(double samplerate, long blocksize)
{
    UIDspObject::dspOn(samplerate, blocksize);

    f_meter->setVectorSize(blocksize);
    f_startclock = 1;
}

t_int Hoa3dMeter::propChannels() const
{
    return f_meter->getNumberOfPlanewaves();
}

void Hoa3dMeter::propSetChannels(t_int n)
{
    auto d = pd_clip_minmax(n, 4, HOA_MAX_PLANEWAVES);
    if (d != f_meter->getNumberOfPlanewaves()) {
        dsp::SuspendGuard dsp;
        f_meter.reset(new Meter3d(d));
        f_vector.reset(new Vector3d(d));

        f_meter->computeRendering();
        f_vector->computeRendering();

        resizeInputs(f_meter->getNumberOfPlanewaves());

        bg_layer_.invalidate();
        led_layer.invalidate();
        vec_layer.invalidate();

        redraw();
    }
}

AtomList Hoa3dMeter::propAngles() const
{
    AtomList res;
    res.reserve(f_meter->getNumberOfPlanewaves() * 2);
    for (size_t i = 0; i < f_meter->getNumberOfPlanewaves(); i++) {
        res.append(std::round(f_meter->getPlanewaveAzimuth(i, false) / HOA_2PI * 3600.) / 10);
        res.append(std::round(f_meter->getPlanewaveElevation(i, false) / HOA_2PI * 3600.) / 10);
    }

    return res;
}

void Hoa3dMeter::propSetAngles(const AtomListView& lv)
{
    for (size_t i = 0; i < f_meter->getNumberOfPlanewaves() * 2 && i < lv.size(); i++) {
        auto& a = lv[i];
        if (a.isFloat()) {
            auto f = a.asT<t_float>();
            if (i % 2) {
                f_meter->setPlanewaveElevation((i - 1) / 2, f / 360.f * HOA_2PI);
                f_vector->setPlanewaveElevation((i - 1) / 2, f / 360.f * HOA_2PI);
            } else {
                f_meter->setPlanewaveAzimuth(i / 2, f / 360.f * HOA_2PI);
                f_vector->setPlanewaveAzimuth(i / 2, f / 360.f * HOA_2PI);
            }
        }
    }

    f_meter->computeRendering();
    f_vector->computeRendering();

    bg_layer_.invalidate();
    led_layer.invalidate();
    vec_layer.invalidate();

    redraw();
}

AtomList Hoa3dMeter::propOffset() const
{
    auto to_angles = [](t_float f) { return std::round(f / HOA_2PI * 3600.) / 10; };
    return {
        Atom(to_angles(f_meter->getPlanewavesRotationX())),
        Atom(to_angles(f_meter->getPlanewavesRotationY())),
        Atom(to_angles(f_meter->getPlanewavesRotationZ()))
    };
}

void Hoa3dMeter::propSetOffset(const AtomListView& lv)
{
    if (lv.empty())
        return;

    double ax, ay, az;
    if (lv[0].isFloat())
        ax = convert::degree2rad(lv.floatAt(0, 0));
    else
        ax = f_meter->getPlanewavesRotationX();

    if (lv.size() > 1 && lv[1].isFloat())
        ay = convert::degree2rad(lv.floatAt(1, 0));
    else
        ay = f_meter->getPlanewavesRotationY();

    if (lv.size() > 2 && lv[2].isFloat())
        az = convert::degree2rad(lv.floatAt(2, 0));
    else
        az = f_meter->getPlanewavesRotationZ();

    f_meter->setPlanewavesRotation(ax, ay, az);
    f_vector->setPlanewavesRotation(ax, ay, az);

    f_meter->computeRendering();
    f_vector->computeRendering();

    bg_layer_.invalidate();
    led_layer.invalidate();
    vec_layer.invalidate();
    redraw();
}

void Hoa3dMeter::propSetVectors(t_symbol* type)
{
    static const args::ArgChecker chk("MODE:s=none|energy|both|velocity");
    if (!chk.checkSilent(AtomListView(Atom(type))))
        return chk.usage();

    f_vector_type = type;
    vec_layer.invalidate();
    redraw();
}

void Hoa3dMeter::propSetRotation(t_int s)
{
    f_clockwise = bool(s);

    bg_layer_.invalidate();
    led_layer.invalidate();
    vec_layer.invalidate();
    redraw();
}

void Hoa3dMeter::propSetView(t_symbol* view)
{
    static const args::ArgChecker chk("MODE:s=top|bottom|topnextbottom|toponbottom");
    if (!chk.checkSilent(AtomListView(Atom(view))))
        return chk.usage();

    if (view != f_view) {
        f_view = view;
        rawLastRect() = rawRect();
        eobj_attr_setvalueof(asEObj(), gensym("size"), 0, NULL);
    }
}

void Hoa3dMeter::clockTick()
{
    if (f_vector_type == sym_both())
        f_vector->process(f_signals.get(), f_vector_coords);
    else if (f_vector_type == sym_velocity())
        f_vector->processVelocity(f_signals.get(), f_vector_coords);
    else if (f_vector_type == sym_energy())
        f_vector->processEnergy(f_signals.get(), f_vector_coords + 3);

    f_meter->tick(1000.f / (float)f_interval);
    bg_layer_.invalidate();
    led_layer.invalidate();
    vec_layer.invalidate();
    redraw();

    if (canvas_dspstate)
        f_clock.delay(f_interval);
}

void Hoa3dMeter::m_beatles()
{
    dsp::SuspendGuard dsp;
    f_meter.reset(new Meter3d(101));
    f_vector.reset(new Vector3d(101));
    f_vector_type = sym_none();
    for (size_t i = 0; i < f_meter->getNumberOfPlanewaves(); i++) {
        f_meter->setPlanewaveAzimuth(i, (double)i / (double)f_meter->getNumberOfPlanewaves() * HOA_2PI - HOA_PI2);
        f_vector->setPlanewaveAzimuth(i, (double)i / (double)f_meter->getNumberOfPlanewaves() * HOA_2PI - HOA_PI2);
        f_meter->setPlanewaveElevation(i, (double)i / (double)(f_meter->getNumberOfPlanewaves() - 1) * HOA_PI);
        f_vector->setPlanewaveElevation(i, (double)i / (double)(f_meter->getNumberOfPlanewaves() - 1) * HOA_PI);
    }

    f_meter->computeRendering();
    f_vector->computeRendering();

    bg_layer_.invalidate();
    led_layer.invalidate();
    vec_layer.invalidate();
    redraw();
    resizeInputs(f_meter->getNumberOfPlanewaves());
}

const t_rgba& Hoa3dMeter::segmentColor(bool peak, float db) const
{
    if (peak) {
        return f_color_over_signal;
    } else if (db >= -12.) {
        return f_color_hot_signal;
    } else if (db >= -21) {
        return f_color_warm_signal;
    } else if (db >= -30.) {
        return f_color_tepid_signal;
    } else if (db >= -39.) {
        return f_color_cold_signal;
    } else {
        return prop_color_border;
    }
}

void Hoa3dMeter::drawLeds()
{
    auto r = rect();
    UIPainter p = led_layer.painter(rect());
    if (!p)
        return;

    t_rgba black = rgba_addContrast(prop_color_background, -HOA_CONTRAST_DARKER);
    t_rgba white = rgba_addContrast(prop_color_background, HOA_CONTRAST_LIGHTER);
    p.setMatrix(1, 0, 0, -1, f_center, f_center);

    const float width = f_radius - 2.;
    const bool top = (f_view == sym_bottom()) ? false : true;

    for (size_t i = 0; i < f_meter->getNumberOfPlanewaves(); i++) {
        auto& path = f_meter->getPlanewavePath(i, top);
        if (path.size() > 2) {
            float angle1 = Math<double>::azimuth(path[0].x, path[0].y);
            float radius1 = Math<double>::radius(path[0].x, path[0].y);
            p.moveTo(path[0].x * width, path[0].y * width);
            for (size_t j = 1; j < path.size(); j++) {
                const float angle2 = Math<double>::azimuth(path[j].x, path[j].y);
                const float radius2 = Math<double>::radius(path[j].x, path[j].y);
                const float extend = Math<double>::wrap_pi(angle2 - angle1);
                if (fabs(extend) > HOA_EPSILON && fabs(radius2 - radius1) < HOA_EPSILON && fabs(fabs(extend) - HOA_PI) > 1e-3) {
                    p.drawArcTo(0, 0, extend);
                } else {
                    p.drawLineTo(path[j].x * width, path[j].y * width);
                }
                angle1 = angle2;
                radius1 = radius2;
            }

            const float angle2 = Math<double>::azimuth(path[0].x, path[0].y);
            const float radius2 = Math<double>::radius(path[0].x, path[0].y);
            const float extend = Math<double>::wrap_pi(angle2 - angle1);
            if (fabs(extend) > HOA_EPSILON && fabs(radius2 - radius1) < HOA_EPSILON && fabs(fabs(extend) - HOA_PI) > 1e-3) {
                p.drawArcTo(0, 0, extend);
            } else {
                p.drawLineTo(path[0].x * width, path[0].y * width);
            }

            p.setColor(segmentColor(f_meter->getPlanewaveOverLed(i), f_meter->getPlanewaveEnergy(i)));
            p.fillPreserve();
            p.setLineWidth(2);
            p.setColor(white);
            p.strokePreserve();
            p.setLineWidth(1);
            p.setColor(black);
            p.stroke();
        }
    }

    if (f_view == sym_toponbottom() || f_view == sym_topnextbottom()) {
        if (f_view == sym_toponbottom()) {
            p.setMatrix(1, 0, 0, -1, f_center, r.w + f_center);
        } else {
            p.setMatrix(1, 0, 0, -1, r.h + f_center, f_center);
        }

        for (size_t i = 0; i < f_meter->getNumberOfPlanewaves(); i++) {
            auto& path = f_meter->getPlanewavePath(i, false);
            if (path.size() > 2) {
                float angle1 = Math<double>::azimuth(path[0].x, path[0].y);
                float radius1 = Math<double>::radius(path[0].x, path[0].y);
                p.moveTo(path[0].x * width, path[0].y * width);
                for (size_t j = 1; j < path.size(); j++) {
                    const float angle2 = Math<double>::azimuth(path[j].x, path[j].y);
                    const float radius2 = Math<double>::radius(path[j].x, path[j].y);
                    const float extend = Math<double>::wrap_pi(angle2 - angle1);
                    if (fabs(extend) > HOA_EPSILON && fabs(radius2 - radius1) < HOA_EPSILON && fabs(fabs(extend) - HOA_PI) > 1e-3) {
                        p.drawArcTo(0, 0, extend);
                    } else {
                        p.drawLineTo(path[j].x * width, path[j].y * width);
                    }
                    angle1 = angle2;
                    radius1 = radius2;
                }

                const float angle2 = Math<double>::azimuth(path[0].x, path[0].y);
                const float radius2 = Math<double>::radius(path[0].x, path[0].y);
                const float extend = Math<double>::wrap_pi(angle2 - angle1);
                if (fabs(extend) > HOA_EPSILON && fabs(radius2 - radius1) < HOA_EPSILON && fabs(fabs(extend) - HOA_PI) > 1e-3) {
                    p.drawArcTo(0, 0, extend);
                } else {
                    p.drawLineTo(path[0].x * width, path[0].y * width);
                }

                p.setColor(segmentColor(f_meter->getPlanewaveOverLed(i), f_meter->getPlanewaveEnergy(i)));
                p.fillPreserve();
                p.setLineWidth(2);
                p.setColor(white);
                p.strokePreserve();
                p.setLineWidth(1);
                p.setColor(black);
                p.stroke();
            }
        }
    }
}

void Hoa3dMeter::drawVectors()
{
    UIPainter p = vec_layer.painter(rect());
    if (!p)
        return;

    p.setMatrix(1, 0, 0, -1, f_center, f_center);
    const auto size = f_center / 32.;

    if (f_vector_type == sym_both() || f_vector_type == sym_energy()) {
        auto rad = Math<float>::radius(f_vector_coords[3], f_vector_coords[4], f_vector_coords[5]);
        auto distance = (fabs(rad) * 0.5 + 0.5);
        auto color = rgba_addContrast(f_color_energy_vector, -(1. - distance));
        p.setColor(color);

        double x1, y1;
        if (!f_clockwise) {
            x1 = f_vector_coords[3] * f_radius;
            y1 = f_vector_coords[4] * f_radius;
        } else {
            double ang = -Math<float>::azimuth(f_vector_coords[3], f_vector_coords[4], f_vector_coords[5]);
            x1 = Math<float>::abscissa(rad * f_radius, ang);
            y1 = Math<float>::ordinate(rad * f_radius, ang);
        }

        if ((f_vector_coords[5] >= 0
                && (f_view == sym_top() || f_view == sym_toponbottom() || f_view == sym_topnextbottom()))
            || (f_vector_coords[5] <= 0 && f_view == sym_bottom())) {
            p.drawCircle(x1, y1, size * distance);
            p.fill();
        } else if (f_vector_coords[5] <= 0 && f_view == sym_toponbottom()) {
            p.drawCircle(x1, y1 - f_center * 2, size * distance);
            p.fill();
        } else if (f_vector_coords[5] <= 0 && f_view == sym_topnextbottom()) {
            p.drawCircle(x1 + f_center * 2, y1, size * distance);
            p.fill();
        }
    }

    if (f_vector_type == sym_both() || f_vector_type == sym_velocity()) {
        auto rad = Math<float>::radius(f_vector_coords[0], f_vector_coords[1], f_vector_coords[2]);
        auto distance = (fabs(rad) * 0.5 + 0.5);
        auto color = rgba_addContrast(f_color_velocity_vector, -(1. - distance));
        p.setColor(color);

        double x1, y1;
        if (!f_clockwise) {
            x1 = f_vector_coords[0] * f_radius;
            y1 = f_vector_coords[1] * f_radius;
        } else {
            double ang = -Math<float>::azimuth(f_vector_coords[0], f_vector_coords[1], f_vector_coords[2]);
            x1 = Math<float>::abscissa(rad * f_radius, ang);
            y1 = Math<float>::ordinate(rad * f_radius, ang);
        }

        if ((f_vector_coords[2] >= 0
                && (f_view == sym_top() || f_view == sym_toponbottom() || f_view == sym_topnextbottom()))
            || (f_vector_coords[2] <= 0 && f_view == sym_bottom())) {
            p.drawCircle(x1, y1, size * distance);
            p.fill();
        } else if (f_vector_coords[2] <= 0 && f_view == sym_toponbottom()) {
            p.drawCircle(x1, y1 - f_center * 2, size * distance);
            p.fill();
        } else if (f_vector_coords[2] <= 0 && f_view == sym_topnextbottom()) {
            p.drawCircle(x1 + f_center * 2, y1, size * distance);
            p.fill();
        }
    }
}

void Hoa3dMeter::drawBackground()
{
    UIPainter p = bg_layer_.painter(rect());
    if (!p)
        return;

    auto black = rgba_addContrast(prop_color_background, -HOA_CONTRAST_DARKER);
    auto white = rgba_addContrast(prop_color_background, HOA_CONTRAST_LIGHTER);
    if (f_view == sym_topnextbottom()) {
        p.setColor(white);
        p.setLineWidth(3);
        p.drawCircle(f_center, f_center, f_radius);
        p.strokePreserve();
        p.setColor(black);
        p.setLineWidth(1);
        p.stroke();

        p.setColor(white);
        p.setLineWidth(3);
        p.drawLine(f_center * 2, 0, f_center * 2, f_center * 2);
        p.setColor(black);
        p.setLineWidth(1);
        p.drawLine(f_center * 2, 0, f_center * 2, f_center * 2);

        p.setColor(white);
        p.setLineWidth(3);
        p.drawCircle(f_center * 3, f_center, f_radius);
        p.strokePreserve();
        p.setColor(black);
        p.setLineWidth(1);
        p.stroke();
    } else if (f_view == sym_toponbottom()) {
        p.setColor(white);
        p.setLineWidth(3);
        p.drawCircle(f_center, f_center, f_radius);
        p.strokePreserve();
        p.setColor(black);
        p.setLineWidth(1);
        p.stroke();

        p.setColor(white);
        p.setLineWidth(3);
        p.drawLine(0, f_center * 2, f_center * 2, f_center * 2);
        p.setColor(black);
        p.setLineWidth(1);
        p.drawLine(0, f_center * 2, f_center * 2, f_center * 2);

        p.setColor(white);
        p.setLineWidth(3);
        p.drawCircle(f_center, f_center * 3, f_radius);
        p.strokePreserve();
        p.setColor(black);
        p.setLineWidth(1);
        p.stroke();
    } else {
        p.setColor(white);
        p.setLineWidth(3);
        p.drawCircle(f_center, f_center, f_radius);
        p.strokePreserve();
        p.setColor(black);
        p.setLineWidth(1);
        p.stroke();
    }
}

void Hoa3dMeter::setup()
{
    UIObjectFactory<Hoa3dMeter> obj("hoa.3d.meter~", EBOX_IGNORELOCKCLICK | EBOX_GROWLINK);
    obj.setDefaultSize(225, 225);
    obj.hideProperty("send");
    obj.hideProperty("fontname");
    obj.hideProperty("fontweight");
    obj.hideProperty("fontslant");
    obj.hideProperty("fontsize");

    //    obj.addMethod("beatles", &Hoa3dMeter::m_beatles);

    obj.addIntProperty("channels", _("Channels"), DEF_NCHAN, &Hoa3dMeter::prop_channels, _("Main"));
    obj.setPropertyAccessor("channels", &Hoa3dMeter::propChannels, &Hoa3dMeter::propSetChannels);
    obj.setPropertyRange("channels", MIN_NCHAN, MAX_NCHAN);

    obj.addVirtualProperty("angles", _("Angles"),
        "0 90 0 -19.5 120 -19.5 240 -19.5",
        &Hoa3dMeter::propAngles, &Hoa3dMeter::propSetAngles, _("Main"));

    obj.addVirtualProperty("offset", _("Offset of Channels"),
        "0 0 0",
        &Hoa3dMeter::propOffset, &Hoa3dMeter::propSetOffset, _("Main"));

    obj.addBoolProperty("clockwise", _("Clockwise"), false, &Hoa3dMeter::f_clockwise);
    obj.setPropertyAccessor("clockwise", &Hoa3dMeter::propRotation, &Hoa3dMeter::propSetRotation);

    obj.addMenuProperty("view", _("View of Channels"), "top", &Hoa3dMeter::f_view, "top bottom topnextbottom toponbottom");
    obj.setPropertyAccessor("view", &Hoa3dMeter::propView, &Hoa3dMeter::propSetView);

    obj.addMenuProperty("vectors", _("Vectors"), "energy", &Hoa3dMeter::f_vector_type, "none energy velocity both");
    obj.setPropertyAccessor("vectors", &Hoa3dMeter::propVectors, &Hoa3dMeter::propSetVectors);

    obj.addIntProperty("interval", _("Refresh time (ms)"), 50, &Hoa3dMeter::f_interval);
    obj.setPropertyRange("interval", 20, 1000);
    obj.setPropertyUnits("interval", "msec");

    obj.addColorProperty("cold_color", _("Cold Signal Color"), "0. 0.6 0. 0.8", &Hoa3dMeter::f_color_cold_signal);
    obj.addColorProperty("tepid_color", _("Tepid Signal Color"), "0.6 0.73 0. 0.8", &Hoa3dMeter::f_color_tepid_signal);
    obj.addColorProperty("warm_color", _("Warm Signal Color"), "0.85 0.85 0. 0.8", &Hoa3dMeter::f_color_warm_signal);
    obj.addColorProperty("hot_color", _("Hot Signal Color"), "1. 0.6 0. 0.8", &Hoa3dMeter::f_color_hot_signal);
    obj.addColorProperty("over_color", _("Overload Signal Color"), "1. 0. 0. 0.8", &Hoa3dMeter::f_color_over_signal);
    obj.addColorProperty("energy_color", _("Energy Vector Color"), "0. 0. 1. 0.8", &Hoa3dMeter::f_color_energy_vector);
    obj.addColorProperty("velocity_color", _("Velocity Vector Color"), "1. 0. 0. 0.8", &Hoa3dMeter::f_color_velocity_vector);
}

void setup_spat_hoa_3d_meter()
{
    Hoa3dMeter::setup();
}
